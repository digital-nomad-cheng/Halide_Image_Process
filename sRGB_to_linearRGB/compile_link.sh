g++ sRGB_to_linearRGB_main.cpp \
	-std=c++11 \
	-L /usr/local/lib/ \
	-I /Users/vincent/Lib/Halide/tools/ \
	-lHalide -ljpeg -lpng \
	-o sRGB_to_linearRGB_main