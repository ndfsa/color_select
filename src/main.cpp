#include <Magick++.h>

#include <iostream>
#include <random>
#include <string>

std::string getColor(double red, double green, double blue);

int main(int argc, char **argv)
{
    Magick::InitializeMagick("");

    Magick::Image image;
    double red = 0;
    double green = 0;
    double blue = 0;
    try
    {
        image.read(argv[1]);
        int imgWidth = image.columns();
        int imgHeight = image.rows();
        int channels = image.channels();

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(1, 10);

        const MagickCore::Quantum *pixel_cache = image.getConstPixels(0, 0, imgWidth, imgHeight);

        for (int offset = dist(mt); offset < imgHeight * imgWidth * channels; offset += channels * dist(mt))
        {
            red += (double)pixel_cache[offset] / (imgHeight * imgWidth) * 5;
            green += (double)pixel_cache[offset + 1] / (imgHeight * imgWidth) * 5;
            blue += (double)pixel_cache[offset + 2] / (imgHeight * imgWidth) * 5;
        }
        std::cout << "R: " << red << " G: " << green << " B: " << blue << std::endl;
    }
    catch (Magick::Exception &err)
    {
        std::cout << "Caught exception: " << err.what() << std::endl;
        return 1;
    }
    std::cout << getColor(red, green, blue) << std::endl;
    return 0;
}

std::string getColor(double red, double green, double blue)
{
    bool r = (red > 7000);
    bool g = (green > 7000);
    bool b = (blue > 7000);
    if (r && g && b)
        return "WHITE";
    if (r && g && !b)
        return "YELLOW";
    if (r && !g && b)
        return "MAGENTA";
    if (r && !g && !b)
        return "RED";
    if (!r && g && b)
        return "CYAN";
    if (!r && g && !b)
        return "GREEN";
    if (!r && !g && b)
        return "BLUE";
    if (!r && !g && !b)
        return "BLACK";
    return "ERROR";
}
