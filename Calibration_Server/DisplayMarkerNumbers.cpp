#include "Calibration_Server.h"

//Display Marker Center numbers
void DisplayMarkerNumbers(cv::Mat Ima,
						  std::vector<cv::Point> Center)
{
	cv::Mat Ima1 = Ima.clone();
	cv::destroyAllWindows();
	cv::namedWindow("Centers", 0);
	for(int i = 0; i < Center.size(); i++)
	{
		cv::Point P(Center[i].x, Center[i].y );
		std::stringstream ss;
		ss << i;
		std::string text = ss.str();
		cv::circle(Ima1, P, 2, cv::Scalar(0,255,0), 2);
		cv::putText(Ima1, text, P, 0, 1, cv::Scalar(0,0,255), 2);
		cv::imshow("Centers", Ima1);
		//cv::waitKey();

	}
	cv::Mat Ima2 = Ima1.clone();
	cv::putText(Ima2, "Press return and check the output screen",
	   			cv::Point(25, 50), 3, 1.5, cv::Scalar(0,0,255),2);
	cv::imshow("Centers", Ima2);
	cv::waitKey();
}





//Display Corner Numbers
void DisplayMarkerNumbers(cv::Mat Ima,
						  std::vector<std::vector<cv::Point> > EndPoints)
{
	cv::Mat Ima1 = Ima.clone();
	cv::destroyAllWindows();
	cv::namedWindow("Marker Corners", 0);
	for(int i = 0; i < EndPoints.size(); i++)
	{
		for(int j = 0; j < EndPoints[i].size(); j++)
		{
			cv::Point P(EndPoints[i][j].x, EndPoints[i][j].y );
			std::stringstream ss;
			ss << 4*i+j;
			std::string text = ss.str();
			cv::circle(Ima1, P, 2, cv::Scalar(0,255,0), 2);
			cv::putText(Ima1, text, P, 0, 1, cv::Scalar(255,0,255), 2);
			cv::imshow("Marker Corners", Ima1);
			//cv::waitKey();
		}
	}
	
	cv::Mat Ima2 = Ima1.clone();
	cv::putText(Ima2, "Press return and check the output screen",
	   			cv::Point(25, 50), 3, 1.5, cv::Scalar(0,0,255),2);
	cv::imshow("Marker Corners", Ima2);
	cv::waitKey();
}





void DisplayMarkerNumbers(cv::Mat Ima,
						  std::vector<cv::Point> Center1,
						  cv::Mat Imb,
						  std::vector<cv::Point> Center2,
						  std::string str1,
						  std:: string str2)
{
	cv::Mat Ima1 = Ima.clone();
	cv::destroyAllWindows();
	cv::namedWindow("Image1", 0);
	for(int i = 0; i < Center1.size(); i++)
	{
		cv::Point P(Center1[i].x, Center1[i].y );
		std::stringstream ss;
		ss << i;
		std::string text = ss.str();
		cv::circle(Ima1, P, 2, cv::Scalar(0,255,0), 2);
		cv::putText(Ima1, text, P, 0, 1, cv::Scalar(255,0,0), 2);
		cv::imshow("Image1", Ima1);
		//cv::waitKey();

	}
	cv::Mat Ima2 = Ima1.clone();
	
	cv::Mat Imb1 = Imb.clone();
	cv::namedWindow("Image2", 0);
	for(int i = 0; i < Center2.size(); i++)
	{
		cv::Point P(Center2[i].x, Center2[i].y );
		std::stringstream ss;
		ss << i;
		std::string text = ss.str();
		cv::circle(Ima1, P, 2, cv::Scalar(0,255,0), 2);
		cv::putText(Imb1, text, P, 0, 1, cv::Scalar(255,0,0), 2);
		cv::imshow("Image2", Imb1);
		//cv::waitKey();

	}
	cv::Mat Imb2 = Imb1.clone();
	cv::putText(Imb2, str1, cv::Point(25, 50), 3, 1.5, cv::Scalar(0,0,255),2);
	cv::putText(Imb2, str2, cv::Point(25, 125), 3, 1.5, cv::Scalar(0,0,255),2);
	cv::imshow("Image2", Imb2);
	cv::waitKey();
}




