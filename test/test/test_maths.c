#include "unity.h"
#include "maths.h"
#include <stdio.h>
#include <string.h>
#include <gsl/gsl_matrix.h>
#include <xc.h>
#include "test_maths_fixtures.inc"
#include "exception.h"
#include "eigen3x3.h"


void suiteSetUp(void) {
    exception_init();
}



void test_find_median(void) {
    TEST_ASSERT_EQUAL_INT16(5, find_median((int16_t[]){3,5,6},3));
    TEST_ASSERT_EQUAL_INT16(5, find_median((int16_t[]){5},1));
    TEST_ASSERT_EQUAL_INT16(5, find_median((int16_t[]){4,5,99},3));
    TEST_ASSERT_EQUAL_INT16(5, find_median((int16_t[]){4,5,99},3));
    TEST_ASSERT_EQUAL_INT16(5, find_median((int16_t[]){4,5,99},3));
    TEST_ASSERT_EQUAL_INT16(0, find_median((int16_t[]){-10,0,1},3));
}

void test_cross_product(void) {
    char text[24];
    double dictionary[12][3][3] = {
        {{0,0,0}, {0,0,0}, {0,0,0}},
        {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
        {{1.966021,0.996371,1.456462},{0.950379,0.894036,1.886508},{0.577531,-2.324722,0.810763}},
        {{0.080797,1.369236,0.536854},{1.393359,0.653164,1.249846},{1.360681,0.647046,-1.855063}},
        {{0.487519,0.220578,1.785382},{1.130160,1.245123,1.146483},{-1.970131,1.458834,0.357733}},
        {{1.781504,1.491375,1.293967},{0.491547,1.181605,0.133473},{-1.329900,0.398263,1.371954}},
        {{0.928645,0.048362,0.300580},{0.877071,1.622896,1.308908},{-0.424509,-0.951881,1.464677}},
        {{1.473763,1.755599,0.533648},{0.850665,1.980126,0.610196},{0.014570,-0.445330,1.424811}},
        {{1.866873,0.693270,1.198487},{0.981590,1.248449,1.572356},{-0.406183,-1.758966,1.650190}},
        {{0.245519,1.642427,0.415957},{0.467185,0.636266,1.353699},{1.958693,-0.138030,-0.611103}},
        {{0.893915,1.630456,0.372925},{0.158390,0.384927,1.943209},{3.024769,-1.677997,0.085844}},
        {{0.123774,0.762176,0.541222},{0.779024,0.305382,0.958143},{0.564994,0.303032,-0.555955}},
    };
    double result[3];
    int i;
    for (i=0; i<12; i++) {
        cross_product(dictionary[i][0], dictionary[i][1], result);
        snprintf(text,24,"Iteration: %d",i);
        TEST_ASSERT_EQUAL_DOUBLE_ARRAY_MESSAGE(dictionary[i][2], result, 3, text);
    }
}

void test_distance2(void) {
    char text[24];
    struct test_field {
        vectorr a;
        vectorr b;
        float d;
    };
    struct test_field test_cases[12] = {
        {{0,0,0}, {0,0,0}, 0},
        {{0,0,0}, {1,1,0.5}, 1.5},
        {{1.845249,-1.601869,-0.196011}, {-0.583596,-1.369197,-0.639945}, 2.480020},
        {{0.888094,-1.085290,-1.288514}, {1.723163,1.216220,0.854184}, 3.253528},
        {{0.782409,-0.009716,-0.660612}, {-1.606374,-0.808486,1.946461}, 3.625072},
        {{1.771424,0.083158,1.763770}, {-1.727456,1.653323,-1.723321}, 5.183375},
        {{-1.868018,1.375409,1.905125}, {0.219487,1.185364,-1.818671}, 4.273224},
        {{1.956783,0.556068,1.465883}, {-0.322669,0.348396,1.699103}, 2.300744},
        {{1.480682,1.706582,-0.764786}, {-0.054106,-1.162548,-0.167927}, 3.308129},
        {{0.617992,1.701167,-0.953138}, {-0.836447,-1.843415,1.828230}, 4.734497},
        {{0.524529,1.527086,0.252254}, {1.606613,-1.199171,-1.679043}, 3.511879},
        {{-1.680228,0.429262,-1.084019}, {-1.194537,-1.268790,1.358341}, 3.014033}
    };
    int i;
    float result;
    for (i=0; i<12; i++) {
        result = distance2(test_cases[i].a, test_cases[i].b);
        snprintf(text,24,"Iteration: %d",i);
        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(test_cases[i].d*test_cases[i].d, result, text);
    }
}

void test_apply_matrix(void) {
    char text[24];
    struct test_field {
        vectorr a;
        matrixx b;
        vectorr d;
    };
    struct test_field test_cases[10] = {
        {{0.79098467,0.07649972,0.24209043}, {{0.6230616,0.30957003,0.26718755,0.70437192}, {0.96695392,0.01832908,0.13572914,0.11834949}, {0.57523733,0.8322825,0.53136917,0.57118746}}, {1.28556967,0.91745611,1.21850014},},
        {{0.91862726,0.99028628,0.86438511}, {{0.37872779,0.71727596,0.32032065,0.84977538}, {0.28809744,0.63951441,0.61745101,0.1683465}, {0.79533361,0.36612074,0.44494214,0.60766597}}, {2.184874,1.60001847,2.0854468},},
        {{0.99167319,0.08990193,0.32353437}, {{0.50475283,0.32204001,0.16970497,0.23372424}, {0.57450737,0.96530449,0.14518835,0.53995854}, {0.1201111,0.94011728,0.82712555,0.97063062}}, {0.8181315,1.24343825,1.44186348},},
        {{0.19317896,0.86949998,0.05271839}, {{0.22060202,0.90845232,0.29496543,0.49251093}, {0.95270281,0.92981365,0.1390425,0.29505907}, {0.49396324,0.82946925,0.22544792,0.7892409}}, {1.34057596,1.29490425,1.61777295},},
        {{0.79147716,0.455724,0.4496157}, {{0.81950733,0.70053661,0.80882163,0.86925039}, {0.28216592,0.84982123,0.99576293,0.76703048}, {0.83005419,0.17858848,0.87457068,0.43589646}}, {2.20078197,1.82535294,1.56747316},},
        {{0.61772688,0.76337261,0.21776282}, {{0.52751824,0.13943648,0.71185735,0.22066658}, {0.94720683,0.60700503,0.72789155,0.75957344}, {0.62406829,0.41574086,0.18694281,0.08817871}}, {0.80798683,1.96656729,0.83175684},},
        {{0.28065251,0.57431041,0.24619685}, {{0.05134379,0.55757276,0.84903185,0.20903587}, {0.50452973,0.51109706,0.33084255,0.49748011}, {0.79307629,0.0790304,0.2495196,0.18502941}}, {0.75269445,1.0140584,0.51442718},},
        {{0.39892813,0.91420688,0.40341234}, {{0.15556849,0.3159763,0.68798851,0.47033212}, {0.73929815,0.23997447,0.24313742,0.18353051}, {0.25029087,0.93577246,0.95457712,0.49456332}}, {1.09880352,0.79592828,1.8349892},},
        {{0.20070548,0.1888553,0.81865248}, {{0.84066256,0.84895541,0.96180203,0.31757581}, {0.91762738,0.50294907,0.16022033,0.94957129}, {0.56017059,0.2593978,0.85514724,0.82548882}}, {1.43401273,1.35989351,1.68697518},},
        {{0.35844997,0.13327308,0.06991459}, {{0.9943198,0.37217212,0.51089757,0.09600686}, {0.30925501,0.35277769,0.21581782,0.6901498}, {0.84408199,0.50912163,0.21479571,0.82976765}}, {0.53774049,0.86310683,1.21519838},}
    };
    int i;
    vectorr d;
    for (i=0; i<3; i++) {
        apply_matrix(test_cases[i].a, test_cases[i].b, d);
        
        snprintf(text,24,"Iteration: %d",i);
        TEST_ASSERT_EQUAL_DOUBLE_ARRAY_MESSAGE(test_cases[i].d, d,3, text);
    }
}


void test_matrix_multiply(void) {
    char text[24];
    struct test_field {
        matrixx delta;
        matrixx result;
    };
    struct test_field test_cases[10] = {
        {{{0.5426,-0.9585,0.2673,0.4976}, {-0.003,-0.5504,-0.6039,0.5211}, {-0.6618,-0.8233,0.3707,0.9068}}, {{0.5426,-0.9585,0.2673,0.4976}, {-0.003,-0.5504,-0.6039,0.5211}, {-0.6618,-0.8233,0.3707,0.9068}}},
        {{{0.4435,-0.4162,0.8355,0.4292}, {0.0851,-0.7157,-0.2533,0.3483}, {-0.1163,-0.132,0.2355,0.0263}}, {{-0.311,-0.8839,0.6797,1.1906}, {0.216,0.5209,0.361,-0.212}, {-0.2186,-0.0098,0.1359,0.1132}}},
        {{{0.8173,-0.3615,-0.8191,-0.3986}, {-0.772,0.6574,-0.9062,0.2526}, {0.0952,0.6386,-0.6021,0.7137}}, {{-0.1532,-0.9027,0.3136,0.5584}, {0.5802,1.0337,-0.4106,-0.9086}, {0.2399,0.2544,0.2134,0.6235}}},
        {{{-0.349,-0.67,-0.2149,-0.8131}, {0.6422,-0.6977,-0.2318,0.8885}, {0.9753,-0.0874,0.6522,-0.4973}}, {{-0.3868,-0.4322,0.1198,-0.5333}, {-0.5588,-1.3599,0.4384,1.7365}, {-0.0437,-0.8048,0.4809,0.5334}}},
        {{{-0.9214,-0.2856,-0.8408,-0.3891}, {-0.3386,0.5477,-0.9201,-0.141}, {-0.3701,0.273,-0.3073,-0.9138}}, {{0.5527,1.4633,-0.6399,-0.8422}, {-0.1349,0.142,-0.2429,0.4998}, {0.004,0.0361,-0.0724,-0.4063}}},
        {{{0.2112,0.0269,0.1957,-0.4756}, {-0.3983,-0.9492,-0.3939,-0.5158}, {0.1152,0.131,-0.0497,-0.4144}}, {{0.2112,0.0269,0.1957,-0.4756}, {-0.3983,-0.9492,-0.3939,-0.5158}, {0.1152,0.131,-0.0497,-0.4144}}},
        {{{0.9542,-0.1185,-0.3635,0.0396}, {0.1563,0.7079,-0.8638,-0.0709}, {0.5639,0.4372,0.172,-0.9258}}, {{0.2068,0.0905,0.2514,-0.2025}, {-0.3484,-0.7809,-0.2053,-0.1524}, {-0.0352,-0.3773,-0.0704,-1.4908}}},
        {{{0.3469,-0.6816,-0.899,-0.3244}, {-0.7839,-0.6422,0.7717,-0.2693}, {-0.5625,0.505,-0.7862,0.4892}}, {{0.3409,0.9028,0.2905,1.0496}, {0.0344,0.1394,-0.1196,-1.1631}, {-0.2645,-0.1486,-0.1897,1.6982}}},
        {{{0.2901,-0.9027,-0.5028,0.0848}, {-0.5465,-0.2372,0.8445,0.8507}, {0.1335,0.0669,-0.9703,0.9558}}, {{0.2008,0.2108,0.2876,0.5855}, {-0.4178,-0.6519,-0.2906,1.9871}, {0.3045,0.2741,0.2148,-0.6297}}},
        {{{0.1684,0.4177,-0.7029,-0.1431}, {0.3878,-0.7908,-0.1208,-0.6676}, {0.014,0.6381,-0.8198,0.6001}}, {{-0.3548,-0.4295,-0.224,1.2281}, {0.3715,0.5642,0.3154,-1.9358}, {-0.5134,-0.6377,-0.3575,2.3924}}},
    };
    matrixx calib;
    int i,j,k;
    for (i=0; i<2; i++) {
        memcpy(calib, identity, sizeof(calib));
        for (j=0; j<5; j++) {
            matrix_multiply(calib, test_cases[i*5+j].delta);
            for (k=0; k<3; k++) {
                snprintf(text,24,"Iteration: %d:%d",i*5+j,k);
                TEST_ASSERT_EQUAL_DOUBLE_ARRAY_MESSAGE(test_cases[i*5+j].result[k], calib[k], 4, text);            
            }
        }
    }
}

void test_apply_offset(void) {
    char text[24];
    struct test_field {
        double x, y, z;
        matrixx initial;
        matrixx result;
    };
    struct test_field test_cases[11] = {
        {2,3,4, {{1,0,0,0}, {0,1,0,0},{0,0,1,0}}, {{1,0,0,2}, {0,1,0,3}, {0,0,1,4}}},
        {-1,-1,-1, {{1,0,0,1}, {0,1,0,1}, {0,0,1,1}}, {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}}},
        {-1.544063, 1.314725, -1.812415, {{0.1702,-1.4313,-0.5066,0.6965}, {-0.2327,-0.2639,0.4711,0.0526}, {0.6016,0.4042,1.2209,0.0866}},  {{0.1702,-1.4313,-0.5066,-0.8475}, {-0.2327,-0.2639,0.4711,1.3673}, {0.6016,0.4042,1.2209,-1.7258}}},
        {1.950502, -0.174782, 1.304491, {{0.1903,1.2771,-1.2042,1.4274}, {-0.5934,1.0186,-0.8162,1.5357}, {-0.698,-1.3399,-0.4299,-1.6262}},  {{0.1903,1.2771,-1.2042,3.3779}, {-0.5934,1.0186,-0.8162,1.361}, {-0.698,-1.3399,-0.4299,-0.3217}}},
        {1.519661, 1.052962, 1.512387, {{0.3895,1.6113,0.1382,0.3608}, {-1.8429,-0.5713,-1.6815,-0.7782}, {-0.6771,1.0953,-1.8402,-0.282}},  {{0.3895,1.6113,0.1382,1.8805}, {-1.8429,-0.5713,-1.6815,0.2748}, {-0.6771,1.0953,-1.8402,1.2304}}},
        {1.908323, -0.236905, -0.726909, {{0.4223,0.0539,0.3913,-0.9511}, {-0.7965,-1.8984,-0.7877,-1.0317}, {0.2303,0.262,-0.0995,-0.8288}},  {{0.4223,0.0539,0.3913,0.9572}, {-0.7965,-1.8984,-0.7877,-1.2686}, {0.2303,0.262,-0.0995,-1.5557}}},
        {-1.567745, -1.284389, 1.543308, {{0.3125,1.4157,-1.7276,-0.1419}, {1.1278,0.8744,0.3441,-1.8516}, {-0.5974,0.2528,-0.8011,0.0493}},  {{0.3125,1.4157,-1.7276,-1.7096}, {1.1278,0.8744,0.3441,-3.136}, {-0.5974,0.2528,-0.8011,1.5926}}},
        {0.267000, 0.133884, -1.940560, {{-1.1249,1.01,-1.5725,0.9784}, {-0.1209,0.393,-1.4095,-1.2639}, {0.5803,-1.8055,-1.0055,0.1696}},  {{-1.1249,1.01,-1.5725,1.2454}, {-0.1209,0.393,-1.4095,-1.13}, {0.5803,-1.8055,-1.0055,-1.7709}}},
        {0.260505, 0.357391, -1.207597, {{0.2921,1.167,0.2462,1.5093}, {0.3368,0.8354,-1.4059,-0.2862}, {0.7756,-1.5815,-0.2416,-1.3352}},  {{0.2921,1.167,0.2462,1.7698}, {0.3368,0.8354,-1.4059,0.0712}, {0.7756,-1.5815,-0.2416,-2.5428}}},
        {1.001858, -1.295831, -0.165943, {{-0.8164,-1.8498,-1.8773,-0.1876}, {0.9795,0.2292,-0.4595,-1.3277}, {1.353,0.3962,1.1309,1.394}},  {{-0.8164,-1.8498,-1.8773,0.8143}, {0.9795,0.2292,-0.4595,-2.6235}, {1.353,0.3962,1.1309,1.2281}}},
        {0.622907, 1.214273, -0.595146, {{-0.0639,1.3775,-1.3007,-1.9415}, {1.3951,0.9707,-0.1732,-0.3324}, {-1.5331,-0.6453,-1.6214,0.8633}},  {{-0.0639,1.3775,-1.3007,-1.3186}, {1.3951,0.9707,-0.1732,0.8819}, {-1.5331,-0.6453,-1.6214,0.2682}}},
    };
    matrixx result;
    int i,k;
    for (i=0; i<11; i++) {
        memcpy(result, test_cases[i].initial, sizeof(result));
        apply_offset(test_cases[i].x, test_cases[i].y, test_cases[i].z, result);
        for (k=0; k<3; k++) {
            snprintf(text,24,"Iteration: %d:%d", i, k);
            TEST_ASSERT_EQUAL_DOUBLE_ARRAY_MESSAGE(test_cases[i].result[k], result[k], 4, text);            
        }
    }
}
   
