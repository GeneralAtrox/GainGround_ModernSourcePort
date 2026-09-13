#include "gain_ground/checkpoint_writer.h"

#include "gground_checkpoints.h"

#include <algorithm>
#include <array>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include <zlib.h>

namespace gain_ground {
namespace {

constexpr std::size_t kHeaderBytes = 256;
constexpr std::size_t kEventHeaderBytes = 32;
constexpr std::size_t kCompressionBufferBytes = 1024 * 1024;
constexpr std::uint16_t kNoRegionFilter = 0xffffU;

[[noreturn]] void fail(const std::string &message)
{
    throw std::runtime_error("Gain Ground checkpoint writer: " + message);
}

template <typename Unsigned>
void append_little_endian(std::vector<std::uint8_t> &destination, Unsigned value)
{
    static_assert(std::is_unsigned_v<Unsigned>);
    for (std::size_t byte = 0; byte != sizeof(Unsigned); ++byte) {
        destination.push_back(static_cast<std::uint8_t>(value & 0xffU));
        value >>= 8U;
    }
}

template <typename Unsigned, std::size_t Size>
void append_little_endian(std::array<std::uint8_t, Size> &destination, std::size_t &offset, Unsigned value)
{
    static_assert(std::is_unsigned_v<Unsigned>);
    if (sizeof(Unsigned) > Size - offset)
        fail("internal fixed-header geometry overflow");
    for (std::size_t byte = 0; byte != sizeof(Unsigned); ++byte) {
        destination[offset++] = static_cast<std::uint8_t>(value & 0xffU);
        value >>= 8U;
    }
}

template <std::size_t DestinationSize, std::size_t SourceSize>
void append_bytes(
    std::array<std::uint8_t, DestinationSize> &destination,
    std::size_t &offset,
    const std::array<std::uint8_t, SourceSize> &source)
{
    if (SourceSize > DestinationSize - offset)
        fail("internal fixed-header geometry overflow");
    std::copy(source.begin(), source.end(), destination.begin() + static_cast<std::ptrdiff_t>(offset));
    offset += SourceSize;
}

std::uint16_t read_u16(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    if (offset > bytes.size() || bytes.size() - offset < 2)
        fail("payload is too short for its region selector");
    return static_cast<std::uint16_t>(bytes[offset]) |
        static_cast<std::uint16_t>(static_cast<std::uint16_t>(bytes[offset + 1]) << 8U);
}

std::uint32_t read_u32(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    if (offset > bytes.size() || bytes.size() - offset < 4)
        fail("payload is too short for its dynamic-size field");
    std::uint32_t value{};
    for (unsigned shift = 0; shift != 32; shift += 8)
        value |= static_cast<std::uint32_t>(bytes[offset++]) << shift;
    return value;
}

const generated::CaptureCheckpointEventContract &event_contract(std::uint16_t event_type)
{
    const auto found = std::find_if(
        generated::kCaptureCheckpointEvents.begin(),
        generated::kCaptureCheckpointEvents.end(),
        [event_type](const auto &item) { return item.event_type == event_type; });
    if (found == generated::kCaptureCheckpointEvents.end())
        fail("event type " + std::to_string(event_type) + " is outside the selected checkpoint contract");
    return *found;
}

void validate_payload(
    const generated::CaptureCheckpointEventContract &contract,
    std::span<const std::uint8_t> payload)
{
    if (contract.payload_bytes != 0 && payload.size() != contract.payload_bytes) {
        fail(std::string(contract.event_name) + " payload has " + std::to_string(payload.size()) +
            " bytes; contract requires " + std::to_string(contract.payload_bytes));
    }
    if (contract.event_type == 3U) {
        constexpr std::size_t prefix_bytes = 24;
        if (payload.size() < prefix_bytes)
            fail("memorySnapshot payload is shorter than its 24-byte prefix");
        const auto declared = read_u32(payload, 8);
        if (declared != payload.size() - prefix_bytes)
            fail("memorySnapshot dataBytes differs from the appended snapshot bytes");
    }
    if (contract.region_filter != kNoRegionFilter && read_u16(payload, 2) != contract.region_filter) {
        fail(std::string(contract.event_name) + " payload does not match required region " +
            std::to_string(contract.region_filter));
    }
}

std::uint8_t hex_nibble(char value)
{
    if (value >= '0' && value <= '9')
        return static_cast<std::uint8_t>(value - '0');
    if (value >= 'a' && value <= 'f')
        return static_cast<std::uint8_t>(value - 'a' + 10);
    if (value >= 'A' && value <= 'F')
        return static_cast<std::uint8_t>(value - 'A' + 10);
    fail("identity contains a non-hexadecimal character");
}

template <std::size_t Size>
std::array<std::uint8_t, Size> digest_from_hex(std::string_view value)
{
    if (value.size() != Size * 2)
        fail("identity hexadecimal length differs");
    std::array<std::uint8_t, Size> result{};
    for (std::size_t index = 0; index != Size; ++index)
        result[index] = static_cast<std::uint8_t>((hex_nibble(value[index * 2]) << 4U) | hex_nibble(value[index * 2 + 1]));
    return result;
}

std::array<std::uint8_t, kHeaderBytes> make_header(const CheckpointStreamIdentity &identity)
{
    std::array<std::uint8_t, kHeaderBytes> result{};
    constexpr std::array<std::uint8_t, 8> magic{'G', 'G', 'R', 'C', 'A', 'P', '1', 0};
    std::size_t offset{};
    append_bytes(result, offset, magic);
    append_little_endian(result, offset, std::uint32_t{1});
    append_little_endian(result, offset, std::uint32_t{kHeaderBytes});
    append_little_endian(result, offset, std::uint32_t{kEventHeaderBytes});
    append_little_endian(result, offset, std::uint32_t{0x01020304U});
    append_bytes(result, offset, identity.base_mame_commit);
    append_bytes(result, offset, identity.identity_sha256);
    append_bytes(result, offset, identity.rom_manifest_sha256);
    append_bytes(result, offset, identity.executable_sha256);
    append_bytes(result, offset, identity.configuration_sha256);
    append_bytes(result, offset, identity.replay_sha256);
    append_bytes(result, offset, identity.source_manifest_sha256);
    append_little_endian(result, offset, std::uint32_t{1});
    append_little_endian(result, offset, std::uint32_t{1});
    append_little_endian(result, offset, std::uint32_t{kCompressionBufferBytes});
    append_little_endian(result, offset, std::uint32_t{0});
    append_little_endian(result, offset, std::uint32_t{0});
    if (offset != result.size())
        fail("internal fixed-header geometry differs");
    return result;
}

} // namespace

CheckpointPayload &CheckpointPayload::u8(std::uint8_t value)
{
    bytes_.push_back(value);
    return *this;
}

CheckpointPayload &CheckpointPayload::u16(std::uint16_t value)
{
    append_little_endian(bytes_, value);
    return *this;
}

CheckpointPayload &CheckpointPayload::u32(std::uint32_t value)
{
    append_little_endian(bytes_, value);
    return *this;
}

CheckpointPayload &CheckpointPayload::s32(std::int32_t value)
{
    append_little_endian(bytes_, static_cast<std::uint32_t>(value));
    return *this;
}

CheckpointPayload &CheckpointPayload::u64(std::uint64_t value)
{
    append_little_endian(bytes_, value);
    return *this;
}

CheckpointPayload &CheckpointPayload::s64(std::int64_t value)
{
    append_little_endian(bytes_, static_cast<std::uint64_t>(value));
    return *this;
}

CheckpointPayload &CheckpointPayload::bytes(std::span<const std::uint8_t> value)
{
    bytes_.insert(bytes_.end(), value.begin(), value.end());
    return *this;
}

class CheckpointProjectionWriter::Implementation {
public:
    Implementation(std::filesystem::path path, const CheckpointStreamIdentity &identity) : path_(std::move(path))
    {
        stream_.open(path_, std::ios::binary | std::ios::trunc);
        if (!stream_)
            fail("cannot open " + path_.string());
        const auto header = make_header(identity);
        stream_.write(reinterpret_cast<const char *>(header.data()), static_cast<std::streamsize>(header.size()));
        if (!stream_)
            fail("cannot write capture header to " + path_.string());
        if (deflateInit(&zstream_, 1) != Z_OK)
            fail("zlib initialization failed");
        zlib_initialized_ = true;
    }

