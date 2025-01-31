#include "convertors_functions.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (dst == NULL) {
    return CONVERT_ERROR;
  } else {
    memset(dst, 0, sizeof(s21_decimal));
    int sign = 0;
    if (src < 0) {
      sign = 1;
      if (src == MIN) {
        dst->bits[0] = 0x80000000;  // это MIN в шестнадцатиричной СС
      } else {
        dst->bits[0] = -src;
      }
    } else {
      dst->bits[0] = src;
    }
    if (sign) {
      s21_set_sign(dst);
    }
  }
  return CONVERT_OK;
}