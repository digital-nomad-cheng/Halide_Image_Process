#include "color_balance_opencv.hpp"

int main()
{
	std::string image_path = "/Users/vincent/Documents/Repo/Halide_Image_Process/color_balance/resources/color_balance_medium.jpg";

	cv::Mat img = cv::imread(image_path, 1);
    cv::Mat result;
	cv::imshow("img", img);
    cv::waitKey(0);	
    
    color_balance_opencv(img, result);
    
    cv::imshow("balance_color", result);
    cv::waitKey(0);
}