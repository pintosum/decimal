#include "../decimal.h"

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  int result = !s21_is_less(a, b);
  return result;
}
