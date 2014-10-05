#include "Calibration_Server.h"

cv::Mat Find_Fundamental_Matrix(std::vector<cv::Point2f> points1, 
							 std::vector<cv::Point2f> points2,
							 std::vector<cv::DMatch> matches,
							 std::vector<cv::KeyPoint> &keypoints1,
							 std::vector<cv::KeyPoint> &keypoints2)
{
	cv::Mat T, Td;
	std::vector<cv::Point2f> points1_norm = points1;
	std::vector<cv::Point2f> points2_norm = points2;
	std::vector<uchar> inliers(points1.size(),0);
	std::vector<cv::DMatch> outMatches;
	cv::Mat fundemental= cv::findFundamentalMat(cv::Mat(points1_norm), cv::Mat(points2_norm), inliers, CV_FM_RANSAC);//, 0.0000001, 2.22);
	// extract the surviving (inliers) matches
	std::vector<uchar>::const_iterator itIn= inliers.begin();
	std::vector<cv::DMatch>::const_iterator itM= matches.begin();
	// for all matches
	for ( ;itIn!= inliers.end(); ++itIn, ++itM)
	{
		if (*itIn)
		{
			outMatches.push_back(*itM);
		}
	}
	std::cout << std::endl << "Fundamental Matrix = " << fundemental << std::endl;
	std::cout << std::endl << "Determinant of Fundamental Matrix = " << cv::determinant(fundemental) << std::endl;
	points1_norm.clear();
	points2_norm.clear();
	for(std::vector<cv::DMatch>::const_iterator it = outMatches.begin(); it != outMatches.end(); it++)
	{
		float x = keypoints1[it ->queryIdx].pt.x;
		float y = keypoints1[it ->queryIdx].pt.y;
		points1_norm.push_back(cv::Point2f(x, y));
		x = keypoints2[it ->trainIdx].pt.x;
		y = keypoints2[it ->trainIdx].pt.y;
		points2_norm.push_back(cv::Point2f(x, y));
	}

	fundemental= cv::findFundamentalMat(cv::Mat(points1_norm), cv::Mat(points2_norm), inliers, CV_FM_RANSAC);//, 0.0000001, 2.22);
	std::cout << std::endl << "Fundamental Matrix = " << fundemental << std::endl;
	std::cout << std::endl << "Determinant of Fundamental Matrix = " << cv::determinant(fundemental) << std::endl;
	cv::Mat U, s, Vt, W, S, Ft, V;
	cv::SVDecomp(fundemental, s, U, Vt);
	S = ( cv::Mat_<double>(3,3) << s.at<double>(0,0), 0, 0, 0, s.at<double>(1,0), 0, 0, 0, s.at<double>(2,0) );
	S.at<double>(2,2) = 0;
	fundemental = U * S * Vt;
	std::cout << std::endl << "Fundamental Matrix = " << fundemental << std::endl;
	std::cout << std::endl << "Determinant of Fundamental Matrix = " << cv::determinant(fundemental) << std::endl;
	
	return fundemental;
}