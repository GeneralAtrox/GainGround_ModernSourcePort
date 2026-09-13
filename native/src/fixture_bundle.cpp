#include "gain_ground/fixture_bundle.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string_view>

#include <zlib.h>

namespace gain_ground {
namespace {

constexpr std::size_t kBundleHeaderBytes = 384;
constexpr std::size_t kBundleRecordBytes = 152;
constexpr std::size_t kFixturePrefixBytes = 240;
constexpr std::size_t kEdgeBytes = 16;
constexpr std::size_t kMemoryPrestateBytes = 20;
constexpr std::size_t kMemoryDeltaBytes = 24;
constexpr std::size_t kCallBytes = 20;
constexpr std::size_t kHardwareBytes = 20;

[[noreturn]] void fail(const std::string &message)
{
    throw std::runtime_error("Gain Ground fixture bundle: " + message);
}

class Reader {
public:
    explicit Reader(const std::vector<std::uint8_t> &bytes) : bytes_(bytes) {}

    [[nodiscard]] std::size_t offset() const noexcept { return offset_; }
    [[nodiscard]] std::size_t remaining() const noexcept { return bytes_.size() - offset_; }

    std::uint8_t u8()
    {
        need(1);
        return bytes_[offset_++];
    }

    std::uint16_t u16()
    {
        need(2);
        const auto value = static_cast<std::uint16_t>(bytes_[offset_]) |
            (static_cast<std::uint16_t>(bytes_[offset_ + 1]) << 8U);
        offset_ += 2;
        return value;
    }

    std::uint32_t u32()
    {
        need(4);
        std::uint32_t value{};
        for (unsigned shift = 0; shift != 32; shift += 8)
            value |= static_cast<std::uint32_t>(bytes_[offset_++]) << shift;
        return value;
    }

    std::uint64_t u64()
    {
        need(8);
        std::uint64_t value{};
        for (unsigned shift = 0; shift != 64; shift += 8)
            value |= static_cast<std::uint64_t>(bytes_[offset_++]) << shift;
        return value;
    }

    template <std::size_t Size>
    std::array<std::uint8_t, Size> bytes()
    {
        need(Size);
        std::array<std::uint8_t, Size> result{};
        std::copy_n(bytes_.begin() + static_cast<std::ptrdiff_t>(offset_), Size, result.begin());
        offset_ += Size;
        return result;
    }

    void zeroes(std::size_t count, std::string_view field)
    {
        need(count);
        const auto begin = bytes_.begin() + static_cast<std::ptrdiff_t>(offset_);
        if (!std::all_of(begin, begin + static_cast<std::ptrdiff_t>(count), [](std::uint8_t value) { return value == 0; }))
            fail(std::string(field) + " reserved bytes are nonzero");
        offset_ += count;
    }

private:
    void need(std::size_t count) const
    {
        if (count > remaining())
            fail("truncated binary field");
    }

