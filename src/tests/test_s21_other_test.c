#include "test.h"
#include "../decimal.h"

START_TEST(test_s21_truncate_positive_no_fraction) {
  s21_decimal input = {{10, 0, 0, 0}};
  s21_decimal result;
  ck_assert_uint_eq(s21_truncate(input, &result), 0);
  ck_assert_uint_eq(result.bits[0], 10);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_truncate_positive_with_fraction) {
  s21_decimal input = {{105, 0, 0, 1 << 16}};
  s21_decimal result;
  ck_assert_uint_eq(s21_truncate(input, &result), 0);
  ck_assert_uint_eq(result.bits[0], 10);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_truncate_negative_with_fraction) {
  s21_decimal input = {{105, 0, 0, (1 << 16) | (1 << 31)}};
  s21_decimal result;
  ck_assert_uint_eq(s21_truncate(input, &result), 0);
  ck_assert_uint_eq(result.bits[0], 10);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], (1U << 31));
}
END_TEST

START_TEST(test_s21_truncate_null_result) {
  s21_decimal input = {{105, 0, 0, (1 << 16) | (1 << 31)}};
  ck_assert_uint_eq(s21_truncate(input, NULL), 1);
}
END_TEST

START_TEST(test_s21_floor_positive_no_fraction) {
  s21_decimal input = {{10, 0, 0, 0}};
  s21_decimal result;
  ck_assert_uint_eq(s21_floor(input, &result), 0);
  ck_assert_uint_eq(result.bits[0], 10);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_floor_negative_with_fraction) {
  s21_decimal input = {{105, 0, 0, (1 << 16) | (1 << 31)}};
  s21_decimal result;
  ck_assert_uint_eq(s21_floor(input, &result), 0);
  ck_assert_uint_eq(result.bits[0], 11);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], (1U << 31));
}
END_TEST

START_TEST(test_s21_round_positive_up) {
  s21_decimal input = {{105, 0, 0, 1 << 16}};
  s21_decimal result;
  ck_assert_uint_eq(s21_round(input, &result), 0);
  ck_assert_uint_eq(result.bits[0], 11);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_round_negative_down) {
  s21_decimal input = {{104, 0, 0, (1 << 16) | (1 << 31)}};
  s21_decimal result;
  ck_assert_uint_eq(s21_round(input, &result), 0);
  ck_assert_uint_eq(result.bits[0], 10);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], (1U << 31));
}
END_TEST

START_TEST(test_s21_negate_positive) {
  s21_decimal input = {{10, 0, 0, 0}};
  s21_decimal result;
  ck_assert_uint_eq(s21_negate(input, &result), 0);
  ck_assert_uint_eq(result.bits[0], 10);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], (1U << 31));
}
END_TEST

START_TEST(test_s21_negate_negative) {
  s21_decimal input = {{10, 0, 0, 1 << 31}};
  s21_decimal result;
  ck_assert_uint_eq(s21_negate(input, &result), 0);
  ck_assert_uint_eq(result.bits[0], 10);
  ck_assert_uint_eq(result.bits[1], 0);
  ck_assert_uint_eq(result.bits[2], 0);
  ck_assert_uint_eq(result.bits[3], 0);
}
END_TEST

Suite *test_s21_other_tests(void) {
  Suite *s = suite_create("s21_other_tests");
  TCase *tc_truncate = tcase_create("truncate");
  TCase *tc_floor = tcase_create("floor");
  TCase *tc_round = tcase_create("round");
  TCase *tc_negate = tcase_create("negate");

  tcase_add_test(tc_truncate, test_s21_truncate_positive_no_fraction);
  tcase_add_test(tc_truncate, test_s21_truncate_positive_with_fraction);
  tcase_add_test(tc_truncate, test_s21_truncate_negative_with_fraction);
  tcase_add_test(tc_truncate, test_s21_truncate_null_result);
  tcase_add_test(tc_floor, test_s21_floor_positive_no_fraction);
  tcase_add_test(tc_floor, test_s21_floor_negative_with_fraction);

  tcase_add_test(tc_round, test_s21_round_positive_up);
  tcase_add_test(tc_round, test_s21_round_negative_down);

  tcase_add_test(tc_negate, test_s21_negate_positive);
  tcase_add_test(tc_negate, test_s21_negate_negative);

  suite_add_tcase(s, tc_truncate);
  suite_add_tcase(s, tc_floor);
  suite_add_tcase(s, tc_round);
  suite_add_tcase(s, tc_negate);

  return s;
}
