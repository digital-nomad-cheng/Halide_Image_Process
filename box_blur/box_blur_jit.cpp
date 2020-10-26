#include <Halide.h>
#include <halide_image_io.h>

Halide::Func box3x3(Halide::Buffer<uint8_t> &input, int schedule)
{
	Halide::Func clamped("clamped");
	Halide::Func blur_x("blur_x");
	Halide::Func blur_y("blur_y");
	Halide::Func blur("blur");

	clamped = Halide::BoundaryCondition:reapeat_edge(input);

	blur_x(x, y, c) = cast<float>(clamped(x-1, y, c)+clamped(x, y, c)+clamped(x+1, y, c))/3.0f;
	blur_y(x, y, c) = (blur_x(x, y-1)+blur(x, y)+blur(x, y+1)) / 3.0f;
	blur(x, y, c) = Halide::clamp(Halide::cast<uint8_t>(blur_y(x, y, c)), 0, 255);

	if (schedule == 0) {
		// No need, it's implicit
		blur_x.compute_inline();
		blur_y.compute_inline();
	} else if (schedule == 1) {
		// Compute root
		blur_x.compute_root();
		blux_y.compute_root();
	} else {
		// high level shcedule

	}
	
	return blur;
}

int main(int argc, char **argv) 
{


	return 0;
}