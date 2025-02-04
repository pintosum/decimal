#include "test.h"

#include "../convertors/convertors_functions.h"

START_TEST(s21_int_and_decimal_max) {
  s21_decimal value_1 = {0};
  value_1.bits[0] = MAX;
  int a = 0;
  int res = s21_from_decimal_to_int(value_1, &a);
  ck_assert_int_eq(res, CONVERT_OK);

  s21_decimal value_2 = {0};
  a = MAX;
  s21_decimal test = {{0x7FFFFFFF}};
  s21_from_int_to_decimal(a, &value_2);
  ck_assert_int_eq(value_2.bits[0], test.bits[0]);
  ck_assert_int_eq(s21_get_sign(value_2), 0);
}
END_TEST

START_TEST(s21_int_and_decimal_min) {
  s21_decimal value_1 = {0};
  value_1.bits[0] = MIN;
  int a = 0;
  int res = s21_from_decimal_to_int(value_1, &a);
  ck_assert_int_eq(res, CONVERT_OK);
  ck_assert_int_eq(a, MIN);

  s21_decimal value_2 = {0};
  a = MIN;
  s21_decimal test = {{0x80000000}};
  s21_from_int_to_decimal(a, &value_2);
  ck_assert_int_eq(value_2.bits[0], test.bits[0]);
  ck_assert_int_eq(s21_get_sign(value_2), 1);
}
END_TEST

START_TEST(s21_int_and_decimal_big_pow) {
  s21_decimal value_1 = {0};
  int result = 184467440;

  value_1.bits[0] = 344;
  value_1.bits[1] = 4294967295;
  int a = 0;
  s21_set_degree(&value_1, 11);
  s21_from_decimal_to_int(value_1, &a);
  ck_assert_int_eq(a, result);
}
END_TEST

START_TEST(s21_int_and_decimal_error) {
  s21_decimal value_2 = {0};
  int* b = NULL;
  int res = s21_from_decimal_to_int(value_2, b);
  ck_assert_int_eq(res, CONVERT_ERROR);

  s21_decimal* dst = NULL;
  int c = 0;
  res = s21_from_int_to_decimal(c, dst);
  ck_assert_int_eq(res, CONVERT_ERROR);

  s21_decimal value_1 = {0};
  value_1.bits[0] = MIN + 1;
  int a = 0;
  res = s21_from_decimal_to_int(value_1, &a);
  ck_assert_int_eq(res, CONVERT_ERROR);

  s21_decimal value_3 = {0};
  value_3.bits[0] = -1;
  a = 0;
  res = s21_from_decimal_to_int(value_3, &a);
  ck_assert_int_eq(res, CONVERT_ERROR);
}
END_TEST

START_TEST(s21_int_and_decimal_sign) {
  s21_decimal value_1 = {0};
  int a = -5;
  s21_decimal test = {{5}};
  s21_from_int_to_decimal(a, &value_1);
  ck_assert_int_eq(value_1.bits[0], test.bits[0]);
  ck_assert_int_eq(s21_get_sign(value_1), 1);
}
END_TEST

START_TEST(s21_int_and_decimal_whith_pow) {
  s21_decimal value_1 = {0};
  int a = 0;
  value_1.bits[0] = 5;
  s21_set_degree(&value_1, 20);
  s21_from_decimal_to_int(value_1, &a);
  ck_assert_int_eq(a, 0);
}
END_TEST

START_TEST(s21_int_and_decimal_nul) {
  s21_decimal value_1 = {0};
  s21_decimal test = {0};
  int a = 0;
  s21_from_int_to_decimal(a, &value_1);
  ck_assert_int_eq(value_1.bits[0], test.bits[0]);
  ck_assert_int_eq(s21_get_sign(value_1), 0);
  a = 123456;
  test.bits[0] = 123456;
  s21_from_int_to_decimal(a, &value_1);
  ck_assert_int_eq(value_1.bits[0], test.bits[0]);
  s21_set_sign(&value_1);
  int b = 0;
  s21_from_decimal_to_int(value_1, &b);
  ck_assert_int_eq(value_1.bits[0], test.bits[0]);
  ck_assert_int_eq(s21_get_sign(value_1), 1);
}
END_TEST

START_TEST(s21_decimal_float_normal) {
  s21_decimal value = {{8}};
  float a = 0;
  float b = 8.0;
  s21_from_decimal_to_float(value, &a);
  ck_assert_int_eq(s21_get_sign(value), 0);
  ck_assert_float_eq(a, b);
}
END_TEST

