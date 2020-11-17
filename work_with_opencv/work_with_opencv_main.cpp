#include <opencv2/opencv.hpp>
#include <Halide.h>

/*
 Data stored in Halide is organized as planar mode, while data in opencv Mat is organized in interleaved mode.
 Planar mode is stored as RRRGGGBBBs, while interleaved mode is stored as RGBRGBRGB.
 We have to use the make_interleaved function to read interleaved data into planar mode.
 Code: https://github.com/halide/Halide/blob/025f054e7ad5693be14bbabc41b7816b5c122bb3/src/runtime/HalideBuffer.h#L1723
 */

int main(int argc, char **argv) 
{
	std::string image_path = "/Users/vincent/Documents/Repo/Halide_Image_Process/images/rgb.png";
	cv::Mat image_bgr = cv::imread(image_path, 1);
	cv::Mat image_gray(image_bgr.size(), CV_8UC1);


	// Wrong: Halide::Buffer<uint8_t> buffer_bgr = Halide::Buffer(image_bgr.data, image_bgr.cols, image_bgr.rows, image_bgr.channels());
	Halide::Buffer<uint8_t> buffer_bgr = Halide::Buffer<uint8_t>::make_interleaved(image_bgr.data, image_bgr.cols, image_bgr.rows, image_bgr.channels());
    Halide::Buffer<uint8_t> buffer_gray(image_gray.data, image_gray.cols, image_gray.rows);

	Halide::Func GrayScale;
    Halide::Var x, y;

    Halide::Expr gray =
        0.114f * buffer_bgr(x, y, 0) +     //B
        0.587f * buffer_bgr(x, y, 1) +     //G
        0.299f * buffer_bgr(x, y, 2);      //R

    GrayScale(x, y) = Halide::cast<uint8_t>(Halide::min(gray, 255));
    GrayScale.realize(buffer_gray);

    cv::Mat result;
	cv::imshow("image_bgr", image_bgr);
	cv::imshow("image_gray", image_gray);
    cv::waitKey(0);	
}
