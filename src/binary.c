#include "binary.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "decimal.h"
void swap_ptr(void **ptr1, void **ptr2) {
  void *temp = *ptr1;
  *ptr1 = *ptr2;
  *ptr2 = temp;
}

int level_decimals(s21_decimal *value1, s21_decimal *value2, int *last_digit) {
  int exp_diff = value2->fields.exp - value1->fields.exp;
  printf("exp dif  : %d\n",exp_diff);
  if (exp_diff < 0) {
    swap_ptr((void **)&value1, (void **)&value2);
    exp_diff = -exp_diff;
  }
  int len = s21_decimal_len_of_number(*value1);
  if (30 - len < exp_diff) {
    exp_diff = exp_diff - 30 + len;
    while (30 - value2->fields.exp-- > 0)
      *value2 = s21_decimal_divide_by_ten(*value1, last_digit);
  }
  *value1 = s21_decimal_mult_by_pow_of_ten(value1, exp_diff);
  printf("value exp : %d\n",value1->fields.exp);
  value1->fields.exp += exp_diff;
  return 1;
}

s21_decimal s21_decimal_from_string(const char *dec) {
  s21_decimal ret = {0};
  int len = strlen(dec);
  int i = 0;
  if (dec[0] == '-') {
    ret.fields.sign = 1;
    i++;
  }
  int max_len = 29;
  while (i < len) {
    if (i == max_len)
      break;
    if (s21_decimal_is_zero(&ret)) {
      max_len++;
    }

    int digit = 0;
    if (dec[i] <= '9' && dec[i] >= '0') {
      digit = dec[i++] - '0';
    } else if (dec[i] == '.') {
      ret.fields.exp = len < max_len ? len - ++i : max_len - ++i;
      continue;
    } else {
      return (s21_decimal){0};
    }
    s21_decimal n = s21_decimal_mult_by_pow_of_ten(&ret, 1);
    s21_decimal dig = {{digit, 0, 0, 0}};
    ret = s21_add_mantisses(n, dig);
  }
  return ret;
}

void print_s21_decimal(s21_decimal r, char *name) {
  printf("%s : \n  ", name);
  char str[36] = {1};
  int len = s21_decimal_len_of_number(r);
  int exp = r.fields.exp;
  if (exp < len) {
    len++;
  }
  if (r.fields.sign)
    printf("-");
  while (exp >= len) {
    printf("0");
    if (exp == r.fields.exp)
      printf(".");
    exp--;
  }
  int i = 0;
  do {
    i++;
    int digit = 0;
    r = s21_decimal_divide_by_ten(r, &digit);
    str[--len] = digit + '0';
    if (r.fields.exp == i) {
      str[--len] = '.';
      exp--;
    }
  } while (!s21_decimal_is_zero(&r));

  printf("%s\n", str);
}

void print_dec(s21_decimal r, char *name) {
  printf("%s : ", name);
  printf("%u %u %u  %u\n", r.fields.mantissa[0], r.fields.mantissa[1],
         r.fields.mantissa[2], r.fields.zero_bytes);
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

s21_decimal s21_shift_mantissa_right_one(s21_decimal value) {
  s21_decimal val = value;
  unsigned int carry = value.fields.zero_bytes & 1;
  val.fields.zero_bytes = value.fields.zero_bytes >> 1;
  for (int i = 2; i >= 0; i--) {
    val.fields.mantissa[i] = value.fields.mantissa[i] >> 1;
    val.fields.mantissa[i] |= carry << 31;
    carry = value.fields.mantissa[i] & 1;
  }

  return val;
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
  int ret = 0;
  for (int i = 0; i < 3; i++)
    ret += value->fields.mantissa[i];
  ret += value->fields.zero_bytes;
  return ret == 0;
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

s21_decimal s21_decimal_set_bit(s21_decimal a, unsigned int bit) {
  unsigned int i = bit / 32;
  unsigned int b = bit % 32;
  a.bits[i] ^= 1 << b;
  return a;
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  a.bits[3] = a.fields.zero_bytes;
  b.bits[3] = b.fields.zero_bytes;
  int f = s21_decimal_most_significant_bit(a) / 32;
  int s = s21_decimal_most_significant_bit(b) / 32;
  f = f > s ? f : s;
  return a.bits[f] >= b.bits[f];
}

unsigned int s21_decimal_get_bit(s21_decimal a, unsigned int bit) {
  unsigned int i = bit / 32;
  unsigned int b = bit % 32;
  unsigned int ret = a.bits[i] >> b;
  return ret & 1;
}

s21_decimal s21_div_mantisses(s21_decimal num, s21_decimal div,
                              s21_decimal *remainder) {
  s21_decimal quot = {0};
  s21_decimal rem = {0};
  for (int i = s21_decimal_most_significant_bit(num) - 1; i >= 0; i--) {
    rem = s21_shift_mantissa_left_one(rem);
    rem.bits[0] |= s21_decimal_get_bit(num, i);
    if (s21_is_greater_or_equal(rem, div)) {
      rem = s21_sub_mantisses(rem, div);
      quot = s21_decimal_set_bit(quot, i);
    }
  }
  quot.fields.sign = num.fields.sign != div.fields.sign ? 1 : 0;
  if (remainder)
    *remainder = rem;
  return quot;
}

s21_decimal s21_normalize_decimal(s21_decimal value) {
  while (value.fields.exp && s21_decimal_is_divisible_by_ten(value)) {
    value = s21_decimal_divide_by_ten(value, NULL);
    value.fields.exp--;
  }
  int last_digit = 0;
  while(value.fields.zero_bytes){
    value = s21_decimal_divide_by_ten(value, &last_digit);
    value.fields.exp--;
  }
  if(last_digit > 5 || (last_digit == 5 && value.bits[0] % 2)){
    value = s21_add_mantisses(value, s21_decimal_get_one());
  }
  return value;
}

int s21_is_valid_decimal(s21_decimal *val) {
  return val->fields.exp <= 28 && !val->fields.signal_bits &&
         !val->fields.zero_bytes;
}
