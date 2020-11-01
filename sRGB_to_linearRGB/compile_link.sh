g++ sRGB_to_linearRGB_jit.cpp \
	-std=c++11 \
	-L /usr/local/lib/ \
	-I /Users/vincent/Lib/Halide/tools/ \
	-lHalide -ljpeg -lpng \
	-o sRGB_to_linearRGB_jit