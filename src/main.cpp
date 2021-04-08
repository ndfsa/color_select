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
int count(double *color_counter, Color color, double saturation, double value);
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
        int imgWidth = image.columns();
        int imgHeight = image.rows();
        int channels = image.channels();
        image.colorSpace(MagickCore::HSVColorspace);

        //MagickCore::Quantum *pixel_cache = image.getPixels(0, 0, imgWidth, imgHeight);
        const MagickCore::Quantum *pixel_cache = image.getConstPixels(0, 0, imgWidth, imgHeight);
        double max_side = fmax(imgHeight, imgWidth);
        if (max_side >= 860)
        {
            const int sample = floor(0.0047 * max_side - 1);
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist(1, sample);
            //std::cout << "avg sample: " << sample << std::endl;

            for (int offset = dist(mt) * 3; offset < imgHeight * imgWidth * 3; offset += 3 * dist(mt))
            {
                int last = count(color_counter, getColor(pixel_cache[offset]), pixel_cache[offset + 1],
                                 pixel_cache[offset + 2]);
                //changePixel(pixel_cache, offset, last);
            }
        }
        else
        {
            for (int i = 0; i < imgHeight * imgWidth * 3; i += 3)
            {
                int last = count(color_counter, getColor(pixel_cache[i]), pixel_cache[i + 1], pixel_cache[i + 2]);
                //changePixel(pixel_cache, i, last);
            }
        }
        //image.write("/home/ndfsa/Pictures/test/test.png");
        std::cout << getMaxColor(color_counter) << std::endl;
    }
    catch (Magick::Exception &err)
    {
        std::cout << "Caught exception: " << err.what() << std::endl;
        return 1;
    }
    return 0;
}

int count(double *color_counter, Color color, double saturation, double value)
{
    int s = (value > 9830 ? (saturation > -0.3333 * value + 29053 ? color : (value > 42597 ? 7 : 0)) : 0);
    ++color_counter[s];
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
        //std::cout << "Color: " << i << " = " << color_counter[i] << std::endl;
    }
    return maxi;
}

Color getColor(double hue)
{
    if (hue <= 4551)
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
    else if (hue <= 61530)
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
