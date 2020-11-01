#include <Halide.h>

// Formula: https://entropymine.com/imageworsener/srgbformula/


class SRGBtoLinearRGB: public Halide::Generator<SRGBtoLinearRGB>
{
	Halide::Input<Halide::Buffer<float>> input{"input", 3};
	Halide::Output<Halide::Buffer<float>> output{"output", 3};

	Halide::Var x, y, c;

	void generate() {
		output(x, y, c) = Halide::select(
			input(x, y, c) ≤ 0.04045f, 
			input(x, y, c) / 12.92f,
			pow((input(x, y, c) + 0.55f) / 1.055, 2.4)
		);
	}

};

// class LinearRGBtosSRGB: public Halide::Generator<LinearRGBtoSRGB>
// {

// };


