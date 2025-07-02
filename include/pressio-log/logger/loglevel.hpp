
#ifndef PRESSIOLOG_LOGGER_LOGLEVEL_HPP_
#define PRESSIOLOG_LOGGER_LOGLEVEL_HPP_

#include <iostream>

namespace pressiolog {

enum class LogLevel : int {
    none,
    sparse,
    error,
    warning,
    info,
    debug
};

inline std::ostream& operator<<(std::ostream& os, LogLevel level) {
    switch (level) {
        case LogLevel::none:    return os << "none";
        case LogLevel::sparse:  return os << "sparse";
        case LogLevel::error:   return os << "error";
        case LogLevel::warning: return os << "warning";
        case LogLevel::info:    return os << "info";
        case LogLevel::debug:   return os << "debug";
        default:                return os << "unknown";
    }
}

} // end namespace pressiolog

#endif  // PRESSIOLOG_LOGGER_LOGLEVEL_HPP_
