#include "binary.h"

#include <stdint.h>
#include <stdio.h>

#include "decimal.h"

void print_dec(s21_decimal r, char *name) {
  printf("%s : %u %u %u  %u\n", name, r.fields.mantissa[0],
         r.fields.mantissa[1], r.fields.mantissa[2], r.fields.zero_bytes);
}

s21_decimal shift_mantissa_left_one(s21_decimal value) {
  // int overflowing = value->zero_bytes >> 6 & 1;
  // if (!overflowing) {
  int carry = 0;
  for (int i = 0; i < 3; i++) {
    value.fields.mantissa[i] = value.fields.mantissa[i] << 1;
    value.fields.mantissa[i] |= carry;
    carry = value.fields.mantissa[i] >> 31 & 1;
  }

  value.fields.zero_bytes = value.fields.zero_bytes << 1;
  value.fields.zero_bytes |= carry;
  //} else
  // value.fields.signal_bits |= OVERFLOW;

  return value;
}

s21_decimal shift_mantissa_right_one(s21_decimal value) {
  unsigned int carry = value.fields.zero_bytes & 1;
  value.fields.zero_bytes = value.fields.zero_bytes >> 1;
  for (int i = 2; i >= 0; i--) {
    value.fields.mantissa[i] = value.fields.mantissa[i] >> 1;
    value.fields.mantissa[i] |= carry << 31;
    carry = value.fields.mantissa[i] & 1;
  }

  return value;
}

s21_decimal shift_mantissa_left(s21_decimal value, unsigned int shift) {
  while (shift--) {
    value = shift_mantissa_left_one(value);
  }
  return value;
}

s21_decimal shift_mantissa_right(s21_decimal value, unsigned int shift) {
  while (shift--) {
    value = shift_mantissa_left_one(value);
  }
  return value;
}

s21_decimal decimal_xor(s21_decimal *val1, s21_decimal *val2) {
  s21_decimal ret = *val1;
  for (int i = 0; i < 3; i++) {
    ret.fields.mantissa[i] ^= val2->fields.mantissa[i];
  }
  ret.fields.zero_bytes ^= val2->fields.zero_bytes;
  return ret;
}

s21_decimal decimal_and(s21_decimal *val1, s21_decimal *val2) {
  s21_decimal ret = *val1;
  for (int i = 0; i < 3; i++) {
    ret.fields.mantissa[i] &= val2->fields.mantissa[i];
  }
  ret.fields.zero_bytes &= val2->fields.zero_bytes;
  return ret;
}

s21_decimal twos_complement(s21_decimal value) {
  for (int i = 0; i < 3; i++) {
    value.fields.mantissa[i] = ~value.fields.mantissa[i];
  }
  value.fields.zero_bytes = ~value.fields.zero_bytes;
  value = add_mantisses(value, get_one());
  return value;
}

s21_decimal get_one() { return (s21_decimal){{1, 0, 0, 0}}; }

int decimal_is_zero(s21_decimal *value) {
  int ret = 1;
  for (int i = 0; i < 3; i++) ret *= !value->fields.mantissa[i];
  ret *= !value->fields.zero_bytes;
  return ret;
}

s21_decimal add_mantisses(s21_decimal val1, s21_decimal val2) {
  s21_decimal ret = val1;
  // ret.sign = 0;
  while (!decimal_is_zero(&val2)) {
    s21_decimal carry_bits = decimal_and(&ret, &val2);
    carry_bits = shift_mantissa_left_one(&carry_bits);
    ret = decimal_xor(&ret, &val2);
    val2 = carry_bits;
  }
  return ret;
}

s21_decimal sub_mantisses(s21_decimal val1, s21_decimal val2) {
  val2 = twos_complement(&val2);
  val2 = add_mantisses(val1, val2);
  if (val2.zero_bytes == 0xFFFF) {
    val2 = twos_complement(&val2);
    val2.sign = 1;
  }
  return val2;
}

s21_decimal mult_by_pow_of_ten(s21_decimal *value, int power) {
  s21_decimal ret = *value;
  while (power--) {
    ret = add_mantisses(shift_mantissa_left(&ret, 3),
                        shift_mantissa_left(&ret, 1));
  }
  return ret;
}

int most_significant_bit(s21_decimal value) {
  int ret = 0;
  while (!decimal_is_zero(&value)) {
    value = shift_mantissa_right_one(&value);
    ret++;
  }
  return ret;
}

int len_of_number(s21_decimal value) {
  double log_of_2 = 0.301;
  int binary_len = most_significant_bit(value);
  return (int)(binary_len * log_of_2) + 1;
}

int divisible_by_ten(s21_decimal value) {
  uint64_t sum = (uint64_t)value.mantissa[0] + (uint64_t)value.mantissa[1] +
                 (uint64_t)value.mantissa[2] + (uint64_t)value.zero_bytes;
  int divisible = !(sum % 5) && !(value.mantissa[0] % 2);
  return divisible;
}

s21_decimal div_by_ten(s21_decimal value, int *remainder) {
  uint64_t rem = 0;
  for (int i = 2; i >= 0; i--) {
    uint64_t digit = (uint64_t)value.mantissa[i] + rem * (1ULL << 32);
    value.mantissa[i] = digit / 10;
    rem = digit % 10;
  }
  if (remainder) *remainder = rem;
  return value;
}

/*s21_decimal div_mantisses(s21_decimal num, s21_decimal div, s21_decimal
*remainder){ s21_decimal quot = {0}; s21_decimal rem = {0}; for(int i =
most_significant_bit(num); i >=0; i--){ rem = shift_mantissa_left(&rem, 1);
    if(num.mantissa[0] & 1){
      quot.mantissa[0] |= 1U;
    }
    else{
      quot.mantissa[0] &= ~(1U);
    }
    if(rem >= div){
      rem -= div;
      quot.mantissa[0] |= 1U;
    }

  }
}*/

s21_decimal normalize_decimal(s21_decimal value) {
  while (value.exp && divisible_by_ten(value)) {
    value = div_by_ten(value, NULL);
    value.exp--;
  }
  return value;
}

int s21_valid_s21_decimal(s21_decimal *val) {
  return val->exp <= 28 && !val->signal_bits && !val->zero_bytes;
}

/*int main() {
  s21_decimal f = {{0x0000A768, 0xb9873ad3, 0x8f2ab2}};
  // s21_decimal l = {{0xF, 0, 0}};
  // // s21_decimal s = div_by_ten(&l);
  // s21_decimal sub = sub_mantisses(f, l);
  // print_dec(sub, "diff");

  // printf("%u %u  %u\n", f.mantissa[0], f.mantissa[1], f.zero_bytes);
}
*/