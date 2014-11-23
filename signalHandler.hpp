/***************************quit_signal_handler header************************************/


#ifndef __SIGNALHANDLER_H__
#define __SIGNALHANDLER_H__

#include <signal.h>
#include <stdio.h>

extern "C"
{
	void quit_signal_handler(int);
}

#endif // End of signal_handler
