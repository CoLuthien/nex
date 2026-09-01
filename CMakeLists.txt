
cmake_minimum_required(VERSION 3.16)

set(LNPU_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${LNPU_MODULE_PATH}/sdk")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


option(LNPU_WITH_AMD_NPU "With AMD NPU" OFF)

project(nex
    VERSION 0.0.0
    LANGUAGES CXX C ASM
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(NEX_THIRDPARTY_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

include(GenerateExportHeader)
include(FetchContent)


include(compile-options)

add_subdirectory(lnpu-cmn)
add_subdirectory(lnpu-nex)

add_subdirectory(samples)
