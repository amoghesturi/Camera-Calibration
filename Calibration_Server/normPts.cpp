#include "calib_new.h"

void normPts(std::vector<cv::Point2f> &Points1, cv::Mat &T)
{
	std::vector<cv::Point2f> pts1 = Points1;
	double sumx = 0, sumy = 0;
	for(int i = 0; i < pts1.size(); i++)
	{
		sumx += pts1[i].x;
		sumy += pts1[i].y;
	}
	sumx = sumx/pts1.size();
	sumy = sumy/pts1.size();

	double average_x = sumx;
	double average_y = sumy;
	double average_dist = 0;

	for(int i = 0; i < pts1.size(); i++)
	{
		double x = pts1[i].x;
		double y = pts1[i].y;
		double distance = sqrt( pow((x - average_x),2) + pow((y - average_y),2) );
		average_dist += distance;
	}
	average_dist = average_dist/pts1.size();
	double scale = sqrt(2.0)/average_dist;

	T = ( cv::Mat_<double>(3,3) << scale, 0, -average_x*scale, 0, scale, -average_y*scale, 0, 0, 1);
	std::cout << std::endl << "T = " << T;

	//Points1.clear();
	for(int i = 0; i < pts1.size(); i++)
	{
		double x = pts1[i].x;
		double y = pts1[i].y;
		cv::Mat X = (cv::Mat_<double>(3,1) << x, y, 1);
		cv::Mat A;
		A = T * X;
		std::cout << A.at<double>(2,0);
		Points1[i].x = A.at<double>(0,0);
		Points1[i].y = A.at<double>(1,0);
	}
}