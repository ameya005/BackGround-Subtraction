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

 #define THRESHOLD 100
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
    //Mat noiseModelMean, noiseModelVariance;
    //VideoWriter vwMask("Mask.avi", VideoWriter::fourcc('X','2','6','4'), 30, Size(720,1280), false);
    //VideoWriter vwForeground("ForeGround.avi", VideoWriter::fourcc('X','2','6','4'), 30, Size(720,1280), false);
    int count = 1;

    /*
    if(!vwMask.isOpened())
    {
        std::cout<<"vid1 failed!";
    }

    */
    Mat bgimg;
    std::vector < std::vector <Point> > objects;
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
        GaussianBlur( frameGray, frameGray, Size(5,5), 3.5, 3.5);
        bgModel->apply(frameGray, fgMask, update_bg_model ? -1 : 0); //Creates a foreground mask

        GaussianBlur(fgMask, fgMask, Size(11, 11), 3.5, 3.5);
        threshold(fgMask, fgMask, 10, 255, THRESH_BINARY);
        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5,5), Point(-1,-1));
        dilate(fgMask, fgMask, kernel , Point(-1,-1), 3, BORDER_CONSTANT, morphologyDefaultBorderValue() );


        
        fgImg = Scalar::all(0);
        frameGray.copyTo(fgImg, fgMask);

        bgModel->getBackgroundImage(bgimg);

        Mat contours;
        fgMask.copyTo(contours);

       // Mat hier;
        findContours(contours, objects, RETR_LIST, CHAIN_APPROX_NONE);
        Mat output=Mat::zeros(frameGray.rows, frameGray.cols, CV_8UC3);
        int idxMax = 0;
        int maxArea = 0;
        float distRatio;
        int angularPosition;
        Point centroid;
        if(objects.size() > 0)
        {
            Rect bdRect;
            std::vector <Rect> bdrectVec; 
            for(int idx =0; idx<objects.size();idx++)
            {
                // Scalar color( rand()&255, rand()&255, rand()&255 );
                // drawContours(output, objects, idx, color,  FILLED, 8 );
                // bdRect = boundingRect(objects[idx]);
                // bdrectVec.push_back(bdRect);
                // rectangle(frame, bdRect, color, LINE_8, 0);
                int area = contourArea(objects[idx]);
                if( area >= maxArea && area > THRESHOLD )
                {
                    idxMax = idx;
                    maxArea = area;
                }
               
            }
            
            Scalar color( rand()&255, rand()&255, rand()&255);
            bdRect = boundingRect(objects[idxMax]);
         // bdrectVec.push_back(bdRect);
            rectangle(frame, bdRect, color, LINE_8, 0);

            /********************Angle analysis*************************/

            Moments contMom;
            contMom = moments(objects[idxMax], true);
            centroid = Point( (contMom.m10/contMom.m00) , (contMom.m01/contMom.m00) );

            //std::cout<<"\n Centroid:"<<centroid.x<<" "<<centroid.y; 

            distRatio = (float) ( ((frame.cols/2.0) -centroid.x) / (frame.cols/2.0) );
            std::cout<<"\n Distance "<<distRatio;
            angularPosition = distRatio * 33;
            std::cout<<" Angle="<<angularPosition; 


        }
        else
        {
            std::cout<<"\n No contours";

        }

     // std::cout<<"\n Num Labels"<<numLabels;
        imshow("image", frame);
        //i
        mshow("foreground mask", fgMask);
        //imshow("foreground image", fgImg);
        //labels.convertTo(output, CV_8U);
        imshow("Labels", output);
        if(!bgimg.empty())
          imshow("mean background image", bgimg );
        //vwMask << fgMask;
        //vwForeground << fgImg;
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


        
    }

    imwrite("bgImg.jpeg", bgimg);
    return 0;

 }