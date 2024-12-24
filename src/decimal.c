#include "decimal.h"


int s21_valid_decimal(s21_decimal* value){
  s21_decimal_h *val = (s21_decimal_h*)value;
  uint32_t flag = val->zero_bits + val->zero_bytes + val->exp;
  return flag <= 28;
}




int s21_add(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
  int ret = 0;

  if (res && s21_valid_decimal(&val1) && s21_valid_decimal(&val2)){

  }
  else {
    ret = 4;
    if(res){
     // *res = INF;
    }
  }

  return ret;
}
