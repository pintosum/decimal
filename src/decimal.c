#include "decimal.h"

#include <stdio.h>

#include "binary.h"
#include "uint256.h"


int s21_sub(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
  int ret = 0;
  s21_decimal result = {0};

  if (res && s21_is_valid_decimal(&val1) && s21_is_valid_decimal(&val2)) {
    uint32_t exp = level_decimals(&val1, &val2, NULL);
    if (!val1.fields.sign && val2.fields.sign) {
      result = s21_add_mantisses(val1, val2);
      result.fields.sign = val1.fields.sign;
    } else if (val1.fields.sign && !val2.fields.sign) {
      result = s21_add_mantisses(val2, val1);
      result.fields.sign = val1.fields.sign;
    } else {
      result = s21_sub_mantisses(val1, val2);
    }
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

int s21_add(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
  int ret = 0;
  s21_decimal result = {0};

  if (res && s21_is_valid_decimal(&val1) && s21_is_valid_decimal(&val2)) {
    uint32_t exp = level_decimals(&val1, &val2, NULL);
    if (!val1.fields.sign && val2.fields.sign) {
      result = s21_sub_mantisses(val1, val2);
    } else if (val1.fields.sign && !val2.fields.sign) {
      result = s21_sub_mantisses(val2, val1);
    } else {
      result = s21_add_mantisses(val1, val2);
    }
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

int s21_div(s21_decimal a, s21_decimal b, s21_decimal *res) {
  int ret = 1;
  if (res && s21_is_valid_decimal(&a) && s21_is_valid_decimal(&b)) {
    s21_decimal rem = {0};
    s21_decimal result = {0};
    result.fields.sign = a.fields.sign != b.fields.sign ? 1 : 0;
    level_decimals(&a, &b, NULL);
    result.fields.exp = -1;
    s21_decimal temp = {0};
    do {
      temp = s21_div_mantisses(a, b, &rem);

      result = s21_decimal_mult_by_pow_of_ten(&result, 1);
      result = s21_add_mantisses(result, temp);
      a = s21_decimal_mult_by_pow_of_ten(&rem, 1);

      result.fields.exp++;
    } while (!s21_decimal_is_zero(&rem) &&
             s21_decimal_len_of_number(result) < 29);
    ret = 0;
    if (s21_is_valid_decimal(&result)) {
      *res = result;
    } else
      ret = 1;
  }
  return ret;
}

/*int main() {
  s21_decimal a = s21_decimal_from_string("-11111111111111111111111111");
  s21_decimal b = s21_decimal_from_string("-111111111111111111111111111");
  printf("a sign : %d\n", a.fields.sign);
  printf("r sign : %d\n", b.fields.sign);
  s21_decimal result = {0};
  int ret = s21_sub(a, b, &result);
  printf("ret : %d\n", ret);

  print_s21_decimal(a, "a");
  print_s21_decimal(b, "b");
  print_s21_decimal(result, "result");
  print_dec(result, "ints");
}*/
