#include "../matslise2d.h"
#include "../util/quadrature.h"
#include <matslise/util/scoped_timer.h>

using namespace Eigen;

template<typename Scalar, bool halfrange>
matslise::BasisQuadratureLobatto<Scalar, halfrange>::BasisQuadratureLobatto(
        const matslise::Matslise<Scalar> *matslise) {
    grid = quadrature::lobatto::grid<Scalar>(
            Eigen::Array<Scalar, Eigen::Dynamic, 1>::LinSpaced(503, matslise->domain.min, matslise->domain.max));
    vbar = grid.unaryExpr([&matslise](const Scalar &x) { return matslise->potential(x); });
}

template<typename Scalar, bool halfrange>
Matrix<Scalar, Dynamic, Dynamic>
matslise::BasisQuadratureLobatto<Scalar, halfrange>::dV(
        const typename matslise::Matslise2D<Scalar>::Sector &sector2d, const Scalar &y) {
    if (!init)
        calculateQuadData(sector2d);
    MATSLISE_SCOPED_TIMER("2D quadratures dV");

    const Index &N = sector2d.se2d->config.basisSize;
    Matrix<Scalar, Dynamic, Dynamic> dV
            = Matrix<Scalar, Dynamic, Dynamic>::Zero(N, N);

    for (int i = 0; i < N; ++i) {
        dV(i, i) = sector2d.eigenvalues[i];
    }

    Eigen::Array<Scalar, Eigen::Dynamic, 1> Vdata = grid.unaryExpr(
            [&sector2d, &y](const Scalar &x) -> Scalar { return sector2d.se2d->potential(x, y); }) - vbar;


    Eigen::Array<Scalar, Eigen::Dynamic, 1> result;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j <= i; ++j) {
            if (halfrange && (i % 2) != (j % 2))
                continue;
            result = Vdata * eigenfunctions.col(i) * eigenfunctions.col(j);
            Scalar v = quadrature::lobatto::quadrature(grid, result);
            if constexpr (halfrange)
                v += v;
            dV(i, j) += v;
        }
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < i; ++j) {
            dV(j, i) = dV(i, j);
        }
    return dV;
}

template<typename Scalar, bool halfrange>
void matslise::BasisQuadratureLobatto<Scalar, halfrange>::calculateQuadData(
        const typename matslise::Matslise2D<Scalar>::Sector &sector2d) {
    init = true;
    eigenfunctions.resize(grid.size(), sector2d.eigenfunctions.size());
    long i = 0;
    for (auto &f: sector2d.eigenfunctions) {
        eigenfunctions.col(i++) = (*f)(grid).col(0);
    }
}

#define INSTANTIATE_MORE(Scalar) \
    template class matslise::BasisQuadratureLobatto<Scalar, true>; \
    template class matslise::BasisQuadratureLobatto<Scalar, false>;

#include "instantiate.h"
