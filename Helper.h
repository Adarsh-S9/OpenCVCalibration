#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;

class Helper
{
	static const float SquareSideLength; //meters
	static const Size ChessBoardSize; // intersecting corners
public:
	/**
		@brief Gets image coordinates 2D from given chess board image
		@param Image image in which chess board corners are to be found
		@param AllFoundCorners contains corners of all images
		@param Cam for debug purpose
		@param frame for debug purpose
		@param ShowResult for debug purpose
	*/
	static bool GetChessBoardCorners(Mat Image, vector<vector<Point2f>>& AllFoundCorners, int Cam, int Frame, bool ShowResults = true);
	static void GetObjectPoints(vector<vector<Point3f>>& Corners);
	static void RenderCameras();
};

