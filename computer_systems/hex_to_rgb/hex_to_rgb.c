#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>           // Required for: strcmp()

#define STR_MAX_LEN 50

void hex_to_rgb(char* hex, char* rgb, int lim);

int main() {
  char rgb[STR_MAX_LEN];  
  hex_to_rgb("#00ff00", rgb, STR_MAX_LEN); 
  assert(strcmp(rgb, "rgb(0 255 0)") == 0);

  hex_to_rgb("#0000FFC0", rgb, STR_MAX_LEN); 
  assert(strcmp(rgb, "rgb(0 0 255 / 0.75294)") == 0);

  hex_to_rgb("#123", rgb, STR_MAX_LEN); 
  assert(strcmp(rgb, "rgb(17 34 51)") == 0);

  hex_to_rgb("#00f8", rgb, STR_MAX_LEN); 
  assert(strcmp(rgb, "rgb(0 0 255 / 0.53333)") == 0);

  return EXIT_SUCCESS;
}

/* htoi: convert hex digit into int */
uint8_t htoi(char h) {
  if (h >= 48 && h <= 57)
    return h - 48;
  else if (h == 'a' || h == 'A')
    return 10;
  else if (h == 'b' || h == 'B')
    return 11;
  else if (h == 'c' || h == 'C')
    return 12;
  else if (h == 'd' || h == 'D')
    return 13;
  else if (h == 'e' || h == 'E')
    return 14;
  else if (h == 'f' || h == 'F')
    return 15;
  return 0;
}

/* https://www.w3.org/TR/css-color-4/#hex-notation */
void hex_to_rgb(char* hex, char* rgb, int lim) {
  int len = strlen(hex);
  uint8_t r, g, b;
  double a;
  --len;
  if ((hex[0] == '#' && (len == 3 || len == 4 || len == 6 || len == 8)))
    switch (len) {
      case 3:
        r = htoi(hex[1]) * 16 + htoi(hex[1]);
        g = htoi(hex[2]) * 16 + htoi(hex[2]);
        b = htoi(hex[3]) * 16 + htoi(hex[3]);
        snprintf(rgb, lim, "rgb(%d %d %d)", r,g ,b);
        break;
      case 4:
        r = htoi(hex[1]) * 16 + htoi(hex[1]);
        g = htoi(hex[2]) * 16 + htoi(hex[2]);
        b = htoi(hex[3]) * 16 + htoi(hex[3]);
        a = htoi(hex[4]) * 16 + htoi(hex[4]);
        a /= 255;
        snprintf(rgb, lim, "rgb(%d %d %d / %.5f)", r,g ,b, a);
        break;
      case 6:
        r = htoi(hex[1]) * 16 + htoi(hex[2]);
        g = htoi(hex[3]) * 16 + htoi(hex[4]);
        b = htoi(hex[5]) * 16 + htoi(hex[6]);
        snprintf(rgb, lim, "rgb(%d %d %d)", r, g ,b);
        break;
      case 8:
        r = htoi(hex[1]) * 16 + htoi(hex[2]);
        g = htoi(hex[3]) * 16 + htoi(hex[4]);
        b = htoi(hex[5]) * 16 + htoi(hex[6]);
        a = htoi(hex[7]) * 16 + htoi(hex[8]);
        a /= 255;
        snprintf(rgb, lim, "rgb(%d %d %d / %.5f)", r,g ,b, a);
        break;
    }
}
