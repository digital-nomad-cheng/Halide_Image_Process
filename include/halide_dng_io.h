
#ifndef __HALIDE_DNG_IO__
#define __HALIDE_DNG_IO__


#define TINY_DNG_LOADER_IMPLEMENTATION
#include "tiny_dng_loader.h"

#include <HalideBuffer.h>
#include "CFA.hpp"


namespace Halide 
{
namespace Tools 
{


template<typename T>
struct RAW {
	Halide::Runtime::Buffer<T> buffer;
	Halide::Runtime::Buffer<T> black_level;
	T white_level;
	uint8_t cfa_pattern;
};


} // namespace Tools
} // namespace Halide
#endif  // __HALIDE_DNG_IO__
