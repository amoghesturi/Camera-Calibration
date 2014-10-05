#include "Calibration_Server.h"

void SortCentersTopToBottom (std::vector<cv::Point> &Center1,
							 std::vector<std::vector<cv::Point> > &EndPoints1)
{
	cv::Mat c1, ys = cv::Mat_<int>(Center1.size(),3);
	
	for(int i = 0; i < Center1.size(); i++)
	{
		ys.at<int>(i,0) = Center1[i].x;
		ys.at<int>(i,1) = Center1[i].y;
		ys.at<int>(i,2) = i;
	}

	for(int i = 1; i < ys.rows; i++)
	{
		for(int j = i; j > 0; j--)
		{
            if( ys.at<int>(j,1) < ys.at<int>(j-1,1) )
			{
                int temp1 = ys.at<int>(j,0);
				int temp2 = ys.at<int>(j,1);
				int temp3 = ys.at<int>(j,2);
				ys.at<int>(j,0) = ys.at<int>(j-1,0);
                ys.at<int>(j,1) = ys.at<int>(j-1,1);
				ys.at<int>(j,2) = ys.at<int>(j-1,2);
				ys.at<int>(j-1,0) = temp1;
				ys.at<int>(j-1,1) = temp2;
				ys.at<int>(j-1,2) = temp3;
			}
			else break;
		}
	}
	std::vector<std::vector<cv::Point> > EP_sorted;
	for(int i = 0; i < EndPoints1.size(); i++)
	{
		int j = ys.at<int>(i,2);
		EP_sorted.push_back(EndPoints1[j]);
	}

	//The center at the top of the image is always at the top of the image
	Center1.clear();
	for(int i =0; i < ys.rows; i++)
	{
		cv::Point p(ys.at<int>(i,0), ys.at<int>(i,1));
		Center1.push_back(p);
		//cv::circle(image3, cv::Point(ys.at<int>(i,0), ys.at<int>(i,1)), 3, cv::Scalar(100,255,10), 5);
		/*cv::namedWindow("Sorted", 0);
		cv::imshow("Sorted", image3);
		cv::waitKey();*/
	}
	EndPoints1 = EP_sorted;
}