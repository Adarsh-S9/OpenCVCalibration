#include <string>
#include <opencv2/viz/widgets.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/viz/vizcore.hpp>
#include <opencv2/viz/viz3d.hpp>
#include "Macros.h"

#include "Helper.h"



const float Helper::SquareSideLength = 0.12; //meters
const Size Helper::ChessBoardSize = Size(9, 6); // intersecting corners

bool Helper::GetChessBoardCorners(Mat Image, vector<vector<Point2f>>& AllFoundCorners, int Cam, int Frame, bool ShowResults)
{
	vector<Point2f> PointBuf;
	static int CornerFoundCount = 0;

	bool found = findChessboardCorners(Image, ChessBoardSize, PointBuf, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);
	string Ack = "";

	if (found)
	{
		CornerFoundCount++;
		Ack = "corners Found Cam :" + to_string(Cam) + " Frame : " + to_string(Frame) + " Count : " + to_string(CornerFoundCount);
		try
		{
			// refining pixel coordinates for given 2d points.
			TermCriteria criteria(TermCriteria::EPS | TermCriteria::COUNT, 30, 0.001);
			cv::cornerSubPix(Image, PointBuf, cv::Size(5, 5), cv::Size(-1, -1), criteria);
		}
		catch (const exception& ex)
		{
			cerr << "Catch some exception" << ex.what() << endl;
		}
		AllFoundCorners.push_back(PointBuf);
	}
	else
		Ack = "Corners Not Found Cam: " + to_string(Cam) + " Frame : " + to_string(Frame) + " Count : " + to_string(CornerFoundCount);
	if (ShowResults)
	{
		drawChessboardCorners(Image, ChessBoardSize, PointBuf, found);
		putText(Image, Ack, Point(50, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0));
		imshow("Corners", Image);
		waitKey(10);
		return found;
	}
	else
		return found;
}

void Helper::GetObjectPoints(vector<vector<Point3f>>& Corners)
{
	vector<Point3f> CornersBuf;
	for (int i = 0; i < ChessBoardSize.height; i++)
	{
		for (int j = 0; j < ChessBoardSize.width; j++)
		{
			CornersBuf.push_back(Point3f(j * SquareSideLength, i * SquareSideLength, 0.f));
		}
	}
	Corners.push_back(CornersBuf);
}
void Helper::RenderCameras()
{
	//open files to read intrinsic and extrinsic data
	FileStorage iIFile(IntrinsicSaveFile, FileStorage::READ);
	FileStorage iEFile(ExtrinsicSaveFile, FileStorage::READ);

	//create window to render our cameras
	cv::viz::Viz3d window = cv::viz::Viz3d("Camera Render");

	//lopp over all cam data and display their widgets
	for (int i = 1; i <= NumOfCameras; i++)
	{
		Mat RotationMatrix, CameraMatrix, Translation, Tvec, Rvec;
		//Read data
		iIFile["CM" + to_string(i)] >> CameraMatrix;
		iEFile["RM" + to_string(i)] >> RotationMatrix;
		iEFile["T" + to_string(i)] >> Translation;
		iEFile["Tv" + to_string(i)] >> Tvec;
		iEFile["Rv" + to_string(i)] >> Rvec;

		Matx33d CM = CameraMatrix;

		//creating camera widget
		viz::WCameraPosition CameraPosition = viz::WCameraPosition(CM);
		window.showWidget("Cam" + to_string(i), CameraPosition);

		//set rotationa and translation of camera in window (pose)
		window.setWidgetPose("Cam" + to_string(i), Affine3d(RotationMatrix, Translation));
	}
	while (!window.wasStopped())
	{
		window.spinOnce(1, true);
	}
	iIFile.release();
	iEFile.release();
}
