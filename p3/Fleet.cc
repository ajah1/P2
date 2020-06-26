
#include <iostream>
#include "Util.h"
#include "Fleet.h"
#include <stdexcept>
#include <algorithm>
using namespace std;

Fleet::Fleet(){
	wins = 0;
	losses = 0;
	credits = CREDITS_INITIAL;
}
Fighter Fleet::getFighter(int n) const{
	return fighters[n];
}
int Fleet::getNumFighters() const{
	return fighters.size();
}
int Fleet::getCredits() const{
	return credits;
}

bool Fleet::addFighters(string type, int number, bool side){
	bool ok = false;
	
	try{
		// se inicializa la nave
		Fighter nave(type); 
		// se comprueba que el bando introducido sea el correcto
		if(side == IMPERIAL && nave.isImperial() || side == REBEL && !nave.isImperial()){
			if(nave.getCost() * number > credits){
				Util::error(NO_FUNDS);
			}
			else{
				// se introducen los fighters
				for(int i = 1; i <= number; i++){
					fighters.push_back(nave);
				}
				// se restan los creditos a los que se tenien
				credits -= nave.getCost()*number;
				ok = true;
			}		
		}
		else{
			Util::error(WRONG_FIGHTER_TYPE);
		}
	}
	catch(invalid_argument &e){
		Util::error(WRONG_FIGHTER_TYPE);
	}
	return ok;
}

// módulo adicional, reutilizado de la practica 2 
void Fleet::listFighters(const vector<Fighter> &vf) const{
	for(int i = 0; i < vf.size(); i++){
		cout << "[" << i + 1 << "] " << vf[i];	
		cout << endl;
	}
}

// código reutilizado de la práctica 2
bool Fleet::fight(Fleet &enemy){

	string cadena;
	string aux;
	int resultado;
	//-------------------
	vector<int> numerosCazas; // numero de cazas  introducidos por el usuario
	vector<Fighter> launchimp;
	vector<Fighter> launchRebel;
	//-------------------
	Fighter naveImperial;
	Fighter naveRebelde;
	
	// #############################
	// ### crear los escuadrones ###
	// #############################

	// imperial es la implicita.
	// la nave rebel es la pasada como parametro.
	if(fighters.size() >= 1 && enemy.fighters.size() >= 1){
		cout << "Select fighter numbers: ";
		getline(cin, cadena);
		int i = 0;		
		while(i < cadena.length()){
			while(i < cadena.length() && cadena[i] == ' '){
				i++;
			}
			aux = "";
			while(i < cadena.length() && cadena[i] != ' '){
				aux = aux + cadena[i];
				i++;
			}
			if(aux != ""){
				numerosCazas.push_back(atoi(aux.c_str()));
			}
		}
		std::sort(numerosCazas.begin(), numerosCazas.end(),  std::greater<int>());
		
		int cuantos = 0;
		// crear el escuadron imperial
		// recorrer el vector numeroCazas
		// sacar y meter del imperial los seleccionados en una auxiliar
		for(int p = 0; p  < numerosCazas.size(); p++){
			if(numerosCazas[p] >= 1 && numerosCazas[p] <= fighters.size()){
				cuantos++;
				launchimp.push_back(fighters[(numerosCazas[p] - 1)]);				
				fighters.erase(fighters.begin() + (numerosCazas[p] - 1));
			}
		}
		if(cuantos == 0){
			Util::error(WRONG_NUMBER);
		}
		else{
			// creamos el squadron rebelde de forma aleatoria
			int margin = Util::getRandomNumber(7);
			margin = margin - 3;
			int total = launchimp.size() + margin;
			if(total <= 0)
				total = 1;
			else{
				if(total > enemy.fighters.size()){
					total = enemy.fighters.size();
				}
			}
			int rnunmber, rsimulacion;
			// obtenidos el numero de cazas los sacamos
			for(int p = 1; p <= total; p++){
				rnunmber = Util::getRandomNumber(enemy.fighters.size());
				launchRebel.push_back(enemy.fighters[rnunmber]);
				enemy.fighters.erase(enemy.fighters.begin() + rnunmber);
			}
			// inicialmente se muestran ambos escuadrones
			cout << "Squadron A" << endl;
			listFighters(launchimp);
			cout << "Squadron B" << endl;
			listFighters(launchRebel);

			//##############################
			//### simulación del combate ###
			//##############################
			do{
				rsimulacion = Util::getRandomNumber(100);
				// en caso que el numero aleatorio sea menor que 10
				// se termina la simulación
				if(rsimulacion >= 10){
					// se extrae un fighter y se guarda en naveImperial
					rnunmber = Util::getRandomNumber(launchimp.size());
					naveImperial = launchimp[rnunmber];
					launchimp.erase(launchimp.begin()+rnunmber);
					// se extraer un fighter y se guarda en naveRebel
					rnunmber = Util::getRandomNumber(launchRebel.size());
					naveRebelde = launchRebel[rnunmber];
					launchRebel.erase(launchRebel.begin()+rnunmber);					
					
					cout << "-- begin fight" << endl;
					cout << naveImperial;					
					cout << endl;
					cout << naveRebelde;					
					cout << endl;		
					cout << "--" << endl;
					resultado = naveImperial.fight(naveRebelde);					
					cout << naveImperial;					
					cout << endl;		
					cout << naveRebelde;					
					cout << endl;
					cout << "-- end fight" << endl;

					if(resultado == 0){
						launchRebel.push_back(naveRebelde);
						launchimp.push_back(naveImperial);
					}
					if(resultado == -1){
						wins++;
						enemy.losses++;
						launchimp.push_back(naveImperial);//*
						credits += naveRebelde.getCost();
					}
					if(resultado == 1){
						enemy.wins++;
						losses++;
						launchRebel.push_back(naveRebelde);//*
						enemy.credits += naveImperial.getCost();
					}	
				}

			}while(rsimulacion >= 10 && launchRebel.size() > 0 && launchimp.size() > 0);

			cout << "Squadron A" << endl;
			listFighters(launchimp);
			cout << "Squadron B" << endl;
			listFighters(launchRebel);

			// los cazas supervivientes se añaden al final del vecto
			for(int p = 0; p < launchimp.size(); p++){
				fighters.push_back(launchimp[p]);
			}
			for(int p = 0; p < launchRebel.size(); p++){
				enemy.fighters.push_back(launchRebel[p]);
			}
		}
	}
	else{
		Util::error(NO_FIGHTERS);
	}
}

void Fleet::improveFighter(int nf, char feature, int amount, int cost){
	switch(feature){
		case 'v':
			fighters[nf].increaseVelocity(amount);
		break;
		case 'a':
			fighters[nf].increaseAttack(amount);
		break;
		case 's':
			fighters[nf].increaseShield(amount);
		break;
	}
	fighters[nf].increaseCost(cost);
	credits -= cost;
}

ostream &operator<<(ostream &os, const Fleet &flota){

	os << "credits=" << flota.credits;
	os << ", wins=" << flota.wins; 
	os << ", losses=" << flota.losses << endl;

	for(int i=0; i < flota.fighters.size(); i++){
		os << "[" << i + 1 << "] " << flota.fighters[i];
		cout << endl;	
	}
	return os;
}

