#include "Calibration_Server.h"

int Essential_Matrix(cv::Mat CameraMatrix1,
					 cv::Mat CameraMatrix2,
					 cv::Mat fundemental,
					 std::vector<cv::Point2f> points1, 
					 std::vector<cv::Point2f> points2,
					 std::vector<cv::Point> Center1, 
					 std::vector<cv::Point> Center2,
					 cv::Mat &Essential,
					 cv::Mat &P1, 
					 cv::Mat &P2,
					 float &scale,
					 cv::Mat &points3D)
{
	// Essential Matrix from Fundamental matrix
	cv::Mat K1, K2, K2t;
	K1 = CameraMatrix1;
	K2 = CameraMatrix2;
	cv::transpose(K2,K2t);
	Essential = K2t * fundemental * K1;
	std::cout << std::endl << "Essential Matrix = " << std::endl << Essential << std::endl;
	cv::Mat w, u, vt;
	cv::SVDecomp(Essential, w, u, vt);
	std::cout << std::endl << "w = " << std::endl << w << std::endl;
	double w1 = (w.at<double>(0,0) + w.at<double>(1,0))/2;
	w = ( cv::Mat_<double>(3,3) << w1, 0, 0, 0, w1, 0, 0, 0 ,0);
	std::cout << std::endl << "w = " << std::endl << w << std::endl;
	std::cout << std::endl << "u = " << std::endl << u << std::endl;	
	std::cout << std::endl << "vt = " << std::endl << vt << std::endl;	
	//Essential = u * w * vt;
	std::cout << std::endl << "Essential Matrix = " << std::endl << Essential << std::endl;
	cv::SVDecomp(Essential, w, u, vt);
	std::cout << std::endl << "w = " << std::endl << w << std::endl;
	std::cout << std::endl << "u = " << std::endl << u << std::endl;	
	std::cout << std::endl << "vt = " << std::endl << vt << std::endl;	
	int detu = cv::determinant(u);
	int detv = cv::determinant(vt);
	std::cout << std::endl << "Determinant of U = " << std::endl << detu << std::endl;	
	std::cout << std::endl << "Determinant of V = " << std::endl << detv << std::endl;
	bool ma = (detu == -1) || (detv == -1);
	if( (detu == -1) || (detv == -1) )
	{
		Essential = -Essential;
		cv::SVDecomp(Essential, w, u, vt);
		std::cout << std::endl << "w = " << std::endl << w << std::endl;
		std::cout << std::endl << "u = " << std::endl << u << std::endl;	
		std::cout << std::endl << "vt = " << std::endl << vt << std::endl;	
		std::cout << std::endl << "Determinant of U = " << std::endl << cv::determinant(u) << std::endl;	
		std::cout << std::endl << "Determinant of V = " << std::endl << cv::determinant(vt) << std::endl;
	}
	ma = (cv::determinant(u) == -1);
	ma = (cv::determinant(vt) == -1);

	cv::Mat Kinv1, Kinv2;
	cv::invert(K1, Kinv1);
	cv::invert(K2, Kinv2);
	for (int i = 0; i < points1.size() /*16*/; i++)
	{
		float xd = points2[i].x;
		float yd = points2[i].y;
		float x = points1[i].x;
		float y = points1[i].y;
		cv::Mat X = Kinv1 * (cv::Mat_<double>(3,1) << x, y, 1) ;
		cv::Mat Xd = Kinv2 * (cv::Mat_<double>(3,1) << xd, yd, 1);
		cv::Mat Xdt;
		cv::transpose(Xd, Xdt);
		cv::Mat A;
		A = Xdt * Essential * X;
		std::cout << std::endl << "A = " << A;
	}
	cv::Mat D = (cv::Mat_<double>(3,3) << 0, 1, 0, -1, 0, 0, 0, 0, 1);
	std::cout << std::endl << "D = " << std::endl << D << std::endl;
	cv::Mat Dt;
	cv::transpose(D, Dt);
	cv::Mat Ra = u * D * vt;
	cv::Mat Rb = u * Dt * vt;
	std::cout << std::endl << "Ra = " << std::endl << Ra << std::endl;
	std::cout << std::endl << "Determinant of Ra = " << std::endl << cv::determinant(Ra) << std::endl;
	std::cout << std::endl << "Rb = " << std::endl << Rb << std::endl;
	std::cout << std::endl << "Determinant of Rb = " << std::endl << cv::determinant(Rb) << std::endl;
	cv::Mat Tu = ( cv::Mat_<double>(3,1) << u.at<double>(0,2), u.at<double>(1,2), u.at<double>(2,2) );
	std::cout << std::endl << "Tu = " << std::endl << Tu << std::endl;

	cv::Mat Pa, Pb, Pc, Pd;
	int Correct[4] = {0, 0, 0, 0};
	P1 = ( cv::Mat_<double>(3,4) << 1, 0, 0, 0,
									0, 1, 0, 0,
									0, 0, 1, 0);
	cv::hconcat(Ra, Tu, Pa);
	cv::hconcat(Ra, -Tu, Pb);
	cv::hconcat(Rb, Tu, Pc);
	cv::hconcat(Rb, -Tu, Pd);
	std::cout << std::endl << "P1 = " << std::endl << P1 << std::endl;
	std::cout << std::endl << "Pa = " << std::endl << Pa << std::endl;
	std::cout << std::endl << "Pb = " << std::endl << Pb << std::endl;
	std::cout << std::endl << "Pc = " << std::endl << Pc << std::endl;
	std::cout << std::endl << "Pd = " << std::endl << Pd << std::endl;

	std::vector<cv::Point2f> pnorm1, pnorm2, points1_cen, points2_cen;
	for(int i = 0; i < Center1.size(); i++)
	{
		float x2 = Center2[i].x;
		float y2 = Center2[i].y;
		float x1 = Center1[i].x;
		float y1 = Center1[i].y;
		points1_cen.push_back(cv::Point2f(x1,y1));
		points2_cen.push_back(cv::Point2f(x2,y2));
		cv::Mat X = Kinv1 * (cv::Mat_<double>(3,1) << x1, y1, 1) ;
		cv::Mat Xd = Kinv2 * (cv::Mat_<double>(3,1) << x2, y2, 1);
		std::cout << std::endl << "X = " << std::endl << X << std::endl;
		cv::Point2f pt;
		pt.x = X.at<double>(0,0);
		pt.y = X.at<double>(1,0);
		pnorm1.push_back(pt);
		pt.x = Xd.at<double>(0,0);
		pt.y = Xd.at<double>(1,0);
		pnorm2.push_back(pt);
	}
	
	P2 = Pa;
	cv::triangulatePoints(P1, P2, pnorm1, pnorm2, points3D);
	std::cout << std::endl << " 3 D points are " << std::endl << points3D;
	cv::Point3f Pt3D[4];
	Pt3D[0] = cv::Point3f(points3D.at<float>(0,0)/points3D.at<float>(3,0), points3D.at<float>(1,0)/points3D.at<float>(3,0), points3D.at<float>(2,0)/points3D.at<float>(3,0)  );
	std::cout << std::endl << " 3 D points are " << std::endl << "( " << Pt3D[0].x << ", " << Pt3D[0].y << ", " << Pt3D[0].z << ") ";
	if( Pt3D[0].z > 0 )
		Correct[0] = 1;

	P2 = Pb;
	cv::triangulatePoints(P1, P2, pnorm1, pnorm2, points3D);
	Pt3D[1] = cv::Point3f(points3D.at<float>(0,0)/points3D.at<float>(3,0), points3D.at<float>(1,0)/points3D.at<float>(3,0), points3D.at<float>(2,0)/points3D.at<float>(3,0)  );
	std::cout << std::endl << " 3 D points are " << std::endl << "( " << Pt3D[1].x << ", " << Pt3D[1].y << ", " << Pt3D[1].z << ") ";
	if( Pt3D[1].z > 0 )
		Correct[1] = 1;

	P2 = Pc;
	cv::triangulatePoints(P1, P2, pnorm1, pnorm2, points3D);
	Pt3D[2] = cv::Point3f(points3D.at<float>(0,0)/points3D.at<float>(3,0), points3D.at<float>(1,0)/points3D.at<float>(3,0), points3D.at<float>(2,0)/points3D.at<float>(3,0)  );
	std::cout << std::endl << " 3 D points are " << std::endl << "( " << Pt3D[2].x << ", " << Pt3D[2].y << ", " << Pt3D[2].z << ") ";
	if( Pt3D[2].z > 0 )
		Correct[2] = 1;

	P2 = Pd;
	cv::triangulatePoints(P1, P2, pnorm1, pnorm2, points3D);
	Pt3D[3] = cv::Point3f(points3D.at<float>(0,0)/points3D.at<float>(3,0), points3D.at<float>(1,0)/points3D.at<float>(3,0), points3D.at<float>(2,0)/points3D.at<float>(3,0)  );
	std::cout << std::endl << " 3 D points are " << std::endl << "( " << Pt3D[3].x << ", " << Pt3D[3].y << ", " << Pt3D[3].z << ") ";
	if( Pt3D[3].z > 0 )
		Correct[3] = 1;

	int sum = 0;
	for(int i = 0; i < 4; i++)
	{
		if(Correct[i] == 1)
		{
			switch(i)
			{
			case 0: 
				P2 = Pa;
				break;
			case 1:
				P2 = Pb;
				break;
			case 2: 
				P2 = Pc;
				break;
			case 3:
				P2 = Pd;
				break;
			}
			cv::Mat PT3D = ( cv::Mat_<double>(4,1) << Pt3D[i].x, Pt3D[i].y, Pt3D[i].z, 1);
			cv::Mat PT3DR = P2 * PT3D;
			std::cout << std::endl << "PT3DR = " << std::endl << PT3DR << std::endl;	
			if( PT3DR.at<double>(2,0) < 0 )
				Correct[i] = 0;
			sum += Correct[i];
			if (sum > 1)
			{
				std::cout << std::endl << std::endl << "ERROR: More than 1 pait of R and T satisfies our rule " << std::endl;
				return -1;
			}
		}
	}
	
	for(int i = 0; i < 4; i++)
	{
		if(Correct[i] == 1)
		{
			switch(i)
			{
			case 0: 
				P2 = Pa;
				break;
			case 1:
				P2 = Pb;
				break;
			case 2: 
				P2 = Pc;
				break;
			case 3:
				P2 = Pd;
				break;
			}
		}
	}

	std::cout << std::endl << "Camera Matrix 2 = " << std::endl << P2 << std::endl;
	cv::triangulatePoints(P1, P2, pnorm1, pnorm2, points3D);
	for (int j = 0; j < points3D.cols; j++)
	{
		points3D.at<float>(0,j) = points3D.at<float>(0,j)/points3D.at<float>(3,j);
		points3D.at<float>(1,j) = points3D.at<float>(1,j)/points3D.at<float>(3,j);
		points3D.at<float>(2,j) = points3D.at<float>(2,j)/points3D.at<float>(3,j);
		points3D.at<float>(3,j) = points3D.at<float>(3,j)/points3D.at<float>(3,j);
	}
	std::cout << std::endl << "3D Points after Triangulation are: " << std::endl << points3D;

	//Finding the Scale factor
	float x_sqr = pow( points3D.at<float>(0,0) - 0, 2);
	float y_sqr = pow( points3D.at<float>(1,0) - 0, 2);
	float z_sqr = pow( points3D.at<float>(2,0) - 0, 2);
	float w_sqr = pow( points3D.at<float>(3,0) - 1, 2);

	float y_actual = y_actual_c - y_actual_m;
	float Temp = sqrt(pow(x_actual,2) + pow(z_actual,2));
	const float DISTANCE = sqrt(pow(Temp,2) + pow(y_actual,2));

	float temp_sqr = x_sqr + y_sqr + z_sqr + w_sqr;
	scale = pow(DISTANCE, 2)/temp_sqr;
	scale = std::sqrt(scale);
	std::cout << std::endl << "Scale = " << scale << std::endl;

	cv::triangulatePoints(P1, P2, pnorm1, pnorm2, points3D);
	std::cout << std::endl << "3D Points after Triangulation are: " << std::endl << points3D << std::endl;
	for (int j = 0; j < points3D.cols; j++)
	{
		points3D.at<float>(0,j) = points3D.at<float>(0,j)/points3D.at<float>(3,j);
		points3D.at<float>(1,j) = points3D.at<float>(1,j)/points3D.at<float>(3,j);
		points3D.at<float>(2,j) = points3D.at<float>(2,j)/points3D.at<float>(3,j);
		points3D.at<float>(3,j) = points3D.at<float>(3,j)/points3D.at<float>(3,j);
	}
	std::cout << std::endl << "3D Points after Triangulation are: " << std::endl << points3D << std::endl;
	points3D = points3D * scale;
	std::cout << std::endl << "3D Points after Triangulation are: " << std::endl << points3D << std::endl;
	for(int i = 0; i < points3D.rows; i++)
		for(int j = 0; j < points3D.cols; j++)
			points3D.at<float>(i,j) = cvRound(points3D.at<float>(i,j));
	std::cout << std::endl << "3D Points after Triangulation are: " << std::endl << points3D << std::endl;

	//Verification
	for(int i = 0 ; i < Center1.size() -1; i++)
	{
		float dist = sqrt( pow(points3D.at<float>(0,i)-points3D.at<float>(0,i+1),2) + 
	   					   pow(points3D.at<float>(1,i)-points3D.at<float>(1,i+1),2) +
						   pow(points3D.at<float>(2,i)-points3D.at<float>(2,i+1),2) );
		std::cout << std::endl << "Distance = " << dist << std::endl;
	}

	return 1;
	}