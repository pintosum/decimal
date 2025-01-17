#include "uint256.h"
#include "decimal.h"

uint256 uint256_and(uint256 a, uint256 b) {
  for (int i = 0; i < sizeof(a.bits); i++) {
    a.bits[i] &= b.bits[i];
  }
  return a;
}

uint256 uint256_xor(uint256 a, uint256 b) {
  for (int i = 0; i < sizeof(a.bits); i++) {
    a.bits[i] ^= b.bits[i];
  }
  return a;
}

uint256 shift_uint256_left_one(uint256 a) {
  uint256 shifted = a;
  uint64_t carry = 0;
  for (int i = 0; i < sizeof(a.bits); i++) {
    shifted.bits[i] = a.bits[i] << 1;
    shifted.bits[i] |= carry;
    carry = a.bits[i] >> 63 & 1;
  }
  return shifted;
}

uint256 shift_uint256_right_one(uint256 a) {
  uint256 shifted = a;
  uint64_t carry = 0;
  for (int i = sizeof(a.bits) - 1; i >= 0; i--) {
    shifted.bits[i] = a.bits[i] >> 1;
    shifted.bits[i] |= carry << 63;
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
  for (int i = 0; i < 3; i++)
    ret *= !a.bits[i];
  return ret;
}

uint256 uint256_get_one() { return (uint256){{1, 0, 0, 0}}; }

uint256 uint256_twos_complement(uint256 a) {
  for (int i = 0; i < sizeof a.bits; i++) {
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

uint256 uint256_sub(uint256 a, uint256 b) {
  b = uint256_twos_complement(b);
  b = uint256_add(a, b);
  if (b.bits[3] >> 63 == 1) {
    b = uint256_twos_complement(b);
    b.bits[3] = 1ULL << 63;
  }
  return b;
}

uint256 uint256_from_decimal(s21_decimal a) {
  uint256 ret = {{0, 0, 0, 0}};
  ret.bits[0] = a.bits[0];
  ret.bits[0] += a.bits[1] * (1UL << 32);
  ret.bits[1] = a.bits[2];
  return ret;
}

s21_decimal s21_decimal_from_uint256(uint256 a);

uint256 uint256_mult(uint256 a, uint256 b);
