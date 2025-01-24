#include "binary.h"

#include <stdint.h>
#include <stdio.h>

#include "decimal.h"

void print_dec(s21_decimal r, char *name) {
  printf("%s : %u %u %u  %u\n", name, r.fields.mantissa[0],
         r.fields.mantissa[1], r.fields.mantissa[2], r.fields.zero_bytes);
}

s21_decimal s21_shift_mantissa_left_one(s21_decimal value) {
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

s21_decimal s21_shift_mantissa_right_one(s21_decimal value) {
  unsigned int carry = value.fields.zero_bytes & 1;
  value.fields.zero_bytes = value.fields.zero_bytes >> 1;
  for (int i = 2; i >= 0; i--) {
    value.fields.mantissa[i] = value.fields.mantissa[i] >> 1;
    value.fields.mantissa[i] |= carry << 31;
    carry = value.fields.mantissa[i] & 1;
  }

  return value;
}

s21_decimal s21_shift_mantissa_left(s21_decimal value, unsigned int shift) {
  while (shift--) {
    value = s21_shift_mantissa_left_one(value);
  }
  return value;
}

s21_decimal s21_shift_mantissa_right(s21_decimal value, unsigned int shift) {
  while (shift--) {
    value = s21_shift_mantissa_left_one(value);
  }
  return value;
}

s21_decimal s21_decimal_xor(s21_decimal *val1, s21_decimal *val2) {
  s21_decimal ret = *val1;
  for (int i = 0; i < 3; i++) {
    ret.fields.mantissa[i] ^= val2->fields.mantissa[i];
  }
  ret.fields.zero_bytes ^= val2->fields.zero_bytes;
  return ret;
}

s21_decimal s21_decimal_and(s21_decimal *val1, s21_decimal *val2) {
  s21_decimal ret = *val1;
  for (int i = 0; i < 3; i++) {
    ret.fields.mantissa[i] &= val2->fields.mantissa[i];
  }
  ret.fields.zero_bytes &= val2->fields.zero_bytes;
  return ret;
}

s21_decimal s21_decimal_twos_complement(s21_decimal value) {
  for (int i = 0; i < 3; i++) {
    value.fields.mantissa[i] = ~value.fields.mantissa[i];
  }
  value.fields.zero_bytes = ~value.fields.zero_bytes;
  value = s21_add_mantisses(value, s21_decimal_get_one());
  return value;
}

s21_decimal s21_decimal_get_one() { return (s21_decimal){{1, 0, 0, 0}}; }

int s21_decimal_is_zero(s21_decimal *value) {
  int ret = 1;
  for (int i = 0; i < 3; i++) ret *= !value->fields.mantissa[i];
  ret *= !value->fields.zero_bytes;
  return ret;
}

s21_decimal s21_add_mantisses(s21_decimal val1, s21_decimal val2) {
  s21_decimal ret = val1;
  // ret.sign = 0;
  while (!s21_decimal_is_zero(&val2)) {
    s21_decimal carry_bits = s21_decimal_and(&ret, &val2);
    carry_bits = s21_shift_mantissa_left_one(carry_bits);
    ret = s21_decimal_xor(&ret, &val2);
    val2 = carry_bits;
  }
  return ret;
}

s21_decimal s21_sub_mantisses(s21_decimal val1, s21_decimal val2) {
  val2 = s21_decimal_twos_complement(val2);
  val2 = s21_add_mantisses(val1, val2);
  if (val2.fields.zero_bytes == 0xFFFF) {
    val2 = s21_decimal_twos_complement(val2);
    val2.fields.sign = 1;
  }
  return val2;
}

s21_decimal s21_decimal_mult_by_pow_of_ten(s21_decimal *value, int power) {
  s21_decimal ret = *value;
  while (power--) {
    ret = s21_add_mantisses(s21_shift_mantissa_left(ret, 3),
                        s21_shift_mantissa_left(ret, 1));
  }
  return ret;
}

int s21_decimal_most_significant_bit(s21_decimal value) {
  int ret = 0;
  while (!s21_decimal_is_zero(&value)) {
    value = s21_shift_mantissa_right_one(value);
    ret++;
  }
  return ret;
}

int s21_decimal_len_of_number(s21_decimal value) {
  double log_of_2 = 0.301;
  int binary_len = s21_decimal_most_significant_bit(value);
  return (int)(binary_len * log_of_2) + 1;
}

int s21_decimal_is_divisible_by_ten(s21_decimal value) {
  uint64_t sum = (uint64_t)value.fields.mantissa[0] + (uint64_t)value.fields.mantissa[1] +
                 (uint64_t)value.fields.mantissa[2] + (uint64_t)value.fields.zero_bytes;
  int divisible = !(sum % 5) && !(value.fields.mantissa[0] % 2);
  return divisible;
}

s21_decimal s21_decimal_divide_by_ten(s21_decimal value, int *remainder) {
  uint64_t rem = 0;
  for (int i = 2; i >= 0; i--) {
    uint64_t digit = (uint64_t)value.fields.mantissa[i] + rem * (1ULL << 32);
    value.fields.mantissa[i] = digit / 10;
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

s21_decimal s21_normalize_decimal(s21_decimal value) {
  while (value.fields.exp && s21_decimal_is_divisible_by_ten(value)) {
    value = s21_decimal_divide_by_ten(value, NULL);
    value.fields.exp--;
  }
  return value;
}

int s21_is_valid_decimal(s21_decimal *val) {
  return val->fields.exp <= 28 && !val->fields.signal_bits && !val->fields.zero_bytes;
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
