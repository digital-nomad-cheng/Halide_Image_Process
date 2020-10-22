# opencv
# g++ -std=c++11 color_balance_opencv.cpp -L /usr/local/lib/ -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -o color_balance_opencv

# halide
g++ -std=c++11 color_balance_halide_jit.cpp -L /usr/local/lib/ -I /Users/vincent/Libs/Halide/tools -lHalide -ljpeg -lpng -o color_balance_halide_jit
