#include <vector>
#include <complex>


struct Image {
    int channels;
    int height;
    int width;
    std::vector<int> data;

    Image(int channels, int height, int width):
        channels(channels),
        height(height),
        width(width),
        data(channels*height*width)
    {}

    // get position in data vector
    int& operator()(int channel, int j, int i) {
        return data[(((channel * height) + j) * width) + i];
    }
};


int mandelbrot_pixel_color(std::complex<double> c, int color)
{
    std::complex<double> z(0, 0);

    int max_iterations = 2048;
    for (int i = 0; i < max_iterations; i++) {
        if (abs(z) > 4) {
            color = 255;
            break;
        }

        z = z * z + c;
    }

    return color;
}
