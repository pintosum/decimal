#ifndef S21_UINT256_H
#define S21_UINT256_H
#include "decimal.h"

typedef struct {
  uint64_t bits[4];
} uint256;

uint256 shift_uint256_left_one(uint256);
uint256 shift_uint256_right_one(uint256);
uint256 shift_uint256_left(uint256, unsigned int);
uint256 shift_uint256_right(uint256, unsigned int);
uint256 uint256_xor(uint256, uint256);
uint256 uint256_and(uint256, uint256);

uint256 uint256_add(uint256, uint256);
uint256 uint256_sub(uint256, uint256);
uint256 uint256_mult(uint256, uint256);
uint256 uint256_div(uint256, uint256, uint256 *);
uint256 uint256_divide_by_ten(uint256, unsigned int *);
uint256 uint256_mult_by_pow_of_ten(uint256 ret, int power);

int uint256_is_zero(uint256);
uint256 uint256_twos_complement(uint256);
uint256 uint256_get_one();
uint256 uint256_from_decimal(s21_decimal a);
s21_decimal s21_decimal_from_uint256(uint256 a);

#endif
