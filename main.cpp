#include <iostream>
#include "image.hpp"

extern image load_image(char *filename);
extern void save_image(image im, const char *name);
extern void free_image(image im);
extern void save_png(image im, const char *name);
extern void save_image_binary(image im, const char *fname);
extern image load_image_binary(const char *fname);

int main() {

    try {
        // Test creating an image
        // int width = 4, height = 4, channels = 3;
        // image img = create_image(channels, height, width);

        // std::cout << "Created an image with dimensions: " 
        //           << img.width << "x" << img.height 
        //           << " and channels: " << img.channels 
        //           << " image data: " << img.data[10] << std::endl;

        // // // Test setting and getting pixel values
        // set_pixel(img, 0, 0, 0, 0.5f);
        // float pixel_value = get_pixel(img, 0, 0, 0);
        // std::cout << "Pixel value at (0, 0, 0): " << pixel_value << std::endl;

        // // Test loading an image
        char image_path[] = "dog.jpg"; 
        image gray_image = load_image(image_path);
        
         
        // // Test saving an image
        // image graybar = rgb_to_grayscale(gray_image);
        // save_image(graybar, "gray_dog");

        // shift_image(gray_image, 0, 0.4f);
        // shift_image(gray_image, 1, 0.4f);
        // shift_image(gray_image, 2, 0.4f);
        
        // clamp_image(gray_image);
        // rgb_to_hsv(gray_image);
        // scale_image(gray_image, 1, 2);
        // clamp_image(gray_image);
        // hsv_to_rgb(gray_image);
        // save_image(gray_image, "shifted_dog");

        // Create a sample image (e.g., 3x3 grayscale with 1 channel)
        image img = create_image(1, 3, 3);
        for (int i = 0; i < 9; i++) {
            img.data[i] = i + 1; // Fill with values from 1 to 9
        }

        // Resize the image to 6x6
        image resized = nn_resize(img, 6, 6);

        // Print the resized image
        for (int i = 0; i < resized.height; i++) {
            for (int j = 0; j < resized.width; j++) {
                std::cout << get_pixel(resized, 0, i, j) << " ";
            }
            std::cout << "\n";
        }


        // // Test freeing images
        free_image(gray_image);
        std::cout << "Freed allocated images." << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }



    return 0;
}

