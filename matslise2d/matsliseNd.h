//
// Created by toon on 8/25/20.
//

#ifndef MATSLISE_MATSLISEND_H
#define MATSLISE_MATSLISEND_H

#include <matslise/matslise.h>
#include <matslise/matscs.h>
#include <matslise/util/eigen.h>

namespace matslise {
    template<typename Scalar_>
    class MatsliseNDSector {
    public:
        typedef Scalar_ Scalar;
        std::vector<typename matslise::Matscs<Scalar>::Sector> matscs;

        Scalar min, max;

        Scalar error() const;

    public:
        MatsliseNDSector() {};

        MatsliseNDSector(Scalar _min, Scalar _max) : min(_min), max(_max) {};

        std::pair<Y<Scalar, Eigen::Dynamic>, Eigen::Index>

        propagateWithIndex(
                const Scalar &E, Y<Scalar, Eigen::Dynamic> y0) const;

        template<int r>
        Y<Scalar, Eigen::Dynamic, r> propagate(
                const Scalar &E, Y<Scalar, Eigen::Dynamic, r> y0, const Scalar &a, const Scalar &b,
                bool use_h = true) const;

        bool contains(const Scalar &point) const {
            return point <= max && point >= min;
        }
    };

    template<typename Scalar_, typename Sector_>
    class MatsliseND {
    public:
        typedef Scalar_ Scalar;
        using MatrixXs = Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>;

        Eigen::Index basisSize;
        std::vector<MatrixXs> M;
        std::vector<std::unique_ptr<Sector_>> sectors;
        int matchIndex;
        Y<Scalar, Eigen::Dynamic> dirichletBoundary;
        Scalar tolerance;

    public:
        MatsliseND(Eigen::Index _basisSize, Scalar tolerance) : basisSize(_basisSize), tolerance(tolerance) {
            dirichletBoundary = Y<Scalar, Eigen::Dynamic>::Dirichlet(basisSize);
        }

        Y<Scalar, Eigen::Dynamic>
        propagate(const Scalar &E, const Y<Scalar, Eigen::Dynamic> &y0, const Scalar &a, const Scalar &b,
                  bool use_h = true) const;

        std::pair<MatrixXs, MatrixXs> matchingErrorMatrix(const Scalar &E) const {
            return matchingErrorMatrix(dirichletBoundary, E);
        }

        std::vector<std::pair<Scalar, Scalar>> matchingErrors(const Scalar &E) const {
            return matchingErrors(dirichletBoundary, E);
        }

        std::pair<Scalar, Scalar> matchingError(const Scalar &E) const {
            return matchingError(dirichletBoundary, E);
        }

        std::pair<MatrixXs, MatrixXs> matchingErrorMatrix(
                const Y<Scalar, Eigen::Dynamic> &left, const Scalar &E, bool use_h = true) const;

        std::vector<std::pair<Scalar, Scalar>> matchingErrors(
                const Y<Scalar, Eigen::Dynamic> &left, const Scalar &E, bool use_h = true) const;

        std::pair<Scalar, Scalar> matchingError(
                const Y<Scalar, Eigen::Dynamic> &left, const Scalar &E, bool use_h = true) const;

        std::pair<Scalar, Eigen::Index>
        eigenvalue(const Y<Scalar, Eigen::Dynamic> &left, Scalar Eguess, bool use_h = true) const;

        std::pair<Scalar, Eigen::Index> eigenvalue(const Scalar &Eguess) const {
            return eigenvalue(dirichletBoundary, Eguess);
        }

        Scalar eigenvalueError(const Y<Scalar, Eigen::Dynamic> &left, const Scalar &E) const;

        Scalar eigenvalueError(const Scalar &E) const {
            return eigenvalueError(dirichletBoundary, E);
        }

        std::vector<std::tuple<Eigen::Index, Scalar, Eigen::Index>>
        eigenvalues(const Y<Scalar, Eigen::Dynamic> &left, const Scalar &Emin, const Scalar &Emax) const;

        std::vector<std::tuple<Eigen::Index, Scalar, Eigen::Index>>
        eigenvalues(const Scalar &Emin, const Scalar &Emax) const {
            return eigenvalues(dirichletBoundary, Emin, Emax);
        }

        std::vector<std::tuple<Eigen::Index, Scalar, Eigen::Index>>
        eigenvaluesByIndex(const Y<Scalar, Eigen::Dynamic> &left, Eigen::Index Imin, Eigen::Index Imax) const;

        std::vector<std::tuple<Eigen::Index, Scalar, Eigen::Index>>
        eigenvaluesByIndex(Eigen::Index Imin, Eigen::Index Imax) const {
            return eigenvaluesByIndex(dirichletBoundary, Imin, Imax);
        }

        Eigen::Index estimateIndex(Y<Scalar, Eigen::Dynamic> y, const Scalar &E) const;

        Eigen::Index estimateIndex(const Scalar &E) const {
            return estimateIndex(dirichletBoundary, E);
        }

    protected:
        std::vector<Y<Scalar, Eigen::Dynamic>> eigenfunctionSteps(
                const Y<Scalar, Eigen::Dynamic> &left, const Scalar &E) const;
    };
}


#endif //MATSLISE_MATSLISEND_H
