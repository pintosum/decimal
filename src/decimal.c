#include "decimal.h"

#include <stdio.h>

#include "binary.h"
#include "uint256.h"

void print_bytes(s21_decimal *value) {
  unsigned char *byte = (unsigned char *)value;
  for (int i = sizeof(s21_decimal) - 1; i >= 0; i--) {
    printf("%.2x ", *(byte + i));
    if (i % 4 == 0) {
      printf(" ");
    }
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
  value1->fields.exp += exp_diff;
  return 1;
}

int s21_sub(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
  int ret = 0;
  s21_decimal result = {0};

  if (res && s21_is_valid_decimal(&val1) && s21_is_valid_decimal(&val2)) {
    uint32_t exp = level_decimals(&val1, &val2, NULL);
    /*   puts("val1");
       print_bytes(&val1);
       puts("val2");
       print_bytes(&val2);*/
    if (!val1.fields.sign && val2.fields.sign) {
      result = s21_add_mantisses(val1, val2);
    } else if (val1.fields.sign && !val2.fields.sign) {
      result = s21_add_mantisses(val2, val1);
      // result.sign += dec2->sign;
    } else {
      result = s21_sub_mantisses(val1, val2);
    }
    // result.fields.exp = exp;
    //  print_bytes(&result);
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
    /*   puts("val1");
       print_bytes(&val1);
       puts("val2");
       print_bytes(&val2);*/
    if (!val1.fields.sign && val2.fields.sign) {
      result = s21_sub_mantisses(val1, val2);
    } else if (val1.fields.sign && !val2.fields.sign) {
      result = s21_sub_mantisses(val2, val1);
      // result.sign += dec2->sign;
    } else {
      result = s21_add_mantisses(val1, val2);
    }
    // result.fields.exp = exp;
    //  print_bytes(&result);
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
    level_decimals(&a, &b, NULL);
    //  int exp = 30 - a.fields.exp;
    //   a = s21_decimal_mult_by_pow_of_ten(&a, exp);
    result.fields.exp = -1;
    s21_decimal temp = {0};
    do {
      /*      puts("\n");
            print_bytes(&a);
            puts("\n");*/
      temp = s21_div_mantisses(a, b, &rem);

      /*      puts("\nrem");
            print_bytes(&rem);
            puts("\n");*/
      result = s21_decimal_mult_by_pow_of_ten(&result, 1);
      result = s21_add_mantisses(result, temp);
      a = s21_decimal_mult_by_pow_of_ten(&rem, 1);

      /*puts("\na");
      print_bytes(&a);
      puts("\n");*/

      result.fields.exp++;
      puts("\nloop");
    } while (!s21_decimal_is_zero(&rem) &&
             s21_decimal_len_of_number(result) < 29);
    ret = 0;
    *res = result;
    printf("\nexp : %u\n", result.fields.exp);
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

  s21_decimal f = {0xb2400000, 0x19e0c9ba, 0x21e, 1 << 16};
  s21_decimal s = {0xb2400000, 0x19e0c9ba, 0x21e, 2 << 16};
  s21_decimal num = {10, 0, 0, 0};
  s21_decimal div = {7, 0, 0, 0};
  // s21_decimal s = {0x01, 0x0, 0x0, 2 << 16};
  // s21_decimal f = {0x01, 0x0, 0x0, 1 << 16};
  // s.fields.sign = 0;
  s21_decimal result = {0};
  int ret = s21_mul(num, div, &result);
  printf("ret : %d\n", ret);
  //print_bytes(&result);
  print_dec(result, "result");
  result = s21_decimal_from_string("-1.123456789123456789123456789123123123");
  print_s21_decimal(result, "result");
  //print_bytes(&result);
}
