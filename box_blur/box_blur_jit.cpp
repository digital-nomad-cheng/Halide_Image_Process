#include <Halide.h>
#include <halide_image_io.h>

Halide::Func box3x3_float(Halide::Buffer<float> &input, int schedule)
{
	// box blur float version
	Halide::Func clamped("clamped");
	Halide::Func blur_x("blur_x");
	Halide::Func blur_y("blur_y");
	Halide::Func blur("blur");
    Halide::Var x, y, c;

    blur_x(x, y, c) = 0.0f;
    blur_y(x, y, c) = 0.0f;
	clamped = Halide::BoundaryConditions::repeat_edge(input);
	blur_x(x, y, c) = Halide::cast<float>(clamped(x-1, y, c)+clamped(x, y, c)+clamped(x+1, y, c)) / 3.0f;
	blur_y(x, y, c) = (blur_x(x, y-1, c)+blur_x(x, y, c)+blur_x(x, y+1, c)) / 3.0f;
	blur(x, y, c) = Halide::clamp(blur_y(x, y, c), 0.0f, 255.0f);

	if (schedule == 0) {
		// No need, it's implicit
		blur_x.compute_inline();
		blur_y.compute_inline();
	} else if (schedule == 1) {
		// Compute root
		blur_x.compute_root();
		blur_y.compute_root();
	} else {
		// high level shcedule
        blur_x.compute_at(blur_y, x);
	}
	
	return blur;
}

Halide::Func box3x3_uint8(Halide::Buffer<uint8_t> &input, int schedule)
{
	// box blur float version
	// Halide::Func clamped("clamped");
	// Halide::Func clamped_uint16("clamped_uint16");
	// Halide::Func blur_x("blur_x");
	// Halide::Func blur_y("blur_y");
	// Halide::Func blur("blur");
 //    Halide::Var x, y, c;

    
	// clamped = Halide::BoundaryConditions::repeat_edge(input);
	// clamped_uint16(x, y, c) = Halide::cast<uint16_t>(clamped(x, y, c));
	// blur_x(x, y, c) = Halide::cast<float>(clamped_uint16(x-1, y, c)+clamped_uint16(x, y, c)+clamped_uint16(x+1, y, c)) / 3.0f;
	// blur_y(x, y, c) = (blur_x(x, y-1, c)+blur_x(x, y, c)+blur_x(x, y+1, c)) / 3.0f;
	// blur(x, y, c) = Halide::cast<uint8_t>(Halide::clamp(blur_y(x, y, c), 0.0f, 255.0f));


	Halide::Func input_("input_");
	Halide::Func clamped("clamped");
	Halide::Func blur_x("blur_x");
	Halide::Func blur_y("blur_y");
	Halide::Func blur("blur");
    Halide::Var x, y, c;

    input_(x, y, c) = Halide::cast<uint16_t>(input(x, y, c));
	clamped = Halide::BoundaryConditions::repeat_edge(input_, {{0, input.width()}, {0, input.height()}});
	blur_x(x, y, c) = Halide::cast<float>(clamped(x-1, y, c)+clamped(x, y, c)+clamped(x+1, y, c)) / 3.0f;
	blur_y(x, y, c) = (blur_x(x, y-1, c)+blur_x(x, y, c)+blur_x(x, y+1, c)) / 3.0f;
	blur(x, y, c) = Halide::cast<uint8_t>(Halide::clamp(blur_y(x, y, c), 0.0f, 255.0f));

	if (schedule == 0) {
		// No need, it's implicit
		blur_x.compute_inline();
		blur_y.compute_inline();
	} else if (schedule == 1) {
		// Compute root
		blur_x.compute_root();
		blur_y.compute_root();
	} else {
		// high level shcedule
        blur_x.compute_at(blur_y, x);
	}
	
	return blur;
}


int main(int argc, char **argv) 
{
    Halide::Buffer<float> input_float = Halide::Tools::load_and_convert_image("../images/Lenna.png");
    Halide::Buffer<float> output_float = box3x3_float(input_float, 0).realize(input_float.width(), input_float.height(), input_float.channels());
    Halide::Tools::convert_and_save_image(output_float, "results/Lenna_box_blur_float.png");

    Halide::Buffer<uint8_t> input_uint8 = Halide::Tools::load_image("../images/Lenna.png");
    Halide::Buffer<uint8_t> output_uint8 = box3x3_uint8(input_uint8, 0).realize(input_uint8.width(), input_uint8.height(), input_uint8.channels());
    Halide::Tools::save_image(output_uint8, "results/Lenna_box_blur_uint8.png");

	return 0;
}
