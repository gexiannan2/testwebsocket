// Copyright John Maddock 2006.
// Copyright Paul A. Bristow 2007, 2009
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifdef _MSC_VER
#  pragma warning(disable : 4756) // overflow in constant arithmetic
// Constants are too big for float case, but this doesn't matter for test.
#endif

#include <boost/math/concepts/real_concept.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>
#include <boost/math/special_functions/math_fwd.hpp>
#include <boost/array.hpp>
#include "functor.hpp"

#include "handle_test_result.hpp"
#include "table_type.hpp"

#ifndef SC_
#define SC_(x) static_cast<typename table_type<T>::type>(BOOST_JOIN(x, L))
#endif

template <class Real, typename T>
void do_test_ellint_e2(const T& data, const char* type_name, const char* test)
{
#if !(defined(ERROR_REPORTING_MODE) && !defined(ELLINT_2_FUNCTION_TO_TEST))
   typedef Real                   value_type;

   std::cout << "Testing: " << test << std::endl;
#ifdef ELLINT_2_FUNCTION_TO_TEST
   value_type(*fp2)(value_type, value_type) = ELLINT_2_FUNCTION_TO_TEST;
#elif defined(BOOST_MATH_NO_DEDUCED_FUNCTION_POINTERS)
    value_type (*fp2)(value_type, value_type) = boost::math::ellint_2<value_type, value_type>;
#else
    value_type (*fp2)(value_type, value_type) = boost::math::ellint_2;
#endif
    boost::math::tools::test_result<value_type> result;

    result = boost::math::tools::test_hetero<Real>(
      data,
      bind_func<Real>(fp2, 1, 0),
      extract_result<Real>(2));
   handle_test_result(result, data[result.worst()], result.worst(),
      type_name, "ellint_2", test);

   std::cout << std::endl;
#endif
}

template <class Real, typename T>
void do_test_ellint_e1(T& data, const char* type_name, const char* test)
{
#if !(defined(ERROR_REPORTING_MODE) && !defined(ELLINT_2C_FUNCTION_TO_TEST))
   typedef Real                   value_type;
    boost::math::tools::test_result<value_type> result;

   std::cout << "Testing: " << test << std::endl;

#ifdef ELLINT_2C_FUNCTION_TO_TEST
   value_type(*fp1)(value_type) = ELLINT_2C_FUNCTION_TO_TEST;
#elif defined(BOOST_MATH_NO_DEDUCED_FUNCTION_POINTERS)
   value_type (*fp1)(value_type) = boost::math::ellint_2<value_type>;
#else
   value_type (*fp1)(value_type) = boost::math::ellint_2;
#endif
   result = boost::math::tools::test_hetero<Real>(
      data,
      bind_func<Real>(fp1, 0),
      extract_result<Real>(1));
   handle_test_result(result, data[result.worst()], result.worst(),
      type_name, "ellint_2 (complete)", test);

   std::cout << std::endl;
#endif
}

