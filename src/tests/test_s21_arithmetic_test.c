#include <check.h>

#include "../convertors/convertors_functions.h"
#include "../decimal.h"
#define FALSE 0
#define TRUE 1
#define s21_NAN 3
#define POZITIVE_INF 1
#define NEGATIVE_INF 2
#define OK 0


START_TEST(s21_sub_normal) {
  s21_decimal value_1 = {{155}};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 155;
  value_2.bits[0] = 55;
  s21_sub(value_1, value_2, &result);
  s21_decimal test = {0};
  test.bits[0] = 100;
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  memset(&result, 0, sizeof(s21_decimal));
  value_1.bits[0] = 50;
  value_2.bits[0] = 150;
  s21_sub(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(result.fields.sign, 1);
  memset(&result, 0, sizeof(s21_decimal));
  s21_set_degree(&value_2, 2);
  test.bits[0] = 485;
  s21_sub(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(s21_get_degree(result), 1);
  ck_assert_uint_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(s21_sub_inf) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 55;
  s21_set_sign(&value_1);
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_uint_eq(res, NEGATIVE_INF);
}
END_TEST

START_TEST(s21_sub_one_sign) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 150;
  value_2.bits[0] = 50;
  s21_set_sign(&value_1);
  s21_sub(value_1, value_2, &result);
  s21_decimal test = {0};
  test.bits[0] = 200;
  ck_assert_int_eq(result.bits[0], test.bits[0]);
  int sign = s21_get_sign(result);
  ck_assert_int_eq(sign, 1);
  memset(&result, 0, sizeof(s21_decimal));
  s21_delete_sign(&value_1);
  s21_set_sign(&value_2);
  s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], test.bits[0]);
  int sign1 = s21_get_sign(result);
  ck_assert_int_eq(sign1, 0);
}
END_TEST

START_TEST(s21_sub_two_sign) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 150;
  value_2.bits[0] = 50;
  s21_set_sign(&value_1);
  s21_set_sign(&value_2);

  s21_sub(value_1, value_2, &result);
  s21_decimal test = {0};
  test.bits[0] = 100;
  ck_assert_int_eq(result.bits[0], test.bits[0]);
  int sign = s21_get_sign(result);
  ck_assert_int_eq(sign, 1);
  memset(&result, 0, sizeof(s21_decimal));
  s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], test.bits[0]);
  int sign1 = s21_get_sign(result);
  ck_assert_int_eq(sign1, 1);
  value_1.bits[0] = 50;
  value_2.bits[0] = 150;
  s21_set_sign(&value_1);
  s21_set_sign(&value_2);
  memset(&result, 0, sizeof(s21_decimal));
  s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], test.bits[0]);
  sign1 = s21_get_sign(result);
  ck_assert_int_eq(sign1, 0);
}
END_TEST

START_TEST(s21_div_normal) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 150;
  value_2.bits[0] = 10;
  s21_div(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], 15);
  value_1.bits[0] = 4294967295;
  value_2.bits[0] = 123478698;
  s21_set_degree(&value_2, 5);
  memset(&result, 0, sizeof(s21_decimal));
  s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(s21_get_degree(result), 22);
  memset(&result, 0, sizeof(s21_decimal));
  memset(&value_1, 0, sizeof(s21_decimal));
  memset(&value_2, 0, sizeof(s21_decimal));
  value_1.bits[0] = 4294967294;
  value_1.bits[1] = 1;
  value_2.bits[0] = 2;
  s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], 4294967295);
}
END_TEST

START_TEST(s21_div_scale) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 150;
  value_2.bits[0] = 2;
  s21_set_degree(&value_2, 1);
  s21_div(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], 750);
}
END_TEST

START_TEST(s21_div_zero) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 150;
  value_2.bits[0] = 0;

  int result1 = s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(result1, s21_NAN);
  s21_set_sign(&value_2);
  ck_assert_uint_eq(result1, s21_NAN);
}
END_TEST

