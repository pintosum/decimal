#include "decimal.h"

#include <stdio.h>

#include "binary.h"
#include "uint256.h"

void print_bytes(s21_decimal *value) {
  unsigned char *byte = (unsigned char *)value;
  for (int i = sizeof(s21_decimal) - 1; i >= 0; i--) {
    printf("%.2x ", *(byte + i));
  }
  printf("\n");
}

void swap_ptr(void **ptr1, void **ptr2) {
  void *temp = *ptr1;
  *ptr1 = *ptr2;
  *ptr2 = temp;
}

int level_decimals(s21_decimal *value1, s21_decimal *value2, int *last_digit) {
  int exp_diff = value2->fields.exp - value1->fields.exp;
  if (exp_diff < 0) {
    swap_ptr((void **)&value1, (void **)&value2);
    exp_diff = -exp_diff;
  }
  int len = s21_decimal_len_of_number(*value1);
  if (30 - len < exp_diff) {
    exp_diff = exp_diff - 30 + len;
    while (30 - value2->fields.exp--)
      *value2 = s21_decimal_divide_by_ten(*value1, last_digit);
  }
  *value1 = s21_decimal_mult_by_pow_of_ten(value1, exp_diff);
  return value1->fields.exp + exp_diff;
}

int s21_add(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
  int ret = 0;
  s21_decimal result = {0};

  if (res && s21_is_valid_decimal(&val1) && s21_is_valid_decimal(&val2)) {
    uint32_t exp = level_decimals(&val1, &val2, NULL);
    print_bytes(&val1);
    print_bytes(&val2);
    if (!val1.fields.sign && val1.fields.sign) {
      result = s21_sub_mantisses(val1, val2);
    } else if (val1.fields.sign && !val2.fields.sign) {
      result = s21_sub_mantisses(val2, val1);
      // result.sign += dec2->sign;
    } else {
      result = s21_add_mantisses(val1, val2);
    }
    result.fields.exp = exp;
    print_bytes(&result);
    result = s21_normalize_decimal(result);
    int valid = s21_is_valid_decimal(&result);
    if (valid) {
      *res = result;
    } else if (result.fields.exp > 28 || result.fields.zero_bytes) {
      ret = 1 + result.fields.sign;
    } else {
      ret = 4;
    }

  } else {
    ret = 4;
  }
  return ret;
}

s21_decimal s21_mul_handle(s21_decimal a, s21_decimal b) {
  s21_decimal ret = {0};
  uint256 val1 = uint256_from_decimal(a);
  uint256 val2 = uint256_from_decimal(b);
  uint256 result = uint256_mult(val1, val2);
  ret = s21_decimal_from_uint256(result);
  ret.fields.exp += a.fields.exp + b.fields.exp;
  ret.fields.sign = a.fields.sign != b.fields.sign ? 1 : 0;
  ret = s21_normalize_decimal(ret);
  return ret;
}

int s21_mul(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
  int ret = 0;
  s21_decimal result = {0};

  if (res && s21_is_valid_decimal(&val1) && s21_is_valid_decimal(&val2)) {
    result = s21_mul_handle(val1, val2);
    if (s21_is_valid_decimal(&result)) {
      *res = result;
    } else {
      ret = 1 + result.fields.sign;
    }
  } else {
    ret = 4;
  }
  return ret;
}

int main() {
  // s21_decimal dec = {0};
  // dec_map m = {{0, 0, 0x800000}, 0, 0, 0, 0};

  // print_bytes((s21_decimal *)&m);
  // dec_map s = shift_mantissa_left_one(&m);
  // print_bytes((s21_decimal *)&s);
  // m = shift_mantissa_left_one(&s);

  // print_bytes((s21_decimal *)&m);

  s21_decimal f = {0x21ffffff, 0xaf24ffff, 0, 1 << 16};
  s21_decimal s = {0x12344fff, 0xffff323f, 0xff, 5 << 16};
  s.fields.sign = 1;
  s21_decimal result = {0};
  int ret = s21_mul(f, s, &result);
  printf("ret : %d\n", ret);
  print_bytes(&result);
}
