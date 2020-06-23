//#include<jetson-utils/commandLine.h>
#include<jetson-utils/gstCamera.h>
#include"Camfeed.h"
// Probably need to change cmake settings to get this to link properly
//#include"commandLine.h"
//#include<jetson-utils>

//int Camfeed::camfeed( int argc, char** argv )
Camfeed::Camfeed()
{
	printf("Camfeed constructed.\n");
}

Camfeed::~Camfeed()
{
}

bool Camfeed::testCall()
{

	printf("Function called successfully.\n");

	return 0;
}
