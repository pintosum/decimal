#include "decimal.h"
#include "binary.h"
#include "uint256.h"

int s21_sub(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
  val2.fields.sign++;
  return s21_add(val1, val2, res);
}

int s21_add(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
  int ret = 0;
  s21_decimal result = {0};

  if (res && s21_is_valid_decimal(&val1) && s21_is_valid_decimal(&val2)) {
    uint256 a = {0};
    uint256 b = {0};
    uint256 res256 = {0};
    int result_sign = 0;
    level_decimals_into_uint256(&val1, &val2, &a, &b);
    if (!val1.fields.sign && val2.fields.sign) {
      res256 = uint256_sub(a, b, &result_sign);
    } else if (val1.fields.sign && !val2.fields.sign) {
      res256 = uint256_sub(b, a, &result_sign);
    } else {
      res256 = uint256_add(a, b);
      result_sign = val1.fields.sign;
    }
    result = s21_decimal_from_uint256(res256);
    result.fields.exp += val2.fields.exp;
    if (result_sign)
      result.fields.sign = result_sign;
    result = s21_normalize_decimal(result);
    if (s21_is_valid_decimal(&result)) {
      *res = result;
    } else if (result.fields.signal_bits) {
      ret = result.fields.signal_bits;
    } else {
      ret = 1 + result.fields.sign;
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
    if (!s21_decimal_is_zero(&val1) && !s21_decimal_is_zero(&val2))
      result = s21_mul_handle(val1, val2);
    else
      result.fields.sign = val1.fields.sign != val2.fields.sign ? 1 : 0;
    result = s21_normalize_decimal(result);
    if (s21_is_valid_decimal(&result)) {
      *res = result;
    } else if (result.fields.signal_bits) {
      ret = result.fields.signal_bits;
    } else {
      ret = 1 + result.fields.sign;
    }

  } else {
    ret = 4;
  }
  return ret;
}

int s21_div(s21_decimal a, s21_decimal b, s21_decimal *res) {
  int ret = 4;
  if (s21_decimal_is_zero(&b)) {
    ret = 3;
  } else if (res && s21_is_valid_decimal(&a) && s21_is_valid_decimal(&b)) {
    ret = 0;
    uint256 a256 = uint256_from_decimal(a);
    uint256 b256 = uint256_from_decimal(b);
    uint256 rem = {0};
    uint256 res256 = {0};
    s21_decimal result = {0};
    int exp = a.fields.exp - b.fields.exp - 1;
    uint256 temp = {0};
    int len = 0;
    do {
      temp = uint256_div(a256, b256, &rem);
      res256 = uint256_mult_by_pow_of_ten(res256, 1);
      res256 = uint256_add(res256, temp);
      a256 = uint256_mult_by_pow_of_ten(rem, 1);
      exp++;
      len = len_of_uint256(res256);
    } while (!uint256_is_zero(rem) && len < 30);
    if (exp < 0) {
      res256 = uint256_mult_by_pow_of_ten(res256, -exp);
      exp = 0;
    }
    result = s21_decimal_from_uint256(res256);
    result.fields.exp += exp;
    result.fields.sign = a.fields.sign != b.fields.sign ? 1 : 0;
    result = s21_normalize_decimal(result);
    if (s21_is_valid_decimal(&result)) {
      *res = result;
    } else if (result.fields.signal_bits) {
      ret = result.fields.signal_bits;
    } else {
      ret = 1 + result.fields.sign;
    }
  }
  return ret;
}
