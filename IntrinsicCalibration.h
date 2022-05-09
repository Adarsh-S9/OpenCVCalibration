#pragma once
#include <string>
using namespace std;
class IntrinsicCalibration
{
	const std::string VideoRoot = "../../Video/";
public:
	IntrinsicCalibration();
	void CalaculateIntrinsicParameters();
};