void test_apply_2d_rotation(void) {
    char text[80];
    struct test_field {
        int axes[2];
        double vector[2]; 
        matrixx matrix;
        matrixx result;
    };
    struct test_field test_cases[10] = {
        {{0,2}, {0, 1.0}, {{1.,0.,0.,2.}, {0.,1.,0.,3.}, {0.,0.,1.,0.}},  {{0.,0.,1.,0.}, {0.,1.,0.,3.}, {-1.,0.,0.,-2.}}},
        {{0,2}, {-0.26008969749116223, 0.96558445993033448}, {{-1.917,0.5346,0.9952,-0.006}, {-1.1008,-1.2077,1.0421,-1.3236}, {-1.6466,0.7414,1.8136,-1.9842}},  {{-1.0914,0.5769,1.4923,-1.9144}, {-1.1008,-1.2077,1.0421,-1.3236}, {2.2793,-0.709,-1.4327,0.5218}}},
        {{1,2}, {-0.84828585888627372, 0.52953857424514117}, {{-1.4742,-0.3453,1.1149,0.3356}, {-1.2695,1.3043,-1.5784,-0.8657}, {-1.7377,-1.7742,1.0618,-1.9528}},  {{-1.4742,-0.3453,1.1149,0.3356}, {0.1567,-2.046,1.9012,-0.2998}, {2.1463,0.8144,-0.0649,2.115}}},
        {{2,0}, {-0.54092941491530455, -0.84106799254244979}, {{1.4122,-0.2401,-1.5122,0.9269}, {-1.4449,1.0675,1.328,-0.7609}, {0.3903,1.4896,1.9321,-0.1304}},  {{-0.4356,1.3827,2.443,-0.6111}, {-1.4449,1.0675,1.328,-0.7609}, {-1.3989,-0.6039,0.2267,-0.7091}}},
        {{1,2}, {-0.74117307808825206, 0.67131398638579376}, {{-0.2546,-0.575,0.3485,-1.4021}, {-1.315,-0.4113,0.5518,-0.5099}, {-1.9904,0.1953,-1.4921,-1.6808}},  {{-0.2546,-0.575,0.3485,-1.4021}, {-0.3615,0.436,-1.4107,-0.7504}, {2.358,0.1314,0.7355,1.5881}}},
        {{2,1}, {-0.7178668014923334, 0.69618047610886558}, {{-1.5414,0.3321,-1.0694,0.2261}, {0.0952,-0.5962,0.8213,1.2828}, {-1.4617,0.4189,-0.8674,0.0256}},  {{-1.5414,0.3321,-1.0694,0.2261}, {0.9493,0.1363,0.0143,-0.9387}, {1.1156,-0.7158,1.1945,0.8747}}},
        {{2,0}, {0.66514606292459622, -0.74671327494287199}, {{-1.4377,-0.4478,1.466,1.8274}, {-0.0538,-1.5763,-0.6399,1.3423}, {-0.1382,-1.6286,1.4188,-0.651}},  {{-1.0595,-1.5139,2.0345,0.7294}, {-0.0538,-1.5763,-0.6399,1.3423}, {0.9816,-0.7489,-0.151,-1.7975}}},
        {{1,2}, {0.59981613999177186, 0.80013786199965009}, {{-0.8011,0.0493,0.6939,-1.3632}, {-1.7981,-0.6487,-1.5677,-1.2844}, {1.5433,-0.5385,-1.1249,1.01}},  {{-0.8011,0.0493,0.6939,-1.3632}, {0.1563,-0.82,-1.8405,0.0377}, {2.3644,0.1961,0.5797,1.6335}}},
        {{1,0}, {-0.86330309182238496, -0.50468581478966767}, {{0.5803,-1.8055,-1.0055,0.1696}, {-1.0929,-0.4744,1.6889,1.7014}, {0.267,0.1339,-1.9406,1.9116}},  {{-1.0525,1.3193,1.7205,0.7122}, {0.6506,1.3207,-0.9506,-1.5545}, {0.267,0.1339,-1.9406,1.9116}}},
        {{1,2}, {0.9814718615423802, 0.19160632818499224}, {{-1.4059,-0.2862,0.7756,-1.5815}, {-0.2416,-1.3352,0.0279,1.2761}, {-1.6396,1.2003,0.2605,0.3574}},  {{-1.4059,-0.2862,0.7756,-1.5815}, {-0.5513,-1.0805,0.0773,1.321}, {-1.5629,1.4339,0.2503,0.1063}}},
    };
    matrixx result;
    int i, k;
    for (i=0; i<10; i++) {
        memcpy(result, test_cases[i].matrix, sizeof(result));
        apply_2d_rotation(test_cases[i].axes, test_cases[i].vector, result);
        for (k=0; k<3; k++) {   
            snprintf(text,24,"Iteration: %d:%d", i, k);
            TEST_ASSERT_EQUAL_DOUBLE_ARRAY_MESSAGE(test_cases[i].result[k], result[k], 4, text);            
        }
    }
}

