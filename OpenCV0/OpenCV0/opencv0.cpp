#include <opencv2/opencv.hpp>

#include <iostream>
#include <array>

void saveFramestdout(cv::Mat& frame,int compression);

#if defined(_MSC_VER) || defined(WIN32)  || defined(_WIN32) || defined(__WIN32__) \
    || defined(WIN64)    || defined(_WIN64) || defined(__WIN64__)
# include <io.h>
# include <fcntl.h>
# define SET_BINARY_MODE(handle) setmode(handle, O_BINARY)
#else
# define SET_BINARY_MODE(handle) ((void)0)
#endif

int main(int, char**)
{
	const int FrameForAvgCount = 10;
	cv::VideoCapture vc = cv::VideoCapture();
	std::cout << vc.open("D:\\Develop\\OpenCV0\\Debug\\Cam1_Outdoor.avi") << std::endl;
	if (!vc.isOpened())
    {
        std::cout << "Stream cannot be opened" << std::endl;
        return -1;
    }

	int ex = static_cast<int>(vc.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form
	cv::VideoWriter outputVideo;                                        // Open the output

	cv::Size S = cv::Size((int) vc.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
                (int) vc.get(CV_CAP_PROP_FRAME_HEIGHT));
	
	cv::Mat avg;
	
	cv::Mat frame;
	while (!vc.read(frame));
	avg = frame.clone();
	avg = avg.mul(0.0f);

	avg = avg.mul(0.0);

	cv::Mat mask;
	while (true)
	{
		if(vc.read(frame))
		{
			cv::Mat tmpFrame = frame.clone();

			//int curFrame = nextFrameForAvg;

			cv::addWeighted(avg, 0.6, frame, 0.4, 0.0, avg);

			//cv::addWeighted(avg, 1.0, framesForAvg[curFrame], -1.0 / FrameForAvgCount, 0.0, avg);
			//framesForAvg[curFrame] = frame.clone();
			//cv::addWeighted(avg, 1.0, framesForAvg[curFrame], 1.0 / FrameForAvgCount, 0.0, avg);

			//nextFrameForAvg = (nextFrameForAvg + 1) % FrameForAvgCount;

			cv::addWeighted(frame, 1.0f, avg, -1.0f, 0.0f, mask);
			mask = cv::abs(mask);
			
			cv::cvtColor(mask, mask, CV_RGB2GRAY);
			cv::threshold(mask, mask, 10.0f, 255.0f, cv::THRESH_BINARY);

			int dilation_size = 16;

			cv::Mat element = cv::getStructuringElement( cv::MORPH_ELLIPSE,
                                       cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       cv::Point( dilation_size, dilation_size ) );

			cv::dilate(mask, mask, element);

			for (int x = 0; x < mask.size[0]; ++ x)
				for (int y = 0; y < mask.size[1]; ++y)
				{
					if (mask.at<uchar>(cv::Point(y, x)) > 254)
					{
						cv::Rect rect;
						cv::floodFill(mask, cv::Point(y,x), 255.0f, &rect, 10.0f, 10.0f);
						if (rect.width * rect.height > 40 * 40)
							cv::rectangle(mask, rect, 127.0f, CV_FILLED);
						else
							cv::floodFill(mask, cv::Point(y, x), 0.0f, 0, 10.0f, 10.0f);
					}
				}

			cv::cvtColor(mask, mask, CV_GRAY2RGB);

			cv::addWeighted(mask, 0.5f, frame, 1.0f, 0.0f, frame);
			
			//avg = avg.mul(0.0);
			imshow("cam",frame);

			saveFramestdout(frame, 95);
			//imshow("avg", avg);
			//imshow("mask", mask);
		}
		else
			break;
		cv::waitKey(33);
	}

	return 0;
}

void saveFramestdout(cv::Mat& frame,int compression)
{
    SET_BINARY_MODE(fileno(stdout));
    cv::Mat towrite;
    if(frame.type()==CV_8UC1)
    {
        cvtColor(frame,towrite,CV_GRAY2BGR);
    }else if(frame.type()==CV_32FC3)
    {
        double minVal, maxVal;
        minMaxLoc(frame, &minVal, &maxVal);
        frame.convertTo(towrite, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
    }
    else{
        towrite=frame;
    }
    std::vector<uchar> buffer;
    std::vector<int> param(2);
    param[0]=CV_IMWRITE_JPEG_QUALITY;
    param[1]=compression;//default(95) 0-100
    imencode(".jpg",towrite,buffer,param);
    uchar* a = &buffer[0];
    ::write(fileno(stdout),a,buffer.size());
}