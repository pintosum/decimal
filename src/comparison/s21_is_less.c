#include "../binary.h"
#include "../decimal.h"

int s21_is_less(s21_decimal a, s21_decimal b) {
  int result = 0;

  if (s21_is_valid_decimal(&a) && s21_is_valid_decimal(&b)) {
    int sign_a = a.fields.sign;
    int sign_b = b.fields.sign;

    if (sign_a != sign_b) {
      result = sign_a > sign_b;
    } else {
      s21_decimal a_copy = a;
      s21_decimal b_copy = b;

      level_decimals(&a_copy, &b_copy);

      int i = 2;
      while (i >= 0 && a_copy.bits[i] == b_copy.bits[i]) {
        i--;
      }

      if (i >= 0) {
        if (!sign_a) {
          result = a_copy.bits[i] < b_copy.bits[i];
        } else {
          result = a_copy.bits[i] > b_copy.bits[i];
        }
      }
    }
  }
  return result;
}
