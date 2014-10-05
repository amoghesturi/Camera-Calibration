#include "Calibration_Server.h"

int CaptureAFrame(cv::Mat &image)
{
	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
	if(!capture)
	{
		std::cout << std::endl << "ERROR: capture is Null" << std::endl;
		cv::waitKey();
		return -1;
	}
	cv::namedWindow("my window", CV_WINDOW_AUTOSIZE);
	int success = 0;
	for(;success != 1;)
	{
		IplImage* frame = cvQueryFrame(capture);		
		if (!frame)
		{
			std::cout << std::endl << "ERROR: frame is Null" << std::endl;
			cv::waitKey();
			break;
		}
		image = (cv::Mat)frame;
		cv::imshow("my window", (cv::Mat)frame);
	}
	return 1;
}