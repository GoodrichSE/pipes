#include<iostream>
#include<jetson-utils/commandLine.h>
#include<jetson-utils/gstCamera.h>
//#include"boilerplate.h"
#include"TestClass.h"
//#include"start.h"

using namespace std;

int usage(){

	return 0;
}

int main(int argc, char** argv){

	commandLine cmdl(argc, argv);

	if (cmdl.GetFlag("help")) 
		return usage();

	char* nptr = 0;

	printf("entering main\n");

        gstCamera* camera = gstCamera::Create(
                        cmdl.GetInt("width", gstCamera::DefaultWidth),
                        cmdl.GetInt("height", gstCamera::DefaultHeight),
                        cmdl.GetString("camera"));

        if(!camera) 
        { 
                printf("TestClass: failed to initialize camera.\n"); 
                return -1; 
        } 
 
        printf("TestClass:  successfully initialized camera.\n"); 
        printf("    width:  %u\n", camera->GetWidth()); 
        printf("   height:  %u\n", camera->GetHeight()); 
        printf("    depth:  %u (bpp)\n\n", camera->GetPixelDepth()); 
 
 
        if(!camera->Open()) 
        { 
                printf("TestClass: camera failed to open.\n"); 
                return -2; 
        } 
	

//	TestClass feed = TestClass();
	TestClass feed;
//	TestClass feed();
	feed.testCall();
//	bool success = feed.testWindow();

	return 0;
}
