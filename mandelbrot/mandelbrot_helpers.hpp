#include <vector>


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
