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
 	std::string str;

 	Mat frame, frameGray;
 	VideoCapture cap(argv[1]);
 	//VideoCapture cap(0);
 	if(!cap.isOpened())
 	{
 		std::cout<<"\n Video doesnot exist!";
 		return -1;
 	}
 	char c;
 //Creating a pointer for background subtraction

	Ptr<BackgroundSubtractorMOG2> bgModel = createBackgroundSubtractorMOG2(500 /*history*/, 16 /*varThreshold*/, true/*detectShadows*/ );

	Mat fgMask, fgImg;
	bool update_bg_model=1;
	Mat noiseModelMean, noiseModelVariance;
	VideoWriter vwMask("Mask.avi", VideoWriter::fourcc('X','2','6','4'), 30, Size(720,1280), false);
	VideoWriter vwForeground("ForeGround.avi", VideoWriter::fourcc('X','2','6','4'), 30, Size(720,1280), false);
	int count = 1;

	if(!vwMask.isOpened())
	{
		std::cout<<"vid1 failed!";
	}


    Mat bgimg;
 	while(1)
 	{
 		cap >> frame;
 		//std::cout<<"img size :" << frame.rows <<" "<< frame.cols;
 		cvtColor(frame, frameGray, COLOR_RGB2GRAY);
 		/*
 		if(count == 1)
 		{
 			noiseModelMean.create(frameGray.rows, frameGray.cols, CV_32FC1);
 			noiseModelVariance.create(frameGray.rows, frameGray.cols, CV_32FC1);
 			frameGray.copyTo(noiseModelMean);
 			noiseModelMean /= 255; //Normalizing values of mean

 		}
 		Mat temp;
 		temp.create(frameGray.rows, frameGray.cols, CV_32FC1);
 		temp = frameGray/255.0;
 		noiseModelMean *= count - 1;
 		noiseModelMean = (noiseModelMean + temp) / count; 
 		count++;
 		*/
 		if(fgImg.empty())
 			fgImg.create(frameGray.rows, frameGray.cols, CV_8UC1);

 		bgModel->apply(frameGray, fgMask, update_bg_model ? -1 : 0); //Creates a foreground mask

 		GaussianBlur(fgMask, fgMask, Size(11, 11), 3.5, 3.5);
        threshold(fgMask, fgMask, 10, 255, THRESH_BINARY);
        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5,5), Point(-1,-1));
        dilate(fgMask, fgMask, kernel , Point(-1,-1), 3, BORDER_CONSTANT, morphologyDefaultBorderValue() );


		
		fgImg = Scalar::all(0);
        frameGray.copyTo(fgImg, fgMask);

        bgModel->getBackgroundImage(bgimg);


 		imshow("image", frameGray);
        imshow("foreground mask", fgMask);
        imshow("foreground image", fgImg);

        if(!bgimg.empty())
          imshow("mean background image", bgimg );
      	vwMask << fgMask;
      	vwForeground << fgImg;
        char k = (char)waitKey(30);
        if( k == 27 ) break;
        if( k == ' ' )
        {
            update_bg_model = !update_bg_model;
            if(update_bg_model)
                printf("Background update is on\n");
            else
                printf("Background update is off\n");
        }
		
		//Mat noiseMean(frameGray.rows, frameGray.cols, CV_8UC1);
		//noiseModelMean.copyTo(temp);
		//temp *=255.0;
		//temp.convertTo(noiseMean, CV_8U, 255, 0);
		//noiseMean = floor(noiseMean * 255);
		//imshow("video", frameGray);
		//imshow("noiseMean", noiseMean);
		//imshow("Temp", temp);
		if( waitKey(33) == 27)
			break;
 		//c = waitKey(33);
 	}

 	imwrite("bgImg.jpeg", bgimg);
 	return 0;

 }