START_TEST(s21_decimal_float_null) {
  s21_decimal value = {0};
  float a = 0.0;
  float b = 0.0;
  s21_from_decimal_to_float(value, &a);
  ck_assert_int_eq(s21_get_sign(value), 0);
  ck_assert_float_eq(a, b);
}
END_TEST

START_TEST(s21_decimal_float_degree) {
  s21_decimal value = {{8}};
  float a = 0.0;
  float b = -0.8;
  s21_set_degree(&value, 1);
  s21_set_sign(&value);
  s21_from_decimal_to_float(value, &a);
  ck_assert_float_eq(a, b);
}
END_TEST

START_TEST(s21_decimal_float_big_degree) {
  s21_decimal value = {{8}};
  float a = 0.0;
  float b = -0.0000000000000000000000000008;
  s21_set_degree(&value, 28);
  s21_set_sign(&value);
  s21_from_decimal_to_float(value, &a);
  ck_assert_float_eq(a, b);
}
END_TEST

START_TEST(s21_decimal_float_degree_max_value) {
  s21_decimal value = {0};
  value.bits[0] = 147484107;
  float a = 0.0;
  float b = -1.47484107;
  s21_set_degree(&value, 8);
  s21_set_sign(&value);
  s21_from_decimal_to_float(value, &a);
  ck_assert_float_eq_tol(a, b, 1e-6);
}
END_TEST

START_TEST(s21_decimal_float_degree_max_dec) {
  s21_decimal value = {0};
  value.bits[0] = 214748;

  float a = 0.0;
  float b = -0.00000214748;
  s21_set_degree(&value, 11);
  s21_set_sign(&value);
  s21_from_decimal_to_float(value, &a);
  ck_assert_float_eq_tol(a, b, 1e-6);
}
END_TEST

START_TEST(s21_decimal_float_error) {
  s21_decimal value = {0};
  float* a = NULL;
  int res = s21_from_decimal_to_float(value, a);
  ck_assert_uint_eq(res, CONVERT_ERROR);
}
END_TEST

START_TEST(s21_float_dec_normal) {
  s21_decimal value_2 = {0};
  s21_decimal test = {0};
  float a = 0.8;
  float b = 0.0;
  s21_set_degree(&test, 27);
  int res = s21_from_float_to_decimal(a, &value_2);
  s21_from_decimal_to_float(value_2, &b);
  ck_assert_uint_eq(res, 0);
  ck_assert_uint_eq(s21_get_degree(value_2), s21_get_degree(test));
  ck_assert_float_eq(a, b);
}
END_TEST

START_TEST(s21_float_dec_negative) {
  s21_decimal value_2 = {0};
  s21_decimal test = {0};
  float a = -12.8;
  float b = 0.0;
  s21_set_degree(&test, 26);
  int res = s21_from_float_to_decimal(a, &value_2);
  s21_from_decimal_to_float(value_2, &b);
  ck_assert_uint_eq(res, 0);
  ck_assert_uint_eq(s21_get_degree(value_2), s21_get_degree(test));
  ck_assert_float_eq(a, b);
}
END_TEST

START_TEST(s21_float_dec_error) {
  s21_decimal* value_2 = NULL;
  float a = -12.8;
  int res = s21_from_float_to_decimal(a, value_2);
  ck_assert_uint_eq(res, CONVERT_ERROR);
}
END_TEST

Suite* test_s21_convertors_test(void) {
  Suite* s = suite_create(BLUE "s21_convertors_tests" NOCOLOR);
  TCase* tc = tcase_create("convertors_tc");

  tcase_add_test(tc, s21_int_and_decimal_nul);
  tcase_add_test(tc, s21_int_and_decimal_max);
  tcase_add_test(tc, s21_int_and_decimal_min);
  tcase_add_test(tc, s21_int_and_decimal_whith_pow);
  tcase_add_test(tc, s21_int_and_decimal_big_pow);
  tcase_add_test(tc, s21_int_and_decimal_error);
  tcase_add_test(tc, s21_int_and_decimal_sign);
  tcase_add_test(tc, s21_float_dec_normal);
  tcase_add_test(tc, s21_float_dec_negative);
  tcase_add_test(tc, s21_float_dec_error);
  tcase_add_test(tc, s21_decimal_float_normal);
  tcase_add_test(tc, s21_decimal_float_null);
  tcase_add_test(tc, s21_decimal_float_degree);
  tcase_add_test(tc, s21_decimal_float_big_degree);
  tcase_add_test(tc, s21_decimal_float_degree_max_value);
  tcase_add_test(tc, s21_decimal_float_degree_max_dec);
  tcase_add_test(tc, s21_decimal_float_error);
  suite_add_tcase(s, tc);
  return s;
}
