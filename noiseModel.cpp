/************************************
 *Code for noise Modelling
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
    if(argc!=2)
    {
        std::cout<<"\n Usage: ./noisemodel Path to video";
        return -1;
    }
    Mat frame, gray, sum, sumSq, noiseMean, noiseVar;
    FileStorage fs("test.xml", FileStorage::WRITE);
    VideoCapture cap(argv[1]);
    if(!cap.isOpened())
    {
        std::cout<<"\n Video open failed!";
        return -1;
    }

    Mat temp;
    int count = 1; //Meant to count frames
    while(1)
    {
        cap >> frame;

        cvtColor(frame, gray, COLOR_BGR2GRAY);
        temp.create(frame.rows, frame.cols, CV_32FC1);
        if(count == 1)
        {
            sum.create(gray.rows, gray.cols, CV_32FC1);
            sumSq.create(gray.rows, gray.cols, CV_32FC1);
            noiseMean = Mat::zeros(gray.rows, gray.cols, CV_32FC1);
            noiseVar = Mat::zeros(gray.rows, gray.cols, CV_32FC1);
            // Mat temp;
            // temp.create(gray.rows, gray.cols, CV_32FC1);
            gray.convertTo(temp, CV_32F, 1, 0);
            temp /= 255.0;
            temp.copyTo(sum);

            
        }

        else
        {
            
            gray.convertTo(temp, CV_32F, 1, 0);
            temp = temp/255.0;
            sum += temp;
            noiseMean = sum / count;
            
        }

        count++;
        fs<<"test"<<noiseMean;
        normalize(noiseMean,noiseMean,0,1,NORM_MINMAX);


        imshow("Mean", noiseMean);
        imshow("Video", gray);

        if(waitKey(60) == 27)
            break;
    }

    fs.release();
    return 0;
    
}