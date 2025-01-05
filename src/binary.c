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
    shifted.signal_bits |= OVERFLOW;

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
  ret.zero_bytes ^= val2->zero_bytes;
  return ret;
}

dec_map decimal_and(dec_map *val1, dec_map *val2) {
  dec_map ret = *val1;
  for (int i = 0; i < 3; i++) {
    ret.mantissa[i] &= val2->mantissa[i];
  }
  ret.zero_bytes &= val2->zero_bytes;
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
  ret *= !value->zero_bytes;
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

dec_map sub_mantisses(dec_map val1, dec_map val2) {
  val2 = twos_complement(&val2);
  return sub_mantisses(val1, val2);
}

dec_map mult_by_pow_of_ten(dec_map *value, int power) {
  dec_map ret = *value;
  while (power--) {
    ret = add_mantisses(shift_mantissa_left(&ret, 3),
                        shift_mantissa_left(&ret, 1));
  }
  return ret;
}

int most_significant_bit(dec_map value) {
  int ret = 0;
  while (!decimal_is_zero(&value)) {
    value = shift_mantissa_right_one(&value);
    ret++;
  }
  return ret;
}

int len_of_number(dec_map value){
  double log_of_2 = 0.301;
  int binary_len = most_significant_bit(value);
  return (int)(binary_len * log_of_2) + 1;
}

int divisible_by_ten(dec_map value){
  uint64_t sum = (uint64_t)value.mantissa[0] + (uint64_t)value.mantissa[1] + (uint64_t)value.mantissa[2] + (uint64_t)value.zero_bytes;
  int divisible = !(sum % 5) && !(value.mantissa[0] % 2);
  return divisible;
}

dec_map div_by_ten(dec_map *value){
  dec_map q, r;
  q = add_mantisses(shift_mantissa_right(value, 1), shift_mantissa_right(value, 2));
  q = add_mantisses(q, shift_mantissa_right(&q, 4));
  q = add_mantisses(q, shift_mantissa_right(&q, 8));
  q = add_mantisses(q, shift_mantissa_right(&q, 16));
  q = shift_mantissa_right(&q, 3);
  r = mult_by_pow_of_ten(&q, 1);
  r = sub_mantisses(*value, r);
  r = add_mantisses(r, (dec_map){6, 0, 0});
  r = shift_mantissa_right(&r, 4);
  return add_mantisses(q, r);
}

dec_map normalize_decimal(dec_map value){
  while(value.exp && divisible_by_ten(value)){
    value = div_by_ten(&value);
    value.exp--;
  }
  return value;
}

#include <stdio.h>

int main() {
  dec_map f = {{0xFFFFFFFF, 1, 0}};
  for (int i = 0; i < 100000; i++) {
    f = add_mantisses(f, get_one());
  }
  printf("%u %u  %u\n", f.mantissa[0], f.mantissa[1], f.zero_bytes);
}