START_TEST(s21_div_negative) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 547;
  value_2.bits[0] = 5;
  test.bits[0] = 1094;
  s21_set_sign(&value_2);
  s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  int pow = s21_get_degree(result);
  ck_assert_uint_eq(pow, 1);
  ck_assert_uint_eq(s21_get_sign(result), 1);
  s21_decimal value_3 = {{20}};
  s21_decimal value_4 = {{5}};
  s21_set_sign(&value_4);
  test.bits[0] = 4;
  memset(&result, 0, sizeof(s21_decimal));
  s21_div(value_3, value_4, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(s21_div_pi) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 22;
  value_2.bits[0] = 7;
  test.bits[0] = 3297921316;
  test.bits[1] = 208949406;
  test.bits[2] = 1703746271;
  s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(result.bits[1], test.bits[1]);
  ck_assert_uint_eq(result.bits[2], test.bits[2]);
  int pow = s21_get_degree(result);
  ck_assert_uint_eq(pow, 28);
}
END_TEST

START_TEST(s21_div_normalize) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 4294967295;
  value_2.bits[0] = 2;
  test.bits[0] = 4294967291;
  test.bits[1] = 4;
  s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(result.bits[1], test.bits[1]);
  ck_assert_uint_eq(s21_get_degree(result), 1);
}
END_TEST

START_TEST(s21_div_min_pow) {
  s21_decimal value_1 = {{3333388888}};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_2.bits[0] = 4294967295;
  value_2.bits[1] = 4294967295;
  test.bits[0] = 1409956271;
  test.bits[1] = 4207;
  test.bits[2] = 0;
  s21_set_degree(&value_1, 5);
  s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(s21_get_degree(result), 28);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(result.bits[1], test.bits[1]);
}
END_TEST

START_TEST(s21_div_min_pow_second_dec) {
  s21_decimal value_1 = {{3333388888}};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_2.bits[0] = 22647;
  test.bits[0] = 3777176791;
  test.bits[1] = 1197098455;
  test.bits[2] = 797913073;
  s21_set_degree(&value_1, 5);
  s21_set_degree(&value_2, 3);
  s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(s21_get_degree(result), 25);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(result.bits[1], test.bits[1]);
}
END_TEST

START_TEST(s21_div_pow_small) {
  s21_decimal value_1 = {{291}};
  s21_decimal value_2 = {{123}};
  s21_decimal result = {0};
  s21_decimal test = {0};
  test.bits[0] = 425568406;
  test.bits[1] = 790584587;
  test.bits[2] = 1282531838;
  s21_set_degree(&value_2, 28);
  s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(s21_get_degree(result), 0);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(result.bits[1], test.bits[1]);
  ck_assert_uint_eq(result.bits[2], test.bits[2]);
}
END_TEST

START_TEST(s21_div_max) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 4294967295;
  value_2.bits[1] = 4294967295;
  value_2.bits[2] = 4294967295;
  test.bits[0] = 1;
  s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(s21_get_degree(result), 0);
}
END_TEST

START_TEST(s21_div_pos_over) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 2;
  s21_set_degree(&value_2, 5);
  int over = s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(over, POZITIVE_INF);
  s21_set_sign(&value_1);
  s21_set_sign(&value_2);
  ck_assert_uint_eq(over, POZITIVE_INF);
}
END_TEST

START_TEST(s21_div_neg_over) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 2;
  s21_set_degree(&value_2, 5);
  s21_set_sign(&value_1);
  int over = s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(over, NEGATIVE_INF);
  s21_delete_sign(&value_1);
  s21_set_sign(&value_2);
  ck_assert_uint_eq(over, NEGATIVE_INF);
}
END_TEST

START_TEST(s21_div_one) {
  s21_decimal value_1 = {{745}};
  s21_decimal value_2 = {{1}};
  s21_decimal result = {0};
  s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], value_1.bits[0]);
  ck_assert_uint_eq(s21_get_sign(result), 0);
  memset(&result, 0, sizeof(s21_decimal));
  s21_set_sign(&value_2);
  s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], value_1.bits[0]);
  ck_assert_uint_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(s21_div_pow_norm) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 112;
  test.bits[0] = 1227133512;//1840700270
  test.bits[1] = 2454267026;//3681400539
  test.bits[2] = 3834792228;//383479222
  s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(result.bits[1], test.bits[1]);
  ck_assert_uint_eq(result.bits[2], test.bits[2]);
  ck_assert_uint_eq(s21_get_degree(result), 2);
}
END_TEST

START_TEST(s21_mul_value_2_is_one) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 15;
  value_2.bits[0] = 1;
  test.bits[0] = 15;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
}
END_TEST

START_TEST(s21_mul_value_1_is_zero) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 0;
  value_2.bits[0] = 100;
  test.bits[0] = 0;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
}
END_TEST

