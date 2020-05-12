#include "ofMain.h"
#include "ofApp.h"

#define X_WINDOW 1280
#define Y_WINDOW 720


//===========================================================================================
//*
//*		@mainpage	Synsitive project (compatible with Linux and Windows)
//*		An OpenBCI project easily understanding by non-technical users to analyze brain parts with an EEG.
//*
//*		@file main.cpp
//*
//*		@author	COLLART Quentin, PEREZ-LACOSTE Mallaury, MANTUILA Herlyne, LETHU Florian
//*
//*		Creation date : 23-02-2020
//*		Last modification : 12-05-2020
//*
//===========================================================================================


int main(int argc, char **argv){

	ofSetupOpenGL(X_WINDOW, Y_WINDOW, OF_WINDOW);
	ofRunApp(new ofApp());

	return 0;
}
