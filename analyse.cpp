// analyse.cpp


#define _USE_MATH_DEFINES
#include <math.h>
#include <cstring>
#include <iomanip>

//To comment or uncomment for test
//#define AUTOTEST_ANALYSE

#include "analyse.h"


//=========================================================================
// Board documentation: 
//   https://docs.openbci.com/docs/02Cyton/CytonDataFormat
//   https://docs.openbci.com/docs/02Cyton/CytonLanding
//=========================================================================



#ifdef AUTOTEST_ANALYSE
int CAnalyse::autotest() {
	static bool l_inAutotest = false;
	if (l_inAutotest == true) return 0;
	l_inAutotest = true;

	CAnalyse test;

	test.init(CELLS, NB_SAMPLES, STEP, MIN_FREQ, MAX_FREQ);
	vector<vector<float>> re;
	vector<float> ah;
	for (float i = 0; i < NB_SAMPLES; i++)
	{
		ah.push_back(sin(2 * M_PI * i *(10.0/NB_SAMPLES) )   + sin(2 * M_PI * i * (15.0/NB_SAMPLES))); //sinus 10 Hz et 15HZ
	}
	for (int i = 0; i < CELLS; i++)
	{
		re.push_back(ah);
	}

	test.setNbEchantillons(NB_SAMPLES);
	test.setNbCapteurs(1);
	test.setDatasEntrantes(re);
	test.fenetrageDatas("hamming");
	test.analyseFrequentielle();

	return 1;
}
#endif


//Constructeurs et desctructeurs //Bon
CAnalyse::CAnalyse()
{
#ifdef AUTOTEST_ANALYSE
	autotest();
#endif

}

//#todo AREVOIR ajouter la notion de frequence d'échantillonnage
void CAnalyse::init(int nbCapteurs, int nbEchantillons, float pas, float minFrequence, float maxFrequence)
{
	m_nbCapteurs = nbCapteurs;
	m_nbEchantillons = nbEchantillons; //Doit être une puissance de 2
	m_pas = pas;
	m_minFrequence = minFrequence;
	m_maxFrequence = maxFrequence;
	m_dataRecState = 0; //default state ==> waiting for starting byte

	m_datasEntrantes.clear();
	m_datasFenetrees.clear();
	m_datasAnalysees.clear();

	vector<vector<float>> re;
	vector<float> ah;

	//initialisation des valeurs du tableau à 0
	for (unsigned int i = 0; i < m_nbCapteurs; i++)
	{
	
		vector<float> tmp;
		for (float i = 0; i < NB_SAMPLES; i++)
		{
			tmp.push_back(0.5);
		}
		m_datasEntrantes.push_back(tmp);
		m_datasFenetrees.push_back(tmp);
		m_datasAnalysees.push_back(tmp);
	}

}
CAnalyse::~CAnalyse()
{
}

//Getters //Bon
vector<vector<float>>& CAnalyse::getDatasEntrantes()
{
	return m_datasEntrantes;
}
vector<vector<float>>& CAnalyse::getDatasFenetrees()
{
	return m_datasFenetrees;
}
vector<vector<float>>& CAnalyse::getDatasAnalysees()
{
	return m_datasAnalysees;
}
int CAnalyse::getNbCapteurs()
{
	return m_nbCapteurs;
}
int CAnalyse::getNbEchantillons()
{
	return m_nbEchantillons;
}
float CAnalyse::getPas()
{
	return m_pas;
}
float CAnalyse::getMinFrequence()
{
	return m_minFrequence;
}
float CAnalyse::getMaxFrequence()
{
	return m_maxFrequence;
}

