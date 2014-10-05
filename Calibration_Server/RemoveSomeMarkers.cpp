#include "Calibration_Server.h"

void RemoveSomeMarkers(cv::Mat Ima,
					   std::vector<cv::Point> &Center1,
					   std::vector<std::vector<cv::Point> > &EndPoints1,
					   int unwanted)
{
	std::vector<cv::Point> Center = Center1;
	std::vector<std::vector<cv::Point> > EndPoints = EndPoints1;

	for(int i = 0; i < unwanted; i++)
	{
		std::cout << std::endl << "Enter the number associate with the non-marker" << std::endl;
		int non_marker;
		std::cin >> non_marker;
		//Remove the non_marker
		Center[non_marker] = cv::Point(0,0);
	}
	std::vector<cv::Point> Center2;
	std::vector<std::vector<cv::Point> > EndPoints2;
	for(int i = 0; i < Center.size(); i++)
	{
		cv::Point P = Center[i];
		if( (P.x == 0) && (P.y == 0) )
			continue;
		else
		{
			Center2.push_back(Center1[i]);
			EndPoints2.push_back(EndPoints1[i]);
		}
	}	
	Center1 = Center2;	
	EndPoints1 = EndPoints2;
	//DisplayMarkerNumbers(Ima, Center1);
	cv::destroyAllWindows();
}


	