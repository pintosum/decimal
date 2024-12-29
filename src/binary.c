#include "binary.h"

dec_map shift_mantissa_left_one(dec_map *value) {
  dec_map shifted = *value;
  int overflowing = value->zero_bytes >> 6 & 1;
  if (!overflowing) {
    int carry = 0;
    for (int i = 0; i < 3; i++) {
      shifted.mantissa[i] = value->mantissa[i] << 1;
      shifted.mantissa[i] |= carry;
      carry = value->mantissa[i] >> 31 & 1;
    }

    shifted.zero_bytes = value->zero_bytes << 1;
    shifted.zero_bytes |= carry;
  } else
    shifted.signal_bits |= 0x2;

  return shifted;
}

dec_map shift_mantissa_right_one(dec_map *value) {
  dec_map shifted = *value;
  unsigned int carry = value->zero_bytes & 1;
  shifted.zero_bytes = value->zero_bytes >> 1;
  for (int i = 2; i >= 0; i--) {
    shifted.mantissa[i] = value->mantissa[i] >> 1;
    shifted.mantissa[i] |= carry << 31;
    carry = value->mantissa[i] & 1;
  }

  return shifted;
}

dec_map shift_mantissa_left(dec_map *value, int shift) {
  dec_map ret = *value;
  while (shift--) {
    ret = shift_mantissa_left_one(&ret);
  }
  return ret;
}

dec_map shift_mantissa_right(dec_map *value, int shift) {
  dec_map ret = *value;
  while (shift--) {
    ret = shift_mantissa_right_one(&ret);
  }
  return ret;
}

dec_map decimal_xor(dec_map *val1, dec_map *val2) {
  dec_map ret = *val1;
  for (int i = 0; i < 3; i++) {
    ret.mantissa[i] ^= val2->mantissa[i];
  }
  return ret;
}

dec_map decimal_and(dec_map *val1, dec_map *val2) {
  dec_map ret = *val1;
  for (int i = 0; i < 3; i++) {
    ret.mantissa[i] &= val2->mantissa[i];
  }
  return ret;
}

dec_map twos_complement(dec_map *value) {
  dec_map ret = *value;
  for (int i = 0; i < 3; i++) {
    ret.mantissa[i] = ~ret.mantissa[i];
  }
  ret = add_mantisses(ret, get_one());
  return ret;
}

dec_map get_one() { return (dec_map){{1, 0, 0}, 0, 0, 0}; }

int decimal_is_zero(dec_map *value) {
  int ret = 1;
  for (int i = 0; i < 3; i++) ret *= !value->mantissa[i];
  return ret;
}

dec_map add_mantisses(dec_map val1, dec_map val2) {
  dec_map ret = val1;
  while (!decimal_is_zero(&val2)) {
    dec_map carry_bits = decimal_and(&ret, &val2);
    carry_bits = shift_mantissa_left_one(&carry_bits);
    ret = decimal_xor(&ret, &val2);
    val2 = carry_bits;
  }
  return ret;
}

#include <stdio.h>

int main() {
  dec_map f = {{0xFFFFFFFF, 1, 0}, 0, 0, 0};
  for (int i = 0; i < 100000; i++) {
    f = add_mantisses(f, get_one());
  }
  printf("%d %d\n", f.mantissa[0], f.mantissa[1]);
}