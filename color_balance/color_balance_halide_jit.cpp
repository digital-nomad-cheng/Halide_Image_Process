#include <iostream>

#include <Halide.h>
#include <halide_image_io.h>
#include <halide_image.h>

int main(int argc, char ** argv)
{
	Halide::Buffer<float> input = Halide::Tools::load_and_convert_image("./color_balance.jpg");

	Halide::Var x("x");
	Halide::Var y("y");
	Halide::Var c("c");

	// 1. calculate histogram
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
	cdf(rhist+1, c) = cdf(rhist, c) + hist(x, c); 


	hist_rows.compute_root();
	hist.compute_root();

	// hist_sum.realize(input.width(), input.channels());

	// Halide::Tools::save_image(output, "")
	return 0;

}