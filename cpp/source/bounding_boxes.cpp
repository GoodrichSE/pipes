/*
Displays detected objects with bounding boxes and some other aspects on an openGL window.
*/

// C/++ libraries
#include<iostream>
#include<signal.h>

// nVidia shared libraries
#include<commandLine.h>
#include<gstCamera.h>
#include<glDisplay.h>
#include<detectNet.h>

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
	//printf("entering main\n");
	printf("argc: %i\nargv: %s\n", argc, *argv);

	// Create classification network
	detectNet* detNet = detectNet::Create(argc, argv);

	if(!detNet)
	{
		printf("Pipes: failed to initialize imageNet\n");
		return -1;
	}

	const uint32_t overlayFlags = detectNet::OverlayFlagsFromStr(cmdl.GetString("overlay", "box,label,conf"));

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

	// Continuously capture camera feed
	while(!signal_received)
	{

		float* imgRGBA = NULL;
		detectNet::Detection* det = NULL;
		float* center_x = NULL;
		float* center_y = NULL;

		// Capture image
		if(!camera->CaptureRGBA(&imgRGBA, 1000))
			printf("\nPipes: lost RGBA frame\n");
		
		// TODO: Learn more about the overload that uses Detection**
		const uint32_t obj = detNet->Detect( imgRGBA, camera->GetWidth(), camera->GetHeight(), det, overlayFlags );

		// TODO: Retrieve error message from error code
		if( obj < 0)
		{
			printf("Pipes: Error detecting object. %i", obj);
			return -3;
		}

		if(obj > 0)
		{
			printf("Pipes: Detected object as %s.\n", detNet->GetClassDesc(obj));
			det->Center(center_x,center_y);
			printf("Bounding box features: \nCenter (%f, %f) \nLeft %f \nRight %f \nTop %f \nBottom %f",
				center_x,center_y,
				det->Left,
				det->Right,
				det->Top,
				det->Bottom );
	
		}
		// Update display
		if(display != NULL)
		{
			

			// Using nVidia's glDisplay object
			display->RenderOnce( (float*)imgRGBA, camera->GetWidth(), camera->GetHeight() );

			// Check for user break signal
			if(display->IsClosed())
				signal_received = true;
		}
	}


//	TestClass feed;
//	feed.testCall();

	SAFE_DELETE(camera);
	SAFE_DELETE(display);

	return 0;
}
