#ifndef S21_BINARY_H
#define S21_BINARY_H

#include <stdint.h>

#include "decimal.h"

/*typedef struct {
  uint32_t mantissa[3];
  uint32_t zero_bytes : 16;
  uint32_t exp : 8;
  uint32_t signal_bits : 7;
  uint32_t sign : 1;
} dec_map;*/

s21_decimal s21_shift_mantissa_left_one(s21_decimal);
s21_decimal s21_shift_mantissa_right_one(s21_decimal);
s21_decimal s21_shift_mantissa_left(s21_decimal, unsigned int);
s21_decimal s21_shift_mantissa_right(s21_decimal, unsigned int);
s21_decimal s21_decimal_xor(s21_decimal *, s21_decimal *);
s21_decimal s21_decimal_and(s21_decimal *, s21_decimal *);

s21_decimal s21_decimal_get_one();
int s21_decimal_is_zero(s21_decimal *);

s21_decimal s21_decimal_twos_complement(s21_decimal);
s21_decimal s21_add_mantisses(s21_decimal, s21_decimal);
s21_decimal s21_sub_mantisses(s21_decimal, s21_decimal);
s21_decimal s21_div_mantisses(s21_decimal, s21_decimal, s21_decimal *);

s21_decimal s21_decimal_mult_by_pow_of_ten(s21_decimal *, int);

s21_decimal s21_decimal_divide_by_ten(s21_decimal, int *);
s21_decimal s21_normalize_decimal(s21_decimal);

int s21_decimal_most_significant_bit(s21_decimal);
int s21_decimal_len_of_number(s21_decimal);
int s21_is_valid_decimal(s21_decimal *);

void print_dec(s21_decimal, char *);
void print_s21_decimal(s21_decimal, char *);
s21_decimal s21_decimal_from_string(const char *dec);

#endif
