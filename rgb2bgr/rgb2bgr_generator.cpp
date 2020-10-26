#include <Halide.h>

class RGB2BGR : public Halide::Generator<RBG2BGR>
{
	Halide::Input<Halide::Buffer<float>> input{"input", 3};
	Halide::Output<Halide::Buffer<float>> output{"output", 3};

	void generate() {
		output(x, y, c) = Halide::select(c == 0, input(x, y, 2), c == 1, input(x, y, 1), input(x, y, 2));

	}

	void schedule() {
		if (auto_schedule) {
			input.set_estimates({{0, 2048}, {0, 2048}, {0, 3}});
			output.set_estimates({{0, 2048}, {0, 2048}, {0, 3}});
		}
	}
};

HALIDE_REGISTER_GENERATOR(RGB2BGR, rgb2bgr_gen)
