#include "../image.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>


void free_image(image img){
    // Deallocate the memory for the image data.
    // This is a common pattern in C and C++ to prevent memory leaks.
    delete[] img.data;
    // Set the data pointer to nullptr to avoid using invalid memory.(dangling pointer)
    img.data = nullptr;
}

image create_empty_image(int channels, int height, int width){
    // Create an image struct with the given width, height, and channels.
    // Return the created image.
    image img;
    img.data = nullptr;
    img.width = width;
    img.height = height;
    img.channels = channels;
    
    return img;

}


image create_image(int channels, int height, int width){
    // Call create_empty_image to handle allocation and initialization
    // Return the created image.
    image img = create_empty_image(channels, height, width);
    
    int total_size = width * height * channels;
    img.data = new float[total_size];

    return img;
    
}

float get_pixel(image img, int channel, int height, int width){
    
    // Clamp the width and height to the size of the image.
    // This is a common trick to handle coordinates outside of the image.
    // It's useful for making sure you don't try to access a pixel outside of the image.
    int clamped_width = std::clamp(width, 0, img.width - 1);
    int clamped_height = std::clamp(height, 0, img.height);

    // Validate channel bounds
    if (channel < 0 || channel >= img.channels){
        std::cerr << "Invalid channel: " << channel << std::endl;
        return 0.0f;
    }

    // index = c * (img.width * img.height) + h * img.width + w
    // Calculate the index of the desired pixel using the formula above.
    // Return the value at that index in the data array.
    int index = channel * (img.width * img.height) + clamped_height * img.width + clamped_width;
    // Return the value at the computed index
    return img.data[index];
}

void set_pixel(image img, int channel, int height, int width, float value){
    
    // Check if the provided coordinates are within valid bounds
    if (channel < 0 || channel >= img.channels || width < 0 || width >= img.width || height < 0 || height >= img.height) {
        std::cerr << "Invalid coordinates: channel=" << channel 
                  << ", width=" << width 
                  << ", height=" << height << std::endl;
        return; // Do nothing if coordinates are invalid
    }
    // Calculate the index of the desired pixel using the same formula.
    // Set the value at that index in the data array to the given value.
    int index = channel *(img.width * img.height) + height * img.width + width;
    img.data[index] = value;

}

image copy_image(image img)
{
    // Create a new image with the same dimensions and channels as the input image
    image copy = create_image(img.channels, img.height, img.width);

    // Calculate the total number of elements in the data array
    int total_size = img.channels * img.height * img.width;
    
    // Copy the data from the original image to the copy image.
    // Copy each element manually
    if (img.data != nullptr) {
        for (int i = 0; i < total_size; ++i) {
            copy.data[i] = img.data[i];
        }
    }

    // // Use std::memcpy to copy the data from the input image to the new image
    // if (img.data != nullptr) {
    //     std::memcpy(copy.data, img.data, total_size * sizeof(float));
    // }
    return copy;
}

image rgb_to_grayscale(image img) {

    // Create a new grayscale image with a single channel
    image gray_image = create_image(1, img.height, img.width);
    // Calculate the offset for each channel in planar layout
    int next_channel = img.width * img.height;

    // Convert RGB to grayscale
    for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
            // Calculate pixel index
            int idx = i * img.width + j;

            // Access R, G, B values from their respective channel offsets
            float r = img.data[idx];
            float g = img.data[idx + next_channel];
            float b = img.data[idx + 2 * next_channel];

            // Apply the grayscale formula
            gray_image.data[idx] = 0.299f * r + 0.587f * g + 0.114f * b;
        }
    }

    return gray_image;
}

void shift_image(image img, int channel, float value) {
    /**
    Function: Shifts a specific color channel by a given value.
    Args:
        img: Input image with planar channel layout.
        channel: The channel index to shift (0 for R, 1 for G, 2 for B).
        value: The value to add to the specified channel.
    */


    // Calculate the offset for the specified channel
    int next_channel = img.width * img.height;
    int channel_offset = channel * next_channel;

    // Shift the specified channel
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            int idx = i * img.width + j;

            // Add the value to the specified channel
            img.data[idx + channel_offset] += value;
        }
    }
}
