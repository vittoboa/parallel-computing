#include <vector>
#include <complex>
#include <fstream>


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


void save_ppm_image(Image& image, std::string filename) {
    std::ofstream ofs(filename, std::ofstream::out);

    // write file header
    ofs << "P3" << std::endl;
    ofs << image.width << " " << image.height << std::endl;
    ofs << 255 << std::endl;

    // write all pixels colors
    for (int j = 0; j < image.height; j++) {
        for (int i = 0; i < image.width; i++) {
            ofs << " " << image(0, j, i) << " " << image(1, j, i) << " " << image(2, j, i) << std::endl;
        }
    }

    ofs.close();
}


void parse_args(int argc, char **argv, int& num_threads, std::string& work_allocation) {
    // parse arguments
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--num-threads") {
            // set the number of threads if specified
            num_threads = std::stoi(argv[++i]);
        } else if (std::string(argv[i]) == "--work-allocation") {
            // set the type of work allocation if specified
            work_allocation = argv[++i];
        }
    }
}
