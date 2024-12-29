#include "decimal.h"

#include <stdio.h>

#include "binary.h"

void swap_ptr(void **ptr1, void **ptr2) {
  void *temp = *ptr1;
  *ptr1 = *ptr2;
  *ptr2 = temp;
}

void level_decimals(s21_decimal *value1, s21_decimal *value2) {
  dec_map *val_big = (dec_map *)value1;
  dec_map *val_sml = (dec_map *)value2;
  int exp_diff = val_sml->exp - val_big->exp;
  if (exp_diff < 0) {
    swap_ptr((void **)&val_big, (void **)&val_sml);
    exp_diff = -exp_diff;
  }

  for (int i = 0; i < exp_diff; i++) {
    *val_sml = add_mantisses(shift_mantissa_left(val_sml, 3),
                             shift_mantissa_left(val_sml, 1));
  }
}

int s21_valid_decimal(s21_decimal *value) {
  dec_map *val = (dec_map *)value;
  return val->exp <= 28 && !val->signal_bits && !val->zero_bytes;
}

void print_bytes(s21_decimal *value) {
  unsigned char *byte = (unsigned char *)value;
  for (int i = sizeof(s21_decimal) - 1; i >= 0; i--) {
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
  // s21_decimal dec = {0};
  // dec_map m = {{0, 0, 0x800000}, 0, 0, 0, 0};

  // print_bytes((s21_decimal *)&m);
  // dec_map s = shift_mantissa_left_one(&m);
  // print_bytes((s21_decimal *)&s);
  // m = shift_mantissa_left_one(&s);

  // print_bytes((s21_decimal *)&m);

  double d = 0;
  for (int i = 0; i < 100000; i++) {
    d += 1;
  }
  printf("%f\n", d);
}
