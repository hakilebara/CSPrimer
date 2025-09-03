#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define OUTPUT_SIZE 100000

/* getUTF8Length: return length of UTF8 symbol, returns -1 on continuation bytes */
int getUTF8Length(uint8_t c) {
  if ((c & 0b1000'0000) == 0) return 1;             // 0xxxxxxx
  if ((c & 0b1110'0000) == 0b1100'0000) return 2;   // 110xxxxx
  if ((c & 0b1111'0000) == 0b1110'0000) return 3;   // 1110xxxx
  if ((c & 0b1111'1000) == 0b1111'0000) return 4;   // 11110xxx
  return -1;
}

int main(void) {
  FILE *fptr = fopen("cases", "r");
  assert(fptr != nullptr);

  char *output = (char*)malloc(OUTPUT_SIZE);
  assert(output != nullptr);

  char *output_str = output;
  char * line = nullptr;
  size_t linecap = 0;
  ssize_t linelen;
  uint8_t c;

  while ((linelen = getline(&line, &linecap, fptr)) > 0) {
    /* can't modify line pointer directly because getline expects the same base pointer to
     * malloc/realloc*/
    char *line_cursor = line;
    int byte_limit = (uint8_t)*line_cursor++;

    while (byte_limit > 0 && (c = *line_cursor)) {
      int utf8_len = getUTF8Length(c);
      assert(utf8_len > 0);

      if (utf8_len <= byte_limit)
        while (utf8_len-- > 0) {
          *output++ = *line_cursor++;
          --byte_limit;
        }
      else
       --byte_limit;
    }
    if (byte_limit < 1 && *output != '\n')
      *output++ = '\n';
  }
  
  *output = '\0';
  printf("%s",output_str);

  fclose(fptr);
  free(line);
  free(output_str);
  return EXIT_SUCCESS;
}
