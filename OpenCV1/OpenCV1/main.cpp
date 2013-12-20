#include <opencv2/opencv.hpp>

#include <iostream>
#include <array>

//int main(int argc, char* argv[])
//{
//	cv::Mat image;
//    image = cv::imread("1.jpg", CV_LOAD_IMAGE_COLOR); 
//	if(! image.data )                              // Check for invalid input
//	{
//		std::cout <<  "Could not open or find the image" << std::endl ;
//		return -1;
//	}
//
//	cv::cvtColor(image, image, CV_RGB2GRAY); 
//
//	cv::Mat source = image;
//
//	cv::RNG rng;
//
//	for (int x = 0; x < image.size[0]; ++ x)
//		for (int y = 0; y < image.size[1]; ++y)
//		{
//			int noise = (int)rng.gaussian(10);
//			int cur   = (int)image.at<uchar>(cv::Point(y, x)) + noise;
//			if (cur > 255 || cur < 0) continue;
//			image.at<uchar>(cv::Point(y, x)) = cur;
//		}
//
//	cv::Mat withNoise = image;
//
//	cv::Mat output;
//	// свертка с гауссианом
//	cv::GaussianBlur(withNoise, output, cv::Size(9, 9), 0.0);
//	// суммирование и масштабирование по области
//	//cv::blur(image, output, cv::Size(9, 9));
//	// просто замен€ет пиксель на среднее в области 9 на 9
//	//cv::medianBlur(withNoise, output, 9);
//	//сглаживаютс€ пиксели более менее похожие друг на друга и не сглаживаютс€ непохожие, за счет этого сохран€ютс€ кра€
//	//cv::bilateralFilter(image, output, 7, 150, 150);
//
//	//cv::fastNlMeansDenoising(withNoise, output);
//
//	cv::Mat diff;
//	cv::addWeighted(image, 1.0f, output, -1.0f, 0.0f, diff);
//	diff = cv::abs(diff);
//
//	cv::imshow("source", source);
//	cv::imshow("blured", output);
//	cv::imshow("diff", diff);
//
//	/*cvShowManyImages("gauss", 9, &source, &output, &diff, &source, &output, &diff, &source, &output, &diff);*/
//	//diffAndOutput(source, output);
//
//	cv::waitKey(0);                                         
//    
//	return 0;
//}