#include <Halide.h>

enum Filter {
	LAPLACIAN_0 = 0,
	LAPLACIAN_1,
	LAPLACIAN_2,	
};

class Laplacian : public Halide::Generator<Laplacian>
{
public:

	Input<Buffer<uint8_t>> input{"input", 3};
	Output<Buffer<uint8_t>> output{"output", 3};

	Halide::GeneratorParam<enum Filter> filter{"filter", LAPLACIAN_1,
		{
	        {"laplacian0", LAPLACIAN_0},
	        {"laplacian1", LAPLACIAN_1},
	        {"laplacian2", LAPLACIAN_2},
	    }

	};

	void generate() {
		switch(filter) {
			case LAPLACIAN_0:
				output = laplacian_0(input);
				break;
			case LAPLACIAN_1:
				output = laplacian_1(input);
				break;
			case LAPLACIAN_2:
				output = laplacian_2(input);
				break;
		}
	}

private:
	Halide::Var x{"x"}, y{"y"}, c{"c"};
	Halide::Func clamped{"clamped"};
	Halide::Func gradiant_x{"gradiant_x"};
	Halide::Func gradiant_y{"gradiant_y"};
	Halide::Func gradiant_xy{"gradiant_xy"};

	// d²f(x,y)/dx² + d²f(x,y)/dy²
	// df(x,y)/dx = f(x+1, y) - f(x, y)
	// d²f(x,y)/dx² = f(x+1, y) - 2*f(x, y) + f(x-1, y)

	//  0 -1  0    0  0  0    0 -1  0
	// -1  4 -1 = -1  2 -1 +  0  2  0
	//  0 -1  0    0  0  0    0 -1  0
	Halide::Func laplacian_0(Input<Buffer<uint8_t>> &input) {
		clamped = Halide::BoundaryConditions::repeat_edge(input, {{0, input.width()}, {0, input.height()}});
		gradiant_x(x, y) = -clamped(x-1, y) + 2*clamped(x, y) - clamped(x+1, y);
		gradiant_y(x, y) = -clamped(x, y-1) + 2*clamped(x, y) - clamped(x, y+1);
		gradiant_xy(x, y) = gradiant_x(x, y) + gradiant_y(x, y);
		return gradiant_xy;
	}

	Halide::Func laplacian_1(Input<Buffer<uint8_t>> &input) {
		clamped = Halide::BoundaryConditions::repeat_edge(input, {{0, input.width()}, {0, input.height()}});
		gradiant_x(x, y) = -clamped(x-1, y) + 2*clamped(x, y) - clamped(x+1, y);
		gradiant_y(x, y) = -clamped(x, y-1) + 2*clamped(x, y) - clamped(x, y+1);
		gradiant_xy(x, y) = gradiant_x(x, y) + gradiant_y(x, y);
		return gradiant_xy;
	};
	Halide::Func laplacian_2(Input<Buffer<uint8_t>> &input) {
		clamped = Halide::BoundaryConditions::repeat_edge(input, {{0, input.width()}, {0, input.height()}});
		gradiant_x(x, y) = -clamped(x-1, y) + 2*clamped(x, y) - clamped(x+1, y);
		gradiant_y(x, y) = -clamped(x, y-1) + 2*clamped(x, y) - clamped(x, y+1);
		gradiant_xy(x, y) = gradiant_x(x, y) + gradiant_y(x, y);
		return gradiant_xy;
	};
    //  1             1 -2  1    -1              1 -2  1
    // -2 * 1 -2 1 = -2  4 -2 or  2 * -1 2 -1 = -2  4 -2
    //  1             1 -2  1    -1              1 -2  1



	// -1 -1 -1   0 0 0   1 1 1
	// -1  8 -1 = 0 9 0 - 1 1 1
	// -1 -1 -1   0 0 0   1 1 1
	// 1           1 1 1
	// 1 * 1 1 1 = 1 1 1
	// 1           1 1 1
};	

HALIDE_REGISTER_GENERATOR(Laplacian, laplacian_gen)