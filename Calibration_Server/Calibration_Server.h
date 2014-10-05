#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>
#include <string>

#define DEBUG 1;
#define VERIFICATION_Int 1;
#define UNDISTORT 1
#define hor_box 10       // Number of Horizontal boxes in the chess board
#define ver_box 7        // Number of Vertical boxes in the chess board
#define size_box 2.05    // length of each box in centi meter 

#define DEBUG_sen 1
#define SAME_SENSOR 1

//#define NODATA 1
//#define VERIFICATION_Ser 1
#define HUMAN_INTERVENTION 1

// Measurements in centimeter = 2.54 * inches
#define x_actual   2.54 * 130              // perpendicular distance from Camera to the top marker in x- direction
#define y_actual_c 2.54 * (79 + 3.75)      //height of the camera
#define y_actual_m 2.54 * (55.5 + 3.75)    // height of the center of the top marker
#define z_actual   2.54 * 96    // perpendicular distance from Camera to the top marker in z- direction

int Calibration_Intrinsic(char image_id);
void IntCali(std::vector<cv::Mat> Images, 
			 cv::Mat &cameraMatrix, 
			 cv::Mat &distCoeffs, 
			 char image_id);


int MarkerDetection(std::string s_int,
					std::string s_img,
					std::string s_res);
int CaptureAFrame(cv::Mat &image);
void FindCentersAndCorners1(cv::Mat Ima1, 
							cv::Mat CameraMatrix, 
							cv::Mat DistCoeffs, 
							std::vector<cv::Point> &Center1, 
							std::vector<std::vector<cv::Point> > &EndPoints1, 
							int iindex = 0);


void VerifyCentersAndCorners(std::vector<cv::Point> &Center1,
							 std::vector<std::vector<cv::Point> > &EndPoints1,
							 cv::Mat Ima);
void mouseHandler(int event, 
				  int x, 
				  int y, 
				  int flags, 
				  void* param);
void mouseHandler_zoom(int event, 
				  int x, 
				  int y, 
				  int flags, 
				  void* param);
void DisplayMarkerNumbers(cv::Mat Ima,
						  std::vector<cv::Point> Center);
void DisplayMarkerNumbers(cv::Mat Ima,
						  std::vector<std::vector<cv::Point> > EndPoints);
void DisplayMarkerNumbers(cv::Mat Ima,
						  std::vector<cv::Point> Center1,
						  cv::Mat Imb,
						  std::vector<cv::Point> Center2,
						  std::string str1 = "",
						  std::string str2 = "");
void DisplayMarkerNumbers(cv::Mat Ima,
						  std::vector<std::vector<cv::Point> > EndPoints1,
						  cv::Mat Imb,
						  std::vector<std::vector<cv::Point> > EndPoints2,
						  std::string str1 = "",
						  std:: string str2 = "");
void DisplayMarkerNumbers_Imc(cv::Mat Ima,
						  std::vector<cv::Point> Center1,
						  cv::Mat Imb,
						  std::vector<cv::Point> Center2,
						  std::string str1 = "",
						  std:: string str2 = "");
void AcceptNewMarkers(cv::Mat Ima, 
					  std::vector<cv::Point> &Center1, 
					  std::vector<std::vector<cv::Point> > &EndPoints1,
					  int num_u);
void RemoveSomeMarkers(cv::Mat Ima, 
					   std::vector<cv::Point> &Center1,
					   std::vector<std::vector<cv::Point> > &EndPoints1,
					   int unwanted);
