#include "Calibration_Server.h"

float Find_Ry (cv::Mat Rx3D, cv::Mat &Ry)
{
	float y_actual = y_actual_c - y_actual_m;
	cv::Mat X_actual = ( cv::Mat_<float>(3,1) << x_actual, y_actual, z_actual );
	std::cout << std::endl << "Rx3D = " << Rx3D << std::endl;
	float a = Rx3D.at<float>(0,0);
	float b = Rx3D.at<float>(1,0);
	float c = Rx3D.at<float>(2,0);
	float theta_y, sin_theta_y;
	sin_theta_y = ((c * X_actual.at<float>(0,0)) - (a * X_actual.at<float>(2,0))) / (pow(a,2) + pow(c,2));
	
	theta_y = std::asin(sin_theta_y);
	Ry = ( cv::Mat_<float>(3,3) << std::cos(theta_y),         0,                  std::sin(theta_y), 
		                                   0,                         1,                  0, 
										  -std::sin(theta_y),         0,                  std::cos(theta_y) );
	std::cout << std::endl << "Ry = " << std::endl << Ry << std::endl;
	
	return theta_y;
}