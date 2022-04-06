// C/++ libraries
#include<iostream>
#include<signal.h>

// nVidia shared libraries
#include<commandLine.h>
#include<gstCamera.h>
#include<glDisplay.h>
#include<detectNet.h>

// Libraries built by this package

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

//TODO: Verify
int usage(){

	printf("Improper format. Usage:\n");
	printf("pipes [--network NETWORK] [--camera CAMERA]\n");
        printf("	[--width WIDTH] [--height HEIGHT]\n\n");
	printf("Run an image processing pipeline on a camera stream.\n\n");
	printf("Options:");
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
	//printf("argc: %i\nargv: %s\n", argc, *argv);

	// Create classification network
	detectNet* detNet = detectNet::Create(argc, argv);

	if(!detNet)
	{
		printf("Pipes: failed to initialize detNet\n");
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
		
		// This is const in the Jetson library. Why?
		int img_class = imgNet->Classify(imgRGBA, camera->GetWidth(), camera->GetHeight());

		if(img_class != NULL)
			printf("Pipes: Classified image as %s.\n", imgNet->GetClassDesc(img_class));
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

	SAFE_DELETE(camera);
	SAFE_DELETE(display);

	return 0;
}
