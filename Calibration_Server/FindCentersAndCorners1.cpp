#include "Calibration_Server.h"

void FindCentersAndCorners1(cv::Mat Ima1, 
							cv::Mat CameraMatrix, 
							cv::Mat DistCoeffs, 
							std::vector<cv::Point> &Center1, 
							std::vector<std::vector<cv::Point> > &EndPoints1, 
							int iindex)
{
	cv::Mat image = Ima1;
    cv::Mat imgBW1 = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
	cv::cvtColor(image, imgBW1, CV_BGR2GRAY);


	cv::adaptiveThreshold(imgBW1, imgBW1, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 35, 0);
	/*cv::namedWindow("image", 0);
	cv::imshow("image", imgBW1);
	cv::waitKey();*/
	std::vector<std::vector<cv::Point> > contours1;
	cv::Mat image1 = image.clone();
	
	// Find the Contours
	cv::findContours(imgBW1, contours1, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//cv::drawContours(image1, contours1, -1, cv::Scalar(255, 0, 0), 2, 8);
	//cv::namedWindow("contours", 0);
	//cv::imshow("contours", image1);
	//cv::waitKey();
	
	cv::Mat image2 = image.clone();
	std::vector<std::vector<cv::Point> > quads1;
	std::vector<std::vector<cv::Point> >::const_iterator itc = contours1.begin();
	std::vector<cv::Point> poly1;
	
	// Look for quarangles
	while(itc != contours1.end() -1)
	{		
		cv::approxPolyDP(*itc, poly1, 10, true);
		if(poly1.size() == 4)
			quads1.push_back(poly1);	
		itc++;
	}
	//cv::drawContours(image2, quads1, -1, cv::Scalar(255, 0, 0), 2, 8);
	//cv::namedWindow("contours1", 0);
	//cv::imshow("contours1", image2);
	//cv::waitKey();	

	//Find the centers of the quadrangles
	std::vector<std::pair<cv::Point, int> > centers1;
	std::vector<std::vector<cv::Point> >::const_iterator jq = quads1.begin();
	int count = 0;
	while(jq != quads1.end())
	{
		std::vector<cv::Point> temp = *jq;
		/*std::vector<cv::Point>::const_iterator k = temp.begin();*/
		double sumx = 0; double sumy = 0;
		for (int k = 0; k < temp.size(); k++)
		{
			sumx += temp[k].x;
			sumy += temp[k].y;
		}
		sumx = sumx/temp.size();
		sumy = sumy/temp.size();
		std::pair<cv::Point, int> p1(cv::Point(sumx, sumy), count);
		centers1.push_back(p1);
		cv::circle(image2, p1.first, 2, cv::Scalar(0,0,255), 3);
		count++;
		jq++;
	}
	//cv::namedWindow("Centers", 0);
	//cv::imshow("Centers", image2);
	//cv::waitKey();


	//Find Common Center using search
	std::vector<std::vector<int> > triplet1;
	
	for(int i = 0; i < centers1.size(); i++)
	{
		std::vector<int> temp;
		/*int first = 1;*/
		int cen1x = centers1[i].first.x;
		int cen1y = centers1[i].first.y;
		for(int j = i+1; j < centers1.size(); j++)
		{
			int cen2x = centers1[j].first.x;
			int cen2y = centers1[j].first.y;
			if( (cen1x <= cen2x+2) && (cen1y <= cen2y+2) && (cen1x >= cen2x-2) && (cen1y >= cen2y-2) )
			{
				temp.push_back(i);
				temp.push_back(j);
			}
		}
		if (temp.data() )
		{
			if( (triplet1.size() != 0) )
			{
				std::vector<int> tr1 = triplet1[triplet1.size() -1];
				std::vector<int> tr2 = temp;;
				cen1x = centers1[tr1[tr1.size() -1]].first.x;
				cen1y = centers1[tr1[tr1.size() -1]].first.y;
				int cen2x = centers1[tr2[tr2.size() -1]].first.x;
				int cen2y = centers1[tr2[tr2.size() -1]].first.y;
				if(! ((cen1x <= cen2x+2) && (cen1y <= cen2y+2) && (cen1x >= cen2x+2) && (cen1y >= cen2y+2)) )
					if( tr1[tr1.size() -1] != tr2[tr2.size() -1] )
					{
						triplet1.push_back(temp);
						temp.clear();
					}
			}
			else
			{
				triplet1.push_back(temp);
				temp.clear();
			}
			
		}
	}
				
	/*std::vector<cv::Point> Center1;*/
	std::vector<std::vector<int>> triplet1_t;
	for(int j = 0; j < triplet1.size(); j++)
	{
		std::vector<int> triplet1_temp = triplet1[j];
		double sumx = 0;
		double sumy = 0;
		for(int i = 0; i < triplet1_temp.size(); i++)
		{
			sumx += centers1[triplet1_temp[i]].first.x;
			sumy += centers1[triplet1_temp[i]].first.y;
		}
		sumx = (sumx/triplet1_temp.size());
		sumy = (sumy/triplet1_temp.size());
		bool repeat = 0;
		if( Center1.data())
		{
			for(int k = 0; k < Center1.size(); k++)
			{
				double s1 = Center1[k].x;
				double s2 = Center1[k].y;
				if( (sumx > s1-5) && (sumx < s1 + 5) && (sumy > s2 - 5) && (sumy < s2 + 5) )
				{
					repeat = 1;
					break;
				}
			}
			if (repeat == 1)
				continue;
		}
		triplet1_t.push_back(triplet1_temp);
		cv::Point center1(sumx, sumy);
		Center1.push_back(center1);
		cv::circle(image2, center1, 3, cv::Scalar(255, 255, 255), 5);
		//cv::imshow("Centers", image2);
		//cv::waitKey();
	}
	triplet1 = triplet1_t;
	
	// Find the corners of the markers
	std::vector<std::vector<cv::Point> > end_points1;
	cv::Mat x_ = cv::Mat_<float>(4,3), y_ = cv::Mat_<float>(4,3);
	std::vector<cv::Point> temp_cen, quadrangles1;

	for(int j = 0; j < triplet1.size(); j++)
	{
		std::vector<int> triplet1_temp = triplet1[j];
		std::vector<cv::Point> quad_temp = quads1[triplet1_temp[0]], q_t;		
		/*for(int i = 0; i < quad_temp.size(); i++) 
		{
			x_.at<float>(i,0) = quad_temp[i].x;
			x_.at<float>(i,1) = quad_temp[i].y;
			y_.at<float>(i,0) = quad_temp[i].x;
			y_.at<float>(i,1) = quad_temp[i].y;
			x_.at<float>(i,2) = y_.at<float>(i,2) = i;
		}*/
	
	for(int i = 0; i < 4; i++)
		{
			cv::Point pt(quad_temp[i].x, quad_temp[i].y);
			quadrangles1.push_back(pt);
		}
		end_points1.push_back(quad_temp);		
	}
	cv::namedWindow("Centers",0);
	for(int i = 0; i < quadrangles1.size() ; i++)
	{
		cv::circle(image2, quadrangles1[i], 2, cv::Scalar(0,255,0), 5);
		cv::imshow("Centers", image2);
		//cv::waitKey();
	}

	EndPoints1 = end_points1;
	cv::waitKey();
	cv::Mat image3 = image2.clone();
	cv::destroyAllWindows();
}