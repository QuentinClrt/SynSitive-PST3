#include "CAnalyse.h"

int f = 0;

//Constructeurs et desctructeurs //Bon
CAnalyse::CAnalyse()
{
	m_nbCapteurs=CELLS;
	m_nbEchantillons=NB_SAMPLES; //Doit être une puissance de 2
	m_pas=STEP;
	m_minFrequence=MIN_FREQ;
	m_maxFrequence=MAX_FREQ;

}
CAnalyse::CAnalyse(int nbCapteurs, int nbEchantillons, float pas, float minFrequence, float maxFrequence)
{
	m_nbCapteurs = nbCapteurs;
	m_nbEchantillons = nbEchantillons; //Doit être une puissance de 2
	m_pas = pas;
	m_minFrequence = minFrequence;
	m_maxFrequence = maxFrequence;
}
CAnalyse::~CAnalyse()
{
}

//Getters //Bon
vector<vector<float>> CAnalyse::getDatasEntrantes()
{
	return m_datasEntrantes;
}
vector<vector<float>> CAnalyse::getDatasFenetrees()
{
	return m_datasFenetrees;
}
vector<vector<float>> CAnalyse::getDatasAnalysees()
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




//Acquisition des données
/*
void CAnalyse::acquisitionDatas(ofSerial& p_serial)
{
	int data; //new receive byte 
	vector<Byte> acquisition;

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

					//if (acquisition.size() != 24)
					//{
					//	int a;
					//	a = 1;
					//}

					//analyse data ......
					//#TODO

					//init des variables pour stocker les data
					int newInt[8];
					for (int i = 0; i < 8; i++)
						newInt[i] = 0;

					//transformation des data recues en int
					for (int i = 0; i < 8; i++)
					{
						newInt[i] = (
							((0xFF & acquisition[i * 3]) << 16) |
							((0xFF & acquisition[(i * 3) + 1]) << 8) |
							(0xFF & acquisition[(i * 3) + 2])
							);
						if ((newInt[i] & 0x00800000) > 0) {
							newInt[i] |= 0xFF000000;
						}
						else {
							newInt[i] &= 0x00FFFFFF;
						}
					}


					//envoi des données aux 8 capteurs + suppression de la première info si la chaine est plus grande que 256
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
}*/



