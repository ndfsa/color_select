#include <Magick++.h>

#include <bits/stdc++.h>
#include <cmath>
#include <iostream>
#include <ostream>
#include <random>
#include <string>

enum Color
{
    BLACK,   // 0
    BLUE,    // 1
    GREEN,   // 2
    CYAN,    // 3
    RED,     // 4
    MAGENTA, // 5
    YELLOW,  // 6
    WHITE,   // 7
    ERROR    // 8
};
Color getColor(double hue);
void count(double *color_counter, Color color, double saturation, double value);
int getMaxColor(double *color_counter);

int main(int argc, char **argv)
{
    Magick::InitializeMagick("");

    double color_counter[8];
    memset(color_counter, 0, sizeof(color_counter));

    Magick::Image image;
    try
    {
        image.read("/home/ndfsa/Pictures/test/" + std::string(argv[1]));
        int imgWidth = image.columns();
        int imgHeight = image.rows();
        int channels = image.channels();
        image.colorSpace(MagickCore::HSVColorspace);
        // image.quantizeColors(4);
        // image.quantize();

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(1, 10);

        MagickCore::Quantum *pixel_cache = image.getPixels(0, 0, imgWidth, imgHeight);

        // for (int offset = 0; offset < 15; offset += channels)
        for (int offset = dist(mt); offset < imgHeight * imgWidth * channels; offset += channels * dist(mt))
        {
            // std::cout << pixel_cache[offset] * 255 / 65535 << " " << pixel_cache[offset + 1] * 255 / 65535 << " "
            //          << pixel_cache[offset + 2] * 255 / 65535 << " " << std::endl;
            // std::cout << pixel_cache[offset] * 360 / 65535 << " " << pixel_cache[offset + 1] * 100 / 65535 << " "
            //          << pixel_cache[offset + 2] * 100 / 65535 << " " << std::endl;
            Color aux = getColor(pixel_cache[offset]);
            // std::cout << "color: " << aux << std::endl;
            count(color_counter, aux, pixel_cache[offset + 1], pixel_cache[offset + 2]);
        }
        std::cout << getMaxColor(color_counter) << std::endl;
    }
    catch (Magick::Exception &err)
    {
        std::cout << "Caught exception: " << err.what() << std::endl;
        return 1;
    }
    return 0;
}

void count(double *color_counter, Color color, double saturation, double value)
{
    int s = (value > 1966 ? (saturation > 2621 ? color : 7) : 0);
    color_counter[s] += saturation * value / (65535.0 * 65535.0);
}

int getMaxColor(double *color_counter)
{
    double max = -1;
    double maxi = -1;
    for (int i = 0; i < 8; i++)
    {
        if (color_counter[i] > max)
        {
            max = color_counter[i];
            maxi = i;
        }
        std::cout << color_counter[i] << std::endl;
    }
    return maxi;
}

Color getColor(double hue)
{
    if (hue <= 7463)
    {
        return Color::RED;
    }
    else if (hue <= 13107)
    {
        return Color::YELLOW;
    }
    else if (hue <= 26031)
    {
        return Color::GREEN;
    }
    else if (hue <= 36954)
    {
        return Color::CYAN;
    }
    else if (hue <= 46784)
    {
        return Color::BLUE;
    }
    else if (hue <= 60801)
    {
        return Color::MAGENTA;
    }
    else if (hue <= 65535)
    {
        return Color::RED;
    }
    else
    {
        return Color::ERROR;
    }
}
