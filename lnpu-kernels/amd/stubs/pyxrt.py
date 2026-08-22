"""Compile-only stand-in for pyxrt.

IRON imports pyxrt at module scope -- iron/common/utils.py reaches through
aie.utils.hostruntime.xrtruntime.tensor -- and a couple of its attributes are read as default
arguments, so the import fails on a machine with no XRT even when nothing is going to touch a
device. Everything here exists to satisfy that import.

Every call raises. A device call that quietly did nothing would be far worse than one that stops.
"""


def _no_device(*args, **kwargs):
    raise RuntimeError(
        "pyxrt is stubbed out: this environment bakes kernels and has no XRT. "
        "Run on a machine with an NPU to execute them."
    )


class bo:
    host_only = 1
    cacheable = 2
    device_only = 3
    normal = 0

    def __init__(self, *args, **kwargs):
        _no_device()


class device:
    def __init__(self, *args, **kwargs):
        _no_device()


class hw_context:
    def __init__(self, *args, **kwargs):
        _no_device()


class kernel:
    def __init__(self, *args, **kwargs):
        _no_device()


class xclbin:
    def __init__(self, *args, **kwargs):
        _no_device()


class xclBOSyncDirection:
    XCL_BO_SYNC_BO_TO_DEVICE = 0
    XCL_BO_SYNC_BO_FROM_DEVICE = 1


class ert_cmd_state:
    ERT_CMD_STATE_COMPLETED = 4