void test_apply_2d_scale(void) {
    char text[80];
    struct test_field {
        int axis;
        double scale; 
        matrixx matrix;
        matrixx result;
    };
    struct test_field test_cases[10] = {
        {1, 3.000000, {{1.,0.,0.,2.}, {0.,1.,0.,3.}, {0.,0.,1.,0.}},  {{1.,0.,0.,2.}, {0.,3.,0.,9.}, {0.,0.,1.,0.}}},
        {1, 1.510460, {{-0.805,-0.0216,-0.2279,1.3276}, {0.3333,-1.8993,0.8368,-0.9377}, {-0.9456,-1.3985,0.7353,1.2664}},  {{-0.805,-0.0216,-0.2279,1.3276}, {0.5034,-2.8688,1.264,-1.4164}, {-0.9456,-1.3985,0.7353,1.2664}}},
        {0, -1.638163, {{1.6711,0.8583,0.1702,-1.4313}, {-0.5066,0.6965,-0.2327,-0.2639}, {0.4711,0.0526,0.6016,0.4042}},  {{-2.7375,-1.406,-0.2788,2.3447}, {-0.5066,0.6965,-0.2327,-0.2639}, {0.4711,0.0526,0.6016,0.4042}}},
        {2, -1.474836, {{-0.3553,-1.6926,1.4122,-0.2401}, {-1.5122,0.9269,-1.4449,1.0675}, {1.328,-0.7609,0.3903,1.4896}},  {{-0.3553,-1.6926,1.4122,-0.2401}, {-1.5122,0.9269,-1.4449,1.0675}, {-1.9585,1.1222,-0.5757,-2.1969}}},
        {1, 1.095321, {{-1.3954,-0.4635,1.777,1.9505}, {-0.1748,1.3045,-0.9945,0.3895}, {1.6113,0.1382,0.3608,-1.8429}},  {{-1.3954,-0.4635,1.777,1.9505}, {-0.1914,1.4288,-1.0893,0.4266}, {1.6113,0.1382,0.3608,-1.8429}}},
        {2, -1.998910, {{-0.4685,-1.1005,-1.5414,0.3321}, {-1.0694,0.2261,0.0952,-0.5962}, {0.8213,1.2828,-1.4617,0.4189}},  {{-0.4685,-1.1005,-1.5414,0.3321}, {-1.0694,0.2261,0.0952,-0.5962}, {-1.6418,-2.5642,2.9219,-0.8374}}},
        {1, 0.883194, {{0.9903,-1.4377,-0.4478,1.466}, {1.8274,-0.0538,-1.5763,-0.6399}, {1.3423,-0.1382,-1.6286,1.4188}},  {{0.9903,-1.4377,-0.4478,1.466}, {1.614,-0.0475,-1.3922,-0.5651}, {1.3423,-0.1382,-1.6286,1.4188}}},
        {1, -0.120859, {{-0.5974,0.2528,-0.8011,0.0493}, {0.6939,-1.3632,-1.7981,-0.6487}, {-1.5677,-1.2844,1.5433,-0.5385}},  {{-0.5974,0.2528,-0.8011,0.0493}, {-0.0839,0.1648,0.2173,0.0784}, {-1.5677,-1.2844,1.5433,-0.5385}}},
        {1, -1.326588, {{1.8488,1.2618,0.7769,-1.1572}, {0.0714,1.7987,0.7839,1.6187}, {0.5916,-0.6188,-1.738,-1.1175}},  {{1.8488,1.2618,0.7769,-1.1572}, {-0.0947,-2.3861,-1.0399,-2.1473}, {0.5916,-0.6188,-1.738,-1.1175}}},
        {2, -0.516391, {{1.2687,1.2499,-1.2463,-0.5078}, {0.9768,-1.255,-0.3448,-1.076}, {0.6145,1.0575,1.3395,-1.1511}},  {{1.2687,1.2499,-1.2463,-0.5078}, {0.9768,-1.255,-0.3448,-1.076}, {-0.3173,-0.5461,-0.6917,0.5944}}},
    };
    matrixx result;
    int i, k;
    for (i=0; i<10; i++) {
        memcpy(result, test_cases[i].matrix, sizeof(result));
        apply_scale(test_cases[i].axis, test_cases[i].scale, result);
        for (k=0; k<3; k++) {   
            snprintf(text,24,"Iteration: %d:%d", i, k);
            TEST_ASSERT_EQUAL_DOUBLE_ARRAY_MESSAGE(test_cases[i].result[k], result[k], 4, text);            
        }
    }
}

