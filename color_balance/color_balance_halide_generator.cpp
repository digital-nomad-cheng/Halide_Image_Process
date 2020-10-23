#include <Halide.h>

class ColorBalance : public Halide::Generator<ColorBalance>
{
	// declare input and output buffer as member variables
	Halide::Input<Halide::Buffer<float>> input{"input", 3};
	Halide::Output<Halide::Buffer<float>> output{"output", 3};

	Halide::Var x("x");
	Halide::Var y("y");
	Halide::Var c("c");

	void generate() {
		// 1. calculate histogram
		Halide::Func hist("hist");
		hist(x, c) = 0.0f;
		Halide::RDom r(0, input.width(), 0, input.height());
		Halide::Expr value = Halide::cast<uint8_t>(Halide::clamp(input(r.x, r.y, c)*255.0f, 0.0f, 255.0f));
		hist(value, c) += 1.0f;

		// 2. calculate cumulative distribute function
		Halide::Func cdf("cdf");
		cdf(x, c) = 0.0f;
		Halide::RDom rcdf(0, 256);
		cdf(rcdf.x+1, c) = cdf(rcdf.x, c) + hist(rcdf.x, c); 

		// 3. calculate min, max threshold
		const float s = 0.0265; // the parameter to adjust，it should be better below 0.1
		Halide::Expr num_pixels = input.width() * input.height();
		Halide::Expr min_threshold = num_pixels * s / 2;
		Halide::Expr max_threshold = num_pixels * (1.0f - s / 2);

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

		// 4. adjust the ove dark and over light pixels
		Halide::Func color_balance("color_balance");
		color_balance(x, y, c) = 0.0f;
		color_balance(x, y, c) = Halide::clamp(input(x, y, c), min_sum(c)/255.0f, max_sum(c)/255.0f);
	}

	void schedule() {
		
	}
};
