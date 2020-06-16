//Windows OS includes
#ifdef _WIN32
	#include <vector>
	#include <string>
	#include <iostream>
	#include <complex>
	#include <fstream>
	#include <chrono>
	#include <ctime>
	#include <math.h>
	#include "ofSerial.h"
//Else, Linux OS includes
#else
	#include <vector>
	#include <string>
	#include <iostream>
	#include <fstream>
	#include <complex>
	#include <chrono>
	#include <ctime>
	#include <math.h>
#endif

using namespace std;

#define PI 3.14159265

//Macros
#define NB_SAMPLES 512
#define CELLS 8
#define STEP 1
#define MIN_FREQ 0
#define MAX_FREQ 60

#define STEP_TIME 0.4
#define T_MAX 4.0
#define T_MIN 0.1
#define NB_DATA (int)(T_MAX / STEP_TIME)

typedef unsigned char Byte;
class CAnalyse
{
	//Membres
protected:
	vector<vector<float>> m_datasEntrantes;
	vector<vector<float>> m_datasFenetrees;
	vector<vector<float>> m_datasAnalysees;
	

	int m_nbCapteurs;
	int m_nbEchantillons;
	float m_pas;
	float m_minFrequence;
	float m_maxFrequence;

	//M�thodes
public:
	CAnalyse();
	CAnalyse(int nbCapteurs, int nbEchantillons, float pas, float minFrequence, float maxFrequence);
	virtual ~CAnalyse();

	//Getters
	vector<vector<float>> getDatasEntrantes();
	vector<vector<float>> getDatasFenetrees();
	vector<vector<float>> getDatasAnalysees();
	int getNbCapteurs();
	int getNbEchantillons();
	float getPas();
	float getMinFrequence();
	float getMaxFrequence();
	int m_dataRecState;

	//Setters
	void setDatasEntrantes(vector<vector<float>> tableau);
	void setDatasFenetrees(vector<vector<float>> tableau);
	void setDatasFrequentilles(vector<vector<float>> tableau);
	void setNbCapteurs(int nbCapteurs);
	void setNbEchantillons(int nbEchantillons);
	void setPas(float pas);
	void setMinFrequence(float minFrequence);
	void setMaxFrequence(float maxFrequence);

	//Acquisition des donn�es
	//void acquisitionDatas(ofSerial& p_serial);
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

	//M�thode g�n�rale d'appel
	void general(string fenetrage);

	//Fonction de g�n�rations de sinuosidales
	//Elle s'utilise en remplacement de la fonction acquisitionDatas()
	void genSinuo();
	void genSinuoT();
	void genSinuoT(int Frequence);


};