void test_normalise(void) {
    char text[80];
    struct test_field {
        vectorr a;
        int len;
        vectorr result;
    };
    struct test_field test_cases[14] = {
        {{1,0,5}, 1, {1,0,6}},
        {{-1,0,0}, 1, {-1, 0, 0}},
        {{2,0,0}, 1, {1, 0, 0}},
        {{3,4,0}, 2, {0.6, 0.8, 0}},
        {{1.0853,-1.917,0.5346}, 3, {0.4788,-0.8457,0.2358}},
        {{-0.006,-1.1008,-1.2077}, 3, {-0.0037,-0.6736,-0.7391}},
        {{-1.3236,-1.6466,0.7414}, 3, {-0.5911,-0.7354,0.3312}},
        {{-1.9842,0.0488,1.2505}, 3, {-0.8458,0.0208,0.5331}},
        {{0.887,-0.8325,1.6711}, 3, {0.4291,-0.4028,0.8085}},
        {{0.1702,-1.4313,-0.5066}, 3, {0.1114,-0.9368,-0.3316}},
        {{-0.2327,-0.2639,0.4711}, 3, {-0.3957,-0.4489,0.8012}},
        {{0.6016,0.4042,1.2209}, 3, {0.4237,0.2847,0.8599}},
        {{1.6346,-0.7231,-1.6382}, 3, {0.6742,-0.2982,-0.6757}},
        {{-1.5441,1.3147,-1.8124}, 3, {-0.5677,0.4834,-0.6664}},    
    };
    vectorr result;
    double temp;
    int i,j;
    for (i=0; i<14; i++) {
        memcpy(result, test_cases[i].a, sizeof(result));
        normalise(result, test_cases[i].len);
        snprintf(text,24,"Iteration: %d", i);
        TEST_ASSERT_EQUAL_DOUBLE_ARRAY_MESSAGE(test_cases[i].result, result, test_cases[i].len, text);
        temp = 0;
        for (j=0; j < test_cases[i].len; j++) {
            temp += result[j]*result[j];
        }
        TEST_ASSERT_EQUAL_DOUBLE(1.0, temp);
    }
}