//Setters //Bon
//Faire attention à la taille des tableaux par rapport au nombre de capteurs
void  CAnalyse::setDatasEntrantes(vector<vector<float>> tableau)
{
	m_datasEntrantes = tableau;
	return;
}
void  CAnalyse::setDatasFenetrees(vector<vector<float>> tableau)
{
	m_datasFenetrees = tableau;
	return;
}
void  CAnalyse::setDatasFrequentilles(vector<vector<float>> tableau)
{
	m_datasAnalysees = tableau;
	return;
}
void  CAnalyse::setNbCapteurs(int nbCapteurs)
{
	m_nbCapteurs = nbCapteurs;
	return;
}
void  CAnalyse::setNbEchantillons(int nbEchantillons)
{
	m_nbEchantillons = nbEchantillons;
	return;
}
void  CAnalyse::setPas(float pas)
{
	m_pas = pas;
	return;
}
void  CAnalyse::setMinFrequence(float minFrequence)
{
	m_minFrequence = minFrequence;
	return;
}
void  CAnalyse::setMaxFrequence(float maxFrequence)
{
	m_maxFrequence = maxFrequence;
	return;
}


void CAnalyse::acquisitionDatas(ofSerial & p_serial)
{
	
	int data; //new receive byte 
	
	//Read a new receive byte
	data = p_serial.readByte();
	//While data available
	while (data != OF_SERIAL_NO_DATA && data != OF_SERIAL_ERROR)
	{
		if (m_dataRecState == 0)
		{
			//first byte synchronisation 
			if (data == 0xA0) m_dataRecState = 1;
		}
		else
		{
			m_dataRecState++;

			if (m_dataRecState == 2)
			{
				//data=> sample number
			}
			else if (m_dataRecState <= 26)
			{
				//EEG Data
				acquisition.push_back(data);
			}
			else if (m_dataRecState <= 32)
			{
				//Aux data
			}
			else //byte 33
			{
				if ((data & 0xC0) == 0xC0)
				{

					/*if (acquisition.size() != 24)
					{
						int a;
						a = 1;
					}*/

					//analyse data ......
					//#TODO

					/*init des variables pour stocker les data*/
					int newInt[8];
					for (int i = 0; i < 8; i++)
						newInt[i] = 0;
						
					/*conversion des datas en int*/
					newInt[0] = (
						((0xFF & acquisition[0]) << 16) |
						((0xFF & acquisition[1]) << 8) |
						(0xFF & acquisition[2])
						);
					if ((newInt[0] & 0x00800000) > 0) {
						newInt[0] |= 0xFF000000; //nombre négatif
					}
					else {
						newInt[0] &= 0x00FFFFFF; //nombre positif
					}

					newInt[1] = (
						((0xFF & acquisition[0]) << 16) |
						((0xFF & acquisition[1]) << 8) |
						(0xFF & acquisition[2])
						);
					if ((newInt[1] & 0x00800000) > 0) {
						newInt[1] |= 0xFF000000; //nombre négatif
					}
					else {
						newInt[1] &= 0x00FFFFFF; //nombre positif
					}

					newInt[2] = (
						((0xFF & acquisition[0]) << 16) |
						((0xFF & acquisition[1]) << 8) |
						(0xFF & acquisition[2])
						);
					if ((newInt[2] & 0x00800000) > 0) {
						newInt[2] |= 0xFF000000; //nombre négatif
					}
					else {
						newInt[2] &= 0x00FFFFFF; //nombre positif
					}

					newInt[3] = (
						((0xFF & acquisition[0]) << 16) |
						((0xFF & acquisition[1]) << 8) |
						(0xFF & acquisition[2])
						);
					if ((newInt[3] & 0x00800000) > 0) {
						newInt[3] |= 0xFF000000; //nombre négatif
					}
					else {
						newInt[3] &= 0x00FFFFFF; //nombre positif
					}

					newInt[4] = (
						((0xFF & acquisition[0]) << 16) |
						((0xFF & acquisition[1]) << 8) |
						(0xFF & acquisition[2])
						);
					if ((newInt[4] & 0x00800000) > 0) {
						newInt[4] |= 0xFF000000; //nombre négatif
					}
					else {
						newInt[4] &= 0x00FFFFFF; //nombre positif
					}

					newInt[5] = (
						((0xFF & acquisition[0]) << 16) |
						((0xFF & acquisition[1]) << 8) |
						(0xFF & acquisition[2])
						);
					if ((newInt[5] & 0x00800000) > 0) {
						newInt[5] |= 0xFF000000; //nombre négatif
					}
					else {
						newInt[5] &= 0x00FFFFFF; //nombre positif
					}

					newInt[6] = (
						((0xFF & acquisition[0]) << 16) |
						((0xFF & acquisition[1]) << 8) |
						(0xFF & acquisition[2])
						);
					if ((newInt[6] & 0x00800000) > 0) {
						newInt[6] |= 0xFF000000; //nombre négatif
					}
					else {
						newInt[6] &= 0x00FFFFFF; //nombre positif
					}

					newInt[7] = (
						((0xFF & acquisition[0]) << 16) |
						((0xFF & acquisition[1]) << 8) |
						(0xFF & acquisition[2])
						);
					if ((newInt[7] & 0x00800000) > 0) {
						newInt[7] |= 0xFF000000; //nombre négatif
					}
					else {
						newInt[7] &= 0x00FFFFFF; //nombre positif
					}

					/*envoi des données aux 8 capteurs */
					for (int i = 0; i < 8; i++)
					{
						m_datasEntrantes[i].push_back(newInt[i]); //capteur1
						m_datasEntrantes[i].erase(m_datasEntrantes[i].begin());
					}
		
					//cout << "*";
					//#TODO
				}
				acquisition.clear();
				//Reset receive state
				m_dataRecState = 0;
			}				
		}
		//Read next byte
		data = p_serial.readByte();
	}

	//#TODO a supprimer
	return;


	if (data == OF_SERIAL_NO_DATA)
	{
		printf("\nno data was read");
	}
	else if (data == OF_SERIAL_ERROR)
	{
		printf("\nan error occurred");
	}
	else 
	{
		for (int i = 0; i < 33; i++)
		{
			if (i > 2 && i < 27)
			{
				acquisition.push_back(data);
			}
		}
		for (int i = 0; i < 8; i++)
		{
			int newInt = (
				((0xFF & acquisition[i * 3]) << 16) |
				((0xFF & acquisition[(i * 3) + 1]) << 8) |
				(0xFF & acquisition[(i * 3) + 2])
				);
			if ((newInt & 0x00800000) > 0) {
				newInt |= 0xFF000000;
			}
			else {
				newInt &= 0x00FFFFFF;
			}
			m_datasEntrantes[i].push_back(newInt);

			//Si plus grand que le nombre d'échantillons
			while (m_datasEntrantes[i].size() > m_nbEchantillons)
			{
				m_datasAnalysees.erase(m_datasAnalysees.begin());
			}
		}

	}

	return;
}



