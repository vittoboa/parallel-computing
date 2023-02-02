#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>


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


void process_args(int argc, char **argv, int& cols, int& rows, int& max_iterations, double& epsilon) {
    int mandatory_count = 0;

    // process arguments
    for (int i = 0; i < argc; ++i) {
        if (std::string(argv[i]).compare("--cols") == 0) {
            cols = atoi(argv[++i]);
            mandatory_count++;
        }
        if (std::string(argv[i]).compare("--rows") == 0) {
            rows = atoi(argv[++i]);
            mandatory_count++;
        }
        if (std::string(argv[i]).compare("--max-iterations") == 0) {
            max_iterations = atoi(argv[++i]);
        }
        if (std::string(argv[i]).compare("--epsilon") == 0) {
            epsilon = atof(argv[++i]);
        }
    }

    // error if not all mandatory arguments have been set
    if (mandatory_count < 2) {
        std::cout << "Usage: --cols <int> --rows <int> [--max-iterations <int> --epsilon <double>]" << std::endl;
        exit(-1);
    }
}


void save_matrix(Mat& U, std::string filename) {
    std::ofstream ofs(filename, std::ofstream::out);
    ofs << U.width << "\n" << U.height << "\n";

    for (int i = 0; i < U.height; ++i) {
        for (int j = 0; j < U.width; ++j) {
            ofs << std::fixed << std::setfill(' ') << std::right << std::setw(11) << std::setprecision(6) <<  U(i,j) << " ";
        }
    }

    ofs.close();
}