void test_amax() {
    struct test_field {
        double a;
        double b;
        double result;
    };
    struct test_field test_cases[7] = {
        {0, 0, 0},
        {1, 2, 2},
        {2, 1, 2},
        {1, -1, 1},
        {-1, 1, 1},
        {1, -100, 1},
        {100, -1, 100}
    };
    int i;
    for (i=0; i<7; i++) {
        TEST_ASSERT_EQUAL_DOUBLE(test_cases[i].result,amax(test_cases[i].a, test_cases[i].b));
    }
}

void test_amin() {
    struct test_field {
        double a;
        double b;
        double result;
    };
    struct test_field test_cases[7] = {
        {0, 0, 0},
        {1, 2, 1},
        {2, 1, 1},
        {1, -1, -1},
        {-1, 1, -1},
        {1, -100, -100},
        {100, -1, -1}
    };
    int i;
    for (i=0; i<7; i++) {
        TEST_ASSERT_EQUAL_DOUBLE(test_cases[i].result,amin(test_cases[i].a, test_cases[i].b));
    }
}

void test_aabs() {
    char text[80];
    struct test_field {
        double a;
        double result;
    };
    struct test_field test_cases[7] = {
        {0, 0},
        {1, 1},
        {2, 2},
        {-1, 1},
        {-2, 2},
        {1.3, 1.3},
        {-1.3, 1.3}
    };
    int i;
    for (i=0; i<7; i++) {
        snprintf(text, 80, "Iteration: %d", i);
        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(test_cases[i].result, aabs(test_cases[i].a), text);
    }
}

