cmake_minimum_required(VERSION 3.12)
project(pressio-log CXX)

# set logging macros
#=====================================================================
set(PRESSIO_LOG_LEVEL "basic" CACHE STRING "Set the logging level (None, Basic, Info, Debug)")
set_property(CACHE PRESSIO_LOG_LEVEL PROPERTY STRINGS "None" "Basic" "Info" "Debug")

set(PRESSIO_LOG_LEVEL_MAP_NONE 0)
set(PRESSIO_LOG_LEVEL_MAP_BASIC 1)
set(PRESSIO_LOG_LEVEL_MAP_INFO 2)
set(PRESSIO_LOG_LEVEL_MAP_DEBUG 3)

string(TOLOWER "${PRESSIO_LOG_LEVEL}" PRESSIO_LOG_LEVEL)

if(PRESSIO_LOG_LEVEL STREQUAL "none")
    set(PRESSIO_LOG_LEVEL ${PRESSIO_LOG_LEVEL_MAP_NONE})
elseif(PRESSIO_LOG_LEVEL STREQUAL "basic")
    set(PRESSIO_LOG_LEVEL ${PRESSIO_LOG_LEVEL_MAP_BASIC})
elseif(PRESSIO_LOG_LEVEL STREQUAL "info")
    set(PRESSIO_LOG_LEVEL ${PRESSIO_LOG_LEVEL_MAP_INFO})
elseif(PRESSIO_LOG_LEVEL STREQUAL "debug")
    set(PRESSIO_LOG_LEVEL ${PRESSIO_LOG_LEVEL_MAP_DEBUG})
elseif(PRESSIO_LOG_LEVEL MATCHES "^[0-9]+$")
    # Any number is okay
else()
    message(FATAL_ERROR "Invalid PRESSIO_LOG_LEVEL: '${PRESSIO_LOG_LEVEL}'. Use a number or the corresponding string (0=none, 1=basic, 2=info, 3=debug).")
endif()

add_compile_definitions(PRESSIOLOG_LOG_LEVEL=${PRESSIO_LOG_LEVEL})

#########################

set(PRESSIO_LOG_OUTPUT "console" CACHE STRING "Set the logging output (console, file, both)")
set_property(CACHE PRESSIO_LOG_OUTPUT PROPERTY STRINGS "console" "file" "both")

set(PRESSIO_LOG_OUTPUT_MAP_CONSOLE 0)
set(PRESSIO_LOG_OUTPUT_MAP_FILE 1)
set(PRESSIO_LOG_OUTPUT_MAP_BOTH 2)

string(TOLOWER "${PRESSIO_LOG_OUTPUT}" PRESSIO_LOG_OUTPUT)

if(PRESSIO_LOG_OUTPUT STREQUAL "console")
    set(PRESSIO_LOG_OUTPUT ${PRESSIO_LOG_OUTPUT_MAP_CONSOLE})
elseif(PRESSIO_LOG_OUTPUT STREQUAL "file")
    set(PRESSIO_LOG_OUTPUT ${PRESSIO_LOG_OUTPUT_MAP_FILE})
elseif(PRESSIO_LOG_OUTPUT STREQUAL "both")
    set(PRESSIO_LOG_OUTPUT ${PRESSIO_LOG_OUTPUT_MAP_BOTH})
else()
    message(FATAL_ERROR "Invalid PRESSIO_LOG_OUTPUT: '${PRESSIO_LOG_OUTPUT}'. Use 'console', 'file', or 'both'")
endif()

add_compile_definitions(PRESSIOLOG_OUTPUT=${PRESSIO_LOG_OUTPUT})

option(PRESSIO_SILENCE_WARNINGS "Enable or disable warnings" OFF)
if (PRESSIO_SILENCE_WARNINGS)
  add_compile_definitions(PRESSIOLOG_SILENCE_WARNINGS=1)
else ()
  add_compile_definitions(PRESSIOLOG_SILENCE_WARNINGS=0)
endif()

# install headers
#=====================================================================
include(GNUInstallDirs)

install(
  DIRECTORY include/pressio-log
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# tests
#=====================================================================
# if(PRESSIO_ENABLE_TESTS)
add_library(pressio-log INTERFACE)

target_include_directories(
    pressio-log INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

add_subdirectory(tests)
# endif()