    ~Implementation()
    {
        if (zlib_initialized_)
            deflateEnd(&zstream_);
    }

    void write_event(
        std::uint16_t event_type,
        std::uint8_t event_specific_flags,
        std::uint8_t producer_domain,
        std::span<const std::uint8_t> payload,
        std::int64_t emulated_seconds,
        std::int64_t emulated_attoseconds)
    {
        if (finished_)
            fail("cannot append an event after finish");
        if (payload.size() > std::numeric_limits<std::uint32_t>::max())
            fail("event payload exceeds the GGRCAP1 size field");
        const auto &contract = event_contract(event_type);
        validate_payload(contract, payload);

        std::vector<std::uint8_t> event;
        event.reserve(kEventHeaderBytes + payload.size());
        append_little_endian(event, event_type);
        append_little_endian(event, static_cast<std::uint16_t>(
            static_cast<std::uint16_t>(event_specific_flags) |
            static_cast<std::uint16_t>(static_cast<std::uint16_t>(producer_domain) << 8U)));
        append_little_endian(event, static_cast<std::uint32_t>(payload.size()));
        append_little_endian(event, event_count_);
        append_little_endian(event, static_cast<std::uint64_t>(emulated_seconds));
        append_little_endian(event, static_cast<std::uint64_t>(emulated_attoseconds));
        event.insert(event.end(), payload.begin(), payload.end());
        if (event.size() != kEventHeaderBytes + payload.size())
            fail("internal event geometry differs");
        compress(event, Z_NO_FLUSH);
        ++event_count_;
    }

