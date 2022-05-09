OpenCVCalibration

All the 119 frames from 16 cameras provided in 4 folders intrinsic-part[1-4] are extracted and placed in one folder named "Video", which contains all 119 frames from 16 cameras.<br/>
Frames for extrinsic data are placed in Extrinsic folder. <br/>
Relative directories for the data for Intrinsic and Extrinsic data calculation are to be provided in form of macros in IntrinsicCalibration.h, .c and ExtrinsicCalibration.h, .c respectively.<br/>
Also number of cameras and number of frames, save file names for intrinsic, extrinsic data are to be provided in Macros.h file.<br/>

After calculating intrinsic and extrinsic parameters, camera widgets are rendered using viz module. <br/>
They are as follows
![alt text](http://url/to/img.png)
