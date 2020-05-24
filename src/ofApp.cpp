/**
 *
 * 	@file		ofApp.cpp
 * 	@author		COLLART Quentin, PEREZ-LACOSTE Mallaury, MANTUILA Herlyne, LETHU Florian
 *
 */

#include "ofApp.h"


//To get a final record file of the session (created for each session and can be appended when you want to)
time_t now = time(0);
const char *dt = ctime(&now);
string date_add(dt);
string const nom_rec_file("./data/records/record_" + date_add + ".txt");
ofstream rec_file(nom_rec_file.c_str());

ifstream record_to_display;


//--------------------------------------------------------------
void ofApp::setup(){

	#ifdef _WIN32
		printf("Windows\n");
		#include <window.h> 
	#elif __linux__
		printf("Linux OS\n");
	#else
		printf("Unusuable OS\n");
	#endif

	//Window
	ofSetWindowTitle("SynSitive");
	ofSetWindowPosition(10, 10);
	ofBackground(20, 20, 50);

	//Pictures
	logoopenbci.load("./pictures/logo_openbci.png");
	logosynsitive.load("./pictures/logo_synsitive.png");
	buttonsave.load("./pictures/button-save.png");
	buttonscreenshot.load("./pictures/button-screenshot.png");
	brain.load("./pictures/brain.png");
	axe_hz_t.load("./pictures/axe_hz_t.png");
	axe_v_hz.load("./pictures/axe_v_hz.png");
	more_less.load("./pictures/more_less.png");
	electrode.load("./pictures/electrode.png");
	rec.load("./pictures/rec.png");
	stop_rec.load("./pictures/stop-rec.png");
	folder.load("./pictures/folder.png");
	record_display.load("./pictures/record-view.png");

	//Fonts
	agressive_font.load("./fonts/Minecraft.ttf", 15);
	main_font.load("./fonts/BebasNeue.ttf", 12);
	main_font_bigger.load("./fonts/BebasNeue.ttf", 14);

	/*
	//
	//	WITH the BCI
	//
	//Data frequency analysis setup
	m_analyse.init(8, 256, 0.5, 0, 60);


	//Serial port opened
	#ifdef _WIN32								//tty(S31/x) Linux or COM(17/x) Windows
		if (!(m_serial.setup("COM17", 115200)))
	#else
		if (!(m_serial.setup("ttyS31", 115200)))
	#endif
	{
		printf("No setup ! Can't load the serial port.");
		m_serial.listDevices();
		ofLogError() << "could not open serial port - listing serial devices";
		return 0;
	}

	cout << "setup done" << endl;
	cout << m_serial.isInitialized() << endl;

	unsigned char myKey = 0;
	// =>start send frame 'b'
	bool byteWasWritten = m_serial.writeByte(OPENBCI_STREAM_START);
	if (!byteWasWritten)
	{
		printf("Key was no written");
		ofLogError() << "Aucun bouton appuyé";
	}*/
}

