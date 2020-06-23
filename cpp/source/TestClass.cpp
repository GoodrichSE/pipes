//#include<jetson-utils/commandLine.h>
#include<jetson-utils/gstCamera.h>
#include"TestClass.h"
// Probably need to change cmake settings to get this to link properly
//#include"commandLine.h"
//#include<jetson-utils>

//int TestClass::camfeed( int argc, char** argv )
TestClass::TestClass()
{
	printf("TestClass constructed.\n");
}

TestClass::~TestClass()
{
}

bool TestClass::testCall()
{

	printf("Function called successfully.\n");

	return 0;
}
