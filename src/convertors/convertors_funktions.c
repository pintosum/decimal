#include "convertors_functions.h"

int s21_get_bit(s21_decimal number, int position) {
  return (number.bits[position / 32] >> (position % 32)) & 1U;
}

void s21_set_bit(s21_decimal *number, int position) {
  int mask = 1U << (position % 32);
  number->bits[position / 32] |= mask;
}

int s21_get_sign(s21_decimal number) { return (number.bits[3] >> SIGN) & 1U; }

void s21_set_sign(s21_decimal *number) {
  int mask = (1U << SIGN);
  number->bits[3] |= mask;
}

void s21_delete_sign(s21_decimal *number) {
  int mask = ~(1U << SIGN);
  number->bits[3] &= mask;
}

int s21_get_degree(s21_decimal value) {
  int degree = 0;
  int result = 0;
  for (int i = 0; i < END_POWER - BEGIN_POWER; i++) {
    int temp = s21_get_bit(value, 96 + i + BEGIN_POWER);
    degree = (1U << i) * temp;
    // тут вычисления в двоичной системе, но в degree заносится в десятичной
    result += degree;
  }
  return result;
}

void s21_delete_degree(s21_decimal *value) {
  int sign = s21_get_sign(*value);
  value->bits[3] = 0;
  if (sign) {
    s21_set_sign(value);
  }
}

void s21_set_degree(s21_decimal *value, int deg) {
  s21_delete_degree(value);
  for (int i = BEGIN_POWER; i <= END_POWER && deg; i++) {
    if (deg % 2 == 1) {
      s21_set_bit(value, 96 + i);
    }
    deg /= 2;
  }
}
