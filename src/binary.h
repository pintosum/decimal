#ifndef S21_BINARY_H
#define S21_BINARY_H

#include <stdint.h>

#define OVERFLOW 0x02

typedef struct {
  uint32_t mantissa[3];
  uint32_t zero_bytes : 16;
  uint32_t exp : 8;
  uint32_t signal_bits : 7;
  uint32_t sign : 1;
} dec_map;

dec_map shift_mantissa_left_one(dec_map *value);
dec_map shift_mantissa_right_one(dec_map *value);
dec_map shift_mantissa_left(dec_map *value, int shift);
dec_map shift_mantissa_right(dec_map *value, int shift);
dec_map decimal_xor(dec_map *val1, dec_map *val2);
dec_map decimal_and(dec_map *val1, dec_map *val2);

dec_map get_one();
int decimal_is_zero(dec_map *value);

dec_map twos_complement(dec_map *value);
dec_map add_mantisses(dec_map val1, dec_map val2);
dec_map sub_mantisses(dec_map val1, dec_map val2);

dec_map mult_by_pow_of_ten(dec_map *value, int power);

dec_map normalize_decimal(dec_map *value);

int most_significant_bit(dec_map value);
int len_of_number(dec_map value);

#endif
