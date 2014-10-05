#include "Calibration_Server.h"

int MarkerDetection(std::string s_int,std::string s_img, std::string s_res)
{
	cv::Mat CameraMatrix1, DistCoeffs1, CameraMatrix2, DistCoeffs2;
	cv::FileStorage fs2(s_int, cv::FileStorage::READ);
	fs2["CameraMatrix"] >> CameraMatrix1;
	fs2["DistCoeffs"] >> DistCoeffs1;
	fs2.release();

	cv::Mat Ima1 = cv::imread(s_img);
	//cv::Mat Ima1;
	//CaptureAFrame(Ima1);

	//cv::namedWindow("Image", 0);
	//cv::imshow("Image", Ima1);
	//cv::waitKey();

	cv::Mat Ima;
	cv::undistort(Ima1, Ima, CameraMatrix1, DistCoeffs1);
	Ima1 = Ima.clone();
	//cv::Mat Ima = Ima1.clone();
#ifdef DEBUG
	cv::namedWindow("Undistorted", 0);
	cv::imshow("Undistorted", Ima1);
	cv::waitKey();
#endif
	std::vector<cv::Point> Center1;
	std::vector<std::vector<cv::Point> > EndPoints1;
	// Center1 has 8 points and EndPoints1 has 32 points
	FindCentersAndCorners1(Ima1, CameraMatrix1, DistCoeffs1, Center1, EndPoints1);
	cv::FileStorage fs(s_res, cv::FileStorage::WRITE);
	fs << "Center1" << Center1;
	fs << "EndPoints1" << EndPoints1;
	fs.release();

	return 1;
}