#include "Calibration_Server.h"

void mouseHandler_zoom(int event, int x, int y, int flags, void* param)
{
	//int x2, y2;
	switch(event)
	{
    case CV_EVENT_LBUTTONDOWN:
		std::cout << std::endl << "x = " << x;
		std::cout << std::endl << "y = " << y << std:: endl;
		cv::Mat zoom_point = (cv::Mat_<int>(2,1) << x, y);
		cv::FileStorage FS("zoom_point.xml", cv::FileStorage::WRITE);
		FS << "zoom_point" << zoom_point;
		FS.release();
        break;
   }
}