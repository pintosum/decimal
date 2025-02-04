#include "../binary.h"
#include "../decimal.h"

int s21_is_equal(s21_decimal a, s21_decimal b) {
  int result = 0;

  if (s21_is_valid_decimal(&a) && s21_is_valid_decimal(&b)) {

    if (s21_decimal_is_zero(&a) && s21_decimal_is_zero(&b)) {
      result = 1;
    } else if (a.fields.sign == b.fields.sign) {
      s21_decimal a_copy = a;
      s21_decimal b_copy = b;

      level_decimals((s21_decimal *)&a_copy, (s21_decimal *)&b_copy);

      s21_decimal diff = s21_sub_mantisses(b_copy, a_copy);
      result = s21_decimal_is_zero(&diff);
    }
  }

  return result;
}
