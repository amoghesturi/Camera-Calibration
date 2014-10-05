#include "Calibration_Server.h"

void changeDirection(std::vector<cv::Point>  &EndPoints1,
					 std::vector<cv::Point>  &EndPoints2,
					 cv::Mat Ima1,
					 cv::Mat Imb1)
{
	DisplayMarkerNumbers_Imc(Ima1, EndPoints1, Imb1, EndPoints2, "Press 'Return' and use output screen");
	std::cout << std::endl << "[1, 2] in left Image corresponds to [__, __] in the right Image" << std::endl;
	int arr[4];
	arr[0] = 0;
	std::cin >> arr[1] >> arr[2];
	arr[3] = 6 - arr[0] - arr[1] - arr[2];

	std::vector<cv::Point> EndPoints2a;
	EndPoints2a.push_back(EndPoints2[0]);
	EndPoints2a.push_back(EndPoints2[arr[1]]);
	EndPoints2a.push_back(EndPoints2[arr[2]]);
	EndPoints2a.push_back(EndPoints2[arr[3]]);
	EndPoints2 = EndPoints2a;

	DisplayMarkerNumbers_Imc(Ima1, EndPoints1, Imb1, EndPoints2, "Press 'Return' and use output screen");
	
}