//Fonctions de fenetrage
void CAnalyse::fenetrageDatas(string modeFenetrage)
{
	if (modeFenetrage == "rectangulaire")
	{
		rectangulaire();
		cout << "rectangulaire";
	}
	else if (modeFenetrage == "hann")
	{
		hann();

	}
	else if (modeFenetrage == "hamming")
	{
		hamming();
	}
	else if (modeFenetrage == "demiSinus")
	{
		demiSinus();
	}
	else if (modeFenetrage == "barlett")
	{
		barlett();
	}
	else
	{
		cout << "Ce fenetrage n'existe pas" << endl;
	}

	//Si plus grand que le nombre d'échantillons
	for (int i = 0; i < 8; i++)
	{
		while (m_datasFenetrees[i].size() > m_nbEchantillons)
		{
			m_datasFenetrees.erase(m_datasAnalysees.begin());
		}
	}

	return;
}
void CAnalyse::rectangulaire()
{
	vector<float>::iterator it;
	for (unsigned int i = 0; i < m_nbCapteurs; i++)
	{
		for (it = m_datasEntrantes[i].begin(); it != m_datasEntrantes[i].end(); it++)
		{
			m_datasFenetrees[i].push_back(*it);
		}
	}
	return;
}
void CAnalyse::hann()
{
	vector<float>::iterator it;
	for (unsigned int i = 0; i < m_nbCapteurs; i++)
	{
		float t = 0;
		for (it = m_datasEntrantes[i].begin(); it != m_datasEntrantes[i].end(); it++)
		{
			m_datasFenetrees[i].push_back((float)(*it * (0.5 * (1 - cos(2 * M_PI * t / m_nbEchantillons)))));
			t++;
		}
	}
	return;
}
void CAnalyse::hamming()
{
	vector<float>::iterator it;
	for (unsigned int i = 0; i < m_nbCapteurs; i++)
	{
		float t = 0;
		for (it = m_datasEntrantes[i].begin(); it != m_datasEntrantes[i].end(); it++)
		{
			m_datasFenetrees[i].push_back((float)(*it * (0.54 - 0.46 * cos(2 * M_PI * t / m_nbEchantillons))));
			t++;
		}
	}
	return;
}
void CAnalyse::demiSinus()
{
	vector<float>::iterator it;
	for (unsigned int i = 0; i < m_nbCapteurs; i++)
	{
		float t = 0;
		for (it = m_datasEntrantes[i].begin(); it != m_datasEntrantes[i].end(); it++)
		{
			m_datasFenetrees[i].push_back(*it * ((float)sin(M_PI * t / m_nbEchantillons)));
			t++;
		}
	}
	return;
}
void CAnalyse::barlett()
{
	vector<float>::iterator it;
	for (unsigned int i = 0; i < m_nbCapteurs; i++)
	{
		float t = 0;
		for (it = m_datasEntrantes[i].begin(); it != m_datasEntrantes[i].end(); it++)
		{
			if (t < m_nbEchantillons / 2)
			{
				m_datasFenetrees[i].push_back(*it * (2 * t / m_nbEchantillons));
			}
			else
			{
				m_datasFenetrees[i].push_back(*it * (2 - (2 * t / m_nbEchantillons)));
			}
			t++;
		}
	}
	return;
}


