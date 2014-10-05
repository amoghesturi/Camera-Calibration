#include "Calibration_Server.h"

double Calculate_Error(std::vector<cv::Point2f> points1, 
				  std::vector<cv::Point2f> points2,
				  cv::Mat fundemental,
				  std::vector<cv::Vec3f> lines1,
				  std::vector<cv::Vec3f> lines2)
{
	// Verification
	for (int i = 0; i < points1.size() /* 16 */; i++)
	{
		float xd = points2[i].x;
		float yd = points2[i].y;
		float x = points1[i].x;
		float y = points1[i].y;
		cv::Mat X = (cv::Mat_<double>(3,1) << x, y, 1);
		cv::Mat Xd = (cv::Mat_<double>(3,1) << xd, yd, 1);
		cv::Mat Xdt;
		cv::transpose(Xd, Xdt);
		cv::Mat A;
		A = Xdt * fundemental * X;
		std::cout << std::endl << "A = " << A;
		
	}

	double avgErr = 0;
	for( int i = 0; i < points1.size(); i++ )
    {
        double err = fabs(points1[i].x*lines2[i][0] + points1[i].y*lines2[i][1]+ lines2[i][2])/ sqrt( pow(lines2[i][0],2) + pow(lines2[i][1],2) );
        avgErr += err;	
	}
	for( int i = 0; i < points2.size(); i++ )
    {
        double err = fabs(points2[i].x*lines1[i][0] + points2[i].y*lines1[i][1]+ lines1[i][2])/ sqrt( pow(lines1[i][0],2) + pow(lines1[i][1],2) );
        avgErr += err;	
	}
	avgErr = avgErr/(points1.size() * 2);
	std::cout << std::endl <<  "avg err = " <<  avgErr ;

	return avgErr;
}