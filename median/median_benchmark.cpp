#include <iostream>
#include <cmath>

#include <HalideBuffer.h>
#include <halide_benchmark.h>
#include <halide_image_io.h>
#include <opencv2/opencv.hpp>

#include "median_auto_schedule.h"
#include "median_manual_schedule.h"

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

// Auto schedule: 2.34828ms
// Manual schedule: 5.50105ms
// Opencv: 2.22835ms

int main(int argc, char ** argv)
{

	std::string image_path = "/Users/vincent/Documents/Repo/Halide_Image_Process/images/rgb.png";

	Halide::Runtime::Buffer<uint8_t> input = Halide::Tools::load_image(image_path);
	Halide::Runtime::Buffer<uint8_t> output(input.width(), input.height(), 3);

	double auto_schedule_on = Halide::Tools::benchmark(2, 200, [&]() {
        median_auto_schedule(input, output);
    });
    output.device_sync();
    printf("Auto schedule: %gms\n", auto_schedule_on * 1e3);
    // Halide::Runtime::Buffer<float> norm_output = normalize_image(output);
	Halide::Tools::save_image(output, "./results/output_auto_schedule.jpg");

    double auto_schedule_off = Halide::Tools::benchmark(2, 200, [&]() {
        median_manual_schedule(input, output);
    });
    printf("Manual schedule: %gms\n", auto_schedule_off * 1e3);
    // norm_output = normalize_image(output);
	Halide::Tools::convert_and_save_image(output, "./results/output_manual_schedule.jpg");

	
	cv::Mat cv_input = cv::imread(image_path, 1);
	cv::Mat cv_output;
    double opencv = Halide::Tools::benchmark(2, 200, [&]() {
        cv::medianBlur(cv_input, cv_output, 3);
    });
    printf("Opencv: %gms\n", opencv * 1e3);
    cv::imwrite("./results/output_opencv.jpg", cv_output);

	// if (!error) {
	// 	Halide::Runtime::Buffer<float> norm_output = normalize_image(output);
	// 	Halide::Tools::convert_and_save_image(norm_output, "./results/output_aot.jpg");
	// } else {
	// 	throw std::runtime_error("Some error happened.");
	// }	
}