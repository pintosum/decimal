#ifndef S21_CONVERTORS_FUNCTIONS_H
#define S21_CONVERTORS_FUNCTIONS_H

#include "../decimal.h"
#include <math.h>

int s21_get_bit(s21_decimal number, int position);
void s21_set_bit(s21_decimal *number, int position);
int s21_get_sign(s21_decimal number);
void s21_set_sign(s21_decimal *dst);
void s21_delete_sign(s21_decimal *number);
int s21_get_degree(s21_decimal value);
void s21_delete_degree(s21_decimal *value);
void s21_set_degree(s21_decimal *value, int deg);

#endif