//Fonctions de fenetrage
void CAnalyse::fenetrageDatas(string modeFenetrage)
{
	//Destruction du tableau précedent
	m_datasFenetrees.clear();

	//Création du tableau fenetrés
	//Si la tille du tableau précédent est inéfireieur alors cette taille sera la taille de ce tableau
	vector<float> tmp;
	if (m_datasEntrantes.size() < m_nbCapteurs)
	{
		for (short i = 0; i < m_datasEntrantes.size(); i++)
		{
			m_datasFenetrees.push_back(tmp);
		}
	}
	else
	{
		for (short i = 0; i < m_nbCapteurs; i++)
		{
			m_datasFenetrees.push_back(tmp);
		}
	}
	

	if (modeFenetrage == "rectangulaire")
	{
		rectangulaire();
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

	return;
}
void CAnalyse::rectangulaire()
{
	short taille = m_nbCapteurs;
	if (taille > m_datasFenetrees.size())
	{
		taille = m_datasFenetrees.size();
	}

	vector<float>::iterator it;
	for (int i = 0; i < taille; i++)
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
	short taille = m_nbCapteurs;
	if (taille > m_datasFenetrees.size())
	{
		taille = m_datasFenetrees.size();
	}

	vector<float>::iterator it;
	for (int i = 0; i < taille; i++)
	{
		float t = 0;
		for (it = m_datasEntrantes[i].begin(); it != m_datasEntrantes[i].end(); it++)
		{
			m_datasFenetrees[i].push_back((float)(*it * (0.5 * (1 - cos(2 * PI * t / m_nbEchantillons)))));
			t++;
		}
	}
	return;
}
void CAnalyse::hamming()
{
	short taille = m_nbCapteurs;
	if (taille > m_datasFenetrees.size())
	{
		taille = m_datasFenetrees.size();
	}

	vector<float>::iterator it;
	for (int i = 0; i < taille; i++)
	{
		float t = 0;
		for (it = m_datasEntrantes[i].begin(); it != m_datasEntrantes[i].end(); it++)
		{
			m_datasFenetrees[i].push_back((float)(*it * (0.54 - 0.46 * cos(2 * PI * t / m_nbEchantillons))));
			t++;
		}
	}
	return;
}
void CAnalyse::demiSinus()
{
	short taille = m_nbCapteurs;
	if (taille > m_datasFenetrees.size())
	{
		taille = m_datasFenetrees.size();
	}

	vector<float>::iterator it;
	for (int i = 0; i < taille; i++)
	{
		float t = 0;
		for (it = m_datasEntrantes[i].begin(); it != m_datasEntrantes[i].end(); it++)
		{
			m_datasFenetrees[i].push_back(*it * ((float)sin(PI * t / m_nbEchantillons)));
			t++;
		}
	}
	return;
}
void CAnalyse::barlett()
{
	short taille = m_nbCapteurs;
	if (taille > m_datasFenetrees.size())
	{
		taille = m_datasFenetrees.size();
	}

	vector<float>::iterator it;
	for (int i = 0; i < taille; i++)
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
	//Taille tableau suivant
	short taille=m_nbCapteurs;

	//Destruction du tableau précedent
	m_datasAnalysees.clear();

	//Création du tableau fenetrés
	//Si la tille du tableau précédent est inéfireieur alors cette taille sera la taille de ce tableau
	vector<float> tmp;
	if (m_datasFenetrees.size() < m_nbCapteurs)
	{
		for (short i = 0; i < m_datasFenetrees.size(); i++)
		{
			m_datasAnalysees.push_back(tmp);
		}
		taille = m_datasFenetrees.size();
	}
	else
	{
		for (short i = 0; i < m_nbCapteurs; i++)
		{
			m_datasAnalysees.push_back(tmp);
		}
	}
	
	for (int c = 0; c < taille; c++)
	{
		for (float f = m_minFrequence; f < m_maxFrequence ; f += m_pas)
		{
			fft(f,c);
		}
	}

	
}
void CAnalyse::fft(float frequence,int capteur)
{
	complex<float> result1(0, 0);
	complex<float> result2(0, 0);
	complex<float> Wn(0, 0);

	complex<float> resulttmp(0, 0);
	auto I = 1i;
	
	for (long unsigned int i = 0; i < (m_datasFenetrees[capteur].size()/2)-1; i++)
	{
		resulttmp =  exp(I*(-2*PI*frequence*2*i/ m_datasFenetrees[capteur].size()));
		resulttmp *= m_datasFenetrees[capteur][2 * i];
		result1 += resulttmp;
	}
	result1 /= m_datasFenetrees[capteur].size();

	for (long unsigned int i = 0; i < (m_datasFenetrees[capteur].size() / 2) - 1; i++)
	{
		resulttmp = exp(I * (-2 * PI * frequence * 2 * i / m_datasFenetrees[capteur].size()));
		resulttmp *= m_datasFenetrees[capteur][2 * i+1];
		result2 += resulttmp;
	}
	result2 /= m_datasFenetrees[capteur].size();

	Wn = exp(I * (-2 * PI * frequence / m_datasFenetrees[capteur].size()));

	result1 = result1 + Wn * result2;
	m_datasAnalysees[capteur].push_back(sqrt(real(result1)* real(result1)+imag(result1)* imag(result1)));
	//cout <<  sqrt(real(result1) * real(result1) + imag(result1) * imag(result1)) << endl;
	return;
}

//Méthode générale d'appel
void CAnalyse::general(string fenetrage)
{
	fenetrageDatas("rectangulaire");
	analyseFrequentielle();
	return;
}



//Fonction de générations de sinuosidales
void CAnalyse::genSinuo()
{
	vector<vector<float>> re;
	vector<float> ah;
	for (float i = 0; i < m_nbEchantillons; i++)
	{

		ah.push_back((float)cos(2 * PI * 10 * i / m_nbEchantillons) + cos(2 * PI * 20 * i / m_nbEchantillons)); //sinus 10 Hz et 20Hz
	}
	for (int i = 0; i < m_nbCapteurs; i++)
	{
		re.push_back(ah);
	}
	setDatasEntrantes(re);
	return;
}

//Fonction de générations de sinuosidales
void CAnalyse::genSinuoT()
{
	if (f < m_minFrequence)
	{
		f = m_minFrequence;
	}
	else if (f > m_maxFrequence)
	{
		f = m_minFrequence;
	}

	vector<vector<float>> re;
	vector<float> ah;
	for (float i = 0; i < m_nbEchantillons; i++)
	{

		ah.push_back((float)cos(2 * PI * f * i / m_nbEchantillons)); 
	}
	for (int i = 0; i < m_nbCapteurs; i++)
	{
		re.push_back(ah);
	}
	setDatasEntrantes(re);
	f += m_pas;
	return;
}


void CAnalyse::genSinuoT(int Frequence)
{
	if (Frequence < m_minFrequence)
	{
		Frequence = m_minFrequence;
	}
	else if (Frequence > m_maxFrequence)
	{
		Frequence = m_minFrequence;
	}

	vector<vector<float>> re;
	vector<float> ah;
	for (float i = 0; i < m_nbEchantillons; i++)
	{

		ah.push_back((float)cos(2 * PI * Frequence * i / m_nbEchantillons));
	}
	for (int i = 0; i < m_nbCapteurs; i++)
	{
		re.push_back(ah);
	}
	setDatasEntrantes(re);
	return;
}