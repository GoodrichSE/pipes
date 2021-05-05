// C/++ libraries
#include<iostream>
#include<signal.h>

// nVidia shared libraries
#include<commandLine.h>
#include<gstCamera.h>
#include<glDisplay.h>
#include<imageNet.h>

// Libraries built by this package
#include"../lib/TestClass.h"

using namespace std;

bool signal_received = false; // User control flag

// Signal handler for kill command from user
void signal_handler(int signo)
{
	if( signo == SIGINT ) 
	{
		printf("\nreceived SIGINT\n");
		signal_received = true;
	}
}

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
	printf("%s\n", detectNet::Usage());
	return 0;
}

int main(int argc, char** argv){

	commandLine cmdl(argc, argv);

	// Capture user command line args
	if (cmdl.GetFlag("help")) 
		return usage();

	// Attach handler for user input signal
	if(signal(SIGINT, signal_handler) == SIG_ERR)
		printf("\nSignal handler error.\n");

	// Testing
	printf("entering stream_objects main\n");
	printf("argc: %i\nargv: %s\n", argc, *argv);

	// Create classification network
	// TODO: Make sure this CLI args create will work for detectNets here
	detectNet* detNet = detectNet::Create(argc, argv);

	if(!detNet)
	{
		printf("Pipes: failed to initialize detectNet\n");
		return -1;
	}

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

	// Open camera
        if(!camera->Open()) 
        { 
                printf("Pipes: camera failed to open.\n"); 
                return -2; 
        } 

	// Continuously capture camera feed
	while(!signal_received)
	{

		float* imgRGBA;

		// Capture image
		if(!camera->CaptureRGBA(&imgRGBA, 1000))
			printf("\nPipes: lost RGBA frame\n");

		// TODO: Maybe allocate storage vector for detections. If so, use GetMaxDetections
		
		// This is const in the Jetson library. Why?
		// TODO: Fix signature. Detections might be pointer passed in, or an empty list. Can accept Detection** or Detection*
		// TODO: Check if this has any return value. Might just use Detections storage 
		int img_classes = detNet->Detect(imgRGBA, camera->GetWidth(), camera->GetHeight(), detNet->getDetections(), detectNet.OVERLAY_NONE);

		// detNet->sortDetections
		if(img_classes != NULL)
			printf("Pipes: Number of classified objects is %s.\n", detNet->GetNumClasses());
		if(img_classes != NULL)
		{
			// TODO: loop over objects with GetClassDesc for i<GetNumClasses. Or, there's probably something internal to do the same. mClassDesc?
			// printf("Pipes: Object detected as %s.\n", detNet->
		}
	}


//	TestClass feed;
//	feed.testCall();

	SAFE_DELETE(camera);

	return 0;
}
