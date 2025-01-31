#include "../binary.h"
#include "../decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int ret = 0;
  if (result) {
    while (value.fields.exp) {
      value = s21_decimal_divide_by_ten(value, NULL);
      value.fields.exp--;
    }
    *result = value;
  } else {
    ret = 1;
  }
  return ret;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int ret = 0;
  unsigned int exp = (value.bits[3] >> 16) & 0xFF;
  ret = s21_truncate(value, result);
  if (exp && ret == 0 && value.bits[3] >> 31) {
    s21_decimal one = {{1, 0, 0, 1 << 31}};
    s21_add(*result, one, result);
  }
  return ret;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int ret = 0;
  if (value.fields.exp) {
    value.fields.exp--;
    ret = s21_truncate(value, &value);
    int last_digit = 0;
    value = s21_decimal_divide_by_ten(value, &last_digit);
    if (ret == 0 && last_digit >= 5) {
      s21_decimal one = {{1, 0, 0, 0}};
      if (value.fields.sign) {
        s21_negate(one, &one);
      }
      s21_add(value, one, &value);
    }
  }
  *result = value;
  return ret;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int ret = 1;
  if (result) {
    value.fields.sign += 1;
    *result = value;
    ret = 0;
  }
  return ret;
}
