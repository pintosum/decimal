#include "../decimal.h"

int s21_is_greater(s21_decimal a, s21_decimal b) {
  int result = s21_is_less(b, a);
  return result;
}
