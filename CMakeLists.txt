set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.18.0 FATAL_ERROR)
project(pressio-log CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(colors)

# versioning
#=====================================================================
# adapted from Eigen
file(READ "${PROJECT_SOURCE_DIR}/include/pressio-log/version.hpp" _pressiolog_version)
string(REGEX MATCH "define[ \t]+PRESSIOLOG_MAJOR_VERSION[ \t]+([0-9]+)" _major_version_match "${_pressiolog_version}")
set(PRESSIOLOG_MAJOR_VERSION "${CMAKE_MATCH_1}")

string(REGEX MATCH "define[ \t]+PRESSIOLOG_MINOR_VERSION[ \t]+([0-9]+)" _minor_version_match "${_pressiolog_version}")
set(PRESSIOLOG_MINOR_VERSION "${CMAKE_MATCH_1}")

string(REGEX MATCH "define[ \t]+PRESSIOLOG_PATCH_VERSION[ \t]+([0-9]+)" _patch_version_match "${_pressiolog_version}")
set(PRESSIOLOG_PATCH_VERSION "${CMAKE_MATCH_1}")

set(PRESSIOLOG_VERSION_NUMBER ${PRESSIOLOG_MAJOR_VERSION}.${PRESSIOLOG_MINOR_VERSION}.${PRESSIOLOG_PATCH_VERSION})
message("${Magenta}>> PRESSIO-OPS: version = ${PRESSIOLOG_VERSION_NUMBER} ${ColourReset}")

# set logging macros
#=====================================================================
option(PRESSIO_ENABLE_LOGGING "Enable or disable logging with pressio-log" ON)
if (PRESSIO_ENABLE_LOGGING)
  add_compile_definitions(PRESSIOLOG_ENABLED=1)
else ()
  add_compile_definitions(PRESSIOLOG_ENABLED=0)
endif()

option(PRESSIO_SILENCE_WARNINGS "Enable or disable warnings" OFF)
if (PRESSIO_SILENCE_WARNINGS)
  add_compile_definitions(PRESSIOLOG_SILENCE_WARNINGS=1)
else ()
  add_compile_definitions(PRESSIOLOG_SILENCE_WARNINGS=0)
endif()

option(PRESSIO_ENABLE_COLORIZED_OUTPUT "Enable or disable colorized logging" OFF)
if (PRESSIO_ENABLE_COLORIZED_OUTPUT)
  add_compile_definitions(PRESSIOLOG_ENABLE_COLORIZED_OUTPUT=1)
else ()
  add_compile_definitions(PRESSIOLOG_ENABLE_COLORIZED_OUTPUT=0)
endif()

# install headers
#=====================================================================
include(GNUInstallDirs)

install(
  DIRECTORY include/pressio-log
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# tests
#=====================================================================
option(PRESSIOLOG_ENABLE_TESTS "Enable testing for pressio-log library" OFF)
if(PRESSIOLOG_ENABLE_TESTS)
  add_library(pressio-log INTERFACE)

  target_include_directories(
      pressio-log INTERFACE
      "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  )

  add_subdirectory(tests)
endif()
