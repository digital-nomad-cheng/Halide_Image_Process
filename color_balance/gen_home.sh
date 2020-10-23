g++ color_balance_halide_generator.cpp /Users/vincent/Lib/Halide/tools/GenGen.cpp -std=c++11 -fno-rtti -L /usr/local/lib/ -I /Users/vincent/Lib/Halide/tools/ -lHalide -ljpeg -lpng -o color_balance_halide_generator
./color_balance_halide_generator \
	-g color_balance_gen \
	-f auto_schedule_true \
	-e static_library,c_header,schedule \
	-o . \
	-s Adams2019 \
	-p /Users/vincent/Lib/Halide/build/src/autoschedulers/adams2019/libautoschedule_adams2019.so \
	auto_schedule=true target=host