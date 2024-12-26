#pragma once

#include<iostream>

typedef struct{
    int width;
    int height;
    int channels;
    float *data;
}image;

image create_empty_image(int width, int height, int channels);
image create_image(int width, int height, int channels);
void free_image(image img);
float get_pixel(image img, int channel, int width, int height);
void set_pixel(image img, int channel, int width, int height, float value);
image copy_image(image im);
// make_image();
// rgb_to_grayscale();
// shift_image();
// clamp_image();
// three_way_max();
// three_way_min();
// rgb_to_hsv();
// hsv_to_rgb();
