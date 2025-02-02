#include "uint256.h"

#include "decimal.h"
#define sizeofarray(a) (sizeof(a) / sizeof(a[0]))

void print_uint256(uint256 r, char *name) {
  printf("%s : \n  ", name);
  char str[100] = {1};
  int len = len_of_uint256(r);
  int i = 0;
  do {
    i++;
    unsigned int digit = 0;
    r = uint256_divide_by_ten(r, &digit);
    str[--len] = digit + '0';
  } while (!uint256_is_zero(r));

  printf("%s\n", str);
}

void print_uint256_bytes(uint256 a, char *name) {
  puts(name);
  unsigned char *byte = (unsigned char *)&a;
  for (int i = sizeof(uint256) / 4 - 1; i >= 0; i--) {
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
  int ret = 0;
  while(!uint256_is_zero(value)){
    value = uint256_divide_by_ten(value, NULL);
    ret++;
  }
  return ret;
}

unsigned int uint256_get_bit(uint256 a, unsigned int bit) {
  int i = bit / 64;
  int b = bit % 64;
  return (a.bits[i] >> b) & 1;
}

uint256 uint256_set_bit(uint256 a, unsigned int bit) {
  unsigned int i = bit / 64;
  unsigned int b = bit % 64;
  a.bits[i] ^= 1 << b;
  return a;
}

int uint256_is_greater_or_equal(uint256 a, uint256 b) {
  int ret = 0;
  int i = 3;
  while (i >= 0 && a.bits[i] == b.bits[i]) {
    i--;
  }

  if (i >= 0) {
    ret = a.bits[i] >= b.bits[i];
  } else {
    ret = 1;
  }

  return ret;
}

uint256 uint256_div(uint256 num, uint256 div, uint256 *remainder) {
  uint256 quot = {0};
  uint256 rem = {0};
  for (int i = uint256_most_significant_bit(num) - 1; i >= 0; i--) {
    rem = shift_uint256_left_one(rem);
    rem.bits[0] |= uint256_get_bit(num, i);
    if (uint256_is_greater_or_equal(rem, div)) {
      rem = uint256_sub(rem, div);
      quot = uint256_set_bit(quot, i);
    }
  }
  if (remainder)
    *remainder = rem;
  return quot;
}

s21_decimal s21_decimal_from_uint256(uint256 a) {
  s21_decimal ret = {0};
  int msb = uint256_most_significant_bit(a);
  unsigned int digit = 0;
  while (msb > 95) {
    a = uint256_divide_by_ten(a, &digit);
    ret.fields.exp++;
    msb = uint256_most_significant_bit(a);
  }

  ret.bits[0] = a.bits[0] & 0xFFFFFFFF;
  ret.bits[1] = a.bits[0] >> 32;
  ret.bits[2] = a.bits[1] & 0xFFFFFFFF;

  if (digit >= 5) {
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
