#include <vector>


struct Mat {
    int height;
    int width;
    std::vector<double> data;

    Mat(int height, int width):
        height(height),
        width(width),
        data(height*width)
    {}

    // get position in data vector
    double& operator()(int j, int i) {
        return data[(j * width) + i];
    }
};
