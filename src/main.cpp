#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <string>

using namespace cv;

std::string getColor(Mat& centers);
std::string getColor(Vec3f& color);

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "usage: color_picker <path_to_image>" << std::endl;
        return 1;
    }
    std::string image_path = samples::findFile(argv[1]);
    Mat input = imread(image_path, IMREAD_COLOR);
    Mat img;
    cv::resize(input, img, cv::Size(), 0.05, 0.05);
    // convert to float & reshape to a [3 x W*H] Mat 
    //  (so every pixel is on a row of it's own)
    Mat data;
    img.convertTo(data,CV_32F);
    data = data.reshape(1,data.total());

    // do kmeans
    Mat labels, centers;
    kmeans(data, 8, labels, TermCriteria( 5, 5, 1.0), 3, 
           KMEANS_PP_CENTERS, centers);

    // reshape both to a single row of Vec3f pixels:
    centers = centers.reshape(3,centers.rows);
    Mat hsv_centers;
    cvtColor(centers, hsv_centers, COLOR_BGR2HSV, 0);
    std::cout << getColor(hsv_centers) << std::endl;
    //getColor(hsv_centers);
    return 0;
}

std::string getColor(Mat& centers){
    float max = -100;
    int index = 0;
    //for(int i = 0; i < centers.rows; ++i)
    //{
    //    float n = centers.at<Vec3f>(i, 0)[2] + centers.at<Vec3f>(i, 0)[1];
    //    if (max < n)
    //    {
    //        max = n;
    //        index = i;
    //    }
    //    //std::cout << centers.at<Vec3f>(i, 0) << std::endl;
    //}
    //std::cout << centers.at<Vec3f>(index, 0) << std::endl;
    return getColor(centers.at<Vec3f>(index, 0));
}

std::string getColor(Vec3f& color){
    if (color[0] < 44 && color[0] >= 0)
    {
        return "RED";
    }
    else if (color[0] < 76)
    {
        return "YELLOW";
    }
    else if (color[0] < 147)
    {
        return "GREEN";
    }
    else if (color[0] < 196)
    {
        return "CYAN";
    }
    else if (color[0] < 249)
    {
        return "BLUE";
    }
    else if (color[0] < 346)
    {
        return "MAGENTA";
    }
    else if (color[0] <= 360)
    {
        return "RED";
    }
    else
    {
        return "ERROR";
    }

}
