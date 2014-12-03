/************************************
 *Code for background subtraction
 *************.**********************/

 #include "opencv2/core.hpp"
 #include "opencv2/core/utility.hpp"
 #include "opencv2/videoio.hpp"
 #include "opencv2/video.hpp"
 #include "opencv2/highgui.hpp"
 #include "opencv2/imgproc.hpp"
 #include <iostream>
 #include <string>
 using namespace cv;

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cout<<"\n Usage: ./motsal Video_Path"
		return -1;
	}
	VideoCapture cap;
	cap.open(argv[1]);
	if(!cap.isOpened())
	{
		std::cout<<"\n Video path wrong!"
		retun -2;
	}

	Mat frame, frameGray;
	while(1)
	{

	}


}