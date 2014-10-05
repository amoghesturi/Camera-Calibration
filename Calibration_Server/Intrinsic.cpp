#include "Calibration_Server.h"

void IntCali(std::vector<cv::Mat> Images, 
			 cv::Mat &cameraMatrix, 
			 cv::Mat &distCoeffs,
			 char image_id)
{
	std::vector<cv::Mat> image = Images;
	int num_images;
	if(image_id == 'a')
	{
		char *filenames[] = {"Data/Pics/imx6a/1.jpg",
							 "Data/Pics/imx6a/2.jpg",
							 "Data/Pics/imx6a/3.jpg",
							 "Data/Pics/imx6a/4.jpg",
							 "Data/Pics/imx6a/5.jpg",
							 "Data/Pics/imx6a/6.jpg",
							 "Data/Pics/imx6a/7.jpg",
							 "Data/Pics/imx6a/8.jpg",
							 "Data/Pics/imx6a/9.jpg",
							 "Data/Pics/imx6a/10.jpg",
							 "Data/Pics/imx6a/11.jpg",
							 "Data/Pics/imx6a/12.jpg",
							 "Data/Pics/imx6a/13.jpg",
							 "Data/Pics/imx6a/14.jpg",
							 "Data/Pics/imx6a/15.jpg",
							 "Data/Pics/imx6a/16.jpg",
							 "Data/Pics/imx6a/17.jpg",
							 "Data/Pics/imx6a/18.jpg",
							 "Data/Pics/imx6a/19.jpg",
							 "Data/Pics/imx6a/20.jpg",
							 "Data/Pics/imx6a/21.jpg",
							 "Data/Pics/imx6a/22.jpg",
							 "Data/Pics/imx6a/23.jpg"};

		num_images = sizeof( filenames )/sizeof(filenames[0]); // this assumes each string has same length
		//int num_images = Images.size();
		image.clear();
		for(int i = 0; i < num_images; i++)
		{
			cv::Mat frame = cv::imread(filenames[i]);
			image.push_back(frame);
		}
	}

	else if(image_id == 'b')
	{
		char *filenames[] = {"Data/Pics/imx6b/1.jpg",
							 "Data/Pics/imx6b/2.jpg",
							 "Data/Pics/imx6b/3.jpg",
							 "Data/Pics/imx6b/4.jpg",
							 "Data/Pics/imx6b/5.jpg",
							 "Data/Pics/imx6b/6.jpg",
							 "Data/Pics/imx6b/7.jpg",
							 "Data/Pics/imx6b/8.jpg",
							 "Data/Pics/imx6b/9.jpg",
							 "Data/Pics/imx6b/10.jpg",
							 "Data/Pics/imx6b/11.jpg",
							 "Data/Pics/imx6b/12.jpg",
							 "Data/Pics/imx6b/13.jpg",
							 "Data/Pics/imx6b/14.jpg",
							 "Data/Pics/imx6b/15.jpg",
							 "Data/Pics/imx6b/16.jpg",
							 "Data/Pics/imx6b/17.jpg",
							 "Data/Pics/imx6b/18.jpg",
							 "Data/Pics/imx6b/19.jpg"};
		
		num_images = sizeof( filenames )/sizeof(filenames[0]); // this assumes each string has same length
		//int num_images = Images.size();
		image.clear();
		for(int i = 0; i < num_images; i++)
		{
			cv::Mat frame = cv::imread(filenames[i]);
			image.push_back(frame);
		}
	}

	cv::Size boardSize(hor_box-1 ,ver_box-1);				// number of boxes on the chessboard
	std::vector<std::vector <cv::Point2f> > imagePoints;	    // the point positions in pixels.  Contains points from all images.
	std::vector<cv::Point2f> imageCorners;                  // output vectors of image points
	std::vector<std::vector <cv::Point3f> > objectPoints;     // the point positions in world coords
	// Set 3D points
	std::vector<cv::Point3f> objectCorners;                 // 3D real world points
	
	// Initialize 3D Scene Pts: chessboard corners in chessboard 3D reference frame
	// The corners are at 3D location (X,Y,Z)= (i,j,0)
	for (int i=0; i<boardSize.height; i++) 
		for (int j=0; j<boardSize.width; j++) 
			objectCorners.push_back(cv::Point3f(i*size_box, j*size_box, 0.0f)); // *2.05 since each square h as width 2.05cm

	for(int i = 0; i <num_images; i++)
	{
		cv::Size image1Size=image[0].size();
#ifdef DEBUG
		/*cv::namedWindow("ChessBoard image", CV_WINDOW_NORMAL);
		cv::imshow("ChessBoard image", image[i]);*/
		//cv::waitKey(0);
#endif

		// FIND AND DISPLAY CORNERS OF CHESSBOARD IN EACH image
		// Get the chessboard corners
		cv::Mat image1, image1_gray;
		int successes = 0, f;
		image1 = image[i].clone();
		cv::cvtColor(image1, image1_gray, CV_BGR2GRAY);
		cv::namedWindow("image", 0);
		cv::imshow("image", image1);
		//cv::waitKey();

		bool found = cv::findChessboardCorners(image[i],boardSize, imageCorners);
		
		if (found==0) 
		{
			image[i] = image[i-1];
			cv::Size image1Size=image[0].size();
			cv::Mat image1, image1_gray;
			int successes = 0, f;
			image1 = image[i].clone();
			cv::imshow("ChessBoard image", image1);
			//cv::waitKey();
			cv::cvtColor(image1, image1_gray, CV_BGR2GRAY);
			bool f1 = cv::findChessboardCorners(image[i],boardSize, imageCorners);
			cv::imshow("ChessBoard image", image[i]);
			//printf("Did not find chessboard corners in image1.\n");
		}
		image1 = image[i].clone();
		cv::cornerSubPix(image1_gray, imageCorners, cv::Size(5,5), cv::Size(-1,-1), cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 30, 0.1));
		//If we have a good board, add it to our data
		if (imageCorners.size() == boardSize.area())
		{
			imagePoints.push_back(imageCorners);
			objectPoints.push_back(objectCorners);			
		}
		successes++;
		cv::drawChessboardCorners(image1, boardSize, imageCorners, found);
		//Draw the corners
		cv::drawChessboardCorners(image1,boardSize, imageCorners,found); // corners have been found
		cv::namedWindow("ChessBoard Corners", CV_WINDOW_NORMAL);
		cv::imshow("ChessBoard Corners", image1);
		//cv::waitKey(0);
		std::cout << std::endl << i;		
	}

	// output Matrices
	int flag;						// flag to specify how calibration is done
	cv::Mat map1,map2;              // used in image undistortion
	bool mustInitUndistort = true;
	std::vector<cv::Mat> rvecs, tvecs;
	cv::calibrateCamera(objectPoints, imagePoints, image[0].size(), cameraMatrix, distCoeffs, rvecs, tvecs);
	std::cout << "Camera Matrix:" << std::endl << cameraMatrix << std::endl;
	std::cout << "Distortion Matrix:" << std::endl << distCoeffs << std::endl;
	std::vector<cv::Mat> undistorted, undistorted1;
	undistorted.clear();

