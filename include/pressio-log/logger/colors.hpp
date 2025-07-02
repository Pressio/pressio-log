
#ifndef PRESSIOLOG_LOGGER_COLORS_HPP_
#define PRESSIOLOG_LOGGER_COLORS_HPP_

#include <string>

namespace pressiolog { namespace colors {

inline std::string color(std::string str, std::string color_code) {
    #ifdef PRESSIO_ENABLE_COLORIZED_OUTPUT
    return "\033[" + color_code + "m" + str + "\033[0m";
    #endif
    return str;
}

inline std::string green(std::string str) {
    return color(str, "32");
}

inline std::string red(std::string str) {
    return color(str, "31");
}

inline std::string yellow(std::string str) {
    return color(str, "33");
}

}} // end namespace pressiolog::colors

#endif  // PRESSIOLOG_LOGGER_COLORS_HPP_
