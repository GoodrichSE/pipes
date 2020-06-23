#include<iostream>
#include"boilerplate.h"

// Constructor for a shell class
Boilerplate::Boilerplate(){
}

// Destructor
Boilerplate::~Boilerplate(){
}

int Boilerplate::argoption()
{
	printf("Option called.\n");

	return 0;

}

int Boilerplate::call()
{
	printf("Boilerplate called correctly.\n");

	return 0;
}
