#include "../decimal.h"

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  int result = s21_is_less(a, b) || s21_is_equal(a, b);
  return result;
}
