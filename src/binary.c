#include "binary.h"
#include "uint256.h"

#include <stdint.h>

void swap_ptr(void **ptr1, void **ptr2) {
  void *temp = *ptr1;
  *ptr1 = *ptr2;
  *ptr2 = temp;
}

void level_decimals(s21_decimal *value1, s21_decimal *value2) {
  int exp_diff = value2->fields.exp - value1->fields.exp;
  if (exp_diff < 0) {
    swap_ptr((void **)&value1, (void **)&value2);
    exp_diff = -exp_diff;
  }
  *value1 = s21_decimal_mult_by_pow_of_ten(value1, exp_diff);
  value1->fields.exp += exp_diff;
}

void level_decimals_into_uint256(s21_decimal *value1, s21_decimal *value2,
                                 uint256 *a, uint256 *b) {
  int exp_diff = value2->fields.exp - value1->fields.exp;
  if (exp_diff < 0) {
    swap_ptr((void **)&value1, (void **)&value2);
    swap_ptr((void **)&a, (void **)&b);
    exp_diff = -exp_diff;
  }
  *a = uint256_from_decimal(*value1);
  *b = uint256_from_decimal(*value2);
  *a = uint256_mult_by_pow_of_ten(*a, exp_diff);
  value1->fields.exp += exp_diff;
}

s21_decimal s21_shift_mantissa_left_one(s21_decimal value) {
  s21_decimal val = value;
  int carry = 0;
  for (int i = 0; i < 3; i++) {
    val.fields.mantissa[i] = value.fields.mantissa[i] << 1;
    val.fields.mantissa[i] |= carry;
    carry = value.fields.mantissa[i] >> 31 & 1;
  }

  val.fields.zero_bytes = value.fields.zero_bytes << 1;
  val.fields.zero_bytes |= carry;

  return val;
}

s21_decimal s21_shift_mantissa_left(s21_decimal value, unsigned int shift) {
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
  for (int i = 0; i < 3; i++)
    ret *= !value->fields.mantissa[i];
  ret *= !value->fields.zero_bytes;
  return ret;
}

s21_decimal s21_add_mantisses(s21_decimal val1, s21_decimal val2) {
  s21_decimal ret = val1;
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

int s21_decimal_len_of_number(s21_decimal value) {
  int ret = 0;
  while (!s21_decimal_is_zero(&value)) {
    ret++;
    value = s21_decimal_divide_by_ten(value, NULL);
  }
  return ret;
}

int s21_decimal_is_divisible_by_ten(s21_decimal value) {
  uint64_t sum =
      (uint64_t)value.fields.mantissa[0] + (uint64_t)value.fields.mantissa[1] +
      (uint64_t)value.fields.mantissa[2] + (uint64_t)value.fields.zero_bytes;
  int divisible = !(sum % 5) && !(value.fields.mantissa[0] % 2);
  return divisible;
}

s21_decimal s21_decimal_divide_by_ten(s21_decimal value, int *remainder) {
  uint64_t rem = 0;
  uint64_t digit = (uint64_t)value.fields.zero_bytes;
  value.fields.zero_bytes = digit / 10;
  rem = digit % 10;
  for (int i = 2; i >= 0; i--) {
    digit = (uint64_t)value.fields.mantissa[i] + rem * (1ULL << 32);
    value.fields.mantissa[i] = digit / 10;
    rem = digit % 10;
  }
  if (remainder)
    *remainder = rem;
  return value;
}

s21_decimal s21_normalize_decimal(s21_decimal value) {
  if (value.fields.exp > 127) {
    value.fields.exp = 0;
    value.fields.zero_bytes = 0;
    value.fields.signal_bits = INF + value.fields.sign;
  }
  while (value.fields.exp > 28) {
    value = s21_decimal_divide_by_ten(value, NULL);
    value.fields.exp--;
    if (s21_decimal_is_zero(&value)) {
      value.fields.signal_bits = EPS;
    }
  }

  while (value.fields.exp && s21_decimal_is_divisible_by_ten(value)) {
    value = s21_decimal_divide_by_ten(value, NULL);
    value.fields.exp--;
  }
  int last_digit = 0;
  while (value.fields.zero_bytes) {
    value = s21_decimal_divide_by_ten(value, &last_digit);
    value.fields.exp--;
  }
  if (last_digit > 5 || (last_digit == 5 && value.bits[0] % 2)) {
    value = s21_add_mantisses(value, s21_decimal_get_one());
  }
  return value;
}

int s21_is_valid_decimal(s21_decimal *val) {
  return val->fields.exp <= 29 && !val->fields.signal_bits &&
         !val->fields.zero_bytes;
}
