#include "../image.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <cmath>


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
    int clamped_height = std::clamp(height, 0, img.height - 1);

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

void clamp_image(image img){

    int total_size = img.width * img.height * img.channels;

    for(int i=0; i < total_size; i++){
        img.data[i] = std::clamp(img.data[i], 0.f, 1.f);

        // if (img.data[i] < 0.f) img.data[i] = 0.f;
        // else if (img.data[i] > 1.f) img.data[i] = 1.f;
    }
    
}
float three_way_max(float a, float b, float c){
    return (a > b) ? ( (a > c) ? a : c) : ((b > c) ? b : c);
}
    

float three_way_min(float a, float b, float c){
    return (a < b) ? ( (a < c) ? a : c) : ((b < c) ? b : c);
}

void rgb_to_hsv(image img) {
    float H, S, V;  // Hue, Saturation, Value
    float C, H_ = 0;  // Chroma and intermediate Hue
    float R, G, B;  // Red, Green, Blue components

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            // Retrieve pixel values
            R = get_pixel(img, 0, i, j);
            G = get_pixel(img, 1, i, j);
            B = get_pixel(img, 2, i, j);

            // Calculate Value (V), Chroma (C), and Minimum (m)
            V = three_way_max(R, G, B);
            float m = three_way_min(R, G, B);
            C = V - m;

            // Calculate Saturation (S)
            S = (V == 0) ? 0 : (C / V);

            // Calculate Hue (H)
            if (C == 0) {
                H_ = 0;  // Undefined Hue
            } else if (V == R) {
                H_ = (G - B) / C;
            } else if (V == G) {
                H_ = (B - R) / C + 2;
            } else if (V == B) {
                H_ = (R - G) / C + 4;
            }

            // Normalize Hue to range [0, 1]
            if (H_ < 0) {
                H = H_ / 6 + 1;
            } else {
                H = H_ / 6;
            }

            // Set pixel values for HSV
            set_pixel(img, 0, i, j, H);
            set_pixel(img, 1, i, j, S);
            set_pixel(img, 2, i, j, V);
        }
    }
}

void hsv_to_rgb(image img) {
    float H, S, V;  // Hue, Saturation, Value
    float R, G, B;  // Red, Green, Blue
    float C, X, m;  // Chroma, secondary component, and match value

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            // Retrieve HSV values
            H = get_pixel(img, 0, i, j);  // Hue [0, 1]
            S = get_pixel(img, 1, i, j);  // Saturation [0, 1]
            V = get_pixel(img, 2, i, j);  // Value [0, 1]

            // Calculate Chroma (C), Secondary component (X), and Match value (m)
            C = V * S;
            X = C * (1 - fabs(fmod(H * 6, 2) - 1));
            m = V - C;

            // Determine RGB values based on Hue sector
            if (H >= 0 && H < 1.0 / 6) {       // Sector 0
                R = C;
                G = X;
                B = 0;
            } else if (H < 2.0 / 6) {          // Sector 1
                R = X;
                G = C;
                B = 0;
            } else if (H < 3.0 / 6) {          // Sector 2
                R = 0;
                G = C;
                B = X;
            } else if (H < 4.0 / 6) {          // Sector 3
                R = 0;
                G = X;
                B = C;
            } else if (H < 5.0 / 6) {          // Sector 4
                R = X;
                G = 0;
                B = C;
            } else {                           // Sector 5
                R = C;
                G = 0;
                B = X;
            }

            // Add match value to shift [0, 1] range to [m, V]
            R += m;
            G += m;
            B += m;

            // Set RGB values back into the image
            set_pixel(img, 0, i, j, R);
            set_pixel(img, 1, i, j, G);
            set_pixel(img, 2, i, j, B);
        }
    }
}


void scale_image(image img, int channel, float value){

    float pixel;
    for (int i = 0; i < img.height; i++){
        for (int j = 0; j < img.width; j++){
            pixel = get_pixel(img, channel, i, j);
            set_pixel(img, channel, i, j, pixel * value);

            }
        }
}



float nn_interpolate(image img, int channel, float row, float col) {
    // Scale the target coordinates to original image space
    float original_x = col;
    float original_y = row;

    // Find the nearest neighbor in the original image
    int nearest_x = static_cast<int>(std::round(original_x));
    int nearest_y = static_cast<int>(std::round(original_y));

    // Clamp coordinates to ensure they're within bounds
    nearest_x = std::clamp(nearest_x, 0, img.width - 1);
    nearest_y = std::clamp(nearest_y, 0, img.height - 1);

    // Return the pixel value from the original image
    return get_pixel(img, channel, nearest_y, nearest_x);
}


image nn_resize(image img, int new_height, int new_width) {
    // Create a new image with the specified dimensions
    image new_image = create_image(img.channels, new_height, new_width);

    // Scaling factors for mapping new coordinates to original image space
    float scale_x = static_cast<float>(img.width) / (new_width);
    float scale_y = static_cast<float>(img.height) / (new_height);

    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j++) {
            for (int c = 0; c < img.channels; c++) {
                // Compute original coordinates
                float row = i * scale_y;
                float col = j * scale_x;

                // Use nn_interpolate to get the interpolated pixel value
                float pixel_value = nn_interpolate(img, c, row, col);

                // Set the computed pixel value in the new image
                set_pixel(new_image, c, i, j, pixel_value);
            }
        }
    }

    return new_image;
}

float bilinear_interpolate(image img, int c, float h, float w)
{
    // Find the integer coordinates of the top-left corner
    int x1 = static_cast<int>(std::floor(w));
    int y1 = static_cast<int>(std::floor(h));

    // Find the integer coordinates of the bottom-right corner
    int x2 = x1 + 1;
    int y2 = y1 + 1;

    // Clamp coordinates to be within the image bounds
    x1 = std::clamp(x1, 0, img.width - 1);
    y1 = std::clamp(y1, 0, img.height - 1);
    x2 = std::clamp(x2, 0, img.width - 1);
    y2 = std::clamp(y2, 0, img.height - 1);

    // Calculate the fractional parts
    float a = w - x1;
    float b = h - y1;

    // Fetch the pixel values at the four surrounding points
    float I11 = get_pixel(img, c, y1, x1);
    float I21 = get_pixel(img, c, y1, x2);
    float I12 = get_pixel(img, c, y2, x1);
    float I22 = get_pixel(img, c, y2, x2);

    // Perform bilinear interpolation
    return (1 - a) * (1 - b) * I11 + a * (1 - b) * I21 + (1 - a) * b * I12 + a * b * I22;
}

image bilinear_resize(image img, int new_height, int new_width)
{
    // Create a new image with the specified dimensions
    image resized_img = create_image(img.channels, new_height, new_width);

    // Scaling factors for mapping new coordinates to original image space
    float scale_x = static_cast<float>(img.width) / new_width;
    float scale_y = static_cast<float>(img.height) / new_height;

    // Loop over each pixel in the resized image
    for (int i = 0; i < new_height; ++i) {
        for (int j = 0; j < new_width; ++j) {
            for (int c = 0; c < img.channels; ++c) {
                // Compute the original floating-point coordinates
                float original_x = j * scale_x;
                float original_y = i * scale_y;

                // Use bilinear_interpolate to compute the pixel value
                float pixel_value = bilinear_interpolate(img, c, original_y, original_x);

                // Set the interpolated pixel value in the new image
                set_pixel(resized_img, c, i, j, pixel_value);
            }
        }
    }

    return resized_img;
}

