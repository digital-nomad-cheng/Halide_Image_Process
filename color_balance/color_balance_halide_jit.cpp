#include <iostream>

#include <Halide.h>
#include <halide_image_io.h>
#include <halide_image.h>

int main(int argc, char ** argv)
{
	Halide::Buffer<float> input = Halide::Tools::load_and_convert_image("resources/color_balance.jpg");

	Halide::Var x("x");
	Halide::Var y("y");
	Halide::Var c("c");

	// 1. calculate histogram
	// Todo: merge into one stage: RDom(0, input.width(), 0, input.height());
	Halide::Func hist_rows("hist_rows");
	hist_rows(x, y, c) = 0;
	Halide::RDom rx(0, input.width());
	Halide::Expr bin = Halide::cast<int>(Halide::clamp(input(rx, y, c), 0, 255));
	hist_rows(bin, y, c) += 1;

	Halide::Func hist("hist");
	hist(x, c) = 0;
	Halide::RDom ry(0, input.height());
	hist(x, c) += hist_rows(x, ry, c);


	// 2. calculate cumulative distribute function
	Halide::Func cdf("cdf");
	std::cout << "Good" << std::endl;
	cdf(x, c) = 0;
	Halide::RDom rhist(0, 255);
	cdf(rhist+1, c) = cdf(rhist, c) + hist(rhist, c); 


	// 3. calculate min, max threshold
	const double s = 0.0265; // the parameter to adjust，it should be better below 0.1
	const int num_pixels = input.width() * input.height();




	int min_index = 0;
    
	while (hist.at<float>(min_index, 0) < num_pixels * s/2) {
		min_index += 1;
	}

	int max_index = 255;
    
	while (hist.at<float>(max_index, 0) > num_pixels * (1 - s/2)) {
		max_index -= 1;
	}
    
	// min_index and max_index represents the over dark and over light pixels in img
	for (size_t x = 0; x < img.rows; x++) {
		uchar *ptr = img.ptr(x);
		for (size_t y = 0; y < img.cols; y++) {
			if (*ptr > max_index)
				*ptr = max_index;
			if (*ptr < min_index)
				*ptr = min_index;
            ptr += 1;
		}
	}



	hist_rows.compute_root();
	hist.compute_root();

	cdf.realize(input.width(), input.channels());

	// Halide::Tools::save_image(output, "")
	return 0;

}