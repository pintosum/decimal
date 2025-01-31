#include "convertors_functions.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int degree = s21_get_degree(src);
  if (degree) {
    s21_truncate(src, &src);
  }
  if (dst == NULL || src.bits[1] != 0 || src.bits[2] != 0) {
    return CONVERT_ERROR;
  }
  if ((src.bits[0] & 0x80000000) != 0) {
    if (src.bits[0] == 0x80000000) {
      *dst = MIN;
      return CONVERT_OK;
    }
    return CONVERT_ERROR;
  } else {
    if (src.bits[0] > MAX) {
      return CONVERT_ERROR;
    }
  }
  *dst = 0;
  for (int i = 0; i < 31; i++) {
    int temp = s21_get_bit(src, i);
    int power = (1U << i) * temp;
    *dst += power;
  }
  int sign = s21_get_sign(src);
  if (sign) {
    *dst *= -1;
  }
  return CONVERT_OK;
}