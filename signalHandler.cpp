/*******************************************************************
 * Code to handle Ctrl-C without crashing camera application
 *******************************************************************/


#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/opencv_modules.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stdio.h>
#include <signal.h>


volatile int quit_signal = 0;
#ifdef __unix__

extern "C" void quit_signal_handler(int signum)
{
	if(quit_signal!=0)
		exit(0);
	quit_signal = 1;
	printf("\n Camera shut down");
}
#endif



