#include <Halide.h>

class ColorBalance : public Halide::Generator<ColorBalance>
{
public:

	void generate() {
		// 1. calculate histogram
		hist(x, c) = 0.0f;
		Halide::RDom r(0, input.width(), 0, input.height());
		Halide::Expr value = Halide::cast<uint8_t>(Halide::clamp(input(r.x, r.y, c)*255.0f, 0.0f, 255.0f));
		hist(value, c) += 1.0f;

		// 2. calculate cumulative distribute function
		cdf(x, c) = 0.0f;
		Halide::RDom rcdf(0, 256);
		cdf(rcdf.x+1, c) = cdf(rcdf.x, c) + hist(rcdf.x, c); 

		// 3. calculate min, max threshold
		const float s = 0.0265; // the parameter to adjust，it should be better below 0.1
		Halide::Expr num_pixels = input.width() * input.height();
		Halide::Expr min_threshold = num_pixels * s / 2;
		Halide::Expr max_threshold = num_pixels * (1.0f - s / 2);

		min_count(x, c) = 0.0f;
		min_count(x, c) = Halide::select(cdf(x, c) < min_threshold, 1.0f, 0.0f);

		max_count(x, c) = 0.0f;
		max_count(x, c) = Halide::select(cdf(x, c) < max_threshold, 1.0f, 0.0f);

		min_sum(c) = 0.0f;	
		min_sum(c) = Halide::sum(min_count(rcdf.x, c));
		max_sum(c) = 0.0f;
		max_sum(c) = Halide::sum(max_count(rcdf.x, c));


		// 4. adjust the ove dark and over light pixels
		output(x, y, c) = 0.0f;
		output(x, y, c) = Halide::clamp(input(x, y, c), min_sum(c)/255.0f, max_sum(c)/255.0f);
	}

	void schedule() {
		if (auto_schedule) {
			input.set_estimates({{0, 2048}, {0, 2048}, {0, 3}});
			output.set_estimates({{0, 2048}, {0, 2048}, {0, 3}});
		} else {
			hist.compute_root();
			cdf.compute_root();
			min_count.compute_root();
			max_count.compute_root();
			min_sum.compute_root();
			max_sum.compute_root();
		}
	}
private:
	// declare input and output buffer as private member variables
	Input<Halide::Buffer<float>> input{"input", 3};
	Output<Halide::Buffer<float>> output{"output", 3};

	Halide::Var x{"x"};
	Halide::Var y{"y"};
	Halide::Var c{"c"};

	Halide::Func hist{"hist"};
	Halide::Func cdf{"cdf"};
	Halide::Func min_count{"min_count"};
	Halide::Func max_count{"max_count"};
	Halide::Func min_sum{"min_sum"};
	Halide::Func max_sum{"max_sum"};
	// Halide::Func color_balance{"color_balance"};
};

HALIDE_REGISTER_GENERATOR(ColorBalance, color_balance_gen)


