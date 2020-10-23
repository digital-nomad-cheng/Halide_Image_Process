#include <iostream>

#include <Halide.h>
#include <halide_image_io.h>
#include <halide_image.h>

#define __debug_process__ 1


Halide::Tools::Image<float> normalize_image(const Halide::Tools::Image<float> &input) {
    int w = input.width();
    int h = input.height();
    int c = input.channels();

    Halide::Tools::Image<float> output(w, h, c);
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

Halide::Tools::Image<uint8_t> normalize_image(const Halide::Tools::Image<uint8_t> &input) {
    int w = input.width();
    int h = input.height();
    int c = input.channels();

    Halide::Tools::Image<uint8_t> output(w, h, c);
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
        float value = input(x,y,z)- mini;
        output(x,y,z) = (uint8_t)(value*normalizer);
    }

    return output;
}

int main(int argc, char ** argv)
{
	Halide::Buffer<uint8_t> input = Halide::Tools::load_image("resources/color_balance.jpg");

#if __debug_process__
	for (int h = 0; h < input.height(); h++) {
		for (int w = 0; w < input.width(); w++) {
			std::cout << int(input(w, h, 0)) <<  " " << int(input(w, h, 1)) << " " << int(input(w, h, 2)) << std::endl;
		}
	}
#endif
	Halide::Var x("x");
	Halide::Var y("y");
	Halide::Var c("c");

	// 1. calculate histogram
	Halide::Func hist("hist");
	hist(x, c) = 0.0f;
	Halide::RDom r(0, input.width(), 0, input.height());
	Halide::Expr value = Halide::clamp(input(r.x, r.y, c), 0, 255);
	hist(value, c) += 1.0f;

#if __debug_process__
	hist.compute_root();
	Halide::Buffer<float> hist_buffer = hist.realize(255, input.channels());
	for (int i = 0; i < hist_buffer.width(); i++) {
		std::cout << "i " << i << " " << hist_buffer(i, 0) << std::endl;
	}
	Halide::Buffer<float> norm_hist_buffer = normalize_image(hist_buffer);
	Halide::Tools::convert_and_save_image(hist_buffer, "hist_buffer.jpg");
#endif


	// 2. calculate cumulative distribute function
	Halide::Func cdf("cdf");
	cdf(x, c) = 0.0f;
	Halide::RDom rcdf(0, 255);
	cdf(rcdf.x+1, c) = cdf(rcdf.x, c) + hist(rcdf.x, c); 

#if __debug_process__
	cdf.compute_root();
	Halide::Buffer<float> cdf_buffer = cdf.realize(255, input.channels());
	for (int i = 0; i < cdf_buffer.width(); i++) {
		std::cout << "i " << i << " " << cdf_buffer(i, 0) << std::endl;
	}
	Halide::Buffer<float> norm_cdf_buffer = normalize_image(cdf_buffer);

	Halide::Tools::convert_and_save_image(norm_cdf_buffer, "cdf_buffer.jpg");
#endif

	// 3. calculate min, max threshold
	const double s = 0.0265; // the parameter to adjust，it should be better below 0.1
	const int num_pixels = input.width() * input.height();
	const float min_threshold = num_pixels * s / 2;
	const float max_threshold = num_pixels * (1.0 - s / 2);

	Halide::Func min_count("min_count");
	Halide::Func max_count("max_count");
	Halide::Func min_sum("min_sum");
	Halide::Func max_sum("max_sum");

	min_count(x, c) = 0.0f;
	min_count(x, c) = Halide::select(cdf(x, c) < min_threshold, 1.0f, 0.0f);

	max_count(x, c) = 0.0f;
	max_count(x, c) = Halide::select(cdf(x, c) < max_threshold, 1.0f, 0.0f);

	min_sum(c) = 0.0f;	
	min_sum(c) = Halide::sum(min_count(rcdf.x, c));
	max_sum(c) = 0.0f;
	max_sum(c) = Halide::sum(max_count(rcdf.x, c));

#if __debug_process__
	min_count.compute_root();
	Halide::Buffer<float> min_buffer = min_count.realize(255, input.channels());
	Halide::Buffer<float> norm_min_buffer = normalize_image(min_buffer);
	Halide::Tools::convert_and_save_image(norm_min_buffer, "min_buffer.jpg");

	max_count.compute_root();
	Halide::Buffer<float> max_buffer = max_count.realize(255, input.channels());
	Halide::Buffer<float> norm_max_buffer = normalize_image(max_buffer);
	Halide::Tools::convert_and_save_image(norm_max_buffer, "max_buffer.jpg");

	min_sum.compute_root();
	Halide::Buffer<float> min_sum_buffer = min_sum.realize(input.channels());
	for (int i = 0; i < input.channels(); i++) {
		std::cout << "min sum " << min_sum_buffer(i) << std::endl;
	}
	max_sum.compute_root();
	Halide::Buffer<float> max_sum_buffer = max_sum.realize(input.channels());
	for (int i = 0; i < input.channels(); i++) {
		std::cout << "max sum " << max_sum_buffer(i) << std::endl;
	}
#endif

	// 4. adjust the ove dark and over light pixels
	Halide::Func color_balance("color_balance");
	color_balance(x, y, c) = 0;
	color_balance(x, y, c) = Halide::cast<int>(Halide::clamp(Halide::cast<int>(input(x, y, c)), Halide::cast<int>(min_sum(c)), Halide::cast<int>(max_sum(c))));
	

	// int min_index = 0;
    
	// while (hist.at<float>(min_index, 0) < num_pixels * s/2) {
	// 	min_index += 1;
	// }

	// int max_index = 255;
    
	// while (hist.at<float>(max_index, 0) > num_pixels * (1 - s/2)) {
	// 	max_index -= 1;
	// }
    
	// // min_index and max_index represents the over dark and over light pixels in img
	// for (size_t x = 0; x < img.rows; x++) {
	// 	uchar *ptr = img.ptr(x);
	// 	for (size_t y = 0; y < img.cols; y++) {
	// 		if (*ptr > max_index)
	// 			*ptr = max_index;
	// 		if (*ptr < min_index)
	// 			*ptr = min_index;
 	//       ptr += 1;
	// 	}
	// }


	hist.compute_root();
	cdf.compute_root();
	min_count.compute_root();
	max_count.compute_root();
	min_sum.compute_root();
	max_sum.compute_root();

	Halide::Func color_balance_output("output");
	color_balance_output(x, y, c) = Halide::cast<uint8_t>(color_balance(x, y, c)); 

	Halide::Buffer<uint8_t> output =  color_balance_output.realize(input.width(), input.height(), input.channels());
	Halide::Buffer<uint8_t> norm_output = normalize_image(output);
	Halide::Tools::save_image(norm_output, "ouput.jpg");
	return 0;

}