    void finish()
    {
        if (finished_)
            return;
        compress({}, Z_FINISH);
        const int status = deflateEnd(&zstream_);
        zlib_initialized_ = false;
        if (status != Z_OK)
            fail("zlib finalization failed");
        stream_.flush();
        if (!stream_)
            fail("cannot finalize " + path_.string());
        stream_.close();
        if (!stream_)
            fail("cannot close " + path_.string());
        finished_ = true;
    }

    [[nodiscard]] std::uint64_t event_count() const noexcept { return event_count_; }
    [[nodiscard]] const std::filesystem::path &path() const noexcept { return path_; }

private:
    void compress(std::span<const std::uint8_t> input, int flush)
    {
        if (input.size() > std::numeric_limits<uInt>::max())
            fail("logical checkpoint write exceeds the native zlib input limit");
        zstream_.next_in = input.empty() ? Z_NULL : const_cast<Bytef *>(reinterpret_cast<const Bytef *>(input.data()));
        zstream_.avail_in = static_cast<uInt>(input.size());
        do {
            zstream_.next_out = reinterpret_cast<Bytef *>(compression_buffer_.data());
            zstream_.avail_out = static_cast<uInt>(compression_buffer_.size());
            const int status = deflate(&zstream_, flush);
            if (status != Z_OK && status != Z_STREAM_END)
                fail("zlib compression failed with status " + std::to_string(status));
            const auto produced = compression_buffer_.size() - zstream_.avail_out;
            if (produced != 0) {
                stream_.write(
                    reinterpret_cast<const char *>(compression_buffer_.data()),
                    static_cast<std::streamsize>(produced));
                if (!stream_)
                    fail("cannot write compressed event stream to " + path_.string());
            }
            if (flush == Z_FINISH && status == Z_STREAM_END)
                break;
        } while (zstream_.avail_in != 0 || (flush == Z_FINISH && zstream_.avail_out == 0));
    }

    std::filesystem::path path_;
    std::ofstream stream_;
    z_stream zstream_{};
    std::array<std::uint8_t, kCompressionBufferBytes> compression_buffer_{};
    std::uint64_t event_count_{};
    bool zlib_initialized_{};
    bool finished_{};
};

CheckpointProjectionWriter::CheckpointProjectionWriter(
    std::filesystem::path path,
    const CheckpointStreamIdentity &identity) :
    implementation_(std::make_unique<Implementation>(std::move(path), identity))
{
}

CheckpointProjectionWriter::~CheckpointProjectionWriter() = default;

void CheckpointProjectionWriter::write_event(
    std::uint16_t event_type,
    std::uint8_t event_specific_flags,
    std::uint8_t producer_domain,
    std::span<const std::uint8_t> payload,
    std::int64_t emulated_seconds,
    std::int64_t emulated_attoseconds)
{
    implementation_->write_event(
        event_type,
        event_specific_flags,
        producer_domain,
        payload,
        emulated_seconds,
        emulated_attoseconds);
}

void CheckpointProjectionWriter::finish()
{
    implementation_->finish();
}

std::uint64_t CheckpointProjectionWriter::event_count() const noexcept
{
    return implementation_->event_count();
}

const std::filesystem::path &CheckpointProjectionWriter::path() const noexcept
{
    return implementation_->path();
}

std::array<std::uint8_t, 20> checkpoint_sha1(std::string_view hexadecimal)
{
    return digest_from_hex<20>(hexadecimal);
}

std::array<std::uint8_t, 32> checkpoint_sha256(std::string_view hexadecimal)
{
    return digest_from_hex<32>(hexadecimal);
}

} // namespace gain_ground
