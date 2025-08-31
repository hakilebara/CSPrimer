#ifndef BMP_H
#define BMP_H

#include <stdlib.h>

#define BITMAP_MAGIC_NUMBER 0X4D42

typedef struct __attribute__((packed)) {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Pixel;

typedef struct __attribute__((packed)) {
  uint16_t bfType;
  uint32_t bfSize;
  uint16_t bfReserved1;     // Always 0
  uint16_t bfReserved2;     // Always 0
  uint32_t bfOffBits;
  uint32_t biSize;          // Size of the bitmap info header
  uint32_t biWidth;         // Image width in pixel
  uint32_t biHeight;        // Image height in pixel
  uint16_t biPlanes;
  uint16_t biBitCount;      // Bits per pixel
  uint32_t biCompression;   // Compression method
  uint32_t biSizeImage;     // Byte size of pixel data section     
  uint32_t biXPelsPerMeter; // Print resolution
  uint32_t biYPelsPerMeter; // Print resolution
  uint32_t biClrUsed;       // Colours in colour index
  uint32_t biClrImportant;  // Count of 'important' colours
} BMP_Header;

typedef struct {
  BMP_Header header;
  uint32_t width;
  uint32_t height;
  Pixel* pixel_data;
} BMP_Image;

#endif
