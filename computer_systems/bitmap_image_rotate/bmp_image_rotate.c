#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bmp.h"

BMP_Image * bmp_open(const char * filename);
BMP_Image * bmp_save(BMP_Image *img, char* filename);
Pixel read_pixel(int x, int y, BMP_Image *img);
void write_pixel(int x, int y, BMP_Image* img, Pixel p);
void * clean_up(FILE *fp, BMP_Image *img);

int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Usage: \t %s BITMAP OUTPUT\n", argv[0]);
    return EXIT_FAILURE;
  }

  /* read the bitmap file into a struct */
  BMP_Image *img = bmp_open(*++argv);
  if (img == nullptr)
    return EXIT_FAILURE;

  /* create the struct of the bitmap to output */
  BMP_Image *outImg = (BMP_Image*)malloc(sizeof(BMP_Image));
  if (outImg == nullptr)
    return EXIT_FAILURE;

  /* copy the header of original bitmap */
  memcpy(&(outImg->header), &(img->header), sizeof(BMP_Header));

  /* Adjust headers values to match the new bitmap */
  outImg->header.bfOffBits = 54;
  outImg->header.biSize = 40;
  outImg->height = outImg->header.biHeight;
  outImg->width = outImg->header.biWidth;

  outImg->pixel_data = (Pixel*)malloc(outImg->header.biSizeImage);
  if (outImg->pixel_data == nullptr)
    return EXIT_FAILURE;

  for (int y = 0; y < outImg->height; ++y) {
    for (int x = 0; x < outImg->width; ++x) {
      Pixel p = read_pixel(x, y, img);
      /* A 90 degress clockwise rotation is equivalent to
         x->y , y = -x and y += img->height */
      write_pixel(y, -x + outImg->height, outImg, p);
    }
  }

  if (bmp_save(outImg, *++argv) == nullptr)
    return EXIT_FAILURE;

  clean_up(nullptr, outImg);

  return EXIT_SUCCESS;
}

BMP_Image * bmp_open(const char * filename) {
  FILE *fp = nullptr;
  BMP_Image *img = (BMP_Image*)malloc(sizeof(BMP_Image));
  if (img == nullptr) return clean_up(fp, img);

  fp = fopen(filename, "r");
  if (fp == nullptr) {
    printf("Can't open file %s\n", filename);
    return clean_up(fp, img);
  }

  if (fread(&(img->header), sizeof(BMP_Header), 1, fp) != 1)
    return clean_up(fp, img);

  if (img->header.bfType != BITMAP_MAGIC_NUMBER) {
    printf("The file %s is not a valid bitmap\n", filename);
    return clean_up(fp, img);
  }

  img->width = img->header.biWidth;
  img->height = img->header.biHeight;

  /* Set the file position indicator to the offset of actual pixel data */
  if(fseek(fp, img->header.bfOffBits, SEEK_SET) != 0)
    return clean_up(fp, img);

  img->pixel_data = (Pixel*)malloc(img->header.biSizeImage);
  if (img->pixel_data == nullptr)
    return clean_up(fp, img);

  /* Size, in pixels, of the pixel data section */
  uint32_t pixelSizeImage = img->header.biSizeImage/sizeof(Pixel);
  if (fread(img->pixel_data, sizeof(Pixel), pixelSizeImage, fp) != pixelSizeImage)
    return clean_up(fp, img);

  return img;
}

Pixel read_pixel(int x, int y, BMP_Image *img) {
  return img->pixel_data[y * img->height + x];
}

void write_pixel(int x, int y, BMP_Image* img, Pixel p) {
  img->pixel_data[y * img->height + x] = p;
}

void * clean_up(FILE *fp, BMP_Image *img) {
  fclose(fp);
  free(img->pixel_data);
  free(img);
  return nullptr;
}

BMP_Image * bmp_save(BMP_Image *img, char* filename) {
  FILE* fp = fopen(filename, "w");
  if (fp == nullptr)
    return clean_up(fp, img);

  if (fwrite(&(img->header), sizeof(BMP_Header), 1, fp) != 1) 
    return clean_up(fp, img);

  if (fwrite(img->pixel_data, img->header.biSizeImage, 1, fp) != 1)
    return clean_up(fp, img);

  return img;
}
