g++ median_benchmark.cpp median_auto_schedule.a median_manual_schedule.a \
	-std=c++11 \
	-L /usr/local/lib/ \
	-I /Users/vincent/Libs/Halide/tools/ \
	-I /usr/local/include/opencv4 \
	-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lHalide -ljpeg -lpng \
	-o median_benchmark
./median_benchmark