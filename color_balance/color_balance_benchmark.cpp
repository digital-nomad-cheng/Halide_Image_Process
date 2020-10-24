#include <iostream>
#include <cmath>

#include <HalideBuffer.h>
#include <halide_benchmark.h>
#include <halide_image_io.h>
#include <opencv2/opencv.hpp>

#include "color_balance_auto_schedule.h"
#include "color_balance_manual_schedule.h"
#include "color_balance_opencv.hpp"

Halide::Runtime::Buffer<float> normalize_image(const Halide::Runtime::Buffer<float> &input) {
    int w = input.width();
    int h = input.height();
    int c = input.channels();

    Halide::Runtime::Buffer<float> output(w, h, c);
    // compute min/max
    float mini = input(0,0,0);
    float maxi = input(0,0,0);
    for (int z = 0; z < c; ++z) 
    for (int y = 0; y < h; ++y) 
    for (int x = 0; x < w; ++x) 
    {
        mini = fmin(mini, input(x,y,z));
        maxi = fmax(maxi, input(x,y,z));
    }

    float normalizer = 1.0/(maxi-mini + 1e-10);

    for (int z = 0; z < c; ++z) 
    for (int y = 0; y < h; ++y) 
    for (int x = 0; x < w; ++x) 
    {
        output(x,y,z) = input(x,y,z)- mini;
        output(x,y,z) *= normalizer;
    }

    return output;
}

int main(int argc, char ** argv)
{

	Halide::Runtime::Buffer<float> input = Halide::Tools::load_and_convert_image("./resources/color_balance_large.jpg");
	Halide::Runtime::Buffer<float> output(input.width(), input.height(), 3);

	double auto_schedule_on = Halide::Tools::benchmark(2, 200, [&]() {
        color_balance_auto_schedule(input, output);
    });
    printf("Auto schedule: %gms\n", auto_schedule_on * 1e3);
    Halide::Runtime::Buffer<float> norm_output = normalize_image(output);
	Halide::Tools::convert_and_save_image(norm_output, "./results/output_auto_schedule.jpg");

    double auto_schedule_off = Halide::Tools::benchmark(2, 200, [&]() {
        color_balance_manual_schedule(input, output);
    });
    printf("Manual schedule: %gms\n", auto_schedule_off * 1e3);
    norm_output = normalize_image(output);
	Halide::Tools::convert_and_save_image(norm_output, "./results/output_manual_schedule.jpg");

	std::string image_path = "/Users/vincent/Documents/Repo/Halide_Image_Process/color_balance/resources/color_balance_large.jpg";
	cv::Mat cv_input = cv::imread(image_path, 1);
	cv::Mat cv_output;
    double opencv = Halide::Tools::benchmark(2, 200, [&]() {
        color_balance_opencv(cv_input, cv_output);
    });
    printf("Opencv: %gms\n", opencv * 1e3);

	// if (!error) {
	// 	Halide::Runtime::Buffer<float> norm_output = normalize_image(output);
	// 	Halide::Tools::convert_and_save_image(norm_output, "./results/output_aot.jpg");
	// } else {
	// 	throw std::runtime_error("Some error happened.");
	// }	
}