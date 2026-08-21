
cmake_minimum_required(VERSION 3.16)

set(NEX_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

add_subdirectory(nex-cmn)
#add_subdirectory(nex-runtime)