void DisplayMarkerNumbers(cv::Mat Ima,
						  std::vector<std::vector<cv::Point> > EndPoints1,
						  cv::Mat Imb,
						  std::vector<std::vector<cv::Point> > EndPoints2,
						  std::string str1,
						  std:: string str2)
{
	cv::Mat Ima1 = Ima.clone();
	cv::destroyAllWindows();
	cv::namedWindow("Marker Corners 1", 0);
	for(int i = 0; i < EndPoints1.size(); i++)
	{
		for(int j = 0; j < EndPoints1[i].size(); j++)
		{
			cv::Point P(EndPoints1[i][j].x, EndPoints1[i][j].y );
			std::stringstream ss;
			ss << /*4*i+*/j;
			std::string text = ss.str();
			cv::circle(Ima1, P, 2, cv::Scalar(0,255,0), 2);
			cv::putText(Ima1, text, P, 0, 1, cv::Scalar(255,0,255), 2);
			cv::imshow("Marker Corners 1", Ima1);
			//cv::waitKey();
		}
	}
	
	cv::Mat Ima2 = Ima1.clone();
	cv::waitKey();


	cv::Mat Imb1 = Imb.clone();
	cv::namedWindow("Marker Corners 2", 0);
	for(int i = 0; i < EndPoints2.size(); i++)
	{
		for(int j = 0; j < EndPoints2[i].size(); j++)
		{
			cv::Point P(EndPoints2[i][j].x, EndPoints2[i][j].y );
			std::stringstream ss;
			ss << /*4*i+*/j;
			std::string text = ss.str();
			cv::circle(Ima1, P, 2, cv::Scalar(0,255,0), 2);
			cv::putText(Imb1, text, P, 0, 1, cv::Scalar(255,0,255), 2);
			cv::imshow("Marker Corners 2", Imb1);
			//cv::waitKey();
		}
	}
	
	cv::Mat Imb2 = Imb1.clone();
	cv::putText(Ima2, str1,	cv::Point(25, 50), 3, 1.5, cv::Scalar(0,0,255),2);
	cv::imshow("Marker Corners 2", Ima2);
	cv::putText(Imb2, str2, cv::Point(25, 50), 3, 1.5, cv::Scalar(0,0,255),2);
	cv::imshow("Marker Corners 2", Imb2);
	cv::waitKey();
}


void DisplayMarkerNumbers_Imc(cv::Mat Ima,
						  std::vector<cv::Point> Center1,
						  cv::Mat Imb,
						  std::vector<cv::Point> Center2,
						  std::string str1,
						  std:: string str2)
{
	cv::Mat Ima1 = Ima.clone();
	cv::destroyAllWindows();
	//cv::namedWindow("Image1", 0);
	for(int i = 0; i < Center1.size(); i++)
	{
		cv::Point P(Center1[i].x, Center1[i].y );
		std::stringstream ss;
		ss << i;
		std::string text = ss.str();
		cv::circle(Ima1, P, 2, cv::Scalar(0,255,0), 2);
		cv::putText(Ima1, text, P, 0, 1, cv::Scalar(255,0,0), 2);
		//cv::imshow("Image1", Ima1);
		//cv::waitKey();

	}
	cv::Mat Ima2 = Ima1.clone();
	
	cv::Mat Imb1 = Imb.clone();
	//cv::namedWindow("Image2", 0);
	for(int i = 0; i < Center2.size(); i++)
	{
		cv::Point P(Center2[i].x, Center2[i].y );
		std::stringstream ss;
		ss << i;
		std::string text = ss.str();
		cv::circle(Ima1, P, 2, cv::Scalar(0,255,0), 2);
		cv::putText(Imb1, text, P, 0, 1, cv::Scalar(255,0,0), 2);
		//cv::imshow("Image2", Imb1);
		//cv::waitKey();

	}
	cv::Mat Imb2 = Imb1.clone();
	cv::putText(Imb2, str1, cv::Point(25, 50), 3, 1.5, cv::Scalar(0,0,255),2);
	cv::putText(Imb2, str2, cv::Point(25, 125), 3, 1.5, cv::Scalar(0,0,255),2);
	//cv::imshow("Image2", Imb2);
	cv::Mat Imc = CombineImages(Ima2, Imb2);
	cv::namedWindow("Corners", 0);
	cv::imshow("Corners", Imc);
	cv::waitKey();
	
}