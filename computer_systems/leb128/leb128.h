#ifndef LEB128_H
#define LEB128_H

#include <inttypes.h>

int encodeULEB128(uint64_t n, uint8_t* buf, const int len);
uint64_t decodeULEB128(const uint8_t* buf, int len);

#endif
