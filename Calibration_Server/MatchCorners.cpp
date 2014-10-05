#include "Calibration_Server.h"

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
				   int iindex)
{
	//Sort the Centers
	//Centers of Image1
	std::vector<cv::Point> temp_cen, quadrangles1;
	
	SortCentersTopToBottom(Center1, EndPoints1);
	SortCentersTopToBottom(Center2, EndPoints2);
	
	SortEndPoints(EndPoints1, 0);
	SortEndPoints(EndPoints2, iindex);

#ifdef HUMAN_INTERVENTION
	
	std::vector<cv::KeyPoint> key_cen1, key_cen2;
	std::vector<cv::Point2f> epc1, epc2;
	for(int i = 0; i < Center1.size(); i++)
	{
		float x1 = (float)Center1[i].x;
		float y1 = (float)Center1[i].y;
		float x2 = (float)Center2[i].x;
		float y2 = (float)Center2[i].y;
		cv::Point2f p1(x1,y1);
		cv::Point2f p2(x2,y2);
		epc1.push_back(p1);
		epc2.push_back(p2);
	}
	cv::KeyPoint::convert(epc1, key_cen1);
	cv::KeyPoint::convert(epc2, key_cen2);
	for(int i = 0; i < epc1.size(); i++)
	{
		matches_centers.push_back(cv::DMatch(i,i,0,0.0));
	}
	cv::Mat Ima = Ima1.clone();
	cv::Mat Imb = Imb1.clone();
	cv::Mat imageMatches;
	cv::drawMatches(Ima,key_cen1,Imb,key_cen2, matches_centers, imageMatches,cv::Scalar(0,0,255));
	cv::namedWindow("Temperory Matches",CV_WINDOW_NORMAL);
	cv::imshow("Temperory Matches",imageMatches);
	cv::waitKey();

	std::cout << std::endl << "Check if all the matches are right" << std::endl;
	char response;
	std::cin >> response;
	if( (response == 'N') || (response == 'n') )
	{
		changeMatches(key_cen1, key_cen2, matches_centers, Ima, Imb);
	}
	else if( (response == 'y') || (response == 'Y') )
		;

	keypoints_center1 = key_cen1;
	keypoints_center2 = key_cen2;
	matches = matches_centers;
	EditEndPoints(EndPoints1, EndPoints2, matches, Ima1, Imb1);
#endif
	
	cv::destroyAllWindows;
	//cv::namedWindow("Ordered EndPoints", 0);
	std::vector<cv::Point2f> ep1, ep2;
	for(int i = 0; i < EndPoints1.size(); i ++)
		for( int j = 0; j < EndPoints1[i].size(); j++)
		{
			float e1 = EndPoints1[i][j].x;
			float e2 = EndPoints1[i][j].y;
			cv::Point2f E(e1,e2);
			ep1.push_back(E);
			/*cv::circle(Ima1, cv::Point(EndPoints1[i][j]), 2, cv::Scalar(0,255,0), 3);
			cv::imshow("Ordered EndPoints", Ima1);
			cv::waitKey();*/
		}
	for(int i = 0; i < EndPoints2.size(); i ++)
		for( int j = 0; j < EndPoints2[i].size(); j++)
		{
			float x = EndPoints2[i][j].x;
			float y = EndPoints2[i][j].y;
			cv::Point2f E(x,y);
			ep2.push_back(E);
			/*cv::circle(Imb1, cv::Point(EndPoints2[i][j]), 2, cv::Scalar(0,255,0), 3);
			cv::imshow("Ordered EndPoints", Imb1);
			cv::waitKey();*/
		}

	cv::KeyPoint::convert(ep1, keypoints1);
	cv::KeyPoint::convert(ep2, keypoints2);
	matches.clear();
	for(int i = 0; i < ep1.size(); i++)
	{
		matches.push_back(cv::DMatch(i,i,0,0.0));
	}
				   }