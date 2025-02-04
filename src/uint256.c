#include "uint256.h"
#include "decimal.h"

#define sizeofarray(a) (int)(sizeof(a) / sizeof(a[0]))

uint256 uint256_and(uint256 a, uint256 b) {
  for (int i = 0; i < sizeofarray(a.bits); i++) {
    a.bits[i] &= b.bits[i];
  }
  return a;
}

uint256 uint256_xor(uint256 a, uint256 b) {
  for (int i = 0; i < sizeofarray(a.bits); i++) {
    a.bits[i] ^= b.bits[i];
  }
  return a;
}

uint256 shift_uint256_left_one(uint256 a) {
  uint256 shifted = a;
  uint64_t carry = 0;
  for (int i = 0; i < sizeofarray(a.bits); i++) {
    shifted.bits[i] = a.bits[i] << 1;
    shifted.bits[i] |= carry;
    carry = a.bits[i] >> 31;
  }
  return shifted;
}

uint256 shift_uint256_right_one(uint256 a) {
  uint256 shifted = a;
  uint64_t carry = 0;
  for (int i = sizeofarray(a.bits) - 1; i >= 0; i--) {
    shifted.bits[i] = a.bits[i] >> 1;
    shifted.bits[i] |= carry << 31;
    carry = a.bits[i] & 1;
  }
  return shifted;
}

uint256 shift_uint256_left(uint256 a, unsigned int shift) {
  while (shift--) {
    a = shift_uint256_left_one(a);
  }
  return a;
}

uint256 shift_uint256_right(uint256 a, unsigned int shift) {
  while (shift--) {
    a = shift_uint256_right_one(a);
  }
  return a;
}

int uint256_is_zero(uint256 a) {
  int ret = 1;
  for (int i = 0; i < sizeofarray(a.bits); i++)
    ret &= !a.bits[i];
  return ret;
}

uint256 uint256_get_one() { return (uint256){{1, 0, 0, 0}}; }

uint256 uint256_twos_complement(uint256 a) {
  for (int i = 0; i < sizeofarray(a.bits); i++) {
    a.bits[i] = ~a.bits[i];
  }
  a = uint256_add(a, uint256_get_one());
  return a;
}

uint256 uint256_add(uint256 a, uint256 b) {
  while (!uint256_is_zero(b)) {
    uint256 carry_bits = uint256_and(a, b);
    carry_bits = shift_uint256_left_one(carry_bits);
    a = uint256_xor(a, b);
    b = carry_bits;
  }
  return a;
}

uint256 uint256_sub(uint256 a, uint256 b, int *sign) {
  b = uint256_twos_complement(b);
  b = uint256_add(a, b);
  if (b.bits[7] == 0xFFFFFFFF) {
    b = uint256_twos_complement(b);
    if (sign)
      *sign = 1;
  }
  return b;
}

uint256 uint256_from_decimal(s21_decimal a) {
  uint256 ret = {0};
  ret.bits[0] = a.bits[0];
  ret.bits[1] = a.bits[1];
  ret.bits[2] = a.bits[2];
  ret.bits[3] = a.fields.zero_bytes;
  return ret;
}

uint256 uint256_mult(uint256 a, uint256 b) {
  uint256 ret = {0};
  while (!uint256_is_zero(b)) {
    if (b.bits[0] & 1) {
      ret = uint256_add(a, ret);
    }
    b = shift_uint256_right(b, 1);
    a = shift_uint256_left(a, 1);
  }
  return ret;
}

int uint256_most_significant_bit(uint256 value) {
  int ret = 0;
  int i = 8;
  while (!value.bits[--i])
    ;
  while (value.bits[i]) {
    value.bits[i] = value.bits[i] >> 1;
    ret++;
  }
  ret += i * 32;
  return ret;
}

int len_of_uint256(uint256 value) {
  int ret = 0;
  while (!uint256_is_zero(value)) {
    value = uint256_divide_by_ten(value, NULL);
    ret++;
  }
  return ret;
}

unsigned int uint256_get_bit(uint256 a, unsigned int bit) {
  int i = bit / 32;
  int b = bit % 32;
  return (a.bits[i] >> b) & 1;
}

uint256 uint256_set_bit(uint256 a, unsigned int bit) {
  unsigned int i = bit / 32;
  unsigned int b = bit % 32;
  a.bits[i] ^= 1 << b;
  return a;
}

int uint256_is_greater_or_equal(uint256 a, uint256 b) {
  int ret = 0;
  int i = 3;
  while (i >= 0 && a.bits[i] == b.bits[i]) {
    i--;
  }

  if (i >= 0) {
    ret = a.bits[i] >= b.bits[i];
  } else {
    ret = 1;
  }

  return ret;
}

uint256 uint256_div(uint256 num, uint256 div, uint256 *remainder) {
  uint256 quot = {0};
  uint256 rem = {0};
  for (int i = uint256_most_significant_bit(num) - 1; i >= 0; i--) {
    rem = shift_uint256_left_one(rem);
    rem.bits[0] |= uint256_get_bit(num, i);
    if (uint256_is_greater_or_equal(rem, div)) {
      rem = uint256_sub(rem, div, NULL);
      quot = uint256_set_bit(quot, i);
    }
  }
  if (remainder)
    *remainder = rem;
  return quot;
}

s21_decimal s21_decimal_from_uint256(uint256 a) {
  s21_decimal ret = {0};
  int msb = uint256_most_significant_bit(a);
  unsigned int digit = 0;
  while (msb > 96) {
    a = uint256_divide_by_ten(a, &digit);
    ret.fields.exp--;
    msb = uint256_most_significant_bit(a);
  }

  if (digit > 5 || (digit == 5 && a.bits[0] % 2)) {
    a = uint256_add(a, uint256_get_one());
  }
  ret.bits[0] = a.bits[0];
  ret.bits[1] = a.bits[1];
  ret.bits[2] = a.bits[2];

  return ret;
}

uint256 uint256_mult_by_pow_of_ten(uint256 ret, int power) {
  while (power-- > 0) {
    ret = uint256_add(shift_uint256_left(ret, 3), shift_uint256_left(ret, 1));
  }
  return ret;
}

uint256 uint256_divide_by_ten(uint256 value, unsigned int *remainder) {
  uint64_t rem = 0;
  for (int i = 7; i >= 0; i--) {
    uint64_t digit = (uint64_t)value.bits[i] + rem * (1ULL << 32);
    value.bits[i] = digit / 10;
    rem = digit % 10;
  }
  if (remainder)
    *remainder = rem;
  return value;
}