//--------------------------------------------------------------
void ofApp::update(){

	float sum;

	t1 = clock();

	if(temps >= STEP_TIME){

		//Update values
		freq_min = m_analyse.getMinFrequence();
		freq_max = m_analyse.getMaxFrequence();
		step = m_analyse.getPas();

		if(record_view){
			if(record_to_display.is_open() && !record_to_display.eof()){

				vector<vector<float>> current_data_record(CELLS, vector<float> (nb_echant, 0));

				int nb_echant;
				record_to_display >> nb_echant;
				m_analyse.setNbEchantillons(nb_echant);

				for(int j=0; j < nb_echant; j++)
					record_to_display >> current_data_record[0][j] >> current_data_record[1][j] >> current_data_record[2][j] >> current_data_record[3][j] >> current_data_record[4][j] >> current_data_record[5][j] >> current_data_record[6][j] >> current_data_record[7][j];

				m_analyse.setDatasEntrantes(current_data_record);
			}
			else{
				record_view = 0;
				record_to_display.close();
			}
		}
		else {
			nb_echant = (int)((freq_max - freq_min) / step);
			m_analyse.setNbEchantillons(nb_echant);

			//========================================================
			//
			//	WITH the BCI
			//
			//Getting datas each STEP_TIME
			//int a = analyse();
			//m_analyse.acquisitionDatas(m_serial);

			//
			//	WITHOUT the BCI
			//
			//Sinusoidal as an input
			m_analyse.genSinuoT();
			//=========================================================
		}

		//Windowing
		m_analyse.fenetrageDatas("rectangulaire");
		//FFT
		m_analyse.analyseFrequentielle();

		current_data_brut = m_analyse.getDatasEntrantes();
		current_data = m_analyse.getDatasAnalysees();

		//Save the current data into a record file
		if(current_data_brut.size() > 0){
			if(record == 1){

				if(rec_file.is_open())
	  			{
	  				rec_file << nb_echant << endl;

	  				//Write values
					for(short i=0; i < nb_echant; i++){
						for(short j=0; j < CELLS; j++){
							if(j != CELLS-1)
								rec_file << current_data_brut[j][i] << " ";
							else
								rec_file << current_data_brut[j][i];
						}
						rec_file << endl;
					}
	  			}
			}
		}
		//Adding current value into the print queue
		if(current_data.size() > 0){
			if(datas_to_print.size() < NB_DATA){
				datas_to_print.push_back(current_data);
			}
			else{
				datas_to_print.erase(datas_to_print.begin());	//TO VERIFY
				datas_to_print.push_back(current_data);
			}
			

			//Update brain area values and display outputs
			for(short i=0; i < m_analyse.getNbCapteurs(); i++){

				sum = 0.0;

				for(int j=0; j < nb_echant; j++)
					sum += current_data[i][j];

				brain_areas[i] = (sum/nb_echant)*255.0;
			}
		}

		temps = 0;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

		//Draw shapes

	//"Test" box
	ofDrawRectangle(ofGetWidth() - 170, 20, 150, 40);
	ofSetColor(200, 200, 200, 255);
	ofDrawRectangle(ofGetWidth() - 166, 24, 142, 32);
	ofSetColor(20, 20, 20, 255);
	agressive_font.drawString("TEST [t]", ofGetWidth() - 132, 48);

	//Logos
	ofSetColor(235, 235, 235, 190);
	ofNoFill();
	ofDrawBox(145, 55, 0, 300, 110, 20);
	ofFill();
	ofDrawRectangle(6, 6, 295, 107);
	ofSetColor(20, 20, 20, 255);
	main_font.drawString("An OpenBCI project", 170, 50);

	ofSetColor(255, 255, 255, 255);
	brain.draw(ofGetWidth() * 3/5, 160, ofGetWidth() *2/5, ofGetWidth() *2/5);


	//Output waves
	ofSetColor(40, 40, 40, 255);
	ofDrawRectangle(10, 155, ofGetWidth() * 3/5, ofGetHeight() - 164);
	ofSetColor(200, 200, 200, 255);
	ofDrawRectangle(16, 162, (ofGetWidth() * 3/5) - 12, ofGetHeight() - 176);

	if(record_view)
		record_display.draw((ofGetWidth() * 3/5)+15, 155, 30, 30);

		//Draw pictures
	ofSetColor(255, 255, 255, 255);
	logoopenbci.draw(204, 55, 38, 38);
	logosynsitive.draw(5, -10, 150, 150);
	buttonsave.draw(460, 35, 70, 70);
	buttonscreenshot.draw(580, 35, 70, 70);
	folder.draw(350, 35, 70, 70);
	if(record == 0){
		rec.draw(720, 35, 70, 70);
	}
	else if(record == 1){
		stop_rec.draw(720, 35, 70, 70);
	}
	

	//Signal type
	main_font_bigger.drawString("Signal type ", 20, 142);

	if(signal_type == 1){
		ofSetColor(200, 0, 180, 255);
		main_font_bigger.drawString("T", 123, 142);
		ofSetColor(255, 255, 255, 255);
		main_font_bigger.drawString("|", 135, 142);
		main_font_bigger.drawString("F", 150, 142);

		//Axe Hz/t
		for(short i=0; i < m_analyse.getNbCapteurs(); i++){
			if(i%2 == 0){
				axe_hz_t.draw(35, 168 + 61*i, 260, 108);
			}
			else{
				axe_hz_t.draw((ofGetWidth() * 3/10) + 12, 168 + 61*(i-1), 260, 108);
			}
		}
	}
	else if(signal_type == 2){
		ofSetColor(200, 0, 180, 255);
		main_font_bigger.drawString("F", 150, 142);
		ofSetColor(255, 255, 255, 255);
		main_font_bigger.drawString("|", 135, 142);
		main_font_bigger.drawString("T", 123, 142);

		//Axe V/Hz
		for(short i=0; i < m_analyse.getNbCapteurs(); i++){
			if(i%2 == 0){
				axe_v_hz.draw(35, 168 + 61*i, 260, 108);
			}
			else{
				axe_v_hz.draw((ofGetWidth() * 3/10) + 12, 168 + 61*(i-1), 260, 108);
			}
		}
	}


	//Min freq
	char minStr[40];
	sprintf(minStr, "- Min : %.1f Hz", m_analyse.getMinFrequence());
	main_font_bigger.drawString(minStr, 180, 142);

	//Max freq
	char maxStr[40];
	sprintf(maxStr, "- Max : %.1f Hz", m_analyse.getMaxFrequence());
	main_font_bigger.drawString(maxStr, 320, 142);

	//Step
	char stepStr[40];
	sprintf(stepStr, "- Step :   %.1f", m_analyse.getPas());
	main_font_bigger.drawString(stepStr, 470, 142);

	//Cells
	electrode.draw(650, 122, 30, 30);
	char cellsStr[20];
	sprintf(cellsStr, "%d", m_analyse.getNbCapteurs());
	main_font_bigger.drawString(cellsStr, 695, 142);

	more_less.draw(280, 122, 30, 30);
	more_less.draw(430, 122, 30, 30);
	more_less.draw(575, 122, 30, 30);
	more_less.draw(725, 122, 30, 30);


		//Button's hitboxes
	mouseX = ofGetMouseX();
	mouseY = ofGetMouseY();

	//Test button
	if((mouseX >= ofGetWidth()-170)&&(mouseY >= 20)){
		if((mouseX <= ofGetWidth()-20)&&(mouseY <= 60)){
			ofNoFill();
			ofDrawRectangle((ofGetWidth()-175), 15, 162, 52);
			ofFill();
		}
	}

	//Save and screenshot buttons
	if((35 <= mouseY)&&(mouseY <= 105)){
		if((460 <= mouseX)&&(mouseX <= 530)){
			ofNoFill();
			ofDrawRectangle(455, 30, 80, 80);
			ofFill();
		}
		if((580 <= mouseX)&&(mouseX<= 650)){
			ofNoFill();
			ofDrawRectangle(575, 30, 80, 80);
			ofFill();
		}
	}

		//Temporal signal
	if((current_data.size() > 0)  && (signal_type == 1)){

		float counter_to_print = T_MIN;

		for(short c=0; c < datas_to_print.size(); c++){

			for(short i=0; i < m_analyse.getNbCapteurs(); i++){

				for(int j=0; j < nb_echant; j++){

					//DISPLAY OUTPUTS
					ofSetColor(255, 0, 0, (datas_to_print[c][i][j])*255.0);

					if(i%2 == 0){
						ofDrawCircle(37 + (245*counter_to_print/T_MAX), (270 + 61*i) - freq_min*1.4, 1);
					}
					else{
						ofDrawCircle(((ofGetWidth() * 3/10) + 14) + (245*counter_to_print/T_MAX), (270 + 61*(i-1)) - freq_min*1.4, 1);
					}
					
					freq_min += step;
				}
			

				freq_min = m_analyse.getMinFrequence();				
			}

			counter_to_print += STEP_TIME;
		}
	}

		//Frequency signal
	if((current_data.size() > 0) && (signal_type == 2)){

		for(short i=0; i < m_analyse.getNbCapteurs(); i++){

			for(int j=0; j < nb_echant; j++){

				ofSetColor(0, 0, 255, 255);

				if(i%2 == 0){
					ofDrawCircle(39 + (243*freq_min/freq_max), (270 + 61*i) - (80*current_data[i][j]), 1);
				}
				else{
					ofDrawCircle(((ofGetWidth() * 3/10) + 16) + (243*freq_min/freq_max), (270 + 61*(i-1)) - (80*current_data[i][j]), 1);
				}

				freq_min += step;
			}

			freq_min = m_analyse.getMinFrequence();
		}
	}

	ofSetColor(255, 255, 255, 255);

	//Brain areas
	for(short zone=0; zone < m_analyse.getNbCapteurs(); zone++){
		ofSetColor(255, 0, 0, (int)brain_areas[zone]);
		ofDrawCircle((ofGetWidth() * 3/5 + (ofGetWidth() * 1/20)) + (zone*(ofGetWidth() * 2/45)), 120 + (ofGetWidth() *3/11), (ofGetWidth() * 1/45));
	}

	ofSetColor(255, 255, 255, 255);

	t2 = clock();
	temps += (float)((t2-t1)*10)/CLOCKS_PER_SEC;
	//printf("\nTime bf step = %f", temps);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	//Open a new VLC window displaying the little test movie
	if(key == 't'){
		#ifdef __linux__
			if(system("vlc ./data/movies/test_movie.mp4 --play-and-exit 2>/dev/null &")){
				printf("VLC Error while loading the test movie.\n");
			}
		#elif _WIN32
			if(system("\"C:\\Program Files\\VideoLAN\\VLC\vlc.exe\" file:///data/movies/test_movie.mp4 vlc://quit -f")){
				printf("VLC Error while loading the test movie.\n");
			}
		#else
			printf("Can't load the test movie on an unknown OS.\n");
		#endif
	}

	//Save into file
	else if(key == 's'){

		if(current_data.size() > 0){

			time_t now2 = time(0);
			const char *dt2 = ctime(&now2);
			string date_add2(dt2);
			string const nom_sample_file2("./data/samples/sample_" + date_add2 + ".csv");

			ofstream sample_file(nom_sample_file2.c_str());

			if (sample_file.is_open())
	  		{
				sample_file << "Frequency, Analyse ->, Output 1, Output 2, Output 3, Output 4, Output 5, Output 6, Output 7, Output8" << endl;
					
				for(short j=0; j < nb_echant; j++){
					sample_file << freq_min << " Hz, ,";

					for(int i=0; i < m_analyse.getNbCapteurs(); i++)
						sample_file << current_data[i][j] << ",";

					sample_file << endl;
					freq_min += step;
				}

				freq_min = m_analyse.getMinFrequence();

				sample_file.close();

				ofSystemAlertDialog("Sample saved into the samples folder.");
			}
			else cout << "Can't open the sample file.";
		}
	}

	//Take a screenshot
	else if(key == 'x'){
		FILE *fp;
		char path[30];

		#ifdef _WIN32
			fp = fopen("date +\'%d%m%y_%k%M%S\'", "r");
		#else
			fp = popen("date +\'%d%m%y_%k%M%S\'", "r");
		#endif

		if (fp == NULL) {
			printf("Erreur lors de la récupération de la date.\n" );
		}
		else {
			while (fgets(path, sizeof(path), fp) != NULL) {

				char file_name[60];
				strcpy(file_name, "./screenshots/screenshot_");
				strcat(file_name, path);
				file_name[strlen(file_name)-1] = 0;
				strcat(file_name, ".png");

				img.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
				img.save(file_name);

				ofSystemAlertDialog("Screenshot saved.");
				printf("Captured screenshot as : %s\n", file_name);
			}
		}
		pclose(fp);
    }
    else if(key == 'o'){
		auto result  = ofSystemLoadDialog();
		if(result.bSuccess){
			record_to_display.open(result.getPath());

			if(record_to_display.is_open()){
				cout << "Fichier de record '" << result.getPath() << "' lancé." << endl;
				record_view = 1;
			}
			else{
				cout << "Impossible de lancer le record. Fichier en utilisation ?" << endl;
			}
		}
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){


}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

	mouseX = x;
	mouseY = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){


}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
	if(button == 0){

		//Test button
		if((x >= ofGetWidth()-170)&&(y >= 20)){
			if((x <= ofGetWidth()-20)&&(y <= 60)){
			#ifdef __linux__
				if(system("vlc ./data/movies/test_movie.mp4 --play-and-exit 2>/dev/null &")){
					printf("VLC Error while loading the test movie.\n");
				}
			#elif _WIN32
				if(system("\"C:\\Program Files\\VideoLAN\\VLC\vlc.exe\" file:///data/movies/test_movie.mp4 vlc://quit -f"));
					printf("VLC Error while loading the test movie.\n");
				}
			#else
				printf("Can't load the test movie on an unknown OS.\n");
			#endif
			}
		}

		//Save and screenshot buttons
		if((35 <= y)&&(y <= 105)){
			//Open
			if((350 <= x)&&(x<=420)){
				auto result  = ofSystemLoadDialog();
				if(result.bSuccess){
					record_to_display.open(result.getPath());

					if(record_to_display.is_open()){
						cout << "Fichier de record '" << result.getPath() << "' lancé." << endl;
						record_view = 1;
					}
					else{
						cout << "Impossible de lancer le record. Fichier en utilisation ?" << endl;
					}
				}
			}
			//Into file
			if((460 <= x)&&(x <= 530)){

				if(current_data.size() > 0){

					time_t now2 = time(0);
					const char *dt2 = ctime(&now2);
					string date_add2(dt2);
					string const nom_sample_file2("./data/samples/sample_" + date_add2 + ".csv");

					ofstream sample_file(nom_sample_file2.c_str());

					if (sample_file.is_open())
			  		{
						sample_file << "Frequency, Analyse ->, Output 1, Output 2, Output 3, Output 4, Output 5, Output 6, Output 7, Output8" << endl;
							
						for(short j=0; j < nb_echant; j++){
							sample_file << freq_min << " Hz, ,";

							for(int i=0; i < m_analyse.getNbCapteurs(); i++)
								sample_file << current_data[i][j] << ",";

							sample_file << endl;
							freq_min += step;
						}

						freq_min = m_analyse.getMinFrequence();

						sample_file.close();

						ofSystemAlertDialog("Sample saved into the samples folder.");
					}
					else cout << "Can't open the file.";
				}
			}
			//As a screenshot
			if((580 <= x)&&(x<= 650)){
				FILE *fp;
				char path[30];

				#ifdef _WIN32
					fp = fopen("date +\'%d%m%y_%k%M%S\'", "r");
				#else
					fp = popen("date +\'%d%m%y_%k%M%S\'", "r");
				#endif

				if (fp == NULL) {
					printf("\nCan't get the current date. OS or syntax problem ?" );
				}
				else {
					while (fgets(path, sizeof(path), fp) != NULL) {

						char file_name[60];
						strcpy(file_name, "./screenshots/screenshot_");
						strcat(file_name, path);
						file_name[strlen(file_name)-1] = 0;
						strcat(file_name, ".png");

						img.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
						img.save(file_name);

						ofSystemAlertDialog("Screenshot saved.");
						printf("\nCaptured screenshot as : %s\n", file_name);
					}
				}
				pclose(fp);
			}
			if((720 <= x)&&(x<= 790)&&(record == 0)){
				record = 1;
			}
			else if((720 <= x)&&(x<= 790)&&(record == 1)){
				record = 0;
			}
		}

		if(y >= 121 && y <= 153){
			//Change signal type
			//1
			if(x >= 121 && x <= 141){
				signal_type = 1;
			}
			//2
			if(x >= 149 && x <= 179){
				signal_type = 2;
			}
				//Change min freq
				if(x >= 279 && x <= 311){
					if(y <= 136){
						if(m_analyse.getMinFrequence() + m_analyse.getPas() < m_analyse.getMaxFrequence())
						m_analyse.setMinFrequence(m_analyse.getMinFrequence() + m_analyse.getPas());
					}
					else{
						if(m_analyse.getMinFrequence() - m_analyse.getPas() >= 0.0){
							m_analyse.setMinFrequence(m_analyse.getMinFrequence() - m_analyse.getPas());
						}
					}
				}
				//Change max freq
				if(x >= 429 && x <= 461){
					if(y <= 136){
						m_analyse.setMaxFrequence(m_analyse.getMaxFrequence() + m_analyse.getPas());
					}
					else{
						if(m_analyse.getMaxFrequence() - m_analyse.getPas() > m_analyse.getMinFrequence()){
							m_analyse.setMaxFrequence(m_analyse.getMaxFrequence() - m_analyse.getPas());
						}
					}
				}
				//Change step
				if(x >= 574 && x <= 606){
					if(y <= 136){
						m_analyse.setPas(m_analyse.getPas() + 0.1);
					}
					else{
						if(m_analyse.getPas() >= 0.2){
							m_analyse.setPas(m_analyse.getPas() - 0.1);
						}
						else{
							m_analyse.setPas(0.1);
						}
					}
				}
				//Change cells number
				if(x >= 724 && x <= 756){
					if(y <= 136){
						if(m_analyse.getNbCapteurs() < 8){
							m_analyse.setNbCapteurs(m_analyse.getNbCapteurs() + 1);
						}
					}
					else{
						if(m_analyse.getNbCapteurs() > 0){
							m_analyse.setNbCapteurs(m_analyse.getNbCapteurs() - 1);
						}
					}
				}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
