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
void clamp_image(image img);
float three_way_max(float a, float b, float c);
float three_way_min(float a, float b, float c);
void rgb_to_hsv(image im);
void hsv_to_rgb(image img);
void scale_image(image img, int channel, float value);
float nn_interpolate(image img, int channel, float height, float width);
image nn_resize(image img, int new_height, int new_width);