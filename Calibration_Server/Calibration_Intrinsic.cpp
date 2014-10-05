#include "Calibration_Server.h"

int Calibration_Intrinsic(char image_id)
{
	std::vector<cv::Mat> Images;
	std::vector<int> test;
	cv::VideoCapture capture(0);
/////////////////////////// For WebCam Interaction //////////////////////////////		
	/*if(!capture.isOpened())
	{
		std::cout << std::endl << "ERROR: capture is Null" << std::endl;
		cv::waitKey();
		return -1;
	}
	cv::namedWindow("my window", CV_WINDOW_AUTOSIZE);
	cv::Mat frame;*/
	//Images.push_back(cv::Mat::zeros(100,100,0));

	// for the sake of demonstration and verification, I use precaptured images
	/*for(int i = 0; i < 26; i++)
	{
		if(i > 0)
		{
			capture >> frame;
			if (!frame.data)
			{
				std::cout << std::endl << "ERROR: frame is Null" << std::endl;
				cv::waitKey();
				break;
			}
		
			std::stringstream ss;
			ss << i;
			std::string name = ss.str() + ".jpg";
			cv::imwrite(name,frame);
			cv::imshow("my window", frame);
			cv::waitKey(5000);		
		}
		else 
			cv::waitKey(5000);
	}*/

	/*Images.clear();
	for(int i = 0; i < 25; i++)
	{
		std::stringstream ss;
		ss << i + 1;
		std::string name = ss.str() + ".jpg";
		frame = cv::imread(name);
		Images.push_back(frame);

		cv::imshow("my window", frame);
		cv::waitKey(5000);	
	}*/
//////////////////////////////////////////////////////
	cv::Mat CameraMatrix, DistCoeffs;
	IntCali(Images, CameraMatrix, DistCoeffs, image_id);

	return 0;
}