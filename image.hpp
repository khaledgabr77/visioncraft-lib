#pragma once

#include<iostream>

typedef struct{
    int width;
    int height;
    int channels;
    float *data;
}image;

image create_empty_image(int channels, int height, int width);                      // TESTED
image create_image(int channels, int height, int width);                            // TESTED
void free_image(image img);                                                         // TESTED
float get_pixel(image img, int channel, int height, int width);                     // TESTED
void set_pixel(image img, int channel, int height, int width, float value);         // TESTED
image copy_image(image img);                                                        // TESTED

image rgb_to_grayscale(image img);
void shift_image(image img, int channel, float value);
// clamp_image();
// three_way_max();
// three_way_min();
// rgb_to_hsv();
// hsv_to_rgb();
// image load_image(char *filename);
// void save_image(image im, const char *name);
// void save_image_binary(image im, const char *fname);
// image load_image_binary(const char *fname);
// void save_png(image im, const char *name);