void test_get_rotation_matrix() {
    char text[80];
    struct test_field {
        int axes[2];
        double theta;
        matrixx result;
    };
    struct test_field test_cases[] = {
        {{0,1}, 0, {{1,0,0,0},{0,1,0,0},{0,0,1,0}}},
        {{0,1}, M_PI/2, {{0,1,0,0},{-1,0,0,0},{0,0,1,0}}},
        {{1,0}, M_PI/2, {{0,-1,0,0},{1,0,0,0},{0,0,1,0}}},
        {{0,2}, M_PI/2, {{0,0,1,0},{0,1,0,0},{-1,0,0,0}}},
        {{0,1}, M_PI/4, {{0.707,0.707,0,0},{-0.707,0.7070,0,0},{0,0,1,0}}},
    };
    matrixx result;
    int i, k;
    for (i=0; i<5; i++) {
        get_rotation_matrix(test_cases[i].axes, test_cases[i].theta, result);
        for (k=0; k<3; k++) {   
            snprintf(text,24,"Iteration: %d:%d", i, k);
            TEST_ASSERT_EQUAL_DOUBLE_ARRAY_MESSAGE(test_cases[i].result[k], result[k], 4, text);            
        }
    }
}

void test_find_rotation_and_scale_of_ellipse() {
    char text[80];
    struct test_field {
        vectorr *data;
        int axes[2];
        int len;
        int precision;
        double scale;
        double theta; // in degrees
    };
    struct test_field test_cases[5] = {
        {simple1, {0, 1}, 4, 100, 2.0, 0},
        {simple1, {1, 0}, 4, 100, 2.0, M_PI/3},
        {simple2, {2, 0}, 20, 20, 1.2, 2.423},
        {simple3, {1, 2}, 20, 20, 1.2, 1.679},
        {simple4, {0, 1}, 20, 20, 1.2, 1.475},
    };
    int i;
    struct ELLIPSE_PARAM result;
    
    for (i=0; i<5; i++) {
        snprintf(text, 80, "Iteration : %d", i);
        result = find_rotation_and_scale_of_ellipse(test_cases[i].data, test_cases[i].axes, test_cases[i].len, test_cases[i].precision);
        TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(0.1, test_cases[i].scale, result.scale, text);
        //TEST_ASSERT_EQUAL_DOUBLE(-1, result.vector[1]);
        TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(0.25, test_cases[i].theta, result.theta, text);
    }
}
    
