#include "ExtrinsicCalibration.h"
#include <opencv2/opencv.hpp>
#include "Helper.h"
#include "Macros.h"
using namespace cv;

ExtrinsicCalibration::ExtrinsicCalibration()
{
}

void ExtrinsicCalibration::CalculateExtrinsicParameters()
{
	string EFrameDir = Extrinsic;
	FileStorage EFile(ExtrinsicSaveFile, FileStorage::APPEND);
	FileStorage iIFile(IntrinsicSaveFile, FileStorage::READ);
	vector<vector<Point2f>> EImagePoints;
	vector<vector<Point3f>> EObjectPoints;
	for (int Cam = 1; Cam <= NumOfCameras; Cam++)
	{
		//Read image (modify if necessary)
		EFrameDir = EFrameDir.append("View" + to_string(Cam) + ".png");
		Mat AFrame = imread(EFrameDir);

		//Resize image while maintaining aspect ratio
		//Mat ResizedImage;
		//resize(AFrame, ResizedImage, Size(XRes, YRes));


		//preprocess image to improve corner finding
		Mat GrayImg;
		cvtColor(AFrame, GrayImg, COLOR_BGR2GRAY);
		Mat BnW;
		threshold(GrayImg, BnW, 75, 255, THRESH_BINARY);

		//Finding chess board corners (2D) 
		Helper::GetChessBoardCorners(BnW, EImagePoints, Cam, 1, true);
		if (EImagePoints.empty()) { EFrameDir = Extrinsic; continue; }

		//get word co ordinates 3D points
		Helper::GetObjectPoints(EObjectPoints);

		//Calibrate Camre
		cout << "\n\n\n\n************** Extrinsic FOR CAM " << Cam << "\t**************\n\n";
		cout << "size of img pints:" << EImagePoints.size() << "\n";
		cout << "size of obj pints:" << EObjectPoints.size() << "\n";


		Mat CameraMatrix, DistCoefficient;
		//calibrateCamera(EObjectPoints, EImagePoints, Size(BnW.rows, BnW.cols), CameraMatrix, DistCoefficient, RVecs, TVecs);
		iIFile["CM" + to_string(Cam)] >> CameraMatrix;
		iIFile["DC" + to_string(Cam)] >> DistCoefficient;
		Mat ERvec, ETvec, RotationMatrix, Translation;
		try
		{
			solvePnP(EObjectPoints[0], EImagePoints[0], CameraMatrix, DistCoefficient, ERvec, ETvec);
			cout << "Rvec after solve pnp" << ERvec << endl;
			cout << "Tvec after solve pnp" << ETvec << endl;
		}
		catch (const exception& ex)
		{
			cerr << " caught exception Solve PnP\n" << ex.what() << endl;
		}

		//getting translation in object space
		Rodrigues(ERvec, RotationMatrix);
		Mat RotationMatrixT = RotationMatrix.t();
		Translation = -RotationMatrixT * ETvec;

		//print data
		cout << "\nTRANSLATION :" << Translation << endl;
		cout << "\nROTATION MATRIX :" << RotationMatrix << endl;



		//write to file
		EFile << "RM" + to_string(Cam) << RotationMatrix;
		EFile << "T" + to_string(Cam) << Translation;
		EFile << "Rv" + to_string(Cam) << ERvec;
		EFile << "Tv" + to_string(Cam) << ETvec;

		EFrameDir = Extrinsic;

		EObjectPoints.clear(); EImagePoints.clear();
	}
	//close files and release buffers
	EFile.release();
	iIFile.release();
}