//FFT
void CAnalyse::analyseFrequentielle()
{
	for (unsigned int i = 0; i < m_nbCapteurs; i++)
	{
		m_datasAnalysees[i].clear();
	}
	
	for (unsigned int c = 0; c < m_nbCapteurs; c++)
	{
		for (float f = m_minFrequence; f < m_maxFrequence ; f += m_pas)
		{
			fft(f,c);
		}
	}

	//Si la taille du tableau est trop grande
	for (unsigned int i = 0; i < CELLS; i++)
	{
		while (m_datasAnalysees.size() > m_nbEchantillons)
		{
			m_datasAnalysees.erase(m_datasFenetrees.begin());
		}
	}
	return;
}

void CAnalyse::fft(float frequence, int capteur)
{
	complex<float> result1(0, 0);
	complex<float> result2(0, 0);
	complex<float> Wn(0, 0);

	complex<float> resulttmp(0, 0);
	auto I = 1i;
	
	for (unsigned int i = 0; i < (m_nbEchantillons/2)-1; i++)
	{
		resulttmp =  exp(I*(-2*PI*frequence*2*i/m_nbEchantillons));
		resulttmp *= m_datasFenetrees[capteur][2 * i];
		result1 += resulttmp;
	}
	result1 /= m_nbEchantillons;

	for (unsigned int i = 0; i < (m_nbEchantillons / 2) - 1; i++)
	{
		resulttmp = exp(I * (-2 * PI * frequence * 2 * i / m_nbEchantillons));
		resulttmp *= m_datasFenetrees[capteur][2 * i+1];
		result2 += resulttmp;
	}
	result2 /= m_nbEchantillons;

	Wn = exp(I * (-2 * PI * frequence / m_nbEchantillons));

	result1 = result1 + Wn * result2;
	m_datasAnalysees[capteur].push_back(sqrt(real(result1)* real(result1)+imag(result1)* imag(result1)));
	//cout <<  sqrt(real(result1) * real(result1) + imag(result1) * imag(result1)) << endl;

	return;
}