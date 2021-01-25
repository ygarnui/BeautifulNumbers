#pragma once

#if defined(_MSC_VER) && _MSVC_LANG >= 201703L //currently enables parallel execution policies for MSVC only

#include <execution>
#define TRY_PARALLELIZE_PAR_UNSEQ std::execution::par_unseq,

#else //unix-based platforms require Intel TBB, we haven't got it yet

#define TRY_PARALLELIZE_PAR_UNSEQ

#endif
