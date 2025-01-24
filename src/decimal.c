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
  dec_map *val_big = (dec_map *)value1;
  dec_map *val_sml = (dec_map *)value2;
  int exp_diff = val_sml->exp - val_big->exp;
  if (exp_diff < 0) {
    swap_ptr((void **)&val_big, (void **)&val_sml);
    exp_diff = -exp_diff;
  }
  int len = len_of_number(*val_big);
  if (30 - len < exp_diff) {
    exp_diff = exp_diff - 30 + len;
    while (30 - val_sml->exp--) *val_sml = div_by_ten(*val_big, last_digit);
  }
  *val_big = mult_by_pow_of_ten(val_big, exp_diff);
  return val_big->exp + exp_diff;
}

int s21_valid_decimal(s21_decimal *value) {
  dec_map *val = (dec_map *)value;
  return val->exp <= 28 && !val->signal_bits && !val->zero_bytes;
}

int is_negative(s21_decimal val) { return val.bits[3] >> 31; }

int s21_add(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
  int ret = 0;
  dec_map *dec1 = (dec_map *)&val1;
  dec_map *dec2 = (dec_map *)&val2;
  dec_map result = {0};

  if (res && s21_valid_decimal(&val1) && s21_valid_decimal(&val2)) {
    uint32_t exp = level_decimals(&val1, &val2, NULL);
    print_bytes(&val1);
    print_bytes(&val2);
    if (!dec1->sign && dec2->sign) {
      result = sub_mantisses(*dec1, *dec2);
    } else if (dec1->sign && !dec2->sign) {
      result = sub_mantisses(*dec2, *dec1);
      // result.sign += dec2->sign;
    } else {
      result = add_mantisses(*dec1, *dec2);
    }
    result.exp = exp;
    print_bytes((s21_decimal *)&result);
    result = normalize_decimal(result);
    int valid = s21_valid_dec_map(&result);
    if (valid) {
      *res = *(s21_decimal *)(&result);
    } else if (result.exp > 28 || result.zero_bytes) {
      ret = 1 + result.sign;
    } else {
      ret = 4;
    }

  } else {
    ret = 4;
  }
  return ret;
}

s21_decimal s21_mul_handle(s21_decimal a, s21_decimal b, int scale_diff) {
  s21_decimal ret = {0};
  uint256 val1 = uint256_from_decimal(a);
  uint256 val2 = uint256_from_decimal(b);
  if (scale_diff > 0) {
    val2 = uint256_mult_by_pow_of_ten(val2, scale_diff);
  } else {
    val1 = uint256_mult_by_pow_of_ten(val1, -scale_diff);
  }
  uint256 result = uint256_mult(val1, val2);
  ret = s21_decimal_from_uint256(result);
  return ret;
}

int s21_mul(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
  int ret = 0;
  dec_map *dec1 = (dec_map *)&val1;
  dec_map *dec2 = (dec_map *)&val2;
  dec_map *result = (dec_map *)res;

  if (res && s21_valid_decimal(&val1) && s21_valid_decimal(&val2)) {
    *res = s21_mul_handle(val1, val2, dec1->exp - dec2->exp);
    if (dec1->sign != dec2->sign) {
      result->sign = 1;
    } else {
      result->sign = 0;
    }
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

  s21_decimal f = {2, 0, 0, 1 << 31};
  s21_decimal s = {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0 << 31};
  s21_decimal result;
  int ret = s21_add(f, s, &result);
  printf("ret : %d\n", ret);
  print_bytes(&result);
}
