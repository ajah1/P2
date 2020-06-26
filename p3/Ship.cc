
#include <cstdlib>
#include <ctype.h>
#include "Fleet.h"
#include "Util.h"
#include "Ship.h"

const int IMPSHIPCAPACITY = 30;
const int REBSHIPCAPACITY = 35;
const string initialImperialShipDotation = "10tf/  5  tb, 5 ti, 5;ta";
const string initialRebelShipDotation = "10xw,5yw, 8aw, 5bw";


// se inicializa las naves de cada vando
Ship::Ship(bool side){
	this->side = side;
	if(side == IMPERIAL){
		maxCapacity = IMPSHIPCAPACITY;
		addFighters(initialImperialShipDotation);	
	}
	else{
		maxCapacity = REBSHIPCAPACITY;
		addFighters(initialRebelShipDotation);
	}
}
// código reutilizado de la practica 2
// cambia el uso de isdigit y isalpha para saber
// si son o no diígitos / letras
bool Ship::addFighters(string fighters){
	bool exito, stop, ok;
	string tipo, numero;
	int cantidad;

	exito = true;
	stop = false;
	int i = 0;
	while(i < fighters.length() && ! stop){
		while(i < fighters.length() && !isdigit(fighters[i])){
			i++;
		}
		numero = "";
		while(i < fighters.length() && isdigit(fighters[i])){
			numero += fighters[i];
			i++;
		}
		tipo = "";
		while(i < fighters.length() && !isalpha(fighters[i])){
			i++;
		}
		while(i < fighters.length() && isalpha(fighters[i])){
			tipo = tipo + fighters[i];
			i++;
		}
		if(tipo != "" && numero != ""){
			cantidad = atoi(numero.c_str());
			if(fleet.getNumFighters() + cantidad > maxCapacity){		
				Util::error(CAPACITY_EXCEEDED);
				stop = true;
				exito = false;
			}
			else{
				ok = fleet.addFighters(tipo, cantidad, side);
				if(ok == false){
					stop = true;
					exito = false;
				}		
			}
		}
	}
	return exito;
}

bool Ship::improveFighter(){
	bool mejorado;
	int numero;
	char mejora, respuesta;
	int unidad, coste;
	
	mejorado = false;
	cout << "Select fighter number: ";
	cin >> numero;
	cin.get();
	if(numero >= 1 && numero <= fleet.getNumFighters()){
		cout << "What to improve (v/a/s)?";
		cin >> mejora;
		cin.get();
		// si el tipo introducido es correcto
		if(mejora == 'v' || mejora == 'a' || mejora == 's'){
			cout << "Amount: ";
			cin >> unidad;
			cin.get();
                        // calcular el coste en funcion de la mejora
			switch(mejora){
				case 'v':
					coste = 2*unidad;
					break;
				case 'a':
					coste = 3*unidad;
					break;
				case 's':
					coste = (unidad+1)/2;
					break;
			}
			// En caso de tener los creditos suficientes
			// sino muestra error(NO_FOUNDS)
			if(fleet.getCredits() >= coste){
				cout << "That will cost you " << coste << " credits. Confirm? (y/n)";
				cin >>respuesta;
				cin.get();
				if(respuesta == 'y'){
					fleet.improveFighter(numero-1, mejora, unidad, coste);
					cout << "Fighter improved: ";
					cout << fleet.getFighter(numero-1) << endl;
					mejorado = true;
				}
			}
			else{
				Util::error(NO_FUNDS);
			}
		}
		else
			Util::error(UNKNOWN_OPTION);
	}
	else{
		Util::error(WRONG_NUMBER);
	}
	return mejorado;		
}

void Ship::fight(Ship &enemy){
	fleet.fight(enemy.fleet);
}

ostream &operator<<(ostream &os, const Ship &ship){
	os << "Ship info: max. capacity=" << ship.maxCapacity;
	os << ", side=";
	switch(ship.side){
		case IMPERIAL:
			os << "IMPERIAL";
			break;
		case REBEL:
			os << "REBEL";
			break;
	}
	os << ", " << ship.fleet << endl;
	return os;
}


