#include "../matslise3d.h"
#include "../util/quadrature.h"
#include <matslise/util/scoped_timer.h>
#include <map>

using namespace Eigen;
using namespace matslise;
using namespace std;
using namespace quadrature;

template<typename Scalar>
Matslise3D<Scalar>::Matslise3D(
        const std::function<Scalar(Scalar, Scalar, Scalar)> &potential,
        const matslise::Rectangle<Scalar, 3> &domain, const Config &config_)
        : AbstractMatslise3D<Scalar>(potential, domain),
          MatsliseND<Scalar, Matslise3DSector<Scalar>>(config_.xyBasisSize, config_.tolerance), config(config_) {
    MATSLISE_SCOPED_TIMER("3D constructor");
    grid_x = lobatto::grid<Scalar>(ArrayXs::LinSpaced(101, domain.template min<0>(), domain.template max<0>()));
    grid_y = lobatto::grid<Scalar>(ArrayXs::LinSpaced(101, domain.template min<1>(), domain.template max<1>()));

    std::shared_ptr<matslise::sector_builder::SectorBuilder<Matslise3D<Scalar>>> sector_builder
            = config_.zSectorBuilder;
    if (!sector_builder)
        sector_builder = std::make_shared<matslise::sector_builder::AutomaticSectorBuilder<Matslise3D<Scalar>>>(
                config_.tolerance);
    auto sectorsBuild = (*sector_builder)(this, domain.template min<2>(), domain.template max<2>());
    sectors = std::move(sectorsBuild.sectors);
    matchIndex = sectorsBuild.matchIndex;
    Index sectorCount = sectors.size();
    cout << "sectors build" << endl;

    M.reserve(sectorCount - 1);
    const Index &N = config.xyBasisSize;
    for (int k = 0; k < sectorCount - 1; ++k) {
        MatrixXs &r = M.emplace_back(N, N);

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                r(i, j) = lobatto::quadrature<Scalar>(
                        grid_x, grid_y,
                        sectors[k]->eigenfunctions_grid[j] *
                        sectors[k + 1]->eigenfunctions_grid[i]);
        cout << "\n\nM " << k << endl;
        cout << r * r.transpose() - MatrixXs::Identity(N, N) << endl;
    }
}


#include <matslise/util/sectorbuilder.cpp>
#include "../matsliseNd/matsliseNd.impl.h"


#define INSTANTIATE_MORE(Scalar) \
INSTANTIATE_SECTOR_BUILDER(Matslise3D<Scalar>) \
template class matslise::MatsliseND<Scalar, matslise::Matslise2DSector<Scalar>>;

#include "instantiate.h"
