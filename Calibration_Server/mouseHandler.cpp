#include "Calibration_Server.h"

void mouseHandler(int event, int x, int y, int flags, void* param)
{
	//int x2, y2;
	switch(event)
	{
    case CV_EVENT_LBUTTONDOWN:
		std::cout << std::endl << "x = " << x;
		std::cout << std::endl << "y = " << y << std:: endl;
		cv::Mat newMarker = (cv::Mat_<int>(2,1) << x, y);
		cv::FileStorage FS1("newMarker.xml", cv::FileStorage::WRITE);
		FS1 << "newMarker" << newMarker;
		FS1.release();
        break;
   }
}