#pragma once

#include <array>
#include <cstddef>

namespace gain_ground::source_layout_detail {

template <typename T, std::size_t Left, std::size_t Right>
constexpr auto concatenate_arrays(const std::array<T, Left> &left,
                                  const std::array<T, Right> &right) noexcept {
    std::array<T, Left + Right> result{};
    for (std::size_t index = 0; index < Left; ++index) result[index] = left[index];
    for (std::size_t index = 0; index < Right; ++index) result[Left + index] = right[index];
    return result;
}

template <typename T, std::size_t Left, std::size_t Middle, std::size_t Right,
          std::size_t... Rest>
constexpr auto concatenate_arrays(const std::array<T, Left> &left,
                                  const std::array<T, Middle> &middle,
                                  const std::array<T, Right> &right,
                                  const std::array<T, Rest> &...rest) noexcept {
    return concatenate_arrays(concatenate_arrays(left, middle), right, rest...);
}

} // namespace gain_ground::source_layout_detail
