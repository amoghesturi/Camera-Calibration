#include "Calibration_Server.h"

void AcceptNewMarkers(cv::Mat Ima, 
					  std::vector<cv::Point> &Center1, 
					  std::vector<std::vector<cv::Point> > &EndPoints1,
					  int num_u)
{
	cv::Mat Ima1 = Ima.clone();
	for(int i = 0; i < num_u; i++)
	{
		std::cout << std::endl << "Click on the center of the first unidentified marker" << std::endl;
		int x, y;

		//Zoom In
		int mouseParam= CV_EVENT_FLAG_LBUTTON;
		cv::namedWindow("Image",CV_WINDOW_NORMAL);
		cv::putText(Ima1, "Click on the center of the undetected marker to zoom in", 
					cv::Point(25, 50), 3, 1.5, cv::Scalar(0,0,255),2 );
		cv::imshow("Image",Ima1);
		cvSetMouseCallback("Image",mouseHandler_zoom,&mouseParam);
		cv::waitKey();

		cv::Mat pc;
		cv::FileStorage fsm("zoom_point.xml", cv::FileStorage::READ);
		fsm["zoom_point"] >> pc;
		fsm.release();
		cv::Point point_center(pc.at<int>(0,0), pc.at<int>(1,0));
		cv::Rect R(cv::Point(point_center.x-125, point_center.y - 125), cv::Point(point_center.x +125, point_center.y + 125));  // Does not work if the markers are at the end of the image
		//cv::rectangle(Ima, R, cv::Scalar(255,255,0), 1);
		
		cv::Mat subImg = Ima(R);
		
		//Manually select the point by clicking on the center
		mouseParam= CV_EVENT_FLAG_LBUTTON;
		cv::namedWindow("ROI",CV_WINDOW_NORMAL);
		cv::Mat Ima2 = subImg.clone();
		cv::putText(Ima2, "Choose the center of the undetected marker", 
					cv::Point(25, 50), 3, 0.1, cv::Scalar(0,0,255), 1 );
		cv::imshow("ROI",Ima2);
		cvSetMouseCallback("ROI",mouseHandler,&mouseParam);
		cv::waitKey();

		cv::Mat newMarker;
		cv::FileStorage fsm1("newMarker.xml", cv::FileStorage::READ);
		fsm1["newMarker"] >> newMarker;
		fsm1.release();
		cv::Point point_new(newMarker.at<int>(0,0), newMarker.at<int>(1,0));
		cv::circle(Ima2, point_new, 2, cv::Scalar(0,0,255), 2);
		cv::imshow("ROI",Ima2);
		cv::Point P1(R.x + point_new.x, R.y + point_new.y);
		cv::circle(Ima1, P1, 2, cv::Scalar(0,0,255), 2);
		cv::imshow("Image", Ima1);
		cv::waitKey(1000);
		Center1.push_back(P1);

		Ima1 = Ima.clone();
		//DisplayMarkerNumbers(Ima1, Center1);
		cv::destroyAllWindows();

		//Accept EndPoints
		cv::namedWindow("ROI",CV_WINDOW_NORMAL);
		cv::imshow("ROI",Ima2);
		std::vector<cv::Point> end_points;
		/*cv::namedWindow("ROI", CV_WINDOW_NORMAL);*/
		for(int i = 0; i < 4; i++)
		{
			cvSetMouseCallback("ROI",mouseHandler,&mouseParam);
			cv::waitKey();
			cv::Mat newMarker;
			cv::FileStorage fsm1("newMarker.xml", cv::FileStorage::READ);
			fsm1["newMarker"] >> newMarker;
			fsm1.release();
			cv::Point point_new(newMarker.at<int>(0,0), newMarker.at<int>(1,0));
			cv::circle(Ima2, point_new, 2, cv::Scalar(0,0,255), 2);
			cv::Point P1(R.x + point_new.x, R.y + point_new.y);
			cv::circle(Ima1, P1, 2, cv::Scalar(0,0,255), 2);
			end_points.push_back(P1);
			cv::imshow("ROI", Ima2);
			//cv::waitKey();
		}
		EndPoints1.push_back(end_points);

		//cv::namedWindow("ROI", CV_WINDOW_NORMAL);
		cv::namedWindow("Image", 0);
		cv::imshow("Image", Ima1);
		cv::waitKey(1000);
	}
	cv::destroyAllWindows();
}	