//void FindCentersAndCorners1(cv::Mat Ima1, 
//							cv::Mat CameraMatrix, 
//							cv::Mat DistCoeffs, 
//							std::vector<cv::Point> &Center1, 
//							std::vector<std::vector<cv::Point> > &EndPoints1, 
//							int iindex = 0);
void MatchCorners (std::vector<cv::Point> &Center1, 
				   std::vector<cv::Point> &Center2,
				   std::vector<std::vector<cv::Point> > EndPoints1,
				   std::vector<std::vector<cv::Point> > EndPoints2,
				   std::vector<cv::KeyPoint> &keypoints_center1,
				   std::vector<cv::KeyPoint> &keypoints_center2, 
				   std::vector<cv::KeyPoint> &keypoints1,
				   std::vector<cv::KeyPoint> &keypoints2, 
				   std::vector<cv::DMatch> &matches_centers,
				   std::vector<cv::DMatch> &matches,
				   cv::Mat Ima1,
				   cv::Mat Imb1,
				   int iindex = 0);
void SortCentersTopToBottom (std::vector<cv::Point> &Center1, 
							 std::vector<std::vector<cv::Point> > &EndPoints1);
void SortEndPoints(std::vector<std::vector<cv::Point> > &EndPoints1,
				   int iindex = 0);
void changeMatches(std::vector<cv::KeyPoint> &key_cen1, 
				   std::vector<cv::KeyPoint> &key_cen2,
				   std::vector<cv::DMatch> &matches_centers,
				   cv::Mat img1,
				   cv::Mat img2);
void keepCurrentMatch(cv::Mat img1, 
					  std::vector<cv::KeyPoint> &key_cen1,
					  cv::Mat img2,
					  std::vector<cv::KeyPoint>& key_cen2, 
					  std::vector<cv::DMatch> &mat_cen, 
					  cv::Mat &imageMatches);
void EditEndPoints(std::vector<std::vector<cv::Point> > &EndPoints1_, 
				   std::vector<std::vector<cv::Point> > &EndPoints2_, 
				   std::vector<cv::DMatch> matches, 
				   cv::Mat Ima1, 
				   cv::Mat Imb1);
void changeDirection(std::vector<cv::Point>  &EndPoints1,
					 std::vector<cv::Point>  &EndPoints2,
					 cv::Mat Ima1,
					 cv::Mat Imb1);
cv::Mat CombineImages(cv::Mat Ima, cv::Mat Imb);
cv::Mat Find_Fundamental_Matrix(std::vector<cv::Point2f> points1, 
							 std::vector<cv::Point2f> points2,
							 std::vector<cv::DMatch> matches,
							 std::vector<cv::KeyPoint> &keypoints1,
							 std::vector<cv::KeyPoint> &keypoints2);
void DrawEpiLines(std::vector<cv::Point2f> points1, 
				  std::vector<cv::Point2f> points2, 
				  cv::Mat fundemental, 
				  std::vector<cv::Vec3f> &lines1,
				  std::vector<cv::Vec3f> &lines2,
				  cv::Mat image3,
				  cv::Mat image4);
double Calculate_Error(std::vector<cv::Point2f> points1, 
				  std::vector<cv::Point2f> points2,
				  cv::Mat fundemental,
				  std::vector<cv::Vec3f> lines1,
				  std::vector<cv::Vec3f> lines2);
int Essential_Matrix(cv::Mat CameraMatrix1,
					 cv::Mat CameraMatrix2,
					 cv::Mat fundemental,
					 std::vector<cv::Point2f> points1, 
					 std::vector<cv::Point2f> points2,
					 std::vector<cv::Point> Center1, 
					 std::vector<cv::Point> Center2,
					 cv::Mat &Essential,
					 cv::Mat &P1, 
					 cv::Mat &P2,
					 float &scale,
					 cv::Mat &points3D);
float Find_Rx(std::vector<cv::Point> Center1, 
			 std::vector<cv::Point> Center2,
			 cv::Mat CameraMatrix1,
			 cv::Mat CameraMatrix2,
			 cv::Mat P1, 
			 cv::Mat P2,
			 float scale,
			 cv::Mat &Rx,
			 int flag);
float Find_Ry (cv::Mat Rx3D, cv::Mat &Ry);
void FindCenterUsingP(cv::Mat Projection, cv::Mat &Center, cv::Mat Rotation, float Scale);