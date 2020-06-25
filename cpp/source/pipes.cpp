#include<iostream>
#include<jetson-utils/commandLine.h>
#include<jetson-utils/gstCamera.h>
#include<jetson-utils/glDisplay.h>
//#include"boilerplate.h"
#include"TestClass.h"
//#include"start.h"

using namespace std;

int usage(){

	printf("Improper format. Usage:\n");
	printf("pipes [--network NETWORK] [--camera CAMERA]\n");
        printf("	[--width WIDTH] [--height HEIGHT]\n\n");
	printf("Run an image processing pipeline on a camera stream.\n\n");
	printf("Options:");
	printf("	--help			Show this message.\n");
	printf("	--network 	NETWORK	Pre-trained network to load.\n");
	printf("	--camera 	CAMERA	e.g. for VL42 cameras, the /dev/video device to use.\n");
	printf("	--width		INT	Width of stream (default 1280).\n");
	printf("	--height	INT	Height of stream (default 720).\n");
	printf("");
	return 0;
}

int main(int argc, char** argv){

	commandLine cmdl(argc, argv);

	if (cmdl.GetFlag("help")) 
		return usage();

	char* nptr = 0;

	// Testing
	printf("entering main\n");

	// Create camera object
        gstCamera* camera = gstCamera::Create(
                        cmdl.GetInt("width", gstCamera::DefaultWidth),
                        cmdl.GetInt("height", gstCamera::DefaultHeight),
                        cmdl.GetString("camera"));

        if(!camera) 
        { 
                printf("Pipes: failed to initialize camera.\n"); 
                return -1; 
        } 
 
	// Testing
        printf("Pipes:  successfully initialized camera.\n"); 
        printf("    width:  %u\n", camera->GetWidth()); 
        printf("   height:  %u\n", camera->GetHeight()); 
        printf("    depth:  %u (bpp)\n\n", camera->GetPixelDepth()); 
 
	// Create display window
	glDisplay* display = glDisplay::Create();

	// Open camera
        if(!camera->Open()) 
        { 
                printf("Pipes: camera failed to open.\n"); 
                return -2; 
        } 

//	TestClass feed;
//	feed.testCall();

	SAFE_DELETE(camera);
	SAFE_DELETE(display);

	return 0;
}