void test_find_plane() {
    char text[80];
    struct test_field {
        vectorr data[40];
        int axes[2];
        vectorr result;
    };
    struct test_field test_cases[5] = {
        {{{1.4361,-9.8286,-10.1366}, {9.9884,-9.9385,1.1253}, {-6.6328,-9.8855,-7.6774}, {-0.0254,-9.7941,-10.2421}, {-9.935,-10.0334,-0.132}, {1.6615,-10.1799,9.7162}, {3.3312,-10.1533,9.2227}, {0.742,-9.7566,-10.1455}, {4.9652,-10.1697,8.5427}, {8.5954,-10.077,5.0616}, {-3.9089,-9.8443,-9.39}, {9.6088,-9.8379,-3.0529}, {10.0823,-9.9565,0.0546}, {-9.9141,-10.105,-0.9717}, {3.9039,-9.759,-9.4798}, {-7.5386,-9.9598,-6.6886}, {-1.6807,-9.7874,-10.0059}, {-2.5128,-10.1825,9.4901}, {8.7222,-9.8084,-5.1258}, {-2.1366,-9.8183,-9.983}, {-9.5338,-10.051,-2.8424}, {6.3807,-10.0796,7.5425}, {-6.9272,-10.2319,6.9386}, {-4.5538,-9.8342,-9.0984}, {-9.2918,-10.1324,3.3318}, {-9.0601,-10.1171,3.8082}, {-7.3503,-9.8885,-6.9547}, {-9.9003,-10.0217,-0.9945}, {-5.8068,-9.848,-8.3134}, {-7.9752,-9.9271,-6.1047}, {3.5147,-9.7369,-9.6292}, {-9.7944,-10.0648,-1.5736}, {8.428,-9.8562,-5.676}, {-4.1609,-10.1686,8.8253}, {8.4676,-10.0098,5.1787}, {-3.1018,-10.1754,9.2868}, {7.6414,-10.0847,6.3417}, {4.8542,-9.7825,-8.9707}, {9.608,-9.9951,2.7099}, {-6.9373,-9.87,-7.3322}},
        {2,0},
        {0.0076,-0.9998,-0.0199}},
        {{{5.4973,-10.7044,-7.3374}, {-10.1745,-9.2956,3.2009}, {4.6797,-10.6932,-7.9246}, {8.6244,-10.2187,4.5696}, {-8.932,-9.2155,5.8948}, {-4.7062,-10.3115,-8.4737}, {3.1802,-9.6766,9.856}, {-1.1971,-9.3476,10.5538}, {9.3631,-10.453,1.8787}, {-7.1004,-10.0809,-6.9898}, {6.8212,-10.7671,-6.205}, {-10.0048,-9.3099,3.8351}, {-7.6581,-10.0208,-6.4099}, {-9.5438,-9.2433,4.8834}, {9.2055,-10.6013,-1.7498}, {9.346,-10.5504,-0.9525}, {-6.4104,-10.1385,-7.4518}, {-4.314,-9.2486,9.8255}, {-10.1141,-9.5928,-2.1202}, {9.4379,-10.5761,-0.2698}, {6.3866,-9.9284,7.7283}, {2.0776,-10.6397,-9.0386}, {-2.1089,-9.317,10.4325}, {-9.5645,-9.7101,-3.6365}, {8.8891,-10.2091,4.0843}, {2.3964,-9.6046,10.0807}, {-3.4103,-9.3154,10.0966}, {-8.3013,-9.1975,6.8683}, {-9.328,-9.2395,5.2098}, {-2.7725,-9.2784,10.2877}, {-6.2968,-10.133,-7.6092}, {-4.703,-9.2951,9.5776}, {7.4892,-10.0628,6.65}, {6.0183,-9.8855,8.0768}, {6.7193,-10.7579,-6.2994}, {-10.4592,-9.4201,1.6197}, {8.5364,-10.6406,-3.7013}, {-8.0921,-9.9189,-5.9541}, {0.1054,-9.4164,10.5651}, {-3.229,-10.4016,-9.0716}},
        {0,2},
        {-0.0515,-0.9971,0.0563}},
        {{{-10.7342,3.5935,8.4611}, {-9.5158,-7.2504,-7.5772}, {-9.4292,9.2935,-4.9984}, {-9.1522,-3.4273,-10.2308}, {-9.7872,-9.0098,-4.7713}, {-10.8444,-0.6755,9.0357}, {-10.0114,-9.6668,-2.575}, {-9.0001,3.576,-10.3474}, {-10.9401,-2.6273,8.6051}, {-10.7382,4.2379,8.1908}, {-9.8306,10.0785,0.296}, {-9.0186,-0.2034,-10.8189}, {-9.9856,9.9301,1.4088}, {-10.5497,-8.9257,3.0709}, {-9.1094,5.1494,-9.5619}, {-9.8756,10.1915,-0.1001}, {-10.1163,-9.7561,-1.5537}, {-10.2875,-9.7531,0.3286}, {-9.9477,9.9985,0.916}, {-9.8458,10.1513,-0.2228}, {-10.8116,-4.7319,7.7346}, {-10.1377,9.5033,2.8737}, {-9.5532,-7.9913,-6.6628}, {-9.2451,7.7503,-7.4353}, {-10.5682,-8.282,4.3366}, {-10.0184,9.8701,1.4303}, {-10.2603,8.9605,3.9376}, {-10.1928,8.9466,4.0163}, {-9.0124,3.8222,-10.1418}, {-8.9849,3.3474,-10.4163}, {-9.8686,10.1854,0.1205}, {-9.1181,6.5675,-8.5887}, {-9.3171,8.6225,-6.1939}, {-10.6006,-7.9268,4.8847}, {-10.9158,-2.0912,8.7346}, {-9.0646,5.7055,-9.1756}, {-9.4454,-7.2073,-7.5823}, {-10.5313,6.5245,6.7523}, {-10.8225,1.5652,8.9239}, {-9.0943,5.5844,-9.3049}},
        {2,1},
        {-0.9956,0.0194,-0.092}},
        {{{5.9681,-9.8609,8.2149}, {-2.1782,-10.4752,-9.2783}, {4.1761,-9.6725,9.4132}, {-10.4501,-9.4672,-1.217}, {6.164,-9.9467,7.977}, {-10.582,-9.3433,0.0569}, {-10.5132,-9.2686,1.8881}, {3.552,-9.6386,9.7084}, {0.87,-10.6972,-9.2627}, {2.945,-10.7978,-8.6677}, {9.2304,-10.4346,2.2795}, {-8.3611,-9.879,-5.6352}, {9.0311,-10.3478,3.4105}, {-6.0386,-10.1214,-7.7838}, {8.9999,-10.3536,3.4543}, {9.3122,-10.5418,1.5075}, {1.298,-10.6895,-9.1182}, {5.1612,-9.831,8.7545}, {8.8783,-10.3858,3.7504}, {-3.2694,-10.3847,-9.016}, {5.8532,-9.8804,8.2688}, {9.3369,-10.5504,1.602}, {-10.1864,-9.1524,3.2561}, {6.2243,-10.8871,-6.6448}, {-9.0194,-9.7894,-4.743}, {-10.5198,-9.2833,1.6695}, {-9.7554,-9.1652,4.593}, {9.2329,-10.4552,2.54}, {-10.5564,-9.3201,0.6883}, {-7.6394,-9.052,7.6583}, {8.8693,-10.6922,-2.2828}, {1.0556,-10.703,-9.1721}, {9.1888,-10.4037,2.5085}, {-6.5135,-9.0963,8.7052}, {-3.4874,-9.1622,10.1389}, {9.0478,-10.4083,2.9242}, {1.7191,-9.5263,10.3033}, {-4.6482,-10.2497,-8.5122}, {7.5562,-10.0603,6.4541}, {4.9251,-9.7631,8.9609}},
        {0,2},
        {-0.062,-0.996,0.0639}},
        {{{-0.4021,-10.7091,-9.2394}, {10.326,-3.1123,-9.1573}, {-4.0442,-9.5471,-9.5618}, {-6.2952,-7.9283,-9.868}, {10.2998,1.8258,-9.5159}, {-5.1542,7.2769,-10.9708}, {-1.4265,8.9743,-10.8033}, {-9.1165,1.4818,-10.798}, {-2.5891,8.6834,-10.8765}, {4.498,-9.9494,-9.0237}, {1.5669,-10.7048,-9.1002}, {-7.4773,-6.6666,-9.9904}, {8.6871,-6.5893,-8.9738}, {-8.6816,2.7481,-10.8546}, {-5.8089,6.869,-10.9045}, {-9.2141,0.4198,-10.6357}, {10.4484,-2.4545,-9.1856}, {10.2772,2.0023,-9.5344}, {0.8734,-10.6961,-9.2086}, {-6.8348,5.779,-10.9239}, {-8.1191,-5.6305,-10.1909}, {10.2646,1.7781,-9.4891}, {10.23,-3.4338,-9.0746}, {-9.0224,1.833,-10.8268}, {-0.4554,-10.6358,-9.2865}, {-1.0098,-10.5783,-9.327}, {-5.7226,6.8664,-10.9581}, {-8.2405,-5.3276,-10.1703}, {9.1068,4.6073,-9.7666}, {3.344,-10.3183,-8.9977}, {-8.3172,3.6513,-10.8749}, {-7.3693,-6.6619,-9.9822}, {10.0693,2.7062,-9.635}, {9.2627,4.2931,-9.7357}, {10.5447,0.4171,-9.3917}, {-5.1724,-8.8831,-9.6997}, {-8.5561,3.0987,-10.8411}, {8.305,5.738,-9.9383}, {10.0438,2.5654,-9.5447}, {-5.7917,-8.3402,-9.8034}},
        {1,0},
        {0.0648,-0.076,-0.995}}
    };
    int i;
    vectorr result;
    for (i=0; i<5; i++) {
        find_plane(test_cases[i].data, test_cases[i].axes, 40, result);
        snprintf(text, 80, "Iteration: %d", i);
        TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(0.01, 0, distance2(result, test_cases[i].result), text);
    }
}

