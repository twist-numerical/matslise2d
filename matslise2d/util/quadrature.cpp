#include "./quadrature.h"
#include <vector>

#ifdef MATSLISE2D_QUADMATH

#include <boost/multiprecision/float128.hpp>

using boost::multiprecision::float128;

#endif

using namespace Eigen;
using namespace quadrature;

namespace quadrature::gauss_kronrod {
    template<typename Scalar>
    const Array<Scalar, 31, 1> abscissae = (Array<Scalar, 31, 1>()
            << -0.998002298693397060285172840152271l, -0.987992518020485428489565718586613l, -0.967739075679139134257347978784337l, -0.937273392400705904307758947710209l, -0.897264532344081900882509656454496l, -0.848206583410427216200648320774217l, -0.790418501442465932967649294817947l, -0.724417731360170047416186054613938l, -0.650996741297416970533735895313275l, -0.570972172608538847537226737253911l, -0.485081863640239680693655740232351l, -0.394151347077563369897207370981045l, -0.299180007153168812166780024266389l, -0.201194093997434522300628303394596l, -0.101142066918717499027074231447392l, 0.000000000000000000000000000000000l, 0.101142066918717499027074231447392l, 0.201194093997434522300628303394596l, 0.299180007153168812166780024266389l, 0.394151347077563369897207370981045l, 0.485081863640239680693655740232351l, 0.570972172608538847537226737253911l, 0.650996741297416970533735895313275l, 0.724417731360170047416186054613938l, 0.790418501442465932967649294817947l, 0.848206583410427216200648320774217l, 0.897264532344081900882509656454496l, 0.937273392400705904307758947710209l, 0.967739075679139134257347978784337l, 0.987992518020485428489565718586613l, 0.998002298693397060285172840152271l
    ).finished();
    template<typename Scalar>
    const Array<Scalar, 31, 1> weightsKronrod = (Array<Scalar, 31, 1>()
            << 0.005377479872923348987792051430128l, 0.015007947329316122538374763075807l, 0.025460847326715320186874001019653l, 0.035346360791375846222037948478360l, 0.044589751324764876608227299373280l, 0.053481524690928087265343147239430l, 0.062009567800670640285139230960803l, 0.069854121318728258709520077099147l, 0.076849680757720378894432777482659l, 0.083080502823133021038289247286104l, 0.088564443056211770647275443693774l, 0.093126598170825321225486872747346l, 0.096642726983623678505179907627589l, 0.099173598721791959332393173484603l, 0.100769845523875595044946662617570l, 0.101330007014791549017374792767493l, 0.100769845523875595044946662617570l, 0.099173598721791959332393173484603l, 0.096642726983623678505179907627589l, 0.093126598170825321225486872747346l, 0.088564443056211770647275443693774l, 0.083080502823133021038289247286104l, 0.076849680757720378894432777482659l, 0.069854121318728258709520077099147l, 0.062009567800670640285139230960803l, 0.053481524690928087265343147239430l, 0.044589751324764876608227299373280l, 0.035346360791375846222037948478360l, 0.025460847326715320186874001019653l, 0.015007947329316122538374763075807l, 0.005377479872923348987792051430128l
    ).finished();
    template<typename Scalar>
    const Eigen::Array<Scalar, 15, 1> weightsGauss = (Array<Scalar, 15, 1>()
            << 0.030753241996117268354628393577204l, 0.070366047488108124709267416450667l, 0.107159220467171935011869546685869l, 0.139570677926154314447804794511028l, 0.166269205816993933553200860481209l, 0.186161000015562211026800561866423l, 0.198431485327111576456118326443839l, 0.202578241925561272880620199967519l, 0.198431485327111576456118326443839l, 0.186161000015562211026800561866423l, 0.166269205816993933553200860481209l, 0.139570677926154314447804794511028l, 0.107159220467171935011869546685869l, 0.070366047488108124709267416450667l, 0.030753241996117268354628393577204l
    ).finished();

#ifdef MATSLISE2D_QUADMATH

