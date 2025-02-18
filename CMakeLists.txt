set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.18.0 FATAL_ERROR)
project(pressio-log CXX)

if (NOT TARGET pressio-log)
  add_library(pressio-log INTERFACE)
  target_include_directories(pressio-log INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(colors)

# versioning (adapted from eigen)
#=====================================================================
file(READ "${PROJECT_SOURCE_DIR}/include/pressio-log/version.hpp" _pressiolog_version)
string(REGEX MATCH "define[ \t]+PRESSIOLOG_MAJOR_VERSION[ \t]+([0-9]+)" _major_version_match "${_pressiolog_version}")
set(PRESSIOLOG_MAJOR_VERSION "${CMAKE_MATCH_1}")

string(REGEX MATCH "define[ \t]+PRESSIOLOG_MINOR_VERSION[ \t]+([0-9]+)" _minor_version_match "${_pressiolog_version}")
set(PRESSIOLOG_MINOR_VERSION "${CMAKE_MATCH_1}")

string(REGEX MATCH "define[ \t]+PRESSIOLOG_PATCH_VERSION[ \t]+([0-9]+)" _patch_version_match "${_pressiolog_version}")
set(PRESSIOLOG_PATCH_VERSION "${CMAKE_MATCH_1}")

set(PRESSIOLOG_VERSION_NUMBER ${PRESSIOLOG_MAJOR_VERSION}.${PRESSIOLOG_MINOR_VERSION}.${PRESSIOLOG_PATCH_VERSION})
message("${Magenta}>> PRESSIO-LOG: version = ${PRESSIOLOG_VERSION_NUMBER} ${ColourReset}")

# logging options
#=====================================================================
if (NOT DEFINED PRESSIO_SILENCE_WARNINGS)
  option(PRESSIO_SILENCE_WARNINGS "Enable or disable warnings" OFF)
endif ()

if (PRESSIO_SILENCE_WARNINGS)
  target_compile_definitions(pressio-log INTERFACE PRESSIO_SILENCE_WARNINGS=1)
else ()
  target_compile_definitions(pressio-log INTERFACE PRESSIO_SILENCE_WARNINGS=0)
endif ()

if (NOT DEFINED PRESSIO_ENABLE_COLORIZED_OUTPUT)
  option(PRESSIO_ENABLE_COLORIZED_OUTPUT "Enable or disable colorized logging" OFF)
endif ()

if (PRESSIO_ENABLE_COLORIZED_OUTPUT)
  target_compile_definitions(pressio-log INTERFACE PRESSIO_ENABLE_COLORIZED_OUTPUT=1)
else ()
  target_compile_definitions(pressio-log INTERFACE PRESSIO_ENABLE_COLORIZED_OUTPUT=0)
endif ()

message("colorized output: ${PRESSIO_ENABLE_COLORIZED_OUTPUT}")

# fmt
#=====================================================================
if (NOT DEFINED PRESSIO_ENABLE_EXTERNAL_FMT)
  option(PRESSIO_ENABLE_EXTERNAL_FMT "Allow using an externally provided fmt library" OFF)
endif ()

if (PRESSIO_ENABLE_EXTERNAL_FMT)
  target_compile_definitions(pressio-log INTERFACE PRESSIO_ENABLE_EXTERNAL_FMT=1)
  find_package(fmt QUIET)

  if (fmt_FOUND)
    message(STATUS "Using system-installed fmt")
    target_link_libraries(pressio-log INTERFACE fmt::fmt-header-only)
  elseif (fmt_INCLUDE_DIR)
    message(STATUS "Using fmt from fmt_INCLUDE_DIR: ${fmt_INCLUDE_DIR}")
    target_include_directories(pressio-log INTERFACE SYSTEM ${fmt_INCLUDE_DIR})
  elseif (fmt_ROOT)
    message(STATUS "Using fmt from fmt_ROOT: ${fmt_ROOT}")
    target_include_directories(pressio-log INTERFACE SYSTEM ${fmt_ROOT}/include)
  elseif (fmt_DIR)
    message(STATUS "Using fmt from fmt_DIR: ${fmt_DIR}")
    target_include_directories(pressio-log INTERFACE SYSTEM ${fmt_DIR}/include)
  else ()
    message(WARNING "PRESSIO_ENABLE_EXTERNAL_FMT is ON, but fmt was not found. "
                    "Use fmt_INCLUDE_DIR, fmt_ROOT, or fmt_DIR to provide the correct path.")
  endif ()

else ()
  target_compile_definitions(pressio-log INTERFACE PRESSIO_ENABLE_EXTERNAL_FMT=0)
endif ()


# install headers
#=====================================================================
include(GNUInstallDirs)

install(
  DIRECTORY include/pressio-log
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# tests
#=====================================================================
option(PRESSIOLOG_ENABLE_TESTS "Enable testing for pressio-log library" OFF)
if(PRESSIOLOG_ENABLE_TESTS)

  target_include_directories(
      pressio-log INTERFACE
      "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>;${CMAKE_CURRENT_SOURCE_DIR}/lib/fmt/include"
  )

  add_subdirectory(tests)
endif()