void test_sqrtm() {
    char text[80];
    struct test_field {
        double data[9];
        double result[9];
    };
    struct test_field test_cases[5] = {
        {{3.7713,0.3848,0.4159, 0.3848,3.4985,0.4927, 0.4159,0.4927,3.1691} , {1.9365,0.0975,0.1087, 0.0975,1.8632,0.1326, 0.1087,0.1326,1.7719}},
        {{3.0883,0.3447,0.783, 0.3447,3.5122,0.7672, 0.783,0.7672,3.2919} , {1.7419,0.0833,0.2169, 0.0833,1.861,0.2052, 0.2169,0.2052,1.7896}},
        {{3.9178,0.4284,0.4922, 0.4284,3.3733,0.5541, 0.4922,0.5541,3.6178} , {1.9725,0.108,0.1233, 0.108,1.8277,0.1454, 0.1233,0.1454,1.8925}},
        {{3.5131,0.7278,0.4601, 0.7278,3.5216,0.4996, 0.4601,0.4996,3.3007} , {1.8608,0.1913,0.1186, 0.1913,1.8623,0.1299, 0.1186,0.1299,1.8082}},
        {{3.114,0.7275,0.1229, 0.7275,3.5476,0.8381, 0.1229,0.8381,3.3517} , {1.7531,0.2,0.0217, 0.2,1.8591,0.2268, 0.0217,0.2268,1.8165}}
    };
    int i;
    double result_array[9];
    gsl_matrix_view result;
    gsl_matrix_view data;
    result = gsl_matrix_view_array(result_array, 3, 3);
    for (i = 0; i<5; i++) {
        data = gsl_matrix_view_array(test_cases[i].data, 3, 3);
        sqrtm(&data.matrix, &result.matrix);
        snprintf(text, 80, "Iteration: %d", i);
        TEST_ASSERT_EQUAL_DOUBLE_ARRAY_MESSAGE(test_cases[i].result, result_array, 9, text);
    }
}

void test_calibrate() {
    CEXCEPTION_T e;
    const char *file;
    const char *reason;
    int line;
    char text[80];
    matrixx result;
    vectorr vector, v_result;
    int k;
    double magnitude;
    printf("Hello\n");
    Try {
    calibrate(cal1, 180, result);
    }
    Catch(e) {
        exception_get_details(&reason, &file, &line);
        printf("%s: %s: %s: %d", exception_get_string(e), reason, file, line);
    }
    for (k=0; k<200; k++) {
        vector[0] = cal1[k*3];
        vector[1] = cal1[k*3+1];
        vector[2] = cal1[k*3+2];
        apply_matrix(vector, result, v_result);
        magnitude = v_result[0]*v_result[0] + v_result[1]*v_result[1] + v_result[2]*v_result[2];
        snprintf(text, 80, "Data: %f, %f, %f, %f", result[0][0], result[1][1], result[1][2], result[1][3]);
        TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(0.05, 1.0, magnitude, text);
    }
}
