g++ color_balance_halide_generator.cpp /Users/vincent/Lib/Halide/tools/GenGen.cpp -std=c++11 -fno-rtti -L /usr/local/lib/ -I /Users/vincent/Lib/Halide/tools/ -lHalide -ljpeg -lpng -o color_balance_halide_generator
./color_balance_halide_generator \
	-g color_balance_gen \
	-f color_balance_auto_schedule \
	-e static_library,c_header,schedule \
	-o . \
	-s Adams2019 \
	-p /Users/vincent/Lib/Halide/build/src/autoschedulers/adams2019/libautoschedule_adams2019.so \
	auto_schedule=true target=host

./color_balance_halide_generator \
	-g color_balance_gen \
	-f color_balance_manual_schedule \
	-e static_library,c_header,schedule \
	-o . \
	auto_schedule=false target=host

g++ -std=c++11 \
	-c color_balance_opencv.cpp \
	-I /usr/local/include/opencv4 
ar rvs color_balance_opencv.a color_balance_opencv.o


g++ color_balance_benchmark.cpp color_balance_opencv.a color_balance_auto_schedule.a color_balance_manual_schedule.a \
	-std=c++11 \
	-L /usr/local/lib/ \
	-I /Users/vincent/Lib/Halide/tools/ \
	-I /usr/local/include/opencv4 \
	-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lHalide -ljpeg -lpng \
	-o color_balance_benchmark