    template<>
    const Eigen::Array<float128, 31, 1> abscissae<float128> = (Array<float128, 31, 1>()
            << -0.998002298693397060285172840152271q, -0.987992518020485428489565718586613q, -0.967739075679139134257347978784337q, -0.937273392400705904307758947710209q, -0.897264532344081900882509656454496q, -0.848206583410427216200648320774217q, -0.790418501442465932967649294817947q, -0.724417731360170047416186054613938q, -0.650996741297416970533735895313275q, -0.570972172608538847537226737253911q, -0.485081863640239680693655740232351q, -0.394151347077563369897207370981045q, -0.299180007153168812166780024266389q, -0.201194093997434522300628303394596q, -0.101142066918717499027074231447392q, 0.000000000000000000000000000000000q, 0.101142066918717499027074231447392q, 0.201194093997434522300628303394596q, 0.299180007153168812166780024266389q, 0.394151347077563369897207370981045q, 0.485081863640239680693655740232351q, 0.570972172608538847537226737253911q, 0.650996741297416970533735895313275q, 0.724417731360170047416186054613938q, 0.790418501442465932967649294817947q, 0.848206583410427216200648320774217q, 0.897264532344081900882509656454496q, 0.937273392400705904307758947710209q, 0.967739075679139134257347978784337q, 0.987992518020485428489565718586613q, 0.998002298693397060285172840152271q
    ).finished();

    template<>
    const Eigen::Array<float128, 31, 1> weightsKronrod<float128> = (Array<float128, 31, 1>()
            << 0.005377479872923348987792051430128q, 0.015007947329316122538374763075807q, 0.025460847326715320186874001019653q, 0.035346360791375846222037948478360q, 0.044589751324764876608227299373280q, 0.053481524690928087265343147239430q, 0.062009567800670640285139230960803q, 0.069854121318728258709520077099147q, 0.076849680757720378894432777482659q, 0.083080502823133021038289247286104q, 0.088564443056211770647275443693774q, 0.093126598170825321225486872747346q, 0.096642726983623678505179907627589q, 0.099173598721791959332393173484603q, 0.100769845523875595044946662617570q, 0.101330007014791549017374792767493q, 0.100769845523875595044946662617570q, 0.099173598721791959332393173484603q, 0.096642726983623678505179907627589q, 0.093126598170825321225486872747346q, 0.088564443056211770647275443693774q, 0.083080502823133021038289247286104q, 0.076849680757720378894432777482659q, 0.069854121318728258709520077099147q, 0.062009567800670640285139230960803q, 0.053481524690928087265343147239430q, 0.044589751324764876608227299373280q, 0.035346360791375846222037948478360q, 0.025460847326715320186874001019653q, 0.015007947329316122538374763075807q, 0.005377479872923348987792051430128q
    ).finished();

    template<>
    const Eigen::Array<float128, 15, 1> weightsGauss<float128> = (Array<float128, 15, 1>()
            << 0.030753241996117268354628393577204q, 0.070366047488108124709267416450667q, 0.107159220467171935011869546685869q, 0.139570677926154314447804794511028q, 0.166269205816993933553200860481209q, 0.186161000015562211026800561866423q, 0.198431485327111576456118326443839q, 0.202578241925561272880620199967519q, 0.198431485327111576456118326443839q, 0.186161000015562211026800561866423q, 0.166269205816993933553200860481209q, 0.139570677926154314447804794511028q, 0.107159220467171935011869546685869q, 0.070366047488108124709267416450667q, 0.030753241996117268354628393577204q
    ).finished();
#endif


    template<typename Scalar, typename Value>
    inline std::pair<Value, Scalar> applyGaussKronrod(
            const std::function<Eigen::Array<Value, Eigen::Dynamic, 1>(
                    const Eigen::Array<Scalar, Eigen::Dynamic, 1> &)> &f, Scalar a, Scalar b,
            const std::function<Scalar(const Value &)> &error) {
        Scalar h = (b - a) / 2;
        Eigen::Array<Scalar, Eigen::Dynamic, 1> x = a + h + h * abscissae<Scalar>;
        Eigen::Array<Value, Eigen::Dynamic, 1> y = f(x);
        Value valueKonrad;
        Value valueGauss;
        if constexpr(std::is_same<Scalar, Value>::value) {
            valueKonrad = (weightsKronrod<Scalar> * y).sum();
            valueGauss = (weightsGauss<Scalar> *
                          Eigen::Map<Eigen::Array<Value, 15, 1>, 0, Eigen::InnerStride<2>>(y.data() + 1)).sum();
        } else {
            valueKonrad = weightsKronrod<Scalar>(0) * y(0);
            for (Index i = 1; i < 31; ++i)
                valueKonrad += weightsKronrod<Scalar>(i) * y(i);

            valueGauss = weightsGauss<Scalar>(0) * y(1);
            for (Index i = 1; i < 15; ++i)
                valueGauss += weightsGauss<Scalar>(i) * y(1 + 2 * i);
        }
        valueGauss *= h;
        valueKonrad *= h;
        return {valueKonrad, error((valueKonrad - valueGauss))};
    }

