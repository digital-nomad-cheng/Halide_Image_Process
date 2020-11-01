#include <Halide.h>
#include <halide_image_io.h>

int main(int argc, char ** argv) 
{
	Halide::Buffer<float> input = Halide::Tools::load_and_convert_image("../images/sRGB.png");
	Halide::Buffer<float> output(input.width(), input.height(), input.channels());

	Halide::Func SRGBtoLinearRGB;
	Halide::Var x, y, c;
	Halide::Expr s_to_linear = Halide::select(
		input(x, y, c) <= 0.04045f, 
		input(x, y, c) / 12.92f,
		pow((input(x, y, c) + 0.055f) / 1.055f, 2.4f)
	);
	SRGBtoLinearRGB(x, y, c) = Halide::clamp(s_to_linear, 0.0f, 1.0f);
	SRGBtoLinearRGB.realize(output);

	Halide::Tools::convert_and_save_image(output, "results/sRGB_to_linearRGB.png");

	input = Halide::Tools::load_and_convert_image("../images/linearRGB.png");
	Halide::Func LinearRGBtoSRGB;
	Halide::Expr linear_to_s = Halide::select(
		input(x, y, c) <= 0.0031308f, 
		input(x, y, c) * 12.92f,
		pow(input(x, y, c), 1/2.4f)*1.055f - 0.055f
	);
	LinearRGBtoSRGB(x, y, c) = Halide::clamp(linear_to_s, 0.0f, 1.0f);
	LinearRGBtoSRGB.realize(output);

	Halide::Tools::convert_and_save_image(output, "results/linearRGB_to_sRGB.png");


}