#include "Calibration_Server.h"

void EditEndPoints(std::vector<std::vector<cv::Point> > &EndPoints1_, 
				   std::vector<std::vector<cv::Point> > &EndPoints2_, 
				   std::vector<cv::DMatch> matches, 
				   cv::Mat Ima1, 
				   cv::Mat Imb1)
{
	std::vector<std::vector<cv::Point> > EndPoints1; 
	std::vector<std::vector<cv::Point> > EndPoints2; 
	for(int i = 0; i < matches.size(); i++)
	{ 
		int a = matches[i].queryIdx;
		int b = matches[i].trainIdx;
		EndPoints1.push_back(EndPoints1_[a]);
		EndPoints2.push_back(EndPoints2_[b]);
	}
	std::vector<cv::Point>  EP1, EP2;
	std::string str1 = "'0' in the left image corresponds to ____ in right image";
	std::string str2 = "Press 'Return' and Enter the value in the output window";

	for(int i = 0; i < EndPoints1.size(); i++)
	{
		/*int j = matches[i].trainIdx;*/
		std::vector<cv::Point2f> ep1, ep2;
		std::vector<cv::KeyPoint> k1, k2;
		DisplayMarkerNumbers_Imc(Ima1, EndPoints1[i], Imb1, EndPoints2[i], str1, str2);

		std::cout << std::endl << "'0' in the left image corresponds to ____ in right image" << std::endl;
		int a;
		std::cin >> a;

		EP2 = EndPoints2[i];
		for(int j = 0; j < 4; j++)
			EndPoints2[i][j] = EP2[(a + j)%4];

R:
		std::cout << std::endl << "Do points 0 to 3 in the left Image match with  points 1 to 4 in the right Image ? (Y/N)" << std::endl;
		char response;
		std::cin >> response;
		if( (response == 'n') || (response == 'N') )
			changeDirection(EndPoints1[i], EndPoints2[i], Ima1, Imb1);
		else if( (response == 'y') || (response == 'Y') )
			continue;
		else
		{
			std::cout << std::endl << "Enter Y for yes or N for No only" << std::endl;
			goto R;
		}
			

		//DisplayMarkerNumbers(Ima1, EndPoints1[i], Imb1, EndPoints2[i]);
	}
	DisplayMarkerNumbers(Ima1, EndPoints1, Imb1, EndPoints2);
	EndPoints1_ = EndPoints1;
	EndPoints2_ = EndPoints2;
}