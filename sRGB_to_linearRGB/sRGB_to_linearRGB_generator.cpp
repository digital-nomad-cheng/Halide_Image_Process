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

	void schedule() {
		void schedule() {
			if (!auto_schedule) {
				output.vectorize(x, natural_vector_size<float>()).parallel(y);
			}
		}
	}

};

HALIDE_REGISTER_GENERATOR(SRGBtoLinearRGB, sRGB_to_linearRGB)


class LinearRGBtosSRGB: public Halide::Generator<LinearRGBtoSRGB>
{
	Halide::Input<Halide::Buffer<float>> input{"input", 3};
	Halide::Output<Halide::Buffer<float>> output{"output", 3};

	Halide::Var x, y, c;

	void generate() {
		output(x, y, c) = Halide::select(
			input(x, y, c) <= 0.0031308f, 
			input(x, y, c) * 12.92f,
			pow(input(x, y, c), 1/2.4f)*1.055f - 0.055f
		);
	}

	void schedule() {
		if (!auto_schedule) {
        	output.vectorize(x, natural_vector_size<float>()).parallel(y);
        }
	}
};

HALIDE_REGISTER_GENERATOR(LinearRGBtoSRGB, linearRGB_to_sRGB)


