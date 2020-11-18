#include <Halide.h>

class Median : public Halide::Generator<Median>
{
public:
	Input<Halide::Buffer<uint8_t>> input{"input", 3};
	Output<Halide::Buffer<uint8_t>> output{"output", 3};

	void generate() {
		clamped = Halide::BoundaryConditions::repeat_edge(input, {{0, input.width()}, {0, input.height()}} );
		median_x(x, y, c) = median3(clamped(x-1, y, c), clamped(x, y, c), clamped(x+1, y, c));
		median_y(x, y, c) = median3(median_x(x, y-1, c), median_x(x, y, c), median_x(x, y+1, c));
		output(x, y, c) = median_y(x, y, c);
	};

	void schedule() {
		if (auto_schedule) {
			input.set_estimates({{0, 3000}, {0, 3000}, {0, 3}});
			output.set_estimates({{0, 3000}, {0, 3000}, {0, 3}});
		} else {
			int vector_size = get_target().natural_vector_size(input.type());
			output
				.split(y, yo, yi, 64).parallel(yo)
				.split(x, xo, xi, vector_size).vectorize(xi);
			median_y.compute_at(output, yi);
			median_y
				.split(y, yo, yi, 64).parallel(yo)
				.split(x, xo, xi, vector_size).vectorize(xi);
			median_x.compute_at(median_y, yi);
			median_x
				.split(y, yo, yi, 64).parallel(yo)
				.split(x, xo, xi, vector_size).vectorize(xi);
			clamped.compute_root();

		}
	}

private:
	Halide::Var x{"x"}, y{"y"}, c{"c"};
	Halide::Var xi{"xi"}, xo{"xo"}, yi{"yi"}, yo{"yo"};
	Halide::Func clamped{"clamped"};
	Halide::Func median_x{"median_x"};
	Halide::Func median_y{"median_y"};
	Halide::Expr median3(Halide::Expr a, Halide::Expr b, Halide::Expr c) {

		return min(max(min(a, b),c), max(a, b));    
	}
};

HALIDE_REGISTER_GENERATOR(Median, median_gen)