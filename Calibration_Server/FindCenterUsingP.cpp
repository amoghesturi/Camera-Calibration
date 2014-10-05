#include "Calibration_Server.h"

void FindCenterUsingP(cv::Mat Projection, cv::Mat &Center1, cv::Mat Rotation, float Scale)
{
	cv::Mat Center;
	cv::Mat U, D, Vt, V;
	cv::SVDecomp(Projection, D, U, Vt, 4);
	Center = Vt.row(3);
	cv::transpose(Center, Center);
	Center = Center / Center.at<double>(3,0);
	Center = (Center / Center.at<double>(3,0)) * Scale;
	Center1 = ( cv::Mat_<double>(3,1)  << Center.at<double>(0,0), Center.at<double>(1,0), Center.at<double>(2,0) );
	Rotation.convertTo(Rotation, 6);
	Center1 = (Rotation * Center1)/2.54;
	std::cout << std::endl << "Center of the Second Camera (inches)= " << std::endl << Center1 << std::endl;
}