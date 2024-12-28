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
dec_map twos_complement(dec_map *value);
dec_map get_one();
dec_map sum_mantisses(dec_map val1, dec_map val2);

#endif
