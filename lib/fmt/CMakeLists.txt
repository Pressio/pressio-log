project(FMT CXX)


function(add_headers VAR)
set(headers ${${VAR}})
foreach (header ${ARGN})
set(headers ${headers} include/fmt/${header})
endforeach()
set(${VAR} ${headers} PARENT_SCOPE)
endfunction()

# Define the fmt library with headers only
add_headers(FMT_HEADERS core.h format.h format-inl.h ostream.h)

add_library(fmt INTERFACE)  # Change to INTERFACE for header-only
add_library(fmt::fmt-header-only ALIAS fmt)  # Alias to fmt-header-only

target_include_directories(fmt SYSTEM INTERFACE
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

message(STATUS "Installing header-only fmt in")

install(
  DIRECTORY                include/fmt
  DESTINATION              include
  CONFIGURATIONS           ${build_type_list}
  FILES_MATCHING PATTERN   "*"
)
