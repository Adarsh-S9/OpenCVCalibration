#pragma once
#include <string>
using namespace std;

class ExtrinsicCalibration
{
	const string Extrinsic = "../../Extrinsic/frame1/";
public:
	ExtrinsicCalibration();
	void CalculateExtrinsicParameters();
};