#ifdef UNDISTORT
	for (int i = 0; i < num_images; i++)
	{
		cv::namedWindow("Distorted Image", 0);
		cv::imshow("Distorted Image", image[i]);
		//cv::waitKey();
		cv::Mat un;
		//Undistorting the image
		cv::undistort(image[i], un, cameraMatrix, distCoeffs);
#ifdef DEBUG
		cv::namedWindow("Undistorted Image", CV_WINDOW_NORMAL);
		cv::imshow("Undistorted Image", un);
		cv::waitKey(0);
#endif
		undistorted.push_back(un);
	}
#endif

	if(image_id == 'a')
	{
		cv::FileStorage fs("Data/IntCali1_a.xml", cv::FileStorage::WRITE);
		fs << "CameraMatrix" << cameraMatrix;
		fs << "DistCoeffs" << distCoeffs;
		fs.release();
	}
	else if(image_id == 'b')
	{
		cv::FileStorage fs("Data/IntCali1_b.xml", cv::FileStorage::WRITE);
		fs << "CameraMatrix" << cameraMatrix;
		fs << "DistCoeffs" << distCoeffs;
		fs.release();
	}
#ifdef VERIFICATION_Int
	for(int i = 0; i <num_images; i++)
	{
		cv::Mat un = undistorted[i];
		cv::Mat rotation_1, rotation_2, rotation_3;
		cv::Rodrigues(rvecs[i], rotation_1);
		//cv::Rodrigues(rvecs[1], rotation_2);
		std::cout << std::endl << "Roatation Matrix 1 = " << rotation_1 << std::endl;
		//std::cout << std::endl << "Roatation Matrix 2 = " << rotation_2 << std::endl;
		std::cout << std::endl << "Translation Vector 1 = " << tvecs[i] << std::endl;
		//std::cout << std::endl << "Translation Vector 2 = " << tvecs[1] << std::endl;
		cv::Mat Rt1, Rt2;
		cv::hconcat(rotation_1, tvecs[i], Rt1);
		std::cout << std::endl << "Extrinsic Matrix = " << Rt1 << std::endl;
		cv::Mat ProjectionMatrix1;
		cv::gemm(cameraMatrix, Rt1, 1, Rt1, 0, ProjectionMatrix1);
		ProjectionMatrix1 = ProjectionMatrix1/ProjectionMatrix1.at<double>(2,3);
		std::cout << std::endl << "Projection Matrix P1 = " << ProjectionMatrix1 << std::endl;
		
		cv::Mat P1 = (cv::Mat_<double>(4,1) << 0, 0, 0, 1);
		//cv::Mat P2 = (cv::Mat_<double>(4,1) << 20, 17.5, 0, 1);
		std::cout << std::endl << P1;
		cv::Mat p1(3,1,CV_32FC1),p2(1,3,CV_32FC1);
		cv::gemm(ProjectionMatrix1, P1, 1, P1, 0, p1);
		p1 = p1/p1.at<double>(2);
		std::cout << std::endl << p1;
		cv::circle(un, cv::Point(cvCeil(p1.at<double>(0)),cvCeil(p1.at<double>(1))), 3, cv::Scalar(255,0,0), 3);
		cv::namedWindow("Verification", CV_WINDOW_NORMAL);
		cv::imshow("Verification", un);
		cv::waitKey(0);
	}
#endif
}