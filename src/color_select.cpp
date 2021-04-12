//#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <thread>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

enum Color
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7,
    ERROR = 8
};

Color classify(int hue);
void count(std::vector<double> &color_counter, int start, int size, double thread_number, int sample, cv::Vec3b *ptr);
int getMaxColor(std::vector<double> &color_counter);
void changePixel(cv::Vec3b &pixel, int last);

int main(int argc, char **argv)
{
    // check if path was passed
    if (argc < 2)
    {
        std::cout << "usage: color_counter <Image_Path> [-t <Thread number>]" << std::endl;
        return 1;
    }
    // auto start_time = std::chrono::high_resolution_clock::now();
    // read the image into a Mat
    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);

    // if image is empty, then it could not be read
    if (image.empty())
    {
        std::cout << "Could not open image" << std::endl;
        return 1;
    }

    // change color space RGB->HSV and shape to a vector
    cv::Mat hsv_image;
    cv::cvtColor(image.reshape(0, 1), hsv_image, cv::COLOR_BGR2HSV_FULL);

    // number of threads
    const int thread_number = (argc == 4 && std::string(argv[2]) == "-t") ? std::stoi(argv[3]) : 2;

    // create vector counters
    std::vector<std::vector<double>> counters(thread_number);
    for (std::vector<double> &counter : counters)
    {
        counter = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    }

    // pointer to HSV data
    cv::Vec3b *ptr = hsv_image.ptr<cv::Vec3b>(0);
    // std::cout << "pixels: " << hsv_image.cols << std::endl;

    // create threads
    std::vector<std::thread> threads(thread_number);
    for (int i = 0; i < thread_number; i++)
    {
        int start = floor((double)hsv_image.cols * i / thread_number);
        int size = floor((double)hsv_image.cols * (i + 1.0) / thread_number);
        threads[i] = std::thread(count, std::ref(counters[i]), start, size, thread_number,
                                 floor((size - start) * 2.1633e-5 * thread_number) + 1, ptr);
    }

    // join threads after finishing processes
    for (std::thread &t : threads)
    {
        t.join();
    }

    // sum counters
    for (int i = 1; i < thread_number; i++)
    {
        std::transform(counters[0].begin(), counters[0].end(), counters[i].begin(), counters[0].begin(),
                       std::plus<double>());
    }

    // get max color
    std::cout << getMaxColor(counters[0]) << std::endl;

    // std::cout << "time reading: "
    //          << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() -
    //                                                                   start_time)
    //                 .count()
    //          << std::endl;

    cv::cvtColor(hsv_image.reshape(0, image.rows), image, cv::COLOR_HSV2BGR_FULL);
    std::string windowName = "test result"; // Name of the window
    cv::namedWindow(windowName);            // Create a window
    cv::imshow(windowName, image);          // Show our image inside the created window.
    while ((cv::waitKey(0) & 0xFF) != 27)   // Wait for any keystroke in the window
    {

        continue;
    }
    cv::destroyWindow(windowName); // destroy the created window
    return 0;
}

void count(std::vector<double> &color_counter, int start, int size, double thread_number, int sample, cv::Vec3b *ptr)
{
    if (sample > 4)
    {
        static std::minstd_rand eng{std::random_device{}()};
        static std::uniform_int_distribution<int> dist{1, sample};
        // std::cout << "sample: " << sample << std::endl;

        for (int offset = start + dist(eng); offset < size; offset += dist(eng))
        {
            int s = (ptr[offset][2] > 45
                         ? (ptr[offset][1] > -0.3333 * ptr[offset][2] + 105 ? classify(ptr[offset][0])
                                                                            : (ptr[offset][2] > 160 ? 7 : 0))
                         : 0);
            color_counter[s] += ptr[offset][1] * ptr[offset][2] * 1.5379e-5;
            changePixel(ptr[offset], s);
        }
    }
    else
    {
        for (int offset = start; offset < size; offset++)
        {
            int s = (ptr[offset][2] > 45
                         ? (ptr[offset][1] > -0.3333 * ptr[offset][2] + 113 ? classify(ptr[offset][0])
                                                                            : (ptr[offset][2] > 164 ? 7 : 0))
                         : 0);
            color_counter[s] += ptr[offset][1] * ptr[offset][2] * 1.53e-5;
            changePixel(ptr[offset], s);
        }
    }
}

int getMaxColor(std::vector<double> &color_counter)
{
    double max = -1;
    double maxi = -1;
    for (int i = 1; i < 7; i++)
    {
        if (color_counter[i] > max)
        {
            max = color_counter[i];
            maxi = i;
        }
        // std::cout << "Color: " << i << " = " << color_counter[i] << std::endl;
    }
    return maxi;
}

Color classify(int hue)
{
    if (hue <= 15)
    {
        return Color::RED;
    }
    else if (hue <= 51)
    {
        return Color::YELLOW;
    }
    else if (hue <= 97)
    {
        return Color::GREEN;
    }
    else if (hue <= 143)
    {
        return Color::CYAN;
    }
    else if (hue <= 182)
    {
        return Color::BLUE;
    }
    else if (hue <= 230)
    {
        return Color::MAGENTA;
    }
    else if (hue <= 255)
    {
        return Color::RED;
    }
    else
    {
        return Color::ERROR;
    }
}

void changePixel(cv::Vec3b &pixel, int last)
{
    switch (last)
    {
    case Color::BLACK:
        pixel = cv::Vec3b(0, 255, 0);
        break;
    case Color::BLUE:
        pixel = cv::Vec3b(170, 255, 255);
        break;
    case Color::CYAN:
        pixel = cv::Vec3b(128, 255, 255);
        break;
    case Color::GREEN:
        pixel = cv::Vec3b(85, 255, 255);
        break;
    case Color::MAGENTA:
        pixel = cv::Vec3b(213, 255, 255);
        break;
    case Color::RED:
        pixel = cv::Vec3b(0, 255, 255);
        break;
    case Color::YELLOW:
        pixel = cv::Vec3b(43, 255, 255);
        break;
    case Color::WHITE:
        pixel = cv::Vec3b(0, 0, 255);
        break;
    case Color::ERROR:
        pixel = cv::Vec3b(255, 255, 255);
        break;
    }
}
