#include<iostream>
#include<jetson-utils>

int usage()
{
	printf("Testing boilerplate code.");
	printf("usage: boilerplate [options]");
	printf("optional arguments:");
	printf("	-a	alternate message");

	return 0;
}

int main(int argc, char** argv)
{
	commandLine cmdl(argc, argv);

	if(cmdl.GetFlag("help"))
		return usage();

	printf("Boilerplate called correctly.\n");

	return 0;
}
