#ifndef TEST_H
#define TEST_H
#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "../decimal.h"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define NOCOLOR "\033[0m"
#define BLUE "\033[34m"

Suite *test_s21_convertors_test(void);
Suite *test_s21_arithmetic_suite(void);
Suite *test_s21_other_tests(void);

int wrapper_s21_is_less(s21_decimal a, s21_decimal b);
int wrapper_s21_is_equal(s21_decimal a, s21_decimal b);
int wrapper_s21_is_greater(s21_decimal a, s21_decimal b);
int wrapper_s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int wrapper_s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int wrapper_s21_is_not_equal(s21_decimal a, s21_decimal b);
// Suite *...test(void);
// Suite *...test(void);
// Suite *...test(void);
#endif
