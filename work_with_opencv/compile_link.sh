g++ work_with_opencv_main.cpp \
	-std=c++11 \
	-L /usr/local/lib/ \
	-I /Users/vincent/Lib/Halide/tools/ \
	-I /usr/local/include/opencv4 \
	-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lHalide -ljpeg -lpng \
	-o work_with_opencv_main