    template<typename Scalar, typename Value>
    inline std::pair<Value, Scalar> applyGaussKronrod(
            const std::function<Value(const Scalar &)> &f, Scalar a, Scalar b,
            const std::function<Scalar(const Value &)> &error) {
        return applyGaussKronrod((std::function<Eigen::Array<Value, Eigen::Dynamic, 1>(
                const Eigen::Array<Scalar, Eigen::Dynamic, 1> &)>) [&](
                const Eigen::Array<Scalar, Eigen::Dynamic, 1> &x) -> Eigen::Array<Value, Eigen::Dynamic, 1> {
            return x.unaryExpr(f);
        }, a, b, error);
    }

    template<typename Scalar, typename Value, bool bulk>
    Value adaptive(
            const std::function<typename std::conditional<bulk, Eigen::Array<Value, Eigen::Dynamic, 1>, Value>::type(
                    const typename std::conditional<bulk, Eigen::Array<Scalar, Eigen::Dynamic, 1>, Scalar>::type &)> &f,
            Scalar a, Scalar b, const Scalar &tolerance,
            const std::function<Scalar(const Value &)> &error) {
        typedef std::tuple<Scalar, Scalar, std::pair<Value, Scalar >> Item;
        static std::function<bool(const Item &, const Item &)> comp = [](const Item &a, const Item &b) {
            return std::get<2>(a).second < std::get<2>(b).second;
        };
        std::vector<Item> heap;
        std::pair<Value, Scalar> onInterval = applyGaussKronrod<Scalar, Value>(f, a, b, error);
        heap.push_back({a, b, onInterval});
        Scalar totalError = onInterval.second;
        while (totalError > tolerance) {
            std::tie(a, b, onInterval) = heap.front();
            std::pop_heap(heap.begin(), heap.end(), comp);
            heap.pop_back();
            totalError -= onInterval.second;
            Scalar c = (a + b) / 2;

            onInterval = applyGaussKronrod(f, a, c, error);
            totalError += onInterval.second;
            heap.push_back({a, c, onInterval});
            std::push_heap(heap.begin(), heap.end(), comp);

            onInterval = applyGaussKronrod(f, c, b, error);
            totalError += onInterval.second;
            heap.push_back({c, b, onInterval});
            std::push_heap(heap.begin(), heap.end(), comp);
        }

        Value total = std::get<2>(heap.front()).first;
        for (auto i = heap.begin() + 1; i != heap.end(); ++i)
            total += std::get<2>(*i).first;
        return total;
    }
}


#define INSTANTIATE_GK_BULK(Scalar, Value, bulk) \
template Value quadrature::gauss_kronrod::adaptive<Scalar, Value, bulk>( \
    const std::function<std::conditional<bulk, Eigen::Array<Value, Eigen::Dynamic, 1>, Value>::type( \
        const std::conditional<bulk, Eigen::Array<Scalar, Eigen::Dynamic, 1>, Scalar>::type &)> &, \
    Scalar, Scalar, const Scalar &, const std::function<Scalar(const Value &)> &);\

#define INSTANTIATE_GK(Scalar, Value) \
INSTANTIATE_GK_BULK(Scalar, Value, true) \
INSTANTIATE_GK_BULK(Scalar, Value, false)

#define INSTANTIATE_QUADRATURES(Scalar, Value) \
INSTANTIATE_GK(Scalar, Scalar) \
INSTANTIATE_GK(Scalar, Value)

INSTANTIATE_QUADRATURES(double, MatrixXd)

#ifdef MATSLISE2D_LONG_DOUBLE

typedef Eigen::Matrix<long double, Dynamic, Dynamic> MatrixXld;

INSTANTIATE_QUADRATURES(long double, MatrixXld)

#endif

#ifdef MATSLISE2D_QUADMATH

#include <boost/multiprecision/float128.hpp>

typedef Eigen::Matrix<boost::multiprecision::float128, Dynamic, Dynamic> MatrixXq;

INSTANTIATE_QUADRATURES(boost::multiprecision::float128, MatrixXq)

#endif