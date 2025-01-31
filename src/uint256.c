#include "uint256.h"

#include <stdio.h>

// #include "binary.h"
#include "decimal.h"
#define sizeofarray(a) (sizeof(a) / sizeof(a[0]))

void print_uint256(uint256 a, char *name) {
  puts(name);
  unsigned char *byte = (unsigned char *)&a;
  for (int i = sizeof(uint256) - 1; i >= 0; i--) {
    printf("%.2x ", *(byte + i));
  }
  puts("");
}

uint256 uint256_and(uint256 a, uint256 b) {
  for (int i = 0; i < sizeofarray(a.bits); i++) {
    a.bits[i] &= b.bits[i];
  }
  return a;
}

uint256 uint256_xor(uint256 a, uint256 b) {
  for (int i = 0; i < sizeofarray(a.bits); i++) {
    a.bits[i] ^= b.bits[i];
  }
  return a;
}

uint256 shift_uint256_left_one(uint256 a) {
  uint256 shifted = a;
  uint64_t carry = 0;
  for (int i = 0; i < sizeofarray(a.bits); i++) {
    shifted.bits[i] = a.bits[i] << 1;
    shifted.bits[i] |= carry;
    carry = a.bits[i] >> 63 & 1;
  }
  return shifted;
}

uint256 shift_uint256_right_one(uint256 a) {
  uint256 shifted = a;
  uint64_t carry = 0;
  for (int i = sizeofarray(a.bits) - 1; i >= 0; i--) {
    shifted.bits[i] = a.bits[i] >> 1;
    shifted.bits[i] |= carry << 63;
    carry = a.bits[i] & 1;
  }
  return shifted;
}

uint256 shift_uint256_left(uint256 a, unsigned int shift) {
  while (shift--) {
    a = shift_uint256_left_one(a);
  }
  return a;
}

uint256 shift_uint256_right(uint256 a, unsigned int shift) {
  while (shift--) {
    a = shift_uint256_right_one(a);
  }
  return a;
}

int uint256_is_zero(uint256 a) {
  int ret = 1;
  for (int i = 0; i < sizeofarray(a.bits); i++)
    ret *= !a.bits[i];
  return ret;
}

uint256 uint256_get_one() { return (uint256){{1, 0, 0, 0}}; }

uint256 uint256_twos_complement(uint256 a) {
  for (int i = 0; i < sizeofarray(a.bits); i++) {
    a.bits[i] = ~a.bits[i];
  }
  a = uint256_add(a, uint256_get_one());
  return a;
}

uint256 uint256_add(uint256 a, uint256 b) {
  while (!uint256_is_zero(b)) {
    uint256 carry_bits = uint256_and(a, b);
    carry_bits = shift_uint256_left_one(carry_bits);
    a = uint256_xor(a, b);
    b = carry_bits;
  }
  return a;
}

uint256 uint256_sub(uint256 a, uint256 b) {
  b = uint256_twos_complement(b);
  b = uint256_add(a, b);
  if (b.bits[3] >> 63 == 1) {
    b = uint256_twos_complement(b);
    b.bits[3] = 1ULL << 63;
  }
  return b;
}

uint256 uint256_from_decimal(s21_decimal a) {
  uint256 ret = {{0, 0, 0, 0}};
  ret.bits[0] = a.bits[0];
  ret.bits[0] += a.bits[1] * (1UL << 32);
  ret.bits[1] = a.bits[2];
  return ret;
}

uint256 uint256_mult(uint256 a, uint256 b) {
  uint256 ret = {0};
  while (!uint256_is_zero(b)) {
    if (b.bits[0] & 1) {
      ret = uint256_add(a, ret);
    }
    b = shift_uint256_right(b, 1);
    a = shift_uint256_left(a, 1);
  }
  return ret;
}

uint64_t min(uint64_t a, uint64_t b) {
  if (a < b)
    return a;
  else
    return b;
}

int uint256_divisible_by_ten(uint256 a) {
  uint64_t sum = 0;
  for (int i = 0; i < 4; i++) {
    uint64_t m = min(sum, a.bits[i]);
    sum += a.bits[i];
    if (sum < m)
      sum++;
  }
  return !(sum % 5) && !(a.bits[0] % 2);
}

int uint256_most_significant_bit(uint256 value) {
  int ret = 0;
  while (!uint256_is_zero(value)) {
    value = shift_uint256_right_one(value);
    ret++;
  }
  return ret;
}

int len_of_uint256(uint256 value) {
  double log_of_2 = 0.301;
  int binary_len = uint256_most_significant_bit(value);
  return (int)(binary_len * log_of_2) + 1;
}

s21_decimal s21_decimal_from_uint256(uint256 a) {
  s21_decimal ret = {0};
  int len = len_of_uint256(a);
  unsigned int digit = 0;
  while (len > 29) {
    a = uint256_divide_by_ten(a, &digit);
    len--;
    ret.fields.exp--;
  }

  ret.bits[0] = a.bits[0] & 0xFFFFFFFF;
  ret.bits[1] = a.bits[0] & 0xFFFFFFFF00000000;
  ret.bits[2] = a.bits[1] & 0xFFFFFFFF;

  // printf("exp : %u\nlen : %d\n", ret.fields.exp, len);
  if(digit >= 5){
    a = uint256_add(a, uint256_get_one());
  }
  return ret;
}

uint256 uint256_mult_by_pow_of_ten(uint256 ret, int power) {
  while (power-- > 0) {
    ret = uint256_add(shift_uint256_left(ret, 3), shift_uint256_left(ret, 1));
  }
  return ret;
}

uint256 uint256_divide_by_ten(uint256 value, unsigned int *remainder) {
  uint256 q, r;
  q = uint256_add(shift_uint256_right(value, 1), shift_uint256_right(value, 2));
  q = uint256_add(q, shift_uint256_right(q, 4));
  q = uint256_add(q, shift_uint256_right(q, 8));
  q = uint256_add(q, shift_uint256_right(q, 16));
  q = uint256_add(q, shift_uint256_right(q, 32));
  q = uint256_add(q, shift_uint256_right(q, 64));
  q = uint256_add(q, shift_uint256_right(q, 128));
  q = shift_uint256_right(q, 3);

  r = uint256_mult_by_pow_of_ten(q, 1);
  r = uint256_sub(value, r);

  if (r.bits[0] > 9) {
    q = uint256_add(q, uint256_get_one());
    r.bits[0] -= 10;
  }
  if (remainder)
    *remainder = r.bits[0];
  return q;
}

/*int main() {
  uint256 a = {{0x566553, 0x145213, 0, 0}};
  // print_uint256(a, "a");
  uint256 b = {{0x474, 0x23, 0, 0}};
  // print_uint256(b, "b");
  int arr[10] = {0};
  printf("%lu\n", sizeofarray(a.bits));
  shift_uint256_left(a, 1);
  uint256_add(a, b);
  // print_uint256(uint256_mult(a, b), "res");

  uint64_t l = 0xFFFFFFFFFFFFFFFF;
  printf("l  : %lu\n", l);
  uint256 l2 = {{l, 0x43123fdc, 0xdaf, 0x1238760}};
  unsigned int rem = 0;
  uint256 res = uint256_divide_by_ten(l2, &rem);
  // print_uint256(uint256_divide_by_ten(l2, NULL), "div");
  // printf("l2 : %lu\n", l2.bits[0]);
  // print_uint256(l2, "l2");
  printf("res: %lu\nrem: %u\n", res.bits[0], rem);
  printf("here :: %lu\n", 0x1FFFF1008);
}*/
