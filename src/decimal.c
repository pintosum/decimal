#include "decimal.h"

#include <stdio.h>

#include "binary.h"
#include "uint256.h"

/*int s21_sub(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
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
    print_dec(result, "result");

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
}*/

int s21_sub(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
  val2.fields.sign++;
  return s21_add(val1, val2, res);
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
  // print_uint256(result, "uint256 result");
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
    result = s21_normalize_decimal(result);
    if (s21_is_valid_decimal(&result)) {
      *res = result;
    } else if (result.fields.exp > 28) {
      ret = 2;
    } else if (result.fields.zero_bytes) {
      ret = 1 + result.fields.sign;
    }
  } else {
    ret = 4;
  }
  return ret;
}

int s21_div(s21_decimal a, s21_decimal b, s21_decimal *res) {
  int ret = 1;
  if (s21_decimal_is_zero(&b)) {
    ret = 3;
  } else if (res && s21_is_valid_decimal(&a) && s21_is_valid_decimal(&b)) {
    uint256 a256 = uint256_from_decimal(a);
    uint256 b256 = uint256_from_decimal(b);
    uint256 rem = {0};
    uint256 res256 = {0};
    s21_decimal result = {0};
    int a_exp = a.fields.exp;
    int b_exp = b.fields.exp;
    int exp = a.fields.exp - b.fields.exp - 1;
    printf("exp before div: %d\n", exp);
    uint256 temp = {0};
    /*a256 = uint256_mult_by_pow_of_ten(a256, 30);
    print_uint256(a256, "a256");
    res256 = uint256_div(a256, b256, &rem);*/
    int len = 0;
    do {
      temp = uint256_div(a256, b256, &rem);
      //print_uint256(temp, "temp");

      res256 = uint256_mult_by_pow_of_ten(res256, 1);
      res256 = uint256_add(res256, temp);
      a256 = uint256_mult_by_pow_of_ten(rem, 1);
      exp++;
      len = len_of_uint256(res256);
    } while (!uint256_is_zero(rem) && len < 29);
    printf("exp from div: %d\n", exp);
    print_uint256(res256, "res256");
    if (len >= 29) {
      unsigned int digit = 0;
      res256 = uint256_divide_by_ten(res256, &digit);
      if (digit > 5 || (digit == 5 && res256.bits[0] % 2)) {
        res256 = uint256_add(res256, uint256_get_one());
      }
      exp--;
    }

    ret = 0;
    result = s21_decimal_from_uint256(res256);
    //print_s21_decimal(result, "here");
    result.fields.exp = exp;
    printf("exp from div 2: %d\n", result.fields.exp);
    result.fields.sign = a.fields.sign != b.fields.sign ? 1 : 0;
    if (s21_is_valid_decimal(&result)) {
      *res = result;
    } else
      ret = 1;
  }
  return ret;
}

int main() {
  s21_decimal a = s21_decimal_from_string("5432");
  s21_decimal b = s21_decimal_from_string("2");
  printf("exp a %d\n", a.fields.exp);
  printf("a sign : %d\n", a.fields.sign);
  printf("r sign : %d\n", b.fields.sign);
  s21_decimal result = {0};
  int ret = s21_div(a, b, &result);
  printf("ret : %d\n", ret);

  print_s21_decimal(a, "a");
  print_s21_decimal(b, "b");
  print_s21_decimal(result, "result");
  printf("exp %d\n", result.fields.exp);
  print_dec(result, "ints");
}