START_TEST(s21_mul_value_2_is_zero) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 7;
  value_2.bits[0] = 0;
  test.bits[0] = 0;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
}
END_TEST

START_TEST(s21_mul_max) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 5;
  test.bits[0] = 0;
  test.bits[1] = 0;
  test.bits[2] = 2147483648;
  s21_set_degree(&value_2, 1);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(s21_get_degree(result), 0);
  ck_assert_uint_eq(s21_get_sign(result), 0);
  ck_assert_uint_eq(res, 0);
}
END_TEST

START_TEST(s21_mul_big_power) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 4294967295;
  value_2.bits[0] = 255;
  test.bits[0] = 4294967041;
  test.bits[1] = 254;
  s21_set_degree(&value_2, 10);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(s21_get_degree(result), 10);
  ck_assert_uint_eq(s21_get_sign(result), 0);
  ck_assert_uint_eq(res, 0);
}
END_TEST

START_TEST(s21_mul_big_negative_over) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 15;
  s21_set_degree(&value_2, 1);
  s21_set_sign(&value_2);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(res, NEGATIVE_INF);
}
END_TEST

START_TEST(s21_mul_big_over) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 15;
  s21_set_degree(&value_2, 1);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(res, POZITIVE_INF);
}
END_TEST

START_TEST(s21_mul_norm) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 15;
  s21_set_degree(&value_2, 4);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(s21_get_degree(result), 1);
  ck_assert_uint_eq(res, OK);
}
END_TEST

START_TEST(s21_mul_value_1_is_minus) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  s21_set_sign(&value_1);
  value_1.bits[0] = 15;
  value_2.bits[0] = 1;
  test.bits[0] = 15;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(s21_mul_value_2_is_minus) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 15;
  s21_set_sign(&value_2);
  value_2.bits[0] = 1;
  test.bits[0] = 15;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(s21_mul_value_1_is_zero_minus) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  s21_set_sign(&value_1);
  value_1.bits[0] = 0;
  value_2.bits[0] = 100;
  test.bits[0] = 0;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(s21_mul_value_2_is_zero_minus) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 7;
  s21_set_sign(&value_2);
  value_2.bits[0] = 0;
  test.bits[0] = 0;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(s21_mul_value_1_2_is_minus) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  s21_set_sign(&value_1);
  s21_set_sign(&value_2);
  value_1.bits[0] = 0;
  value_2.bits[0] = 100;
  test.bits[0] = 0;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(s21_mul_max_1) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  s21_set_sign(&value_1);
  value_2.bits[0] = 5;
  s21_set_sign(&value_2);
  test.bits[0] = 0;
  test.bits[1] = 0;
  test.bits[2] = 2147483648;
  s21_set_degree(&value_2, 1);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(s21_get_degree(result), 0);
  ck_assert_uint_eq(s21_get_sign(result), 0);
  ck_assert_uint_eq(res, 0);
}
END_TEST

START_TEST(s21_mul_max_2) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 4294967295;
  s21_set_sign(&value_1);
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 5;
  test.bits[0] = 0;
  test.bits[1] = 0;
  test.bits[2] = 2147483648;
  s21_set_degree(&value_2, 1);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(s21_get_degree(result), 0);
  ck_assert_uint_eq(s21_get_sign(result), 1);
  ck_assert_uint_eq(res, 0);
}
END_TEST

START_TEST(s21_mul_max_3) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  s21_set_sign(&value_1);
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 5;
  test.bits[0] = 0;
  test.bits[1] = 0;
  test.bits[2] = 2147483648;
  s21_set_degree(&value_2, 1);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(s21_get_degree(result), 0);
  ck_assert_uint_eq(s21_get_sign(result), 1);
  ck_assert_uint_eq(res, 0);
}
END_TEST

START_TEST(s21_sum_normal) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {{1535}};

  value_1.bits[0] = 150;
  value_2.bits[0] = 35;
  s21_set_degree(&value_2, 1);
  s21_add(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(s21_get_degree(result), 1);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(s21_sum_both_neg) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {{1535}};
  value_1.bits[0] = 150;
  value_2.bits[0] = 35;
  s21_set_sign(&value_1);
  s21_set_sign(&value_2);
  s21_set_degree(&value_2, 1);
  s21_add(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(s21_get_degree(result), 1);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(s21_sum_error) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 35;

  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, POZITIVE_INF);
  s21_set_sign(&value_2);
  s21_set_sign(&value_1);
  memset(&result, 0, sizeof(s21_decimal));
  res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, NEGATIVE_INF);
}
END_TEST

