#include "Calibration_Server.h"

// Step 1: Internal Calibration -- Finds Camera Matrix and Distortion Coeffiecients of each camera
		   // (2 Cameras) and saves them in a XML file. It uses about 20 images of chessBoard taken 
		   // from the correspnding cameras
// Step 2: Finding Corners and Centers of markers -- 8 Markers are placed in L shape (4 on the wall
		   // or vertical plane and 4 on the floor or horizontal plane)
		   // We are assuming that the CameraMatrix and the Distortion Coefficients 
		   //are obtained saperately for both the cameras. These are stored in saperate XML files
// Step 3: Read the Centers and Corners from the XML files, verify and change manually if required 
		   // centers, corners, matches of centers adn corners. You can add new points (with zoom in function)
		   // delete unwanted markers and change matches if something is wrong.
// Step 4: Wait for the Fundamental matrix, Essential matrix, Center of the second camera, projection matrices 
		   // of both the cameras (saved in an XML) and 3D locations of the markers (displayed on the output screen)

int main()
{
	// Intrinsic Calibration of Camera 1 using ChessBoard
	int sucess_inta = Calibration_Intrinsic('a');
	//Intrinsic Calibration of camera 2 using ChessBoard
	int sucess_intb = Calibration_Intrinsic('b');

	if( (sucess_inta < 1) || (sucess_intb < 1) )
	{
		std::cout << std::endl << "Intrinsic Calibration not successful" << std::endl;
		return -1;
	}
	
	//Assuming that the Internal Claibration is done and both CameraMatrices and Distortion Matrices 
	//for both the cameras are available.
	//The Centers1, Centers2, EndPoints1, EndPoints2 are obtained from both the images

	// Detect markers
	std::string sb_int = "Data/IntCali_b.xml";
	std::string sb_img = "Data/Pics/b1.jpg";
	std::string sb_res = "Data/Sensor1_data.xml";
	std::string sa_int = "Data/IntCali_a.xml";
	std::string sa_img = "Data/Pics/a1.jpg";
	std::string sa_res = "Data/Sensor2_data.xml";
	int success_a = MarkerDetection(sb_int, sb_img, sb_res);   // Detect marker Centers and corners in image1
	int success_b = MarkerDetection(sa_int, sa_img, sa_res);   // Detect marker Centers and corners in image2
	if( (success_a < 1) || (success_b < 1) )
	{
		std::cout << std:: endl << "Marker Detection not successful";
		return -1;
	}

	cv::Mat CameraMatrix1, CameraMatrix2;
	cv::Mat DistCoeffs1, DistCoeffs2;

	// Internal Parameters of Camera 1
	//cv::Mat CameraMatrix1, DistCoeffs1;
	cv::FileStorage fs1("Data/IntCali_b.xml", cv::FileStorage::READ);
	fs1["CameraMatrix"] >> CameraMatrix1;
	fs1["DistCoeffs"] >> DistCoeffs1;;
	fs1.release();

	// Internal Parameters of Camera 2
	//cv::Mat CameraMatrix2, DistCoeffs2;
	cv::FileStorage fs2("Data/IntCali_a.xml", cv::FileStorage::READ);
	fs2["CameraMatrix"] >> CameraMatrix2;
	fs2["DistCoeffs"] >> DistCoeffs2;
	fs2.release();

	std::vector<cv::Point> Center1, Center2;
	std::vector<std::vector<cv::Point> > EndPoints1, EndPoints2;
	std::vector<cv::Point> EP1, EP2;

	//Center and EndPoints from Sensor 1
	cv::FileStorage fsa("Data/Sensor1_data.xml", cv::FileStorage::READ);
	cv::Mat Ima = cv::imread("Data/Pics/b1.jpg");
	cv::Mat Ima1;
	cv::undistort(Ima, Ima1, CameraMatrix1, DistCoeffs1);
	Ima = Ima1.clone();
	fsa["Center1"] >> Center1;
	fsa["EndPoints1"] >> EP1;
	fsa.release();
	for(int i = 0; i < EP1.size()/4; i++)
	{
		std::vector<cv::Point> ep_temp;
		for(int j = 0; j < 4; j++)
		{
			ep_temp.push_back(EP1[(4*i)+j]);
		}
		EndPoints1.push_back(ep_temp);
	}	
	VerifyCentersAndCorners(Center1, EndPoints1, Ima);  // Checks for markers and adds or removes markers manually
		
	//Center and EndPoints from Sensor 2
	cv::FileStorage fsb("Data/Sensor2_data.xml", cv::FileStorage::READ);
	cv::Mat Imb = cv::imread("Data/Pics/a1.jpg");
	cv::Mat Imb1;
	cv::undistort(Imb, Imb1, CameraMatrix2, DistCoeffs2);
	Imb = Imb1.clone();
	fsb["Center1"] >> Center2;
	fsb["EndPoints1"] >> EP2;
	fsb.release();
	for(int i = 0; i < EP2.size()/4; i++)
	{
		std::vector<cv::Point> ep_temp;
		for(int j = 0; j < 4; j++)
			ep_temp.push_back(EP2[4*i + j]);
		EndPoints2.push_back(ep_temp);
	}
	VerifyCentersAndCorners(Center2, EndPoints2, Imb);  // Checks for markers and adds or removes markers manually
	
	std::vector<cv::KeyPoint> keypoints1;
	std::vector<cv::KeyPoint> keypoints2;
	std::vector<cv::DMatch> matches, matches_centers;
	std::vector<cv::KeyPoint> keypoints_center1,keypoints_center2;
	Ima1 = Ima.clone();
	Imb1 = Imb.clone();
	std::vector<cv::Vec3f> lines1, lines2;
	cv::Mat fundemental;
	std::vector<cv::Point2f> points1, points2;
	double avgErr = 50;
	
	for(int iindex = 0; iindex < 4, avgErr > 10 ; iindex++)
		{
			MatchCorners(Center1, Center2, EndPoints1, EndPoints2, 
						 keypoints_center1, keypoints_center2,
						 keypoints1, keypoints2, matches_centers, matches, Ima1, Imb1, iindex);
			cv::Mat imageMatches;
			cv::drawMatches(Ima1,keypoints1,Imb1,keypoints2, matches,imageMatches,cv::Scalar::all(-1));
			cv::namedWindow("Matches",CV_WINDOW_NORMAL);
			cv::imshow("Matches",imageMatches);
			cv::waitKey();

			// Convert keypoints into Point2f
			points1.clear();
			points2.clear();
			for (std::vector<cv::DMatch>::const_iterator it= matches.begin(); it!= matches.end(); ++it) 
			{
				// Get the position of left keypoints
				float x= keypoints1[it->queryIdx].pt.x;
				float y= keypoints1[it->queryIdx].pt.y;
				points1.push_back(cv::Point2f(x,y));
				// Get the position of right keypoints
				x= keypoints2[it->trainIdx].pt.x;
				y= keypoints2[it->trainIdx].pt.y;
				points2.push_back(cv::Point2f(x,y));
			}
		
		// Fundamental Matrix Calculation
		fundemental = Find_Fundamental_Matrix(points1, points2, matches, keypoints1, keypoints2);
		
		////Draw Epipolar Lines
		DrawEpiLines(points1, points2, fundemental, lines1, lines2, Ima, Imb);
		
		//Calculate Error
		avgErr = Calculate_Error(points1, points2,fundemental,lines1,lines2);
		cv::destroyAllWindows;
	}

	cv::Mat Essential;
	cv::Mat P1, P2, points3D;
	float scale;
	int temp = Essential_Matrix(CameraMatrix1, CameraMatrix2, fundemental, points1, points2, Center1, Center2, Essential, P1, P2, scale, points3D);
	if(temp != 1)
	{
		std::cout << std::endl << std::endl << "ERROR: More than 1 pait of R and T satisfies our rule " << std::endl;
		return -1;
	}

	std::vector<cv::Point> Center2a;
	for(int i = 0; i < Center1.size(); i ++)
		Center2a.push_back(Center2[matches_centers[i].trainIdx]);
	Center2 = Center2a;
	DisplayMarkerNumbers(Ima, Center1, Imb, Center2);


	// Rotation along x axis
	cv::Mat Rx;
	float theta_x = Find_Rx(Center1, Center2, CameraMatrix1, CameraMatrix2, P1, P2, scale, Rx, 0);
	std::cout << std::endl << "Points3D = " << std::endl << points3D << std::endl;
	cv::Mat X3d =  ( cv::Mat_<float>(3,1) << points3D.at<float>(0,0),points3D.at<float>(1,0), points3D.at<float>(2,0) );
	std::cout << std::endl << "X3d = " << std::endl << X3d << std::endl;
	cv::Mat Rx3D = Rx * X3d;
	std::cout << std::endl << "Rx3D = " << std::endl << Rx3D << std::endl;
	cv::Mat Rx3Di;

	// Rotation along Y axis
	cv::Mat Ry;
	float theta_y = Find_Ry (Rx3D, Ry);

	cv::Mat Rotation3D = Ry * Rx;
	std::cout << std::endl << "3D Rotation = " << std::endl << Rotation3D << std::endl;
	cv::Mat P3D =  cv::Mat_<float>(3,8);
	points3D.at<float>(0,0);
	for( int i = 0; i < 3; i++)
		for(int j = 0; j < 8; j++)
			P3D.at<float>(i,j) = (float) points3D.at<float>(i,j);
	std::cout << std::endl << "P3D = " << std::endl << P3D << std::endl;
	cv::Mat P3D1 = (Rotation3D * P3D)/2.54;
	std::cout << std::endl << "P3D1 = " << std::endl << P3D1 << std::endl;

	// Find the center of the Camera 2 wrt Camera 1
	cv::Mat C2;
	FindCenterUsingP(P2, C2, Rotation3D, scale);

	//Save the parameters on to an XML file
	cv::FileStorage FS("Calibration_data.xml", cv::FileStorage::WRITE);
	FS << "CameraMatrix1" << CameraMatrix1;
	FS << "CameraMatrix2" << CameraMatrix2;
	FS << "DistCoeffs1" << DistCoeffs1;
	FS << "DistCoeffs2" << DistCoeffs2;
	FS << "Fundamental" << fundemental;
	FS << "Essential" << Essential;
	FS << "Rotation" << Rotation3D;
	FS << "ProjectionMatrix1" << P1;
	FS << "ProjectionMatrix2" << P2;
	FS << "Scale" << scale;
	FS << "C2" << C2;
	FS.release();

#ifdef VERIFICATION
	cv::Mat Kinv1, Kinv2;
	cv::invert(CameraMatrix1, Kinv1);
	cv::invert(CameraMatrix2, Kinv2);
	int mouseParam= CV_EVENT_FLAG_LBUTTON;
	cv::namedWindow("Right Image",CV_WINDOW_NORMAL);
    //cv::imshow("Right Image", Ima1);
	cvSetMouseCallback("Right Image",mouseHandler,&mouseParam);
    cv::namedWindow("Left Image",CV_WINDOW_NORMAL);
    //cv::imshow("Left Image", Imb1);
	cvSetMouseCallback("Left Image",mouseHandler,&mouseParam);
	cv::waitKey();

	cv::Mat points_veri = cv::Mat_<float>(6,2);
	
	points_veri.at<float>(0, 0) = 1150; points_veri.at<float>(0, 1) = 663;
	points_veri.at<float>(1, 0) = 619; points_veri.at<float>(1, 1) = 1090;
	points_veri.at<float>(2, 0) = 1735; points_veri.at<float>(2, 1) = 1884;
	points_veri.at<float>(3, 0) = 191; points_veri.at<float>(3, 1) = 611;
	points_veri.at<float>(4, 0) = 860; points_veri.at<float>(4, 1) = 1612;
	points_veri.at<float>(5, 0) = 2048; points_veri.at<float>(5, 1) = 927;	
	
	std::cout << std::endl << "" << points_veri << std::endl;
	std::vector<cv::Point2f> points1_norm_veri, points2_norm_veri;	 
	cv::Mat points3D_veri;
	for(int i = 0; i < points_veri.rows/2; i++)
	{
		float x2 = points_veri.at<float>(i+3,0);
		float y2 = points_veri.at<float>(i+3,1);
		float x1 = points_veri.at<float>(i,0);
		float y1 = points_veri.at<float>(i,1);
		cv::Mat X = Kinv1 * (cv::Mat_<double>(3,1) << x1, y1, 1) ;
		cv::Mat Xd = Kinv2 * (cv::Mat_<double>(3,1) << x2, y2, 1);
		std::cout << std::endl << "X = " << std::endl << X << std::endl;
		cv::Point2f pt;
		pt.x = X.at<double>(0,0);
		pt.y = X.at<double>(1,0);
		points1_norm_veri.push_back(pt);
		pt.x = Xd.at<double>(0,0);
		pt.y = Xd.at<double>(1,0);
		points2_norm_veri.push_back(pt);
	}
	cv::triangulatePoints(P1, P2, points1_norm_veri, points2_norm_veri, points3D_veri);
	for (int j = 0; j < points3D_veri.cols; j++)
	{
		points3D_veri.at<float>(0,j) = points3D_veri.at<float>(0,j)/points3D_veri.at<float>(3,j);
		points3D_veri.at<float>(1,j) = points3D_veri.at<float>(1,j)/points3D_veri.at<float>(3,j);
		points3D_veri.at<float>(2,j) = points3D_veri.at<float>(2,j)/points3D_veri.at<float>(3,j);
		points3D_veri.at<float>(3,j) = points3D_veri.at<float>(3,j)/points3D_veri.at<float>(3,j);
	}
	std::cout << std::endl << "3D Points after Triangulation are: " << std::endl << points3D_veri << std::endl;
	points3D_veri = points3D_veri * scale;
	std::cout << std::endl << "3D Points after Triangulation are: " << std::endl << points3D_veri << std::endl;
	for(int i = 0; i < points3D_veri.rows; i++)
		for(int j = 0; j < points3D_veri.cols; j++)
			points3D_veri.at<float>(i,j) = cvRound(points3D_veri.at<float>(i,j));
	std::cout << std::endl << "3D Points after Triangulation are: " << std::endl << points3D_veri << std::endl;
	
	cv::Mat P3D_veri =  cv::Mat_<float>(3,3);
	points3D_veri.at<float>(0,0);
	for( int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			P3D_veri.at<float>(i,j) = (float) points3D_veri.at<float>(i,j);
	std::cout << std::endl << "P3D_veri = " << std::endl << P3D_veri << std::endl;
	cv::Mat P3D1_veri = (Rotation3D * P3D_veri)/2.54;
	std::cout << std::endl << "P3D1_veri = " << std::endl << P3D1_veri << std::endl;
#endif

	return 1;
}