    const std::vector<std::uint8_t> &bytes_;
    std::size_t offset_{};
};

std::vector<std::uint8_t> read_exact(std::istream &stream, std::size_t bytes, std::string_view field)
{
    std::vector<std::uint8_t> result(bytes);
    if (bytes != 0)
        stream.read(reinterpret_cast<char *>(result.data()), static_cast<std::streamsize>(bytes));
    if (!stream || stream.gcount() != static_cast<std::streamsize>(bytes))
        fail(std::string(field) + " is truncated");
    return result;
}

void require_magic(const std::vector<std::uint8_t> &bytes)
{
    static constexpr std::array<std::uint8_t, 8> expected{'G', 'G', 'F', 'T', 'S', 'T', '1', 0};
    if (!std::equal(expected.begin(), expected.end(), bytes.begin()))
        fail("magic differs");
}

CpuRegisters read_registers(Reader &reader)
{
    CpuRegisters result;
    for (auto &value : result.data)
        value = reader.u32();
    for (auto &value : result.address)
        value = reader.u32();
    result.program_counter = reader.u32();
    result.status = reader.u16();
    if (reader.u16() != 0)
        fail("register reserved field is nonzero");
    return result;
}

std::uint64_t checked_geometry(
    std::uint32_t edges,
    std::uint32_t prestates,
    std::uint32_t deltas,
    std::uint32_t calls,
    std::uint32_t hardware)
{
    return kFixturePrefixBytes +
        static_cast<std::uint64_t>(edges) * kEdgeBytes +
        static_cast<std::uint64_t>(prestates) * kMemoryPrestateBytes +
        static_cast<std::uint64_t>(deltas) * kMemoryDeltaBytes +
        static_cast<std::uint64_t>(calls) * kCallBytes +
        static_cast<std::uint64_t>(hardware) * kHardwareBytes;
}

std::vector<std::uint8_t> inflate_record(const std::vector<std::uint8_t> &compressed, std::uint64_t raw_bytes)
{
    if (compressed.size() > std::numeric_limits<uInt>::max() || raw_bytes > std::numeric_limits<uInt>::max())
        fail("record exceeds the native zlib one-shot size limit");

    std::vector<std::uint8_t> raw(static_cast<std::size_t>(raw_bytes));
    z_stream stream{};
    stream.next_in = const_cast<Bytef *>(reinterpret_cast<const Bytef *>(compressed.data()));
    stream.avail_in = static_cast<uInt>(compressed.size());
    stream.next_out = reinterpret_cast<Bytef *>(raw.data());
    stream.avail_out = static_cast<uInt>(raw.size());
    if (inflateInit(&stream) != Z_OK)
        fail("zlib initialization failed");
    const int status = inflate(&stream, Z_FINISH);
    const auto remaining_input = stream.avail_in;
    const auto remaining_output = stream.avail_out;
    inflateEnd(&stream);
    if (status != Z_STREAM_END || remaining_input != 0 || remaining_output != 0)
        fail("record has invalid, concatenated, or size-mismatched zlib data");
    return raw;
}

} // namespace

FixtureBundle::FixtureBundle(std::filesystem::path path) : path_(std::move(path))
{
    std::ifstream stream(path_, std::ios::binary);
    if (!stream)
        fail("cannot open " + path_.string());

    const auto raw_header = read_exact(stream, kBundleHeaderBytes, "header");
    require_magic(raw_header);
    Reader reader(raw_header);
    reader.bytes<8>();
    header_.version = reader.u32();
    header_.header_bytes = reader.u32();
    header_.record_header_bytes = reader.u32();
    header_.compression = reader.u32();
    header_.function_count = reader.u64();
    header_.fixture_count = reader.u64();
    header_.base_mame_commit = reader.bytes<20>();
    header_.capture_source_commit = reader.bytes<20>();
    for (auto &identity : header_.identities)
        identity = reader.bytes<32>();
    reader.zeroes(48, "header");
    if (reader.remaining() != 0)
        fail("header geometry differs");
    if (header_.version != 2 || header_.header_bytes != kBundleHeaderBytes ||
        header_.record_header_bytes != kBundleRecordBytes || header_.compression != 1)
        fail("unsupported version, geometry, or compression");
    if (header_.function_count == 0 || header_.function_count > 65536 ||
        header_.fixture_count > std::numeric_limits<std::size_t>::max())
        fail("implausible inventory counts");

    records_.reserve(static_cast<std::size_t>(header_.fixture_count));
    std::vector<std::uint32_t> variants(static_cast<std::size_t>(header_.function_count));
    std::uint32_t previous_function{};
    bool have_previous = false;
    for (std::uint64_t ordinal = 0; ordinal != header_.fixture_count; ++ordinal) {
        const auto raw_record = read_exact(stream, kBundleRecordBytes, "record header");
        Reader record_reader(raw_record);
        const auto record_type = record_reader.u32();
        const auto record_header_bytes = record_reader.u32();
        FixtureBundleRecordIndex record;
        record.function_id = record_reader.u32();
        record.variant_index = record_reader.u32();
        record.control = record_reader.u32();
        record.source_index = record_reader.u32();
        record.source_fixture_ordinal = record_reader.u64();
        record.raw_bytes = record_reader.u64();
        record.compressed_bytes = record_reader.u64();
        record.path_digest = record_reader.bytes<32>();
        record.semantic_digest = record_reader.bytes<32>();
        record.payload_digest = record_reader.bytes<32>();
        const auto record_flags = record_reader.u32();
        record_reader.zeroes(4, "record");
        if (record_type != 1 || record_header_bytes != kBundleRecordBytes || record_reader.remaining() != 0)
            fail("record metadata differs at ordinal " + std::to_string(ordinal));
        if (record.function_id >= header_.function_count || (have_previous && record.function_id < previous_function))
            fail("record function ordering differs at ordinal " + std::to_string(ordinal));
        if (record.variant_index != variants[record.function_id]++)
            fail("record variant ordering differs at ordinal " + std::to_string(ordinal));
        if (record.control < 1 || (record.control > 5 && record.control != 8) || record.raw_bytes < kFixturePrefixBytes || record.compressed_bytes == 0)
            fail("record control or sizes differ at ordinal " + std::to_string(ordinal));
        if ((record_flags & ~1U) != 0U)
            fail("record flags differ at ordinal " + std::to_string(ordinal));
        record.native_replay_eligible = (record_flags & 1U) != 0U;
        const auto position = stream.tellg();
        if (position < 0)
            fail("cannot determine payload position");
        record.payload_file_offset = static_cast<std::uint64_t>(position);
        if (record.compressed_bytes > static_cast<std::uint64_t>(std::numeric_limits<std::streamoff>::max()))
            fail("record compressed size exceeds stream range");
        stream.seekg(static_cast<std::streamoff>(record.compressed_bytes), std::ios::cur);
        if (!stream)
            fail("record payload is truncated at ordinal " + std::to_string(ordinal));
        records_.push_back(record);
        previous_function = record.function_id;
        have_previous = true;
    }
    const auto expected_end = stream.tellg();
    if (expected_end < 0 || static_cast<std::uint64_t>(expected_end) != std::filesystem::file_size(path_))
        fail("trailing bytes or truncated final payload");
}

FixtureRecord FixtureBundle::load(std::size_t record_index) const
{
    if (record_index >= records_.size())
        fail("record index is out of range");
    const auto &index = records_[record_index];
    if (index.compressed_bytes > std::numeric_limits<std::size_t>::max())
        fail("compressed record exceeds addressable memory");

    std::ifstream stream(path_, std::ios::binary);
    if (!stream)
        fail("cannot reopen " + path_.string());
    stream.seekg(static_cast<std::streamoff>(index.payload_file_offset));
    if (!stream)
        fail("cannot seek to record payload");
    const auto compressed = read_exact(stream, static_cast<std::size_t>(index.compressed_bytes), "record payload");
    const auto raw = inflate_record(compressed, index.raw_bytes);
    Reader reader(raw);

    FixtureRecord result;
    result.bundle = index;
    result.fixture_ordinal = reader.u64();
    result.function_id = reader.u32();
    result.cpu = reader.u8();
    result.state = reader.u8();
    result.entry_kind = reader.u8();
    result.control = reader.u8();
    result.entry_pc = reader.u32();
    result.exit_pc = reader.u32();
    result.callsite = reader.u32();
    result.expected_return = reader.u32();
    result.start_instruction = reader.u64();
    result.end_instruction = reader.u64();
    result.entry_frame = reader.u64();
    result.end_frame = reader.u64();
    result.instruction_count = reader.u32();
    const auto edge_count = reader.u32();
    const auto prestate_count = reader.u32();
    const auto delta_count = reader.u32();
    const auto call_count = reader.u32();
    const auto hardware_count = reader.u32();
    result.execution_pc = reader.u32();
    if (result.execution_pc == 0U)
        result.execution_pc = result.entry_pc;
    if (reader.u32() != 0)
        fail("fixture prefix reserved field is nonzero");
    result.entry_registers = read_registers(reader);
    result.exit_registers = read_registers(reader);

    if (reader.offset() != kFixturePrefixBytes ||
        checked_geometry(edge_count, prestate_count, delta_count, call_count, hardware_count) != raw.size())
        fail("fixture payload geometry differs at record " + std::to_string(record_index));
    if (result.fixture_ordinal != index.source_fixture_ordinal || result.function_id != index.function_id ||
        result.control != index.control || result.control < 1 ||
        (result.control > 5 && result.control != 8))
        fail("fixture payload identity differs at record " + std::to_string(record_index));

    result.edges.reserve(edge_count);
    for (std::uint32_t count = 0; count != edge_count; ++count) {
        FixtureEdge item;
        item.from_state = reader.u8();
        item.to_state = reader.u8();
        item.kind = reader.u8();
        if (reader.u8() != 0)
            fail("edge reserved field is nonzero");
        item.opcode = reader.u16();
        if (reader.u16() != 0)
            fail("edge reserved word is nonzero");
        item.source_pc = reader.u32();
        item.target_pc = reader.u32();
        result.edges.push_back(item);
    }
    result.memory_prestates.reserve(prestate_count);
    for (std::uint32_t count = 0; count != prestate_count; ++count) {
        FixtureMemoryPrestate item;
        item.region = reader.u16();
        item.flags = reader.u16();
        item.offset = reader.u32();
        item.initial = reader.u16();
        item.read_mask = reader.u16();
        item.access_count = reader.u32();
        item.first_sequence = reader.u32();
        result.memory_prestates.push_back(item);
    }
    result.memory_deltas.reserve(delta_count);
    for (std::uint32_t count = 0; count != delta_count; ++count) {
        FixtureMemoryDelta item;
        item.region = reader.u16();
        item.write_mask = reader.u16();
        item.offset = reader.u32();
        item.initial = reader.u16();
        item.final_value = reader.u16();
        item.write_count = reader.u32();
        item.first_sequence = reader.u32();
        item.last_sequence = reader.u32();
        result.memory_deltas.push_back(item);
    }
    result.calls.reserve(call_count);
    for (std::uint32_t count = 0; count != call_count; ++count) {
        FixtureCall item;
        item.sequence = reader.u32();
        item.function_id = reader.u32();
        item.target_cpu = reader.u8();
        item.target_state = reader.u8();
        item.kind = reader.u8();
        if (reader.u8() != 0)
            fail("call reserved field is nonzero");
        item.callsite = reader.u32();
        item.target = reader.u32();
        result.calls.push_back(item);
    }
    result.hardware_effects.reserve(hardware_count);
    for (std::uint32_t count = 0; count != hardware_count; ++count) {
        FixtureHardwareEffect item;
        item.sequence = reader.u32();
        item.kind = reader.u8();
        item.cpu = reader.u8();
        item.state = reader.u8();
        if (reader.u8() != 0)
            fail("hardware-effect reserved field is nonzero");
        item.pc = reader.u32();
        item.address = reader.u32();
        item.data = reader.u16();
        item.memory_mask = reader.u16();
        result.hardware_effects.push_back(item);
    }
    if (reader.remaining() != 0)
        fail("fixture payload has trailing fields");
    return result;
}

std::string hexadecimal(const std::uint8_t *bytes, std::size_t count)
{
    std::ostringstream stream;
    stream << std::hex << std::setfill('0');
    for (std::size_t index = 0; index != count; ++index)
        stream << std::setw(2) << static_cast<unsigned>(bytes[index]);
    return stream.str();
}

} // namespace gain_ground
