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




//TODO:Acquisition des données
void CAnalyse::acquisitionDatas()
{
	return;
}



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
	
	for (short i = 0; i < (m_datasFenetrees[capteur].size()/2)-1; i++)
	{
		resulttmp =  exp(I*(-2*PI*frequence*2*i/ m_datasFenetrees[capteur].size()));
		resulttmp *= m_datasFenetrees[capteur][2 * i];
		result1 += resulttmp;
	}
	result1 /= m_datasFenetrees[capteur].size();

	for (short i = 0; i < (m_datasFenetrees[capteur].size() / 2) - 1; i++)
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
