

# Deliberate exception to the one-plugin-per-backend layout that optai-air uses
# (optai_air_qnn, optai_air_onnx, ...): the AMD sources are folded into
# optai_nex to keep the shipped library count down.
#
# The cost is that liboptai_nex.so gains a DT_NEEDED on libxrt_coreutil and
# libaiebu whenever OPTAI_WITH_XRT is on, so a process that only parses onnx
# still needs both present. That is acceptable while a build targets a single
# vendor. Split this back into its own target once a build has to enable two
# vendor backends at once, since the merged library would then require every
# vendor SDK to be installed.

find_package(XRT REQUIRED)
find_package(AIEBU REQUIRED)


target_sources(lnpu_nex
PRIVATE
    operation.cpp
    operation-registry.cpp
    program-registry.cpp
    device.cpp
    command.cpp
    layer.cpp
    executor.cpp

    commands/block-write.cpp
    commands/ddr-patch.cpp
    commands/issue-token.cpp
    commands/queue-push.cpp
    commands/register-write.cpp
    commands/tct-wait.cpp

    descriptor.cpp

    programs/gemm.cpp
    programs/rmsnorm.cpp
)


# The SDKs stay PRIVATE: nothing under optai-nex/include exposes an XRT or
# AIEBU type, so consumers of this backend never need either SDK.
target_link_libraries(lnpu_nex
PRIVATE
    xrt::xrt
    aiebu::aiebu
)

