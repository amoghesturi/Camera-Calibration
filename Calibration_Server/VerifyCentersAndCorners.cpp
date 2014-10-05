#include "Calibration_Server.h"

void VerifyCentersAndCorners(std::vector<cv::Point> &Center1,
							 std::vector<std::vector<cv::Point> > &EndPoints1,
							 cv::Mat Ima)

{
	std::vector<cv::Point> Center = Center1;
	std::vector<std::vector<cv::Point>> EndPoints = EndPoints1;

	//Display Centers
	DisplayMarkerNumbers(Ima, Center);

	std::cout << std::endl << "All the markers have been identified? (Press 'Y' for yes and 'N' for No)" << std::endl;
	char all_markers;
	std::cin >> all_markers;
	if( (all_markers == 'N') || (all_markers == 'n') )
	//Accepts new marker Centers
	{
		std::cout << std::endl << "Enter the number of markers unidentified" << std::endl;
		int num_u;
		std::cin >> num_u;
		AcceptNewMarkers(Ima, Center, EndPoints, num_u);
	}	
	DisplayMarkerNumbers(Ima, Center);

	std::cout << std::endl << "Are there any non-markers identified as markers? (Press 'Y' for yes and 'N' for No)" << std::endl;
	char extra_markers;
	std::cin >> extra_markers;
	// Remove unwanted or wrong identified markers
	if( (extra_markers == 'y') || (extra_markers == 'Y') )
	{
		int u;
		std::cout << std::endl << "Enter the number of non_markers identified" << std::endl;
		std::cin >> u;
		cv::Mat Ima2 = Ima.clone();
		RemoveSomeMarkers(Ima2, Center, EndPoints, u);
	}	
	Center1 = Center;
	EndPoints1 = EndPoints;
	DisplayMarkerNumbers(Ima, Center1);
	DisplayMarkerNumbers(Ima, EndPoints1);
}		