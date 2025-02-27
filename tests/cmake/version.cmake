# adapted from eigen
file(READ "${PROJECT_SOURCE_DIR}/include/pressio-log/version.hpp" _pressiolog_version)
string(REGEX MATCH "define[ \t]+PRESSIOLOG_MAJOR_VERSION[ \t]+([0-9]+)" _major_version_match "${_pressiolog_version}")
set(PRESSIOLOG_MAJOR_VERSION "${CMAKE_MATCH_1}")

string(REGEX MATCH "define[ \t]+PRESSIOLOG_MINOR_VERSION[ \t]+([0-9]+)" _minor_version_match "${_pressiolog_version}")
set(PRESSIOLOG_MINOR_VERSION "${CMAKE_MATCH_1}")

string(REGEX MATCH "define[ \t]+PRESSIOLOG_PATCH_VERSION[ \t]+([0-9]+)" _patch_version_match "${_pressiolog_version}")
set(PRESSIOLOG_PATCH_VERSION "${CMAKE_MATCH_1}")

set(PRESSIOLOG_VERSION_NUMBER ${PRESSIOLOG_MAJOR_VERSION}.${PRESSIOLOG_MINOR_VERSION}.${PRESSIOLOG_PATCH_VERSION})
message("${Magenta}>> PRESSIO-LOG: version = ${PRESSIOLOG_VERSION_NUMBER} ${ColourReset}")
