#include "test.h"

#include "../decimal.h"

START_TEST(test_s21_truncate_positive_no_fraction) {
  s21_decimal input = {{10, 0, 0, 0}};  // 10.0
  s21_decimal result;
  ck_assert_int_eq(s21_truncate(input, &result), 0);
  ck_assert_int_eq(result.bits[0], 10);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_truncate_positive_with_fraction) {
  s21_decimal input = {{105, 0, 0, 1 << 16}};  // 10.5
  s21_decimal result;
  ck_assert_int_eq(s21_truncate(input, &result), 0);
  ck_assert_int_eq(result.bits[0], 10);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_truncate_negative_with_fraction) {
  s21_decimal input = {{105, 0, 0, (1 << 16) | (1 << 31)}};  // -10.5
  s21_decimal result;
  ck_assert_int_eq(s21_truncate(input, &result), 0);
  ck_assert_int_eq(result.bits[0], 10);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], (1 << 31));
}
END_TEST

START_TEST(test_s21_truncate_null_result) {
  s21_decimal input = {{105, 0, 0, (1 << 16) | (1 << 31)}};
  ck_assert_int_eq(s21_truncate(input, NULL), 1);
}
END_TEST

START_TEST(test_s21_truncate_max_positive) {
  s21_decimal input = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};  // Max value
  s21_decimal result;
  ck_assert_int_eq(s21_truncate(input, &result), 0);
  ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[2], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_truncate_min_negative) {
  s21_decimal input = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1 << 31}};  // Min value
  s21_decimal result;
  ck_assert_int_eq(s21_truncate(input, &result), 0);
  ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[2], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[3], 1 << 31);
}
END_TEST

START_TEST(test_s21_floor_positive_no_fraction) {
  s21_decimal input = {{10, 0, 0, 0}};  // 10.0
  s21_decimal result;
  ck_assert_int_eq(s21_floor(input, &result), 0);
  ck_assert_int_eq(result.bits[0], 10);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_floor_positive_with_fraction) {
  s21_decimal input = {{105, 0, 0, 1 << 16}};  // 10.5
  s21_decimal result;
  ck_assert_int_eq(s21_floor(input, &result), 0);
  ck_assert_int_eq(result.bits[0], 10);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_floor_negative_with_fraction) {
  s21_decimal input = {{105, 0, 0, (1 << 16) | (1 << 31)}};  // -10.5
  s21_decimal result;
  ck_assert_int_eq(s21_floor(input, &result), 0);
  ck_assert_int_eq(result.bits[0], 11);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], (1 << 31));
}
END_TEST

START_TEST(test_s21_floor_max_positive) {
  s21_decimal input = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};  // Max value
  s21_decimal result;
  ck_assert_int_eq(s21_floor(input, &result), 0);
  ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[2], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_floor_min_negative) {
  s21_decimal input = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1 << 31}};  // Min value
  s21_decimal result;
  ck_assert_int_eq(s21_floor(input, &result), 0);
  ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[2], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[3], 1 << 31);
}
END_TEST

Suite *s21_other_tests(void) {
  Suite *s = suite_create(BLUE "s21_other_tests" NOCOLOR);
  TCase *tc_truncate = tcase_create("truncate");
  TCase *tc_floor = tcase_create("floor");

  tcase_add_test(tc_truncate, test_s21_truncate_positive_no_fraction);
  tcase_add_test(tc_truncate, test_s21_truncate_positive_with_fraction);
  tcase_add_test(tc_truncate, test_s21_truncate_negative_with_fraction);
  tcase_add_test(tc_truncate, test_s21_truncate_null_result);
  tcase_add_test(tc_truncate, test_s21_truncate_max_positive);
  tcase_add_test(tc_truncate, test_s21_truncate_min_negative);

  tcase_add_test(tc_floor, test_s21_floor_positive_no_fraction);
  tcase_add_test(tc_floor, test_s21_floor_positive_with_fraction);
  tcase_add_test(tc_floor, test_s21_floor_negative_with_fraction);
  tcase_add_test(tc_floor, test_s21_floor_max_positive);
  tcase_add_test(tc_floor, test_s21_floor_min_negative);

  suite_add_tcase(s, tc_truncate);
  suite_add_tcase(s, tc_floor);

  return s;
}
