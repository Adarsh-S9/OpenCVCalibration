#include "IntrinsicCalibration.h"
#include "ExtrinsicCalibration.h"

#include "Helper.h"


int main(int* argc, char** argv)
{
	/*
	const int XRes = 2048, YRes = 1368;
	* 2048 X 1368
	* 1920 X 1283
	* 1280 X 855
	*/
	IntrinsicCalibration* Ic = new IntrinsicCalibration();
	Ic->CalaculateIntrinsicParameters();

	ExtrinsicCalibration* Ec = new ExtrinsicCalibration();
	Ec->CalculateExtrinsicParameters();

	delete Ec;
	delete Ic;

	Helper::RenderCameras();
	destroyAllWindows();
	return 0;
}