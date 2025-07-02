
#ifndef PRESSIOLOG_LOGGER_UTILS_HPP_
#define PRESSIOLOG_LOGGER_UTILS_HPP_

#include <pressio-log/fmt/fmt.h>

#include <sstream>
#include <type_traits>

namespace pressiolog { namespace utils {

// If the argument is formattable, return as is to the fmt::format call
template <typename T>
constexpr auto prep_for_fmt(T&& value)
    -> std::enable_if_t<fmt::is_formattable<std::decay_t<T>>::value, T&&> {
    return std::forward<T>(value);
}

// If the argument is not formattable, convert to a string and return to the fmt::format call.
// This is not ideal, since any formatting related to floats/exponents will be incompatible
template <typename T>
auto prep_for_fmt(T&& value)
    -> std::enable_if_t<!fmt::is_formattable<std::decay_t<T>>::value, std::string> {
    if constexpr (std::is_arithmetic_v<std::decay_t<T>>) {
        return std::to_string(value);
    } else {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
}

}} // end namespace pressiolog::utils

#endif  // PRESSIOLOG_LOGGER_UTILS_HPP_
