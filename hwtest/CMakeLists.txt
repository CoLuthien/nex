# Standalone: builds only the instruction emitter plus this test, so it can be carried to a
# machine with an NPU without bringing protobuf, onnx or the rest of the tree along.
#
# One case file per operator. Adding an operator is adding a file here and a branch in main.cpp;
# nothing else in the test knows how many there are.
cmake_minimum_required(VERSION 3.22)
project(nex-hwtest LANGUAGES CXX)

include(../cmake/sdk/FindXRT.cmake)
include(../cmake/sdk/FindAIEBU.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# XRT: /opt/xilinx/xrt on Linux, the Ryzen AI / NPU driver SDK on Windows.
# Point at it with -DXRT_DIR=<prefix> when it is somewhere else.
if(NOT DEFINED XRT_DIR)
    if(WIN32)
        set(XRT_DIR "$ENV{XILINX_XRT}")
    else()
        set(XRT_DIR "/opt/xilinx/xrt")
    endif()
endif()

message(STATUS "XRT: ${XRT_DIR}")

set(NEX_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../lnpu-nex/src")
set(AMD_SRC "${NEX_SRC}/backend/amd")

add_executable(nex-hwtest
    main.cpp
    support.cpp
    case-rmsnorm.cpp
    case-gemm.cpp

    ${AMD_SRC}/command.cpp
    ${AMD_SRC}/descriptor.cpp
    ${AMD_SRC}/commands/block-write.cpp
    ${AMD_SRC}/commands/ddr-patch.cpp
    ${AMD_SRC}/commands/issue-token.cpp
    ${AMD_SRC}/commands/queue-push.cpp
    ${AMD_SRC}/commands/register-write.cpp
    ${AMD_SRC}/commands/tct-wait.cpp
    ${AMD_SRC}/programs/gemm.cpp
    ${AMD_SRC}/programs/rmsnorm.cpp

    ${AMD_SRC}/device.cpp
    ${AMD_SRC}/operation.cpp
    ${AMD_SRC}/operation-registry.cpp
    ${AMD_SRC}/program-registry.cpp
)

# A program is lowered from a layer_description, which lives in the runtime's public include tree
# and is annotated with the export macro the main build generates. Nothing here builds a shared
# library, so the macro is stubbed rather than bringing the generator -- and the rest of that
# build -- along. Every header this test reaches for through it is interface only.
set(NEX_API_STUB "${CMAKE_CURRENT_BINARY_DIR}/stub")
file(WRITE "${NEX_API_STUB}/nex-api.hpp" "#pragma once\n\n// Written by hwtest/CMakeLists.txt.\n#define NEX_EXPORT\n")
file(WRITE "${NEX_API_STUB}/cmn-api.hpp" "#pragma once\n\n// Written by hwtest/CMakeLists.txt.\n#define CMN_EXPORT\n")

target_include_directories(nex-hwtest PRIVATE
    ${NEX_SRC}
    ${NEX_SRC}/backend                                 # 즉 "amd/command.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../lnpu-nex/include"  # layer_description, 즉 무엇을 내릴지
    ${NEX_API_STUB}
    "${CMAKE_CURRENT_SOURCE_DIR}/../lnpu-cmn/include"  # operation-registry 가 cmn/containers 를 쓴다
    "${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty"        # 그리고 spdlog
    "${XRT_DIR}/include")


target_link_libraries(nex-hwtest PRIVATE xrt::xrt aiebu::aiebu)

if(MSVC)
    target_compile_options(nex-hwtest PRIVATE /Zc:preprocessor /utf-8 /EHsc /MT)
endif()
