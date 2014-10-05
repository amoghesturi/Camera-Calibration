#include "Calibration_Server.h"

void SortEndPoints(std::vector<std::vector<cv::Point> > &EndPoints1,
				   int iindex)
{
	std::vector<cv::Point> temp_cen, quadrangles1;
	std::vector<std::vector<cv::Point> > end_points1;
	cv::Mat x_ = cv::Mat_<float>(4,3), y_ = cv::Mat_<float>(4,3);
	for(int j = 0; j < EndPoints1.size(); j++)
	{
		//std::vector<int> triplet1_temp = triplet1[j];
		std::vector<cv::Point> quad_temp = EndPoints1[j], q_t;		
		for(int i = 0; i < quad_temp.size(); i++) 
		{
			x_.at<float>(i,0) = quad_temp[i].x;
			x_.at<float>(i,1) = quad_temp[i].y;
			y_.at<float>(i,0) = quad_temp[i].x;
			y_.at<float>(i,1) = quad_temp[i].y;
			x_.at<float>(i,2) = y_.at<float>(i,2) = i;
		}
		
		for(int i = 1; i < y_.rows; i++)
		{
			for(int j = i; j > 0; j--)
			{
				if( y_.at<float>(j,1) < y_.at<float>(j-1,1) )
				{
					int temp1 = y_.at<float>(j,0);
					int temp2 = y_.at<float>(j,1);
					int temp3 = y_.at<float>(j,2);
					y_.at<float>(j,0) = y_.at<float>(j-1,0);
					y_.at<float>(j,1) = y_.at<float>(j-1,1);
					y_.at<float>(j,2) = y_.at<float>(j-1,2);
					y_.at<float>(j-1,0) = temp1;
					y_.at<float>(j-1,1) = temp2;
					y_.at<float>(j-1,2) = temp3;
				}
				else break;
			}
		}
		std::cout << std::endl << "y_ sorted = " << std::endl << y_ << std::endl;
		temp_cen = quad_temp;

		int index = (int) y_.at<float>(0,2) + iindex;
		for(int i = 0; i < 4; i++)
		{
			int j = (i + index)%4;
			quad_temp[i] = temp_cen[j];
			cv::Point pt(temp_cen[j].x, temp_cen[j].y);
			quadrangles1.push_back(pt);
		}
		end_points1.push_back(quad_temp);
	}
	EndPoints1.clear();
	EndPoints1 = end_points1;
}