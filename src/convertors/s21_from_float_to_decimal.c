#include "convertors_functions.h"

#ifdef MAX
#undef MAX
#define MAX 79228162514264337593543950335.0F
#endif
#ifdef MIN
#undef MIN
#define MIN 1e-28
#endif

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (src == S21_POSITIVE_INF || src != src || dst == NULL ||
      (fabsf(src) < MIN && fabsf(src) > 0) || fabsf(src) > MAX) {
    return CONVERT_ERROR;
  }
  memset(dst, 0, sizeof(s21_decimal));
  int sign = 0;
  if (src < 0) {
    sign = 1;
  }
  long double src_work = fabsf(src);
  long double exp_src = src_work;
  int exp = 0;
  while (exp_src >= 1) {  //  получаем количество знаков до точки
    exp_src /= 10.0;
    exp++;
  }
  if (exp == 0) {
    exp = 1;
  }
  src_work *= powl((long double)10.0, (long double)(28.0 - exp));
  //  получаем src домноженный до 28 знаков

  char bit[128] = {'\0'};
  for (int i = 0; src_work > 1e-6; i++) {
    src_work = floorl(src_work) / 2;
    if (src_work - floorl(src_work) > 1e-6) {
      bit[i] = '1';
    } else {
      bit[i] = '0';
    }
  }

  for (size_t i = strlen(bit); i > 0; i--) {
    if (bit[i - 1] - '0') {
      s21_set_bit(dst, i - 1);
    }
  }
  s21_set_degree(dst, 28 - exp);
  if (sign) {
    s21_set_sign(dst);
  }
  return CONVERT_OK;
}
