#include "../leb128.h"
#include <assert.h>
#include <stdio.h>
#include <math.h>           // Required for: ceil(), log()
#include <stdlib.h>         // Required for: malloc()
#include <limits.h>         // Required for: INT_MAX

int main() {
  uint64_t varints[] = {0, 1, 150, 300, 1<<30, INT_MAX};

  for (int i = 0; i < (sizeof(varints)/sizeof(uint64_t)); ++i) {
    // how long (in bytes) should the buffer be
    // using ln(a)/ln(base) to find the necessary number of bytes to allocate,
    // need to cater for cases where a is 0 or 1, a can't be negatives since we
    // only consider unsigned integers 
    int buf_size = varints[i] < 2 
      ? 1 
      : ceil(log(varints[i])/log(128));

    uint8_t *buf = (uint8_t*) malloc(buf_size*sizeof(uint8_t));
    if (buf == NULL) {
      return -1;
    }

    int len = encodeULEB128(varints[i], buf, buf_size);
    uint64_t decoded = decodeULEB128(buf, len);

    assert(varints[i] == decoded);

    free(buf);
  } 

  return 0;
}
