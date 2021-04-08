#include <Magick++.h>

#include <bits/stdc++.h>
#include <cmath>
#include <iostream>
#include <ostream>
#include <random>
#include <string>

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

Color getColor(double hue);
int count(double *color_counter, Color color, double saturation, double value, double alpha);
int getMaxColor(double *color_counter);
void changePixel(Magick::Quantum *pixel_cache, int offset, int last);

int main(int argc, char **argv)
{
    Magick::InitializeMagick("");

    double color_counter[8];
    memset(color_counter, 0, sizeof(color_counter));

    Magick::Image image;
    try
    {
        image.read(argv[1]);

        const int imgWidth = image.columns();
        const int imgHeight = image.rows();
        const bool has_alpha = image.alpha();
        const int channels = (has_alpha ? 4 : 3);

        image.colorSpace(MagickCore::HSVColorspace);

        // MagickCore::Quantum *pixel_cache = image.getPixels(0, 0, imgWidth, imgHeight);
        const MagickCore::Quantum *pixel_cache = image.getConstPixels(0, 0, imgWidth, imgHeight);
        double max_side = fmax(imgHeight, imgWidth);
        if (max_side >= 860)
        {
            const int sample = floor(0.0047 * max_side - 1);
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist(1, sample);
            // std::cout << "max sample: " << sample << std::endl;

            for (int offset = dist(mt) * (has_alpha ? 4 : 3); offset < imgHeight * imgWidth * channels;
                 offset += channels * dist(mt))
            {
                count(color_counter, getColor(pixel_cache[offset]), pixel_cache[offset + 1], pixel_cache[offset + 2],
                      has_alpha ? pixel_cache[offset + 3] * 1.53e-5 : 1);
                // changePixel(pixel_cache, offset, last);
            }
        }
        else
        {
            for (int i = 0; i < imgHeight * imgWidth * channels; i += channels)
            {
                count(color_counter, getColor(pixel_cache[i]), pixel_cache[i + 1], pixel_cache[i + 2],
                      has_alpha ? pixel_cache[i + 3] * 1.53e-5 : 1);
                // changePixel(pixel_cache, i, last);
            }
        }
        // image.write("/home/ndfsa/Pictures/test/test.png");
        std::cout << getMaxColor(color_counter) << std::endl;
    }
    catch (Magick::Exception &err)
    {
        std::cout << "Caught exception: " << err.what() << std::endl;
        return 1;
    }
    return 0;
}

int count(double *color_counter, Color color, double saturation, double value, double alpha)
{
    int s = (value > 11796 ? (saturation > -0.3333 * value + 29053 ? color : (value > 42597 ? 7 : 0)) : 0);
    color_counter[s] += alpha * saturation * value * 2.33e-10;
    return s;
}

int getMaxColor(double *color_counter)
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

Color getColor(double hue)
{
    if (hue <= 4004)
    {
        return Color::RED;
    }
    else if (hue <= 13107)
    {
        return Color::YELLOW;
    }
    else if (hue <= 25121)
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
    else if (hue <= 62622)
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
void changePixel(Magick::Quantum *pixel_cache, int offset, int last)
{

    switch (last)
    {
    case 0:
        pixel_cache[offset] = 0;
        pixel_cache[offset + 1] = 0;
        pixel_cache[offset + 2] = 0;
        break;
    case 1:
        pixel_cache[offset] = 43690;
        pixel_cache[offset + 1] = 65535;
        pixel_cache[offset + 2] = 65535;
        break;
    case 2:
        pixel_cache[offset] = 21845;
        pixel_cache[offset + 1] = 65535;
        pixel_cache[offset + 2] = 65535;
        break;
    case 3:
        pixel_cache[offset] = 32767;
        pixel_cache[offset + 1] = 65535;
        pixel_cache[offset + 2] = 65535;
        break;
    case 4:
        pixel_cache[offset] = 0;
        pixel_cache[offset + 1] = 65535;
        pixel_cache[offset + 2] = 65535;
        break;
    case 5:
        pixel_cache[offset] = 54612;
        pixel_cache[offset + 1] = 65535;
        pixel_cache[offset + 2] = 65535;
        break;
    case 6:
        pixel_cache[offset] = 10922;
        pixel_cache[offset + 1] = 65535;
        pixel_cache[offset + 2] = 65535;
        break;
    case 7:
        pixel_cache[offset] = 0;
        pixel_cache[offset + 1] = 0;
        pixel_cache[offset + 2] = 65535;
        break;
    case 8:
        break;
    }
}
