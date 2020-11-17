g++ median_generator.cpp /Users/vincent/Lib/Halide/tools/GenGen.cpp -std=c++11 -fno-rtti -L /usr/local/lib/ -I /Users/vincent/Lib/Halide/tools/ -lHalide -ljpeg -lpng -o median_generator
./median_generator \
	-g median_gen \
	-f median_auto_schedule \
	-e static_library,c_header,schedule \
	-o . \
	-s Adams2019 \
	-p /Users/vincent/Lib/Halide/build/src/autoschedulers/adams2019/libautoschedule_adams2019.so \
	auto_schedule=true target=host

./median_generator \
	-g median_gen \
	-f median_manual_schedule \
	-e static_library,c_header,schedule \
	-o . \
	auto_schedule=false target=host