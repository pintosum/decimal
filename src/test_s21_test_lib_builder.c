#include "decimal.h"
#include "tests/test.h"

int wrapper_s21_is_less(s21_decimal a, s21_decimal b) {
  return s21_is_less(a, b);
}

int wrapper_s21_is_equal(s21_decimal a, s21_decimal b) {
  return s21_is_equal(a, b);
}

int wrapper_s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return s21_is_not_equal(a, b);
}

int wrapper_s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  return s21_is_less_or_equal(a, b);
}

int wrapper_s21_is_greater(s21_decimal a, s21_decimal b) {
  return s21_is_greater(a, b);
}

int wrapper_s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  return s21_is_greater_or_equal(a, b);
}
