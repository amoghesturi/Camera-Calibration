#include "Calibration_Server.h"

void keepCurrentMatch(cv::Mat img1, 
					  std::vector<cv::KeyPoint> &key_cen1,
					  cv::Mat img2,
					  std::vector<cv::KeyPoint>& key_cen2, 
					  std::vector<cv::DMatch> &mat_cen, 
					  cv::Mat &imageMatches)
{
	cv::drawMatches(img1, key_cen1, img2, key_cen2, mat_cen, imageMatches, cv::Scalar(0,255,0));
	cv::imshow("Corrected Matches", imageMatches);
	cv::waitKey();
}