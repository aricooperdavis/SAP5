#include "unity.h"
#include "storage.h"
//#include "memory.h"
#include "mock_memory.h"
#include "utils.h"
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/tmr1.h"
#include <stdio.h>
#include <string.h>
#include <xc.h>

const int target_array[4] = {1,2,3,4};

void test_write_data(void) {
    int zeros[4] = {0,0,0,0};
    write_dword_ExpectAndReturn((void*)target_array, (void*)zeros,0);
    write_dword_ExpectAndReturn((void*)(target_array+2), (void*)(zeros+2),0);
    write_data((uint8_t*) target_array,(uint8_t *) zeros, sizeof(zeros));
    //TEST_ASSERT_EQUAL_INT32_ARRAY(zeros, target_array, 4);
}

/*void test_find_median(void) {*/
/*    TEST_ASSERT_EQUAL_INT16(5, find_median((int16_t[]){3,5,6},3));*/
/*    TEST_ASSERT_EQUAL_INT16(5, find_median((int16_t[]){5},1));*/
/*    TEST_ASSERT_EQUAL_INT16(5, find_median((int16_t[]){4,5,99},3));*/
/*    TEST_ASSERT_EQUAL_INT16(5, find_median((int16_t[]){4,5,99},3));*/
/*    TEST_ASSERT_EQUAL_INT16(5, find_median((int16_t[]){4,5,99},3));*/
/*    TEST_ASSERT_EQUAL_INT16(0, find_median((int16_t[]){-10,0,1},3));*/
/*}*/

/*void test_cross_product(void) {*/
/*    char text[24];*/
/*    vectorr dictionary[12][3] = {*/
/*        {{0,0,0}, {0,0,0}, {0,0,0}},*/
/*        {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},*/
/*        {{1.966021,0.996371,1.456462},{0.950379,0.894036,1.886508},{0.577531,-2.324722,0.810763}},*/
/*        {{0.080797,1.369236,0.536854},{1.393359,0.653164,1.249846},{1.360681,0.647046,-1.855063}},*/
/*        {{0.487519,0.220578,1.785382},{1.130160,1.245123,1.146483},{-1.970131,1.458834,0.357733}},*/
/*        {{1.781504,1.491375,1.293967},{0.491547,1.181605,0.133473},{-1.329900,0.398263,1.371954}},*/
/*        {{0.928645,0.048362,0.300580},{0.877071,1.622896,1.308908},{-0.424509,-0.951881,1.464677}},*/
/*        {{1.473763,1.755599,0.533648},{0.850665,1.980126,0.610196},{0.014570,-0.445330,1.424811}},*/
/*        {{1.866873,0.693270,1.198487},{0.981590,1.248449,1.572356},{-0.406183,-1.758966,1.650190}},*/
/*        {{0.245519,1.642427,0.415957},{0.467185,0.636266,1.353699},{1.958693,-0.138030,-0.611103}},*/
/*        {{0.893915,1.630456,0.372925},{0.158390,0.384927,1.943209},{3.024769,-1.677997,0.085844}},*/
/*        {{0.123774,0.762176,0.541222},{0.779024,0.305382,0.958143},{0.564994,0.303032,-0.555955}},*/
/*    };*/
/*    vectorr result;*/
/*    float a[3], b[3];*/
/*    int i, j;*/
/*    for (i=0; i<12; i++) {*/
/*        cross_product(dictionary[i][0], dictionary[i][1], result);*/
/*        for (j=0; j<3; j++) {*/
/*            a[j] = dictionary[i][2][j];*/
/*            b[j] = result[j];*/
/*        }*/
/*        snprintf(text,24,"Iteration: %d",i);*/
/*        TEST_ASSERT_EQUAL_FIXED_ARRAY_MESSAGE(dictionary[i][2], result, 3, text);*/
/*    }*/
/*}*/


