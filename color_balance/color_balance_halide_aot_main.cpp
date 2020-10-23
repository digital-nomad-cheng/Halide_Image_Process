#include <iostream>
#include <cmath>

#include <HalideBuffer.h>
#include <halide_image_io.h>


#include "color_balance.h"


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
	Halide::Runtime::Buffer<float> input = Halide::Tools::load_and_convert_image("./resources/color_balance.jpg");
	Halide::Runtime::Buffer<float> output(input.width(), input.height(), 3);
	int error = color_balance(input, output);
	if (!error) {
		Halide::Runtime::Buffer<float> norm_output = normalize_image(output);
		Halide::Tools::convert_and_save_image(norm_output, "./results/output_aot.jpg");
	} else {
		throw std::runtime_error("Some error happened.");
	}
	
	
}

