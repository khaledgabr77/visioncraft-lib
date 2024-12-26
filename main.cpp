#include <iostream>
#include "image.hpp"

extern image load_image(char *filename);
extern void save_image(image im, const char *name);
extern void free_image(image im);

int main() {
    try {
        // Test creating an image
        int width = 4, height = 4, channels = 3;
        image img = create_image(width, height, channels);
        // img.data[0] = 0.0f;
        std::cout << "Created an image with dimensions: " 
                  << img.width << "x" << img.height 
                  << " and channels: " << img.channels 
                  << " image data: " << img.data[10] << std::endl;

        // // Test setting and getting pixel values
        set_pixel(img, 0, 0, 0, 0.5f);
        float pixel_value = get_pixel(img, 0, 0, 0);
        std::cout << "Pixel value at (0, 0, 0): " << pixel_value << std::endl;

        // // Test loading an image
        char image_path[] = "../boy.jpg"; // Replace with an actual image path
        image loaded_image = load_image(image_path);
        std::cout << "Loaded an image with dimensions: " 
                  << loaded_image.width << "x" 
                  << loaded_image.height 
                  << " and channels: " << loaded_image.channels << std::endl;

        // // Test saving an image
        const char *output_path = "output_image";
        save_image(loaded_image, output_path);
        std::cout << "Saved the image as " << output_path << ".jpg" << std::endl;

        // // Test freeing images
        free_image(img);
        free_image(loaded_image);
        std::cout << "Freed allocated images." << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }

    return 0;
}

