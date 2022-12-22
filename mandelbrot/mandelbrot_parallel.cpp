#include "mandelbrot_helpers.hpp"


int main(int argc, char **argv)
{
    // define the image to plot the mandelbrot set
    int channels = 3; // red, green, blue
    int width = 800, height = 600;  // define size of output image
    Image image(channels, height, width);

    return 0;
}
