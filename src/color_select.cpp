#include <cmath>
#include <ostream>
#include <random>
#include <string>

#include <opencv2/opencv.hpp>

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
void count(std::vector<double> &color_counter, Color color, double saturation, double value);
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
    const int sample = floor((double)hsv_image.cols * 1.3521e-6) + 1;

    // random number generator
    static std::minstd_rand eng{std::random_device{}()};
    static std::uniform_int_distribution<int> dist{1, sample};
    //std::cout << "sample: " << sample << std::endl;

    // count vector
    std::vector<double> counter(9, 0);

    //pointer to HSV data
    cv::Vec3b *ptr = hsv_image.ptr<cv::Vec3b>(0);
    for (int offset = dist(eng); offset < hsv_image.cols; offset += dist(eng))
    {
        count(counter, classify(ptr[offset][0]), (double)ptr[offset][1], (double)ptr[offset][2]);
        //changePixel(ptr[offset], last);
    }

    //auto it = *max_element(std::begin(counter), std::end(counter));
    //std::cout << it << std::endl;
    std::cout << getMaxColor(counter) << std::endl;


    //cv::cvtColor(hsv_image.reshape(0, image.rows), image, cv::COLOR_HSV2BGR_FULL);
    //std::string windowName = "test result"; // Name of the window

    //cv::namedWindow(windowName); // Create a window

    //cv::imshow(windowName, image); // Show our image inside the created window.

    //cv::waitKey(0); // Wait for any keystroke in the window

    //cv::destroyWindow(windowName); // destroy the created window

    return 0;
}

void count(std::vector<double> &color_counter, Color color, double saturation, double value)
{
    int s = (value > 45 ? (saturation > -0.3333 * value + 113 ? color : (value > 164 ? 7 : 0)) : 0);
    color_counter[s] += saturation * value * 1.53e-5;
    //return s;
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
        //std::cout << "Color: " << i << " = " << color_counter[i] << std::endl;
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
