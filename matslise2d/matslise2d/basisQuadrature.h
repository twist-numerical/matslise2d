#ifndef MATSLISE_BASISQUADRATURE_H
#define MATSLISE_BASISQUADRATURE_H

#define MATSLISE2D_DELTA_V_DEGREE 8

namespace matslise {
    template<typename Scalar>
    class Matslise2D;

    template<typename Scalar>
    class AbstractBasisQuadrature {
    public:
        virtual Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> dV(
                const typename matslise::Matslise2D<Scalar>::Sector &sector2d, const Scalar &y) = 0;

        virtual ~AbstractBasisQuadrature() = default;
    };

    template<typename Scalar, int hmax, bool halfrange = false>
    class BasisQuadratureSymbolic : public AbstractBasisQuadrature<Scalar> {
    public:
        const matslise::Matslise<Scalar> *matslise;
        std::vector<Eigen::Array<Scalar, hmax, 1>> quadData;

        explicit BasisQuadratureSymbolic(const matslise::Matslise<Scalar> *matslise) : matslise(matslise) {
        }

        Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>
        dV(const typename matslise::Matslise2D<Scalar>::Sector &sector2d, const Scalar &y) override;

    public:
        void calculateQuadData(const typename matslise::Matslise2D<Scalar>::Sector &sector2d);
    };

    template<typename Scalar, bool halfrange = false>
    class BasisQuadratureLobatto : public AbstractBasisQuadrature<Scalar> {
    public:
        bool init = false;
        Eigen::Array<Scalar, Eigen::Dynamic, 1> grid;
        Eigen::Array<Scalar, Eigen::Dynamic, 1> vbar;
        Eigen::Array<Scalar, Eigen::Dynamic, Eigen::Dynamic> eigenfunctions;

        explicit BasisQuadratureLobatto(const matslise::Matslise<Scalar> *);

        Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>
        dV(const typename matslise::Matslise2D<Scalar>::Sector &sector2d, const Scalar &y) override;

    public:
        void calculateQuadData(const typename matslise::Matslise2D<Scalar>::Sector &sector2d);
    };

    template<typename Scalar, int hmax, bool halfrange = false>
    using BasisQuadrature = BasisQuadratureLobatto<Scalar, halfrange>;

}

#endif //MATSLISE_BASISQUADRATURE_H
