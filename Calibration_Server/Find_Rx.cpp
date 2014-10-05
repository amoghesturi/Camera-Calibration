#include "Calibration_Server.h"

float Find_Rx(std::vector<cv::Point> Center1, 
			 std::vector<cv::Point> Center2,
			 cv::Mat CameraMatrix1,
			 cv::Mat CameraMatrix2,
			 cv::Mat P1, 
			 cv::Mat P2,
			 float scale,
			 cv::Mat &Rx,
			 int flag)
{
	cv::Mat K1 = CameraMatrix1;
	cv::Mat K2 = CameraMatrix2;
	cv::Mat Kinv1, Kinv2;
	cv::invert(K1, Kinv1);
	cv::invert(K2, Kinv2);
	/*int mouseParam= CV_EVENT_FLAG_LBUTTON;
	cv::namedWindow("Right Image",CV_WINDOW_NORMAL);
    cv::imshow("Right Image",image3);
	cvSetMouseCallback("Right Image",mouseHandler,&mouseParam);*/

	std::vector<cv::Point2f> pnorm1, pnorm2, points1_cen, points2_cen;
	for(int i = 0; i < Center1.size(); i++)
	{
		float x2 = Center2[i].x;
		float y2 = Center2[i].y;
		float x1 = Center1[i].x;
		float y1 = Center1[i].y;
		points1_cen.push_back(cv::Point2f(x1,y1));
		points2_cen.push_back(cv::Point2f(x2,y2));
		cv::Mat X = Kinv1 * (cv::Mat_<double>(3,1) << x1, y1, 1) ;
		cv::Mat Xd = Kinv2 * (cv::Mat_<double>(3,1) << x2, y2, 1);
		cv::Point2f pt;
		pt.x = X.at<double>(0,0);
		pt.y = X.at<double>(1,0);
		pnorm1.push_back(pt);
		pt.x = Xd.at<double>(0,0);
		pt.y = Xd.at<double>(1,0);
		pnorm2.push_back(pt);
	}

	int ss;
	if (flag == 0)
		 ss = points1_cen.size()-1;
	else if(flag == 1)
		 ss = 6;
				
	cv::Mat pts = ( cv::Mat_<float>(6,3) << (float)points1_cen[ss].x, (float)points1_cen[ss].y, 1,
											(float)points1_cen[ss-1].x, (float)points1_cen[ss-1].y, 1,
											(float)points1_cen[ss-2].x, (float)points1_cen[ss-2].y, 1, 
											(float)points2_cen[ss].x, (float)points2_cen[ss].y, 1,
											(float)points2_cen[ss-1].x, (float)points2_cen[ss-1].y, 1,
											(float)points2_cen[ss-2].x, (float)points2_cen[ss-2].y, 1);
	std::vector<cv::Point2f> v_pts1, v_pts2;
	v_pts1.clear(); v_pts2.clear();
	
	std::vector<cv::Point> A,B;
	for(int i = 0; i < 3; i++)
	{
		float x1 = pts.at<float>(i,0);
		float y1 = pts.at<float>(i,1);
		float x2 = pts.at<float>(i+3,0);
		float y2 = pts.at<float>(i+3,1);
		A.push_back(cv::Point(x1,y1));
		B.push_back(cv::Point(x2,y2));
		cv::Mat X = Kinv1 * (cv::Mat_<double>(3,1) << x1, y1, 1) ;
		cv::Mat Xd = Kinv2 * (cv::Mat_<double>(3,1) << x2, y2, 1);
		cv::Point2f pt;
		pt.x = X.at<double>(0,0);
		pt.y = X.at<double>(1,0);
		v_pts1.push_back(pt);
		pt.x = Xd.at<double>(0,0);
		pt.y = Xd.at<double>(1,0);
		v_pts2.push_back(pt);
	}
	cv::Mat im1 = cv::imread("Data/Pics/b1.jpg");
	cv::Mat im2 = cv::imread("Data/Pics/a1.jpg");
	DisplayMarkerNumbers(im1, A);
	DisplayMarkerNumbers(im2, B);

	cv::Mat V_PTS1;

	cv::triangulatePoints(P1, P2, v_pts1, v_pts2, V_PTS1);
	for(int i = 0; i < V_PTS1.cols; i++)
		V_PTS1.col(i) = V_PTS1.col(i)/V_PTS1.at<float>(3,i);
	std::cout << std::endl << "V_PTS1 = " << std::endl << V_PTS1 << std::endl;
	V_PTS1 = V_PTS1 * scale;
	std::cout << std::endl << "3D Points after Triangulation are: " << std::endl << V_PTS1 << std::endl;
	for(int i = 0; i < V_PTS1.rows; i++)
		for(int j = 0; j < V_PTS1.cols; j++)
			V_PTS1.at<float>(i,j) = cvRound(V_PTS1.at<float>(i,j));
	std::cout << std::endl << "3D Points after Triangulation are: " << std::endl << V_PTS1 << std::endl;

	cv::Mat n1, n2;
	n2 = ( cv::Mat_<float>(1,3) << 0, 1, 0 );
	cv::Mat line2 = (cv::Mat_<float>(1,3) << (V_PTS1.at<float>(0,2) - V_PTS1.at<float>(0,0)),
											 (V_PTS1.at<float>(1,2) - V_PTS1.at<float>(1,0)),
											 (V_PTS1.at<float>(2,2) - V_PTS1.at<float>(2,0)) );
	std::cout << std::endl << "X3 - X1 " << std::endl << line2 << std::endl;
	cv::Mat line1 = (cv::Mat_<float>(1,3) << (V_PTS1.at<float>(0,1) - V_PTS1.at<float>(0,0)),
											 (V_PTS1.at<float>(1,1) - V_PTS1.at<float>(1,0)),
											 (V_PTS1.at<float>(2,1) - V_PTS1.at<float>(2,0)) );
	std::cout << std::endl << "X2 - X1 " << std::endl << line1 << std::endl;
	//Finding Norm
	n1 = (line1.cross(line2));
	float n1_norm = sqrt(pow(n1.at<float>(0,0),2) + pow(n1.at<float>(0,1),2) + pow(n1.at<float>(0,2),2));
	n1 = n1/n1_norm;
	//float l2_norm = sqrt(pow(line2.at<float>(0,0),2) + pow(line2.at<float>(0,1),2) + pow(line2.at<float>(0,2),2));
	std::cout << std::endl << "n1 = " << n1 << std::endl;
	std::cout << std::endl << "n2 = " << n2 << std::endl;
	// Angle between two planes
	n1_norm = sqrt(pow(n1.at<float>(0,0),2) + pow(n1.at<float>(0,1),2) + pow(n1.at<float>(0,2),2));
	float n2_norm = sqrt(pow(n2.at<float>(0,0),2) + pow(n2.at<float>(0,1),2) + pow(n2.at<float>(0,2),2));
	float cos_theta_x = n1.dot(n2) / (n1_norm * n2_norm);
	float theta_x = std::acos(cos_theta_x);
	if(theta_x > (float)(22/14))
	{
		cos_theta_x = - cos_theta_x;
		theta_x = std::acos(cos_theta_x);
	}
	// Convert from [0 Pi] to [-pi/2 to pi/2]
	//theta_x = -((22/7) - theta_x);
	Rx = ( cv::Mat_<float>(3,3) << 1,         0,                    0, 
	                               0,         std::cos(-theta_x),  -std::sin(-theta_x),
								   0,         std::sin(-theta_x),  std::cos(-theta_x) );
	std::cout << std::endl << "Rx = " << std::endl << Rx << std::endl;

	return theta_x;
}