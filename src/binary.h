#ifndef S21_BINARY_H
#define S21_BINARY_H

#include <stdint.h>

#include "uint256.h"
#define INF 0x1
#define EPS 0x2

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

s21_decimal s21_decimal_mult_by_pow_of_ten(s21_decimal *, int);

s21_decimal s21_decimal_divide_by_ten(s21_decimal, int *);
s21_decimal s21_normalize_decimal(s21_decimal);

int s21_decimal_len_of_number(s21_decimal);
int s21_is_valid_decimal(s21_decimal *);
void level_decimals(s21_decimal *, s21_decimal *);
void level_decimals_into_uint256(s21_decimal *, s21_decimal *, uint256 *,
                                 uint256 *);
int s21_decimal_mantisses_are_equal(s21_decimal, s21_decimal);

#endif
