#include "../decimal.h"

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  int result = !s21_is_equal(a, b);
  return result;
}
