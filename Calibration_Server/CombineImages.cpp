#include "Calibration_Server.h"

cv::Mat CombineImages(cv::Mat Ima, cv::Mat Imb)
{
	int r = (Ima.rows > Imb.rows ? Ima.rows: Imb.rows);
	int c =  Ima.cols + Imb.cols;

	cv::Mat Imc(r, c, Ima.type() );
	cv::hconcat(Ima, Imb, Imc);
	return Imc;
}