template <typename T>
void test_spots(T, const char* type_name)
{
    BOOST_MATH_STD_USING
    // Function values calculated on http://functions.wolfram.com/
    // Note that Mathematica's EllipticE accepts k^2 as the second parameter.
    //
    // We can't use these for variable precision types as the test values dependent on Pi
    // get initialized once, and thereafter have the "wrong" precision when the function is
    // called a second time.
    //
#ifndef TEST_UDT
    static const std::array<std::array<T, 3>, 28> data1 = {{
        {{ SC_(0.0), SC_(0.0), SC_(0.0) }},
        {{ SC_(-10.0), SC_(0.0), SC_(-10.0) }},
        {{ SC_(-1.0), SC_(-1.0), SC_(-0.84147098480789650665250232163029899962256306079837) }},
        {{ SC_(-4.0), SC_(0.87890625) /*T(900) / 1024*/, SC_(-3.1756145986492562317862928524528520686391383168377) }},
        {{ SC_(8.0), SC_(-0.5859375) /*T(-600) / 1024*/, SC_(7.2473147180505693037677015377802777959345489333465) }},
        {{ SC_(1e-05), SC_(0.78125) /*T(800) / 1024*/, SC_(9.999999999898274739584436515967055859383969942432E-6) }},
        {{ SC_(1e+05), SC_(0.09765625) /*T(100) / 1024*/, SC_(99761.153306972066658135668386691227343323331995888) }},
        {{ SC_(1e+10), SC_(-0.5), SC_(9.3421545766487137036576748555295222252286528414669e9) }},
        {{ SC_(7.3786976294838206464e19) /*static_cast<T>(ldexp(T(1), 66))*/, SC_(0.390625) /*T(400) / 1024*/, SC_(7.0886102721911705466476846969992069994308167515242e19) }},
        {{ SC_(9.3536104789177786765035829293842113257979682750464e49) /*static_cast<T>(ldexp(T(1), 166))*/, SC_(0.87890625) /*T(900) / 1024*/, SC_(7.1259011068364515942912094521783688927118026465790e49) }},
        {{ SC_(0.25), SC_(1.5), SC_(0.244087118441983436818717707617920319373286836562840) }},
        {{ SC_(0.125), SC_(4.5), SC_(0.118076756678411098995742003403224531993649663256045) }},
        // Test k = 1, see https://github.com/boostorg/math/issues/321
        {{ (boost::math::constants::pi<T>() * 9) / 10, SC_(1.0), SC_(1.69098300562505257589770658281718094113984541009711856893227568864736976859054877514639639790530443125762295596230) }},
        {{ (boost::math::constants::pi<T>() * 11) / 10, SC_(1.0), SC_(2.30901699437494742410229341718281905886015458990288143106772431135263023140945122485360360209469556874237704403769) }},
        {{ boost::math::constants::pi<T>(), SC_(1.0), SC_(2.0) }},
        {{ boost::math::constants::pi<T>() / 2, SC_(1.0), SC_(1.0) }},
        {{ (3 * boost::math::constants::pi<T>()) / 2, SC_(1.0), SC_(3.0) }},
        {{ 2 * boost::math::constants::pi<T>(), SC_(1.0), SC_(4.0) }},
        {{ (20 * boost::math::constants::pi<T>()) / 21, SC_(1.0), SC_(1.85095773382382555307064528472278244309033056100177750424044959888147151556780639619867349409562910808166870200808) }},
        {{ (20 * boost::math::constants::pi<T>()) / 19, SC_(1.0), SC_(2.16459459028073389414365205908793841951217248335965412335147127404907690285247170629778731438009399303864259295133) }},
        {{ -(boost::math::constants::pi<T>() * 9) / 10, SC_(1.0), SC_(-1.69098300562505257589770658281718094113984541009711856893227568864736976859054877514639639790530443125762295596230) }},
        {{ -(boost::math::constants::pi<T>() * 11) / 10, SC_(1.0), SC_(-2.30901699437494742410229341718281905886015458990288143106772431135263023140945122485360360209469556874237704403769) }},
        {{ -boost::math::constants::pi<T>(), SC_(1.0), SC_(-2.0) }},
        {{ -boost::math::constants::pi<T>() / 2, SC_(1.0), SC_(-1.0) }},
        {{ -(3 * boost::math::constants::pi<T>()) / 2, SC_(1.0), SC_(-3.0) }},
        {{ -2 * boost::math::constants::pi<T>(), SC_(1.0), SC_(-4.0) }},
        {{ -(20 * boost::math::constants::pi<T>()) / 21, SC_(1.0), SC_(-1.85095773382382555307064528472278244309033056100177750424044959888147151556780639619867349409562910808166870200808) }},
        {{ -(20 * boost::math::constants::pi<T>()) / 19, SC_(1.0), SC_(-2.16459459028073389414365205908793841951217248335965412335147127404907690285247170629778731438009399303864259295133) }},
    }};

    do_test_ellint_e2<T>(data1, type_name, "Elliptic Integral E: Mathworld Data");
#endif

#include "ellint_e2_data.ipp"

    do_test_ellint_e2<T>(ellint_e2_data, type_name, "Elliptic Integral E: Random Data");

    // Function values calculated on http://functions.wolfram.com/
    // Note that Mathematica's EllipticE accepts k^2 as the second parameter.
    static const std::array<std::array<typename table_type<T>::type, 2>, 10> data2 = {{
        {{ SC_(-1.0), SC_(1.0) }},
        {{ SC_(0.0), SC_(1.5707963267948966192313216916397514420985846996876) }},
        {{ SC_(0.09765625) /*T(100) / 1024*/, SC_(1.5670445330545086723323795143598956428788609133377) }},
        {{ SC_(0.1953125) /*T(200) / 1024*/, SC_(1.5557071588766556854463404816624361127847775545087) }},
        {{ SC_(0.29296875) /*T(300) / 1024*/, SC_(1.5365278991162754883035625322482669608948678755743) }},
        {{ SC_(0.390625) /*T(400) / 1024*/, SC_(1.5090417763083482272165682786143770446401437564021) }},
        {{ SC_(-0.5), SC_(1.4674622093394271554597952669909161360253617523272) }},
        {{ SC_(-0.5859375) /*T(-600) / 1024*/, SC_(1.4257538571071297192428217218834579920545946473778) }},
        {{ SC_(-0.78125) /*T(-800) / 1024*/, SC_(1.2927868476159125056958680222998765985004489572909) }},
        {{ SC_(-0.87890625) /*T(-900) / 1024*/, SC_(1.1966864890248739524112920627353824133420353430982) }},
    }};

    do_test_ellint_e1<T>(data2, type_name, "Elliptic Integral E: Mathworld Data");

#include "ellint_e_data.ipp"

    do_test_ellint_e1<T>(ellint_e_data, type_name, "Elliptic Integral E: Random Data");

    static const std::array<std::array<typename table_type<T>::type, 3>, 72> small_angles = { {
       {{ SC_(0.00097656250000000000000000000000000000000000000000000), SC_(0.5), SC_(0.00097656246119489873806295171767681128826061680891539) }},{{ SC_(0.00048828125000000000000000000000000000000000000000000), SC_(0.5), SC_(0.00048828124514936177847275804383491089917731651869089) }},{{ SC_(0.00024414062500000000000000000000000000000000000000000), SC_(0.5), SC_(0.00024414062439367020469080959924292294147407037569089) }},{{ SC_(0.00012207031250000000000000000000000000000000000000000), SC_(0.5), SC_(0.00012207031242420877503577978533579671450656676021144) }},{{ SC_(0.000061035156250000000000000000000000000000000000000000), SC_(0.5), SC_(0.000061035156240526096862267116434822602398026203555135) }},{{ SC_(0.000030517578125000000000000000000000000000000000000000), SC_(0.5), SC_(0.000030517578123815762107245722156263286910312978330942) }},{{ SC_(0.000015258789062500000000000000000000000000000000000000), SC_(0.5), SC_(0.000015258789062351970263388913163340973814136929083865) }},{{ SC_(7.6293945312500000000000000000000000000000000000000e-6), SC_(0.5), SC_(7.6293945312314962829230890795991108894734462126936e-6) }},{{ SC_(3.8146972656250000000000000000000000000000000000000e-6), SC_(0.5), SC_(3.8146972656226870353653697266430602974836595561218e-6) }},{{ SC_(1.9073486328125000000000000000000000000000000000000e-6), SC_(0.5), SC_(1.9073486328122108794206707030707941437882919842603e-6) }},{{ SC_(9.5367431640625000000000000000000000000000000000000e-7), SC_(0.5), SC_(9.5367431640621385992758382186011213067376941980499e-7) }},{{ SC_(4.7683715820312500000000000000000000000000000000000e-7), SC_(0.5), SC_(4.7683715820312048249094797723177223079355575583105e-7) }},{{ SC_(2.3841857910156250000000000000000000000000000000000e-7), SC_(0.5), SC_(2.3841857910156193531136849713832334805104830239272e-7) }},{{ SC_(1.1920928955078125000000000000000000000000000000000e-7), SC_(0.5), SC_(1.1920928955078117941392106214180141285643896716624e-7) }},{{ SC_(5.9604644775390625000000000000000000000000000000000e-8), SC_(0.5), SC_(5.9604644775390616176740132767709895180494181165759e-8) }},{{ SC_(2.9802322387695312500000000000000000000000000000000e-8), SC_(0.5), SC_(2.9802322387695311397092516595963259352981751091479e-8) }},{{ SC_(1.4901161193847656250000000000000000000000000000000e-8), SC_(0.5), SC_(1.4901161193847656112136564574495392495854593212863e-8) }},{{ SC_(7.4505805969238281250000000000000000000000000000000e-9), SC_(0.5), SC_(7.4505805969238281077670705718119235956296952243088e-9) }},{{ SC_(3.7252902984619140625000000000000000000000000000000e-9), SC_(0.5), SC_(3.7252902984619140603458838214764904348802078740605e-9) }},{{ SC_(1.8626451492309570312500000000000000000000000000000e-9), SC_(0.5), SC_(1.8626451492309570309807354776845613039046039833520e-9) }},{{ SC_(9.3132257461547851562500000000000000000000000000000e-10), SC_(0.5), SC_(9.3132257461547851559134193471057016297384356039070e-10) }},{{ SC_(4.6566128730773925781250000000000000000000000000000e-10), SC_(0.5), SC_(4.6566128730773925780829274183882127037128569700108e-10) }},{{ SC_(2.3283064365386962890625000000000000000000000000000e-10), SC_(0.5), SC_(2.3283064365386962890572409272985265879639681374864e-10) }},{{ SC_(1.1641532182693481445312500000000000000000000000000e-10), SC_(0.5), SC_(1.1641532182693481445305926159123158234954916739431e-10) }},{{ SC_(5.8207660913467407226562500000000000000000000000000e-11), SC_(0.5), SC_(5.8207660913467407226554282698903947793693632351656e-11) }},{{ SC_(2.9103830456733703613281250000000000000000000000000e-11), SC_(0.5), SC_(2.9103830456733703613280222837362993474211703619812e-11) }},{{ SC_(1.4551915228366851806640625000000000000000000000000e-11), SC_(0.5), SC_(1.4551915228366851806640496604670374184276462939222e-11) }},{{ SC_(7.2759576141834259033203125000000000000000000000000e-12), SC_(0.5), SC_(7.2759576141834259033202964505837967730345578669885e-12) }},{{ SC_(3.6379788070917129516601562500000000000000000000000e-12), SC_(0.5), SC_(3.6379788070917129516601542438229745966293197333606e-12) }},{{ SC_(1.8189894035458564758300781250000000000000000000000e-12), SC_(0.5), SC_(1.8189894035458564758300778742278718245786649666697e-12) }},{{ SC_(9.0949470177292823791503906250000000000000000000000e-13), SC_(0.5), SC_(9.0949470177292823791503903115348397807233312083370e-13) }},{{ SC_(4.5474735088646411895751953125000000000000000000000e-13), SC_(0.5), SC_(4.5474735088646411895751952733168549725904164010421e-13) }},{{ SC_(2.2737367544323205947875976562500000000000000000000e-13), SC_(0.5), SC_(2.2737367544323205947875976513521068715738020501303e-13) }},{{ SC_(1.1368683772161602973937988281250000000000000000000e-13), SC_(0.5), SC_(1.1368683772161602973937988275127633589467252562663e-13) }},{{ SC_(5.6843418860808014869689941406250000000000000000000e-14), SC_(0.5), SC_(5.6843418860808014869689941398597041986834065703329e-14) }},{{ SC_(2.8421709430404007434844970703125000000000000000000e-14), SC_(0.5), SC_(2.8421709430404007434844970702168380248354258212916e-14) }},{{ SC_(1.4210854715202003717422485351562500000000000000000e-14), SC_(0.5), SC_(1.4210854715202003717422485351442922531044282276615e-14) }},{{ SC_(7.1054273576010018587112426757812500000000000000000e-15), SC_(0.5), SC_(7.1054273576010018587112426757663028163805352845768e-15) }},{{ SC_(3.5527136788005009293556213378906250000000000000000e-15), SC_(0.5), SC_(3.5527136788005009293556213378887566020475669105721e-15) }},{{ SC_(1.7763568394002504646778106689453125000000000000000e-15), SC_(0.5), SC_(1.7763568394002504646778106689450789502559458638215e-15) }},{{ SC_(8.8817841970012523233890533447265625000000000000000e-16), SC_(0.5), SC_(8.8817841970012523233890533447262705628199323297769e-16) }},{{ SC_(4.4408920985006261616945266723632812500000000000000e-16), SC_(0.5), SC_(4.4408920985006261616945266723632447578524915412221e-16) }},{{ SC_(2.2204460492503130808472633361816406250000000000000e-16), SC_(0.5), SC_(2.2204460492503130808472633361816360634815614426528e-16) }},{{ SC_(1.1102230246251565404236316680908203125000000000000e-16), SC_(0.5), SC_(1.1102230246251565404236316680908197423101951803316e-16) }},{{ SC_(5.5511151231257827021181583404541015625000000000000e-17), SC_(0.5), SC_(5.5511151231257827021181583404541008497627439754145e-17) }},{{ SC_(2.7755575615628913510590791702270507812500000000000e-17), SC_(0.5), SC_(2.7755575615628913510590791702270506921578429969268e-17) }},{{ SC_(1.3877787807814456755295395851135253906250000000000e-17), SC_(0.5), SC_(1.3877787807814456755295395851135253794884803746159e-17) }},{{ SC_(6.9388939039072283776476979255676269531250000000000e-18), SC_(0.5), SC_(6.9388939039072283776476979255676269392043504682698e-18) }},{{ SC_(3.4694469519536141888238489627838134765625000000000e-18), SC_(0.5), SC_(3.4694469519536141888238489627838134748224188085337e-18) }},{{ SC_(1.7347234759768070944119244813919067382812500000000e-18), SC_(0.5), SC_(1.7347234759768070944119244813919067380637398510667e-18) }},{{ SC_(8.6736173798840354720596224069595336914062500000000e-19), SC_(0.5), SC_(8.6736173798840354720596224069595336911343623138334e-19) }},{{ SC_(4.3368086899420177360298112034797668457031250000000e-19), SC_(0.5), SC_(4.3368086899420177360298112034797668456691390392292e-19) }},{{ SC_(2.1684043449710088680149056017398834228515625000000e-19), SC_(0.5), SC_(2.1684043449710088680149056017398834228473142549036e-19) }},{{ SC_(1.0842021724855044340074528008699417114257812500000e-19), SC_(0.5), SC_(1.0842021724855044340074528008699417114252502193630e-19) }},{{ SC_(5.4210108624275221700372640043497085571289062500000e-20), SC_(0.5), SC_(5.4210108624275221700372640043497085571282424617037e-20) }},{{ SC_(2.7105054312137610850186320021748542785644531250000e-20), SC_(0.5), SC_(2.7105054312137610850186320021748542785643701514630e-20) }},{{ SC_(1.3552527156068805425093160010874271392822265625000e-20), SC_(0.5), SC_(1.3552527156068805425093160010874271392822161908079e-20) }},{{ SC_(6.7762635780344027125465800054371356964111328125000e-21), SC_(0.5), SC_(6.7762635780344027125465800054371356964111198478848e-21) }},{{ SC_(3.3881317890172013562732900027185678482055664062500e-21), SC_(0.5), SC_(3.3881317890172013562732900027185678482055647856731e-21) }},{{ SC_(1.6940658945086006781366450013592839241027832031250e-21), SC_(0.5), SC_(1.6940658945086006781366450013592839241027830005529e-21) }},{{ SC_(8.4703294725430033906832250067964196205139160156250e-22), SC_(0.5), SC_(8.4703294725430033906832250067964196205139157624099e-22) }},{{ SC_(4.2351647362715016953416125033982098102569580078125e-22), SC_(0.5), SC_(4.2351647362715016953416125033982098102569579761606e-22) }},{{ SC_(2.1175823681357508476708062516991049051284790039062e-22), SC_(0.5), SC_(2.1175823681357508476708062516991049051284789999498e-22) }},{{ SC_(1.0587911840678754238354031258495524525642395019531e-22), SC_(0.5), SC_(1.0587911840678754238354031258495524525642395014586e-22) }},{{ SC_(5.2939559203393771191770156292477622628211975097656e-23), SC_(0.5), SC_(5.2939559203393771191770156292477622628211975091474e-23) }},{{ SC_(2.6469779601696885595885078146238811314105987548828e-23), SC_(0.5), SC_(2.6469779601696885595885078146238811314105987548055e-23) }},{{ SC_(1.3234889800848442797942539073119405657052993774414e-23), SC_(0.5), SC_(1.3234889800848442797942539073119405657052993774317e-23) }},{{ SC_(6.6174449004242213989712695365597028285264968872070e-24), SC_(0.5), SC_(6.6174449004242213989712695365597028285264968871950e-24) }},{{ SC_(3.3087224502121106994856347682798514142632484436035e-24), SC_(0.5), SC_(3.3087224502121106994856347682798514142632484436020e-24) }},{{ SC_(1.6543612251060553497428173841399257071316242218018e-24), SC_(0.5), SC_(1.6543612251060553497428173841399257071316242218016e-24) }},{{ SC_(8.2718061255302767487140869206996285356581211090088e-25), SC_(0.5), SC_(8.2718061255302767487140869206996285356581211090086e-25) }},{{ SC_(4.1359030627651383743570434603498142678290605545044e-25), SC_(0.5), SC_(4.1359030627651383743570434603498142678290605545044e-25) }}
    } };
    do_test_ellint_e2<T>(small_angles, type_name, "Elliptic Integral E: Small Angles");
    //
    // Test error handling:
    //
    BOOST_CHECK_EQUAL(boost::math::ellint_2(T(1)), T(1));
    BOOST_CHECK_EQUAL(boost::math::ellint_2(T(-1)), T(1));
    BOOST_CHECK_THROW(boost::math::ellint_2(T(1.5)), std::domain_error);
    BOOST_CHECK_THROW(boost::math::ellint_2(T(-1.5)), std::domain_error);
    BOOST_CHECK_THROW(boost::math::ellint_2(T(1.5), T(1.5)), std::domain_error);
}

