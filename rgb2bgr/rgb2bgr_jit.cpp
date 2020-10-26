#include <iostream>

#include <Halide.h>
#include <halide_image_io.h>

Halide::Func rgb2bgr(Halide::Buffer<uint8_t> &src) 
{
	Halide::Var x;
	Halide::Var y;
	Halide::Var c;

	Halide::Func dst;

	// dst(x, y, c) = Halide::select(c == 0, src(x, y, 2), c == 1, src(x, y, 1), src(x, y, 2));
	// or, below is the syntatic sugar of above
	dst(x, y, c) = Halide::mux(c, {src(x, y, 2), src(x, y, 1), src(x, y, 0)});

    return dst;

}

int main(int argc, char ** argv)
{
	Halide::Buffer<uint8_t> src = Halide::Tools::load_image("../images/Lenna.png");
	Halide::Buffer<uint8_t> dst = rgb2bgr(src).realize(src.width(), src.height(), src.channels());
	Halide::Tools::save_image(dst, "./results/Lenna_bgr.png");
}