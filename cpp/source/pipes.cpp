#include<iostream>
#include<signal.h>

#include<commandLine.h>
#include<gstCamera.h>
#include<glDisplay.h>

#include"TestClass.h"

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

	// Continuously capture camera feed
	while(!signal_received)
	{

		float* imgRGBA;

		// Capture image
		if(!camera->CaptureRGBA(&imgRGBA, 1000))
			printf("\nPipes: lost RGBA frame\n");

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
