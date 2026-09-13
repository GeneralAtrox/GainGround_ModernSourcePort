#include "gain_ground/rom_import.h"
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <zlib.h>

namespace gain_ground { namespace {
using Bytes = std::vector<std::uint8_t>;
Bytes read_file(const std::filesystem::path &path, std::size_t limit) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file || file.tellg() < 0 || file.tellg() > static_cast<std::streamoff>(limit))
        throw std::runtime_error("Cannot read ROM file, or file is too large: " + path.filename().string());
    Bytes bytes(static_cast<std::size_t>(file.tellg()));
    file.seekg(0);
    file.read(reinterpret_cast<char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    if (!file) throw std::runtime_error("Incomplete ROM file read");
    return bytes;
}
std::uint32_t number(const Bytes &b, std::size_t offset, unsigned width) {
    if (offset > b.size() || width > b.size() - offset) throw std::runtime_error("Truncated ZIP");
    std::uint32_t value{};
    for (unsigned i = 0; i < width; ++i) value |= std::uint32_t(b[offset+i]) << (i*8U);
    return value;
}
Bytes zip_member(const Bytes &zip, std::string_view wanted, std::size_t expected) {
    if (zip.size() < 22) throw std::runtime_error("Invalid ZIP header");
    std::size_t end = zip.size() - 22;
    for (;;) {
        if (number(zip,end,4) == 0x06054b50U && end+22+number(zip,end+20,2) == zip.size()) break;
        if (end == 0 || zip.size()-end > 65557) throw std::runtime_error("ZIP directory not found");
        --end;
    }
    const auto count = number(zip,end+10,2);
    if (number(zip,end+4,2) || number(zip,end+6,2) || count != number(zip,end+8,2) || count == 65535)
        throw std::runtime_error("Split and ZIP64 archives are not supported");
    std::size_t position = number(zip,end+16,4);
    if (position > end || number(zip,end+12,4) != end-position)
        throw std::runtime_error("Invalid ZIP directory bounds");
    Bytes result;
    bool found{};
    for (unsigned entry = 0; entry < count; ++entry) {
        if (number(zip,position,4) != 0x02014b50U) throw std::runtime_error("Invalid ZIP entry");
        const auto flags=number(zip,position+8,2), method=number(zip,position+10,2);
        const auto checksum=number(zip,position+16,4), compressed=number(zip,position+20,4);
        const auto unpacked=number(zip,position+24,4), length=number(zip,position+28,2);
        const auto next=position+46+length+number(zip,position+30,2)+number(zip,position+32,2);
        const auto local=number(zip,position+42,4);
        if (next > end) throw std::runtime_error("ZIP entry exceeds directory");
        const std::string_view name(reinterpret_cast<const char *>(zip.data()+position+46),length);
        const auto slash=name.find_last_of("/\\");
        const auto basename=slash==name.npos ? name : name.substr(slash+1);
        if (basename == wanted) {
            if (found || (flags & 1U) || unpacked != expected || (method != 0 && method != 8))
                throw std::runtime_error("Duplicate, encrypted, unsupported or incorrect-size ROM member: " + std::string(wanted));
            found=true;
            if (number(zip,local,4) != 0x04034b50U || number(zip,local+8,2) != method || number(zip,local+6,2) != flags)
                throw std::runtime_error("Invalid local ZIP entry");
            const std::size_t start=std::size_t(local)+30+number(zip,local+26,2)+number(zip,local+28,2);
            if (start > position || compressed > position-start) throw std::runtime_error("Invalid compressed ROM bounds");
            result.resize(expected);
            if (method == 0) {
                if (compressed != expected) throw std::runtime_error("Invalid stored ROM length");
                std::copy_n(zip.data()+start,expected,result.data());
            } else {
                z_stream stream{};
                stream.next_in=const_cast<Bytef *>(zip.data()+start); stream.avail_in=compressed;
                stream.next_out=result.data(); stream.avail_out=static_cast<uInt>(result.size());
                if (inflateInit2(&stream,-MAX_WBITS) != Z_OK) throw std::runtime_error("ZIP decompressor unavailable");
                const auto status=inflate(&stream,Z_FINISH);
                const bool complete=status==Z_STREAM_END && stream.total_out==expected && stream.total_in==compressed;
                inflateEnd(&stream);
                if (!complete) throw std::runtime_error("Invalid compressed ROM data");
            }
            if (crc32(0,result.data(),static_cast<uInt>(result.size())) != checksum)
                throw std::runtime_error("ROM ZIP checksum failed");
        }
        position=next;
    }
    if (!found) throw std::runtime_error("ROM set is missing " + std::string(wanted));
    return result;
}
}
std::vector<std::uint8_t> read_rom_member(const std::filesystem::path &source,
    std::string_view name, std::size_t expected_size) {
    if (source.extension()==L".zip" || source.extension()==L".ZIP")
        return zip_member(read_file(source,32U*1024U*1024U),name,expected_size);
    const auto directory=std::filesystem::is_directory(source) ? source : source.parent_path();
    auto bytes=read_file(directory / name,expected_size);
    if (bytes.size()!=expected_size) throw std::runtime_error("Incorrect ROM size: " + std::string(name));
    return bytes;
}
}
