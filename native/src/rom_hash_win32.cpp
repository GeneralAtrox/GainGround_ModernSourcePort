#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <bcrypt.h>
#include <array>
#include <limits>
#include "gain_ground/rom_import.h"
namespace gain_ground {
bool matches_hash(std::span<const std::uint8_t> bytes, std::string_view expected)
{
    if (expected.size() != 64U || bytes.size() > std::numeric_limits<ULONG>::max()) return false;
    BCRYPT_ALG_HANDLE algorithm{};
    if (BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_SHA256_ALGORITHM, nullptr, 0) < 0) return false;
    std::array<UCHAR, 32> digest{};
    const auto status = BCryptHash(algorithm, nullptr, 0, const_cast<PUCHAR>(bytes.data()),
        static_cast<ULONG>(bytes.size()), digest.data(), static_cast<ULONG>(digest.size()));
    BCryptCloseAlgorithmProvider(algorithm, 0);
    if (status < 0) return false;
    constexpr char hex[] = "0123456789abcdef";
    for (std::size_t i = 0; i < digest.size(); ++i)
        if (hex[digest[i] >> 4U] != expected[2U * i] ||
            hex[digest[i] & 15U] != expected[2U * i + 1U]) return false;
    return true;
}

}
