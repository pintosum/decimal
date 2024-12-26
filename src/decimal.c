#include "decimal.h"

#include <stdio.h>


void swap_ptr(void **ptr1, void **ptr2){
  void *temp = *ptr1;
  *ptr1 = *ptr2;
  *ptr2 = temp;
}

void level_decimals(s21_decimal *value1, s21_decimal *value2){
  dec_map *val_bi = (dec_map*) value1;
  dec_map *val_sm = (dec_map*) value2;
  int exp_diff = val_bi->exp - val_sm->exp;
  if(exp_diff < 0){
    swap_ptr((void**)&val_bi, (void**)&val_sm);
    exp_diff = -exp_diff;
  }

  for(int i = 0; i < exp_diff; i++){
    *val_sm->coef = 1;
  }
}


int s21_valid_decimal(s21_decimal* value){
  dec_map *val = (dec_map*)value;
  return val->exp <= 28 && !val->zero_bits && !val->zero_bytes;
}


void print_bytes(s21_decimal *value){
  unsigned char *byte = (unsigned char *)value;
  for (size_t i = 0; i < sizeof(*value); i++){
    printf("%.2x ", *(byte + i));
  }
  printf("\n");
}


int s21_add(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
  int ret = 0;

  if (res && s21_valid_decimal(&val1) && s21_valid_decimal(&val2)){
    return 1;
  }
  else {
    ret = 4;
    if(res){
     // *res = INF;
    }
  }

  return ret;
}

int main(){
  s21_decimal dec = {0};
  // dec_map *val = (dec_map*)&dec;

  print_bytes(&dec);
}
