#ifndef S21_DECIMAL
#define S21_DECIMAL

#include <stdint.h>

typedef struct {
  int32_t bits[4];
} s21_decimal;

typedef struct {
  uint32_t coef[3];
  uint32_t zero_bytes : 16;
  uint32_t exp : 8;
  uint32_t zero_bits : 7;
  uint32_t sign : 1;
} dec_map;

/**
 * @group Arithmetic
 *
 *  @return
 *    0 - OK
 *    1 - number is too large or equal to inf
 *    2 - number is too small or equal to negative inf
 *    3 - division by 0
 *    4 - error
 * */
int s21_add(s21_decimal, s21_decimal, s21_decimal *);
int s21_sub(s21_decimal, s21_decimal, s21_decimal *);
int s21_mul(s21_decimal, s21_decimal, s21_decimal *);
int s21_div(s21_decimal, s21_decimal, s21_decimal *);

/**
 * @group Comparison
 *
 *  @return
 *    0 - FALSE
 *    1 - TRUE
 * */
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

/**
 * @group Convertors
 *
 *    @return
 *      0 - OK
 *      1 - convertation error
 * */
int s21_from_int_to_decimal(int, s21_decimal);
int s21_from_float_to_decimal(float, s21_decimal);
int s21_from_decimal_to_int(s21_decimal, int);
int s21_from_decimal_to_float(s21_decimal, float);

/**
 * @group Other
 *
 *    @return
 *      0 - OK
 *      1 - calculation error
 * */
int s21_floor(s21_decimal, s21_decimal *);
int s21_round(s21_decimal, s21_decimal *);
int s21_truncate(s21_decimal, s21_decimal *);
int s21_negate(s21_decimal, s21_decimal *);

#endif
