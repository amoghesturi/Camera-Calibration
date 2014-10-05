#include "Calibration_Server.h"

void DrawEpiLines(std::vector<cv::Point2f> points1, 
				  std::vector<cv::Point2f> points2, 
				  cv::Mat fundemental, 
				  std::vector<cv::Vec3f> &lines1,
				  std::vector<cv::Vec3f> &lines2,
				  cv::Mat image3,
				  cv::Mat image4)
{
	// draw the left points corresponding epipolar lines in right image
	cv::computeCorrespondEpilines(cv::Mat(points1), 1, fundemental,lines1); 
	// for all epipolar lines
	for (std::vector<cv::Vec3f>::const_iterator it= lines1.begin();it!=lines1.end(); ++it) 
	{
		// draw the line between first and last column
		cv::line(image4, cv::Point(0,-(*it)[2]/(*it)[1]), cv::Point(image4.cols,-((*it)[2]+ (*it)[0]*image4.cols)/(*it)[1]),cv::Scalar(255,255,255));
		//cv::namedWindow("Epipolar Lines2", 0);
		//cv::imshow("Epipolar Lines2", image4);
		//cv::waitKey();	
	}

	// draw the left points corresponding epipolar lines in right image
	cv::computeCorrespondEpilines(cv::Mat(points2), 2, fundemental,lines2); 
	//cv::Mat image3 = Ima.clone();
	// for all epipolar lines
	for (std::vector<cv::Vec3f>::const_iterator it= lines2.begin();it!=lines2.end(); ++it) 
	{
		// draw the line between first and last column
		cv::line(image3, cv::Point(0,-(*it)[2]/(*it)[1]), cv::Point(image3.cols,-((*it)[2]+ (*it)[0]*image3.cols)/(*it)[1]),cv::Scalar(255,255,255));
		//cv::namedWindow("Epipolar Lines1", 0);
		//cv::imshow("Epipolar Lines1", image3);
		//cv::waitKey();
	}

	cv::namedWindow("Epipolar Lines1", 0);
	cv::imshow("Epipolar Lines1", image3);
	cv::namedWindow("Epipolar Lines2", 0);
	cv::imshow("Epipolar Lines2", image4);
	cv::waitKey();
}