#include<iostream>
#include<commandLine.h>

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

	if(cmdl.argc != 1 && cmdl.argc != 2)
		return usage();

	if(cmdl.GetFlag == "option")
		return option;

	printf("Boilerplate called correctly.\n");

	return 0;
}

int option()
{
	printf("Option called.");

	return 0;
}
