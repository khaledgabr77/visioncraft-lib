#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void save_image_stb(image im, const char *name, int png) {
    char buff[256];
    unsigned char *data = (unsigned char*) calloc(im.width * im.height * im.channels, sizeof(char));
    int i, k;
    for (k = 0; k < im.channels; ++k) {
        for (i = 0; i < im.width * im.height; ++i) {
            data[i * im.channels + k] = (unsigned char) std::round(255 * im.data[i + k * im.width * im.height]);
        }
    }
    int success = 0;
    if (png) {
        sprintf(buff, "%s.png", name);
        success = stbi_write_png(buff, im.width, im.height, im.channels, data, im.width * im.channels);
    } else {
        sprintf(buff, "%s.jpg", name);
        success = stbi_write_jpg(buff, im.width, im.height, im.channels, data, 100);
    }
    free(data);
    if (!success) std::cerr << "Failed to write image " << buff << std::endl;
}

void save_png(image im, const char *name) {
    save_image_stb(im, name, 1);
}

void save_image(image im, const char *name) {
    save_image_stb(im, name, 0);
}

image load_image_stb(char *filename, int channels) {
    int w, h, c;
    unsigned char *data = stbi_load(filename, &w, &h, &c, channels);
    if (!data) {
        std::cerr << "Cannot load image \"" << filename << "\"\nSTB Reason: " << stbi_failure_reason() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if (channels) c = channels;
    image im = create_image(c, h, w);
    for (int k = 0; k < c; ++k) {
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                int dst_index = i + w * j + w * h * k;
                int src_index = k + c * i + c * w * j;
                im.data[dst_index] = static_cast<float>(data[src_index]) / 255.0f;
            }
        }
    }
    if (im.channels == 4) im.channels = 3;  // We don't like alpha channels
    free(data);
    return im;
}

image load_image(char *filename) {
    return load_image_stb(filename, 0);
}

void save_image_binary(image im, const char *fname) {
    FILE *fp = fopen(fname, "wb");
    fwrite(&im.width, sizeof(int), 1, fp);
    fwrite(&im.height, sizeof(int), 1, fp);
    fwrite(&im.channels, sizeof(int), 1, fp);
    fwrite(im.data, sizeof(float), im.width * im.height * im.channels, fp);
    fclose(fp);
}

image load_image_binary(const char *fname) {
    int w = 0, h = 0, c = 0;
    FILE *fp = fopen(fname, "rb");
    fread(&w, sizeof(int), 1, fp);
    fread(&h, sizeof(int), 1, fp);
    fread(&c, sizeof(int), 1, fp);
    image im = create_image(c, h, w);
    fread(im.data, sizeof(float), im.width * im.height * im.channels, fp);
    return im;
}

