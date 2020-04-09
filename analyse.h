// analyse.h
#pragma once
using namespace std;

#include <complex>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>

#include "ofSerial.h"

//MACROS
#define NB_SAMPLES 256
#define CELLS 8
#define STEP 0.5
#define MIN_FREQ 0
#define MAX_FREQ 60

#define STEP_TIME 0.2
#define T_MAX 3.0
#define T_MIN 0.05
#define NB_DATA (int)(T_MAX / STEP_TIME)


#define PI 3.14159265
#define OPENBCI_STREAM_START 'b'

typedef unsigned char Byte;


class CAnalyse
{
	//Members
	protected:
		vector<vector<float>> m_datasEntrantes;
		vector<vector<float>> m_datasFenetrees;
		vector<vector<float>> m_datasAnalysees;
		vector<Byte> acquisition;

		unsigned int m_nbCapteurs;
		unsigned int m_nbEchantillons;
		float m_pas;
		float m_minFrequence;
		float m_maxFrequence;

		int m_dataRecState; //Current receive state

	//Methods
	public:
		CAnalyse();
		void init(int nbCapteurs, int nbEchantillons, float pas, float minFrequence, float maxFrequence);
		virtual ~CAnalyse();

	//Autotest
	#ifdef AUTOTEST_ANALYSE
		int autotest();
	#endif

		//Getters
		vector<vector<float>>& getDatasEntrantes();
		vector<vector<float>>& getDatasFenetrees();
		vector<vector<float>>& getDatasAnalysees();
		int getNbCapteurs();
		int getNbEchantillons();
		float getPas();
		float getMinFrequence();
		float getMaxFrequence();

		//Setters
		void setDatasEntrantes(vector<vector<float>> tableau);
		void setDatasFenetrees(vector<vector<float>> tableau);
		void setDatasFrequentilles(vector<vector<float>> tableau);
		void setNbCapteurs(int nbCapteurs);
		void setNbEchantillons(int nbEchantillons);
		void setPas(float pas);
		void setMinFrequence(float minFrequence);
		void setMaxFrequence(float maxFrequence);

		//Acquisition des données
		void acquisitionDatas(ofSerial & p_serial);
		//Fonctions de fenetrage
		void fenetrageDatas(string modeFenetrage);
		void rectangulaire();
		void hann();
		void hamming();
		void demiSinus();
		void barlett();

		//FFT
		void analyseFrequentielle();
		void fft(float frequence, int capteur);
};