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

add_executable(nex-hwtest
    main.cpp
    support.cpp
    case-rmsnorm.cpp
    case-gemm.cpp

    ${NEX_SRC}/amd/command.cpp
    ${NEX_SRC}/amd/descriptor.cpp
    ${NEX_SRC}/amd/commands/block-write.cpp
    ${NEX_SRC}/amd/commands/ddr-patch.cpp
    ${NEX_SRC}/amd/commands/issue-token.cpp
    ${NEX_SRC}/amd/commands/queue-push.cpp
    ${NEX_SRC}/amd/commands/register-write.cpp
    ${NEX_SRC}/amd/commands/tct-wait.cpp
    ${NEX_SRC}/amd/programs/gemm.cpp
    ${NEX_SRC}/amd/programs/rmsnorm.cpp

    ${NEX_SRC}/amd/device.cpp
    ${NEX_SRC}/amd/operation.cpp
    ${NEX_SRC}/amd/operation-registry.cpp
)

target_include_directories(nex-hwtest PRIVATE
    ${NEX_SRC}
    "${CMAKE_CURRENT_SOURCE_DIR}/../lnpu-cmn/include"  # operation-registry 가 cmn/containers 를 쓴다
    "${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty"        # 그리고 spdlog
    "${XRT_DIR}/include")


target_link_libraries(nex-hwtest PRIVATE xrt::xrt aiebu::aiebu)

if(MSVC)
    target_compile_options(nex-hwtest PRIVATE /Zc:preprocessor /utf-8 /EHsc /MT)
endif()