START_TEST(s21_one_greater) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 540;
  value_2.bits[0] = 560;
  s21_decimal test = {{20}};
  s21_set_sign(&value_2);
  s21_add(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(s21_one_sign) {
  s21_decimal value_1 = {{150}};
  s21_decimal value_2 = {{50}};
  s21_decimal result = {0};
  s21_decimal test = {{100}};
  s21_set_sign(&value_1);
  s21_add(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(s21_get_degree(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 1);
  s21_delete_sign(&value_1);
  s21_set_sign(&value_2);
  memset(&result, 0, sizeof(s21_decimal));
  s21_add(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(s21_get_degree(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST


START_TEST(test){
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{2, 0, 0, 0}};
  s21_decimal res = {0};
  s21_mul(a, b, &res);
  ck_assert_uint_eq(res.bits[0], 10);
}
END_TEST
Suite *test_s21_arithmetic_suite(void) {

  Suite *s = suite_create("s21_arithmetic");

  TCase *tc_mul, *tc_sub, *tc_div, *tc_sum , *tc_test;

  tc_mul = tcase_create("s21_mul");
  tcase_add_test(tc_mul, s21_mul_value_2_is_one);
  tcase_add_test(tc_mul, s21_mul_value_1_is_zero);
  tcase_add_test(tc_mul, s21_mul_value_2_is_zero);
  tcase_add_test(tc_mul, s21_mul_max);
  tcase_add_test(tc_mul, s21_mul_big_power);
  tcase_add_test(tc_mul, s21_mul_big_over);
  tcase_add_test(tc_mul, s21_mul_norm);
  tcase_add_test(tc_mul, s21_mul_big_negative_over);
  tcase_add_test(tc_mul, s21_mul_value_1_is_minus);
  tcase_add_test(tc_mul, s21_mul_value_2_is_minus);
  tcase_add_test(tc_mul, s21_mul_value_1_is_zero_minus);
  tcase_add_test(tc_mul, s21_mul_value_2_is_zero_minus);
  tcase_add_test(tc_mul, s21_mul_value_1_2_is_minus);
  tcase_add_test(tc_mul, s21_mul_max_1);
  tcase_add_test(tc_mul, s21_mul_max_2);
  tcase_add_test(tc_mul, s21_mul_max_3);
  suite_add_tcase(s, tc_mul);

  tc_sub = tcase_create("sub");
  tcase_add_test(tc_sub, s21_sub_normal);
  tcase_add_test(tc_sub, s21_sub_one_sign);
  tcase_add_test(tc_sub, s21_sub_two_sign);
  tcase_add_test(tc_sub, s21_sub_inf);
  suite_add_tcase(s, tc_sub);

  tc_sum = tcase_create("sum");
  tcase_add_test(tc_sum, s21_sum_normal);
  tcase_add_test(tc_sum, s21_sum_both_neg);
  tcase_add_test(tc_sum, s21_one_sign);
  tcase_add_test(tc_sum, s21_sum_error);
  tcase_add_test(tc_sum, s21_one_greater);
  suite_add_tcase(s, tc_sum);

  tc_div = tcase_create("div");
  tcase_add_test(tc_div, s21_div_normal);
  tcase_add_test(tc_div, s21_div_scale);
  tcase_add_test(tc_div, s21_div_zero);
  tcase_add_test(tc_div, s21_div_negative);
  tcase_add_test(tc_div, s21_div_normalize);
  tcase_add_test(tc_div, s21_div_max);
  tcase_add_test(tc_div, s21_div_pow_norm);
  tcase_add_test(tc_div, s21_div_pi);
  tcase_add_test(tc_div, s21_div_pos_over);
  tcase_add_test(tc_div, s21_div_neg_over);
  tcase_add_test(tc_div, s21_div_one);
  tcase_add_test(tc_div, s21_div_min_pow);
  tcase_add_test(tc_div, s21_div_min_pow_second_dec);
  tcase_add_test(tc_div, s21_div_pow_small);
  suite_add_tcase(s, tc_div);

  tc_test = tcase_create("s21_test");
  tcase_add_test(tc_test, test);
  suite_add_tcase(s, tc_test);
  return s;
}
