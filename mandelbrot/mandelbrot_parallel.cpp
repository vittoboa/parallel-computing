#include <thread>
#include <atomic>

#include "mandelbrot_helpers.hpp"


void worker_static(Image &image, int thread_id, int num_threads)
{
    // find the first and last row of the part of matrix which was statically assigned to the thread
    int start = (image.height *  thread_id      / num_threads);
    int end   = (image.height * (thread_id + 1) / num_threads);

    // process the part of matrix assigned to the thread
    for (int j = start; j < end; j++) {
        for (int i = 0; i < image.width; i++) {
            // find single number which represent two dimensional matrix position
            double dx = ((double) i / image.width - 0.7) * 2;
            double dy = ((double) j / image.height - 0.5) * 2;
            std::complex<double> c = std::complex<double>(dx, dy);

            // set gray-scale color based on thread-id
            int color = (255 * thread_id) / num_threads;

            // apply mandelbrot algorithm to find color which corresponds to the current pixel
            int pixel_color = mandelbrot_pixel_color(c, color);

            // apply to the image
            for (int ch = 0; ch < image.channels; ch++) {
                image(ch, j, i) = pixel_color;
            }
        }
    }
}


void worker_dynamic(Image &image, std::atomic<int>& counter, int thread_id, int num_threads)
{

    // process all rows assigned to the thread dynamically
    for (;;) {

        // find row to process
        int j = counter++;

        // return if at the end of matrix and there are no more rows to process
        if (j >= image.height) {
            return;
        }

        // process entire row
        for (int i = 0; i < image.width; i++) {
            // find single number which represent two dimensional matrix position
            double dx = ((double) i / image.width - 0.7) * 2;
            double dy = ((double) j / image.height - 0.5) * 2;
            std::complex<double> c = std::complex<double>(dx, dy);

            // set gray-scale color based on thread-id
            int color = (255 * thread_id) / num_threads;

            // apply mandelbrot algorithm to find color which corresponds to the current pixel
            int pixel_color = mandelbrot_pixel_color(c, color);

            // apply to the image
            for (int ch = 0; ch < image.channels; ch++) {
                image(ch, j, i) = pixel_color;
            }
        }
    }
}


int main(int argc, char **argv)
{
    // init variables with default values
    int num_threads = std::thread::hardware_concurrency();
    std::string work_allocation = "static";
    // possibly update default values with the user provided values
    parse_args(argc, argv, num_threads, work_allocation);

    // define the image to plot the mandelbrot set
    int channels = 3; // red, green, blue
    int width = 800, height = 600;  // define size of output image
    Image image(channels, height, width);

    // define counter used to dynamically assign rows to threads
    std::atomic<int> counter(0);

    // spawn threads and pass parameters
	std::vector<std::thread> threads;  // vector to hold the thread objects
    if (work_allocation == "static") {
        for (int t = 0; t < num_threads; t++) {
            threads.push_back(std::thread(worker_static, std::ref(image), t, num_threads));
        }
    } else if (work_allocation == "dynamic") {
        for (int t = 0; t < num_threads; t++) {
            threads.push_back(std::thread(worker_dynamic, std::ref(image), std::ref(counter), t, num_threads));
        }
    }

    // join threads
    for (auto& t : threads) {
        t.join();
    }

    // save image
    save_ppm_image(image, "mandelbrot.ppm");

    return 0;
}
