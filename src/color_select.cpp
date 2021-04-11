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
void count(std::vector<double> &color_counter, int start, int size, cv::Vec3b *ptr);
int getMaxColor(std::vector<double> &color_counter);
void changePixel(cv::Vec3b &pixel, int last);

int main(int argc, char **argv)
{
    // check if path was passed
    if (argc != 2)
    {
        std::cout << "usage: color_counter <Image_Path>" << std::endl;
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

    // sample size
    // std::cout << "sample: " << sample << std::endl;

    // count vector
    std::vector<double> counter1(9, 0);
    std::vector<double> counter2(9, 0);

    // pointer to HSV data
    cv::Vec3b *ptr = hsv_image.ptr<cv::Vec3b>(0);

    std::thread t1(count, std::ref(counter1), 0, floor((double)hsv_image.cols / 2), ptr);
    std::thread t2(count, std::ref(counter2), floor((double)hsv_image.cols / 2), hsv_image.cols, ptr);

    t1.join();
    t2.join();

    std::transform(counter1.begin(), counter1.end(), counter2.begin(), counter1.begin(), std::plus<double>());
    std::cout << getMaxColor(counter1) << std::endl;

    return 0;
}

void count(std::vector<double> &color_counter, int start, int size, cv::Vec3b *ptr)
{
    int sample;
    if (size - start > 739600)
    {
        sample = floor((double)(size - start) * 2.7042e-6) + 1;
        static std::minstd_rand eng{std::random_device{}()};
        static std::uniform_int_distribution<int> dist{1, sample};
        for (int offset = start + dist(eng); offset < size; offset += dist(eng))
        {
            int s = (ptr[offset][2] > 45
                         ? (ptr[offset][1] > -0.3333 * ptr[offset][2] + 113 ? classify(ptr[offset][0])
                                                                            : (ptr[offset][2] > 160 ? 7 : 0))
                         : 0);
            color_counter[s] += ptr[offset][1] * ptr[offset][2] * 1.5379e-5;
            // changePixel(ptr[offset], s);
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
            // changePixel(ptr[offset], s);
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
    else if (hue <= 243)
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
