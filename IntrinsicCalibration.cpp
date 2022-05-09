#include "IntrinsicCalibration.h"
#include <opencv2/opencv.hpp>
#include "Helper.h"
#include "Macros.h"

using namespace cv;
IntrinsicCalibration::IntrinsicCalibration()
{

}

void IntrinsicCalibration::CalaculateIntrinsicParameters()
{
	string FrameDir = VideoRoot;

	//opening file to write intrinsic data
	FileStorage IFile(IntrinsicSaveFile, FileStorage::APPEND);

	vector<vector<Point2f>> IImagePoints;
	vector<vector<Point3f>> IObjectPoints;

	Mat BnW;
	for (int cam = 1; cam <= NumOfCameras; cam++)
	{
		for (int frames = 1; frames <= NumOfFrames; frames++)
		{
			//getting frame (modify if necessary)
			FrameDir.append("frame" + to_string(frames) + "/view" + to_string(cam) + ".png");

			//reading image
			Mat AFrame = imread(FrameDir);

			//Mat ResizedImage;
			//resize(AFrame, ResizedImage, Size(XRes, YRes));

			//converting image to gray scale
			Mat GrayImg;
			cvtColor(AFrame, GrayImg, COLOR_BGR2GRAY);

			//preprocessing to improve pattern recognition
			threshold(GrayImg, BnW, 75, 255, THRESH_BINARY);

			bool found = Helper::GetChessBoardCorners(BnW, IImagePoints, cam, frames, true);

			//if chess board corners are not detected continue
			if (not found)
			{
				FrameDir = VideoRoot;
				continue;
			}

			//get word co ordinates 3D points
			Helper::GetObjectPoints(IObjectPoints);
			FrameDir = VideoRoot;
		}
		cout << "\n\n\n\n************ Intrinsic FOR CAM\t" << cam << "\t*****************\n\n";
		cout << "size of img pints:" << IImagePoints.size() << "\n";
		cout << "size of obj pints:" << IObjectPoints.size() << "\n";

		//Calibrate Camera
		Mat ICameraMatrix, IDistCoefficient, IRVecs, ITVecs;
		calibrateCamera(IObjectPoints, IImagePoints, Size(BnW.rows, BnW.cols), ICameraMatrix, IDistCoefficient, IRVecs, ITVecs);
		if (not ICameraMatrix.empty())
			cout << "cameraMatrix : " << ICameraMatrix << endl;
		if (not IDistCoefficient.empty())
			cout << "distCoeffs : " << IDistCoefficient << endl;

		//write intrinsic parameters to file
		IFile << "CM" + to_string(cam) << ICameraMatrix;
		IFile << "DC" + to_string(cam) << IDistCoefficient;

		IImagePoints.clear(); IObjectPoints.clear();
	}
	
	IFile.release();
}
