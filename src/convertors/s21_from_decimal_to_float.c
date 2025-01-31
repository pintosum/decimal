#include "convertors_functions.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (dst == NULL) {
    return CONVERT_ERROR;
  }
  int sign = s21_get_sign(src);
  int degree = s21_get_degree(src);
  long double result = 0;
  //  переводим в десятичную систему, умножая биты на степень двойки
  for (int i = 0; i < 96; i++) {
    result += pow(2, i) * s21_get_bit(src, i);
  }
  while (degree) {
    result /= 10.0;  //  отделяем дробную часть
    degree--;
  }
  if (sign) {
    result *= -1.0;
  }
  *dst = result;
  return CONVERT_OK;
}
