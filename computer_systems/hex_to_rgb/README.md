# hex_to_rgb

A short program to convert color declarations in from hexadecimal to rgb form.

Follows the [W3C CSS hex notion spec](https://www.w3.org/TR/css-color-4/#hex-notation) 

## Usage

```c
  #define STR_MAX_LEN 50

  char rgb[STR_MAX_LEN];  
  hex_to_rgb("#00ff00", rgb, STR_MAX_LEN); 

  assert(strcmp(rgb, "rgb(0 255 0)") == 0);
```
