/**
 *
 * 	@file		ofApp.h
 * 	@author		COLLART Quentin, PEREZ-LACOSTE Mallaury, MANTUILA Herlyne, LETHU Florian
 *
 *	@details	openFramework documentation : https://openframeworks.cc/documentation/
 *
 */


#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <fstream>
#include <time.h>

#include "ofMain.h"
#include "analyse.h"
#include "server.h"

class ofApp : public ofBaseApp {

	protected:
		ofSerial m_serial;  // serial communication
		CAnalyse m_analyse; // data analysis

	public:

		void setup();
		void update();
		void draw();
		
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		int analyse();

		//Fonts
		ofTrueTypeFont main_font;
		ofTrueTypeFont agressive_font;
		ofTrueTypeFont main_font_bigger;

		//Pictures
		ofImage logosynsitive;
		ofImage logoopenbci;
		ofImage img;
		ofImage buttonsave;
		ofImage buttonscreenshot;
		ofImage brain;
		ofImage axe_hz_t;
		ofImage axe_v_hz;
		ofImage more_less;
		ofImage electrode;

		float freq_min;
		float freq_max;
		float step;
		int nb_echant;

		char signal_type = 1; //1 : Hz/t et 2 : V/Hz
		vector<float> brain_areas{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; //Brain areas
		vector<vector<float>> current_data_brut; //Brut datas (input)
		vector<vector<float>> current_data; //Calculated with an FFT (output)

		vector<vector<vector<float>>> datas_to_print; //To store NB_DATA current_data tab

		float temps;
		clock_t t1, t2;
};
