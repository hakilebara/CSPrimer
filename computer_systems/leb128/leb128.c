#include "leb128.h"

/* encodeULEB128: encode n into unsigned LEB128, return length */
int encodeULEB128(uint64_t n, uint8_t* buf, const int len) {
  int i = 0;

  while (i < len && n > 0) {
    *(buf++) = (n > 127) ? (uint8_t)(n | 1<<7) : (uint8_t)n;
    n >>= 7;
    ++i;
  }
  return i;
}

/* decodeULEB128: decode unsigned LEB128 */
uint64_t decodeULEB128(const uint8_t* buf, int len) {
  uint64_t acc = 0;
  while (--len >= 0)
    /* first set the most significant bit to 0, then convert to big endian by
       multiplying the resulting value by the appropriate power of 2 */
    acc += (buf[len] & ~(1 << 7)) * (1 << (7 * len)); 

  return acc;
}
