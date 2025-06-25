import ctypes

lib = ctypes.CDLL("./libcutlinear-optimizer.so")

lib.start_optimize.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
lib.start_optimize.restype = ctypes.c_int

lib.start_optimize(b"input.txt", b"output.txt")
