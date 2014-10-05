#include "Calibration_Server.h"

void changeMatches(std::vector<cv::KeyPoint> &key_cen1, 
				   std::vector<cv::KeyPoint> &key_cen2,
				   std::vector<cv::DMatch> &matches_centers,
				   cv::Mat img1,
				   cv::Mat img2)
{
	//cv::namedWindow("All Matches", 0);
	cv::namedWindow("Corrected Matches", 0);
	std::vector<cv::KeyPoint> kc1, kc2;
	std::vector<cv::DMatch> mat_cen;
	cv::Mat imageMatches;

	for(int i = 0; i < matches_centers.size(); i++)
	{
		kc1.clear(); 
		kc2.clear();
		//mat_cen.clear();
		for(int j = 0; j <= i; j++)
		{
			kc1.push_back(key_cen1[j]);
			kc2.push_back(key_cen2[j]);
		}
		mat_cen.push_back(cv::DMatch(i,i,0,0.0));
		cv::drawMatches(img1, key_cen1, img2, key_cen2, mat_cen, imageMatches, cv::Scalar(0,0,255));
		cv::imshow("Corrected Matches", imageMatches);
		cv::waitKey();
		char response;
		std::cout << std::endl << "Is the match right? " << std::endl;
		std::cin >> response;
		if( (response == 'y') || (response == 'Y') )
			keepCurrentMatch(img1, key_cen1, img2, key_cen2, mat_cen, imageMatches);
			
		else if( (response == 'N') || (response == 'n') )
		{
			//tryNextMatch(img1, kc1, img2, kc2, mat_cen, imageMatches;
			for(int j = 0; j < matches_centers.size(); j++)
			{
				if(j == i)
					continue;
				else
				{
					mat_cen[i] = cv::DMatch(i,j,0,0.0);
					cv::Mat imageMatches1 = imageMatches.clone();
					cv::drawMatches(img1, key_cen1, img2, key_cen2, mat_cen, imageMatches1, cv::Scalar(0,0,255));
					cv::imshow("Corrected Matches", imageMatches1);
					cv::waitKey();
					char response1;
					std::cout << std::endl << "Is the match right? " << std::endl;
					std::cin >> response1;
					if( (response1 == 'y') || (response1 == 'Y') )
					{
						kc2[i] = key_cen2[j];
						keepCurrentMatch(img1, key_cen1, img2, key_cen2, mat_cen, imageMatches);
						break;
					}
					else if( (response1 == 'N') || (response1 == 'n') )
						continue;
					else
					{
						std::cout << std::endl << "ERROR !! Enter either y or n";
						cv::waitKey();
					}

				}
			}
		}
		else
		{
			std::cout << std::endl << "ERROR !! Enter either y or n";
			cv::waitKey();
		}

	}

	cv::destroyAllWindows();
	key_cen1 = kc1;
	key_cen2 = kc2;
	matches_centers = mat_cen;
	cv::namedWindow("Real Matches", 0);
	cv::Mat img_mat;
	cv::drawMatches(img1, key_cen1, img2, key_cen2, matches_centers, img_mat);
	cv::imshow("Real Matches", img_mat);
	cv::waitKey();
	
}
						

	