#ifndef INSTANTIATE_MORE
#define INSTANTIATE_MORE(Scalar)
#endif

#ifndef INSTANTIATE_MATSLISE2D
#define INSTANTIATE_MATSLISE2D(Scalar)
#endif

#define INSTANTIATE_ALL(Scalar) \
INSTANTIATE_MATSLISE2D(Scalar) \
INSTANTIATE_MORE(Scalar)

INSTANTIATE_ALL(double)

#ifdef MATSLISE2D_long_double

INSTANTIATE_ALL(long double)

#endif

#ifdef MATSLISE2D_float128

#include <boost/multiprecision/float128.hpp>

INSTANTIATE_ALL(boost::multiprecision::float128)

#endif
