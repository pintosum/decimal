#ifndef S21_BINARY_H
#define S21_BINARY_H

#include <stdint.h>

#include "decimal.h"

typedef struct {
  uint32_t mantissa[3];
  uint32_t zero_bytes : 16;
  uint32_t exp : 8;
  uint32_t signal_bits : 7;
  uint32_t sign : 1;
} dec_map;

s21_decimal shift_mantissa_left_one(s21_decimal *value);
s21_decimal shift_mantissa_right_one(s21_decimal *value);
s21_decimal shift_mantissa_left(s21_decimal *value, int shift);
s21_decimal shift_mantissa_right(s21_decimal *value, int shift);
s21_decimal decimal_xor(s21_decimal *val1, s21_decimal *val2);
s21_decimal decimal_and(s21_decimal *val1, s21_decimal *val2);

s21_decimal get_one();
int decimal_is_zero(s21_decimal *value);

s21_decimal twos_complement(s21_decimal *value);
s21_decimal add_mantisses(s21_decimal val1, s21_decimal val2);
s21_decimal sub_mantisses(s21_decimal val1, s21_decimal val2);

s21_decimal mult_by_pow_of_ten(s21_decimal *value, int power);

s21_decimal div_by_ten(s21_decimal value, int *remainder);
s21_decimal normalize_decimal(s21_decimal value);

int most_significant_bit(s21_decimal value);
int len_of_number(s21_decimal value);
int s21_valid_s21_decimal(s21_decimal *val);
void print_dec(s21_decimal, char *);

#endif
