#include "decimal.h"

#include <stdio.h>

int shift_bits_left_in_mantissa(dec_map *value) {
  int ret = 0;
  int overflowing = value->signal_bits >> 6 & 1;
  if (!overflowing) {
    int carry = 0;
    int highest_bit = 0;
    for (int i = 0; i < 3; i++) {
      highest_bit = value->mantissa[i] >> 31 & 1;
      value->mantissa[i] <<= 1;
      value->mantissa[i] |= carry;
      carry = highest_bit;
    }
    value->signal_bits <<= 1;
    value->signal_bits |= carry;
  } else
    ret = 1;
  return ret;
}

void swap_ptr(void **ptr1, void **ptr2) {
  void *temp = *ptr1;
  *ptr1 = *ptr2;
  *ptr2 = temp;
}

void level_decimals(s21_decimal *value1, s21_decimal *value2) {
  dec_map *val_big = (dec_map *)value1;
  dec_map *val_sml = (dec_map *)value2;
  int exp_diff = val_big->exp - val_sml->exp;
  if (exp_diff < 0) {
    swap_ptr((void **)&val_big, (void **)&val_sml);
    exp_diff = -exp_diff;
  }

  for (int i = 0; i < exp_diff; i++) {
    return;
  }
}

int s21_valid_decimal(s21_decimal *value) {
  dec_map *val = (dec_map *)value;
  return val->exp <= 28 && !val->signal_bits && !val->zero_bytes;
}

void print_bytes(s21_decimal *value) {
  unsigned char *byte = (unsigned char *)value;
  for (size_t i = 0; i < sizeof(*value); i++) {
    printf("%.2x ", *(byte + i));
  }
  printf("\n");
}

int s21_add(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
  int ret = 0;

  if (res && s21_valid_decimal(&val1) && s21_valid_decimal(&val2)) {
    return 1;
  } else {
    ret = 4;
    if (res) {
      // *res = INF;
    }
  }

  return ret;
}

int main() {
  s21_decimal dec = {0};
  // dec_map *val = (dec_map*)&dec;

  print_bytes(&dec);
}
