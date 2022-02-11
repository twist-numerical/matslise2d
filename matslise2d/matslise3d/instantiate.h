#define INSTANTIATE_MATSLISE2D(Scalar)\
template class matslise::Matslise3DSector<Scalar>;\
template class matslise::Matslise3D<Scalar>;\
template class matslise::MatsliseND<Scalar, matslise::Matslise3DSector<Scalar>>;

#include "../util/instantiate.h"
