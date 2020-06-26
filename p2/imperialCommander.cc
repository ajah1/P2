
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <fstream>

using namespace std;

typedef struct {
     string type;  // TIE Fighter, X-Wing, ...
     int velocity;
     int attack;    // attack power
     int shield;    // current shield status. if <=0, the fighter is considered destroyed
     int cost;      // cost in credits
} Fighter;

typedef struct{
	bool side;
	int maxCapacity;
	int credits;
	int wins;
	int losses;
}ShipBin;

typedef struct {
    bool side;       // IMPERIAL or REBEL
    int maxCapacity; // maximum capacity of fighters allowed in the ship
    int credits;      // credits remaining to buy fighters, make repairs or improvements
    int wins;         // enemy fighters destroyed
    int losses;        // fighters lost
    vector<Fighter> fighters;  // fighters inside the ship
} Ship;

typedef struct{
	char type[3];
	int velocity;
	int attack;
	int shield;
	int cost;
}FighterBin;

// Fighters table 

const int MAXFIGHTERS=8;

// abbreviations (to use in option "Add fighter")
const string FIGHTERABR[]= { "tf", "tb", "ti", "ta", 
                             "xw", "yw", "aw", "bw"
};

const Fighter FIGHTERTABLE[] = {
 { "TIE-Fighter",      150,  75, 30,  45 },
 { "TIE-Bomber",        80, 150, 45,  75 },
 { "TIE-Interceptor",  180,  65, 30,  55 },
 { "TIE-Advanced",     160,  80, 90,  95 },
 { "X-Wing",           175,  90, 75,  65 },
 { "Y-Wing",            90, 150, 90,  90 },
 { "A-Wing",           200,  60, 50,  45 },
 { "B-Wing",           120, 200, 90, 100 }
};

// ship constants
const bool IMPERIAL=false;
const bool REBEL=true;

const int IMPSHIPCAPACITY=30;
const int REBSHIPCAPACITY=35;

const string initialRebelShipDotation = "10xw,5yw, 8aw, 5bw";
const string initialImperialShipDotation = "10tf/  5  tb, 5 ti, 5;ta";


// ship initial credits
const int CREDITS_INITIAL=2000;

// error numbers
const int UNKNOWN_OPTION=1;
const int WRONG_FIGHTER_TYPE=2;
const int CAPACITY_EXCEEDED=3;
const int WRONG_NUMBER=4;
const int NO_FUNDS=5;
const int NO_FIGHTERS=6;
const int CANT_OPEN_FILE = 7;
const int WRONG_SIDE = 8;
const int WRONG_ARGUMENTS = 9;

//---------------------------------------------------
void error(int n)
{
  cout << "ERROR (" << n << "): " ;
  
  switch (n) 
  {
    case UNKNOWN_OPTION:
      cout << "unknown option" << endl;
      break;
    case WRONG_FIGHTER_TYPE:
      cout << "wrong fighter type" << endl;
      break;
    case CAPACITY_EXCEEDED:
      cout << "ship capactity exceeded" << endl;
      break;
    case WRONG_NUMBER:
      cout << "wrong number" << endl;
      break;
    case NO_FUNDS:
      cout << "not enough credits" << endl;
      break;
    case NO_FIGHTERS:
      cout << "not enough fighters" << endl;

	case CANT_OPEN_FILE: 
	 cout << "can't open file" << endl;
      break;
	case WRONG_SIDE:
		cout << "wrong binary data" << endl;
	break;
	case WRONG_ARGUMENTS:
		cout << "Wrong arguments, syntax: imperialCommander [-l file | -L file | -i file]" << endl;
  }
}

//---------------------------------------------------
int getRandomNumber(int max)
{
   // Genera un número aleatorio entre 0 y maximo-1
   //
   int na;

   na = (int)(((double)max)*rand()/(RAND_MAX+1.0));
   return na;
}

// funcion auxiliar para comprobar si es letra
bool esLetra(char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int getPos(string tipo, bool side){
	int pos = -1, i;
	if(side == IMPERIAL)
	for(i = 0; i < 4; i++){
		if(tipo == FIGHTERABR[i]){
			pos = i;
		}
	}
	else
	for(i = 4; i < 8; i++){
		if(tipo == FIGHTERABR[i]){
			pos = i;
		}
	}
	return pos;
}

void add(Ship &ship, string c){
    
	string numero, tipo;
	bool es;
	int i, pos, coste, num, p;
	i = 0;
	es = false;
        
        // mientras que no llegue al final de la cadena
	while(i < c.length() && !es){      // salta lo que no sea dígito
		while(i < c.length() && (c[i] < '0' || c[i] > '9')){
			i++;
		}
		numero = "";              // comprobar si es digito
		while(i < c.length() && (c[i] >= '0' && c[i] <= '9')){
			numero = numero + c[i];
			i++;		
		}                         // salta lo que no sea letra
		while(i < c.length() && esLetra(c[i]) == false){
			i++;
		}
		tipo = "";                // comprobar si es letra
		while(i < c.length() && ((c[i] >= 'a' && c[i] <= 'z') || (c[i] >= 'A' && c[i] <= 'Z'))){
			tipo = tipo + c[i];
			i++;
		}
                                          
		pos = getPos(tipo, ship.side);
		if(pos == -1){
			es = true;
			error(WRONG_FIGHTER_TYPE);
		}
		else{
			num = atoi(numero.c_str());
			coste = FIGHTERTABLE[pos].cost*num;
                        
			if(coste < ship.credits){
				if(num + ship.fighters.size() > ship.maxCapacity){
                                        es = true;
					error(CAPACITY_EXCEEDED);
				}
				else{
					ship.credits = ship.credits - coste;
					for(p = 1; p <= num; p++){
						ship.fighters.push_back(FIGHTERTABLE[pos]);
					}
				}
			}
			else{
                                es = true;
				error(NO_FUNDS);	
			}
		}		
	}
}

void initializeShip(Ship &ship,bool side){
	ship.credits = CREDITS_INITIAL;
	ship.wins = 0;
	ship.losses = 0;
	ship.side = side;
	if(side == IMPERIAL){
		ship.maxCapacity = IMPSHIPCAPACITY;
		add(ship, initialImperialShipDotation); 	
	}
	else{
		ship.maxCapacity = REBSHIPCAPACITY;
		add(ship, initialRebelShipDotation);
	}
}

void listFighter(const Fighter &f)
{
    cout << f.type << " (v=" << f.velocity;
    cout << " ,a=" << f.attack;
    cout << " ,s=" << f.shield;
    cout << " ,c=" << f.cost << ")";
}

void listFighters(const vector<Fighter> &vf){
        
        int i;

	for(i = 0; i < vf.size(); i++){
		cout << "[" << i + 1 << "] ";
		listFighter(vf[i]);
		cout << endl;
	}
}

void listShip(const Ship &ship)
{
    cout << "Ship info: max. capacity=" << ship.maxCapacity << ", side=";
    
    if(ship.side == IMPERIAL){
        cout << "IMPERIAL, ";
    }
    else{
        cout << "REBEL, ";
    }
    
    cout << "credits=" << ship.credits;
    cout << ", wins=" << ship.wins;
    cout << ", losses=" << ship.losses;
    cout << endl;
    listFighters(ship.fighters);
    
}

void addFighters(Ship &ship){
    
	string tipo;
        
	cout << "Enter fighters: ";
	getline(cin, tipo);
	add(ship, tipo);	
}

bool improveFighter(Ship &ship){
    
	bool mejorado;
	int numero;
	char mejora, respuesta;
	int unidad, coste;
	
	mejorado = false;
	cout << "Select fighter number: ";
	cin >> numero;
	if(numero >= 1 && numero <= ship.fighters.size()){
		cout << "What to improve (v/a/s)?";
		cin >> mejora;
		// si el tipo introducido es correcto
		if(mejora == 'v' || mejora == 'a' || mejora == 's'){
			cout << "Amount: ";
			cin >> unidad;
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
			// En caso de tener los creditos suficientes, sino, error(NO_FOUNDS)
			if(coste <= ship.credits){
				cout << "That will cost you " << coste << " credits. Confirm? (y/n)";
				cin >>respuesta;
				if(respuesta == 'y'){
					switch(mejora){
						case 'v':
							ship.fighters[numero-1].velocity = ship.fighters[numero-1].velocity + unidad;
						break;
						case 'a':
							ship.fighters[numero-1].attack = ship.fighters[numero-1].attack + unidad;
						break;
						case 's':
							ship.fighters[numero-1].shield = ship.fighters[numero-1].shield + unidad;
						break;
					}
					ship.fighters[numero-1].cost = ship.fighters[numero-1].cost + coste;
                                        cout << "Fighter improved: ";
					listFighter(ship.fighters[numero-1]);
					// una vez mejorado se le restan los creditos de la mejora
					ship.credits = ship.credits - coste;
					mejorado = true;
				}
			}
			else{
				error(NO_FUNDS);
			}
		}
		else
			error(UNKNOWN_OPTION);
	}
	else
		error(WRONG_NUMBER);
	return mejorado;
}

int fight(Fighter &fg1,Fighter &fg2){
    
	const int divisor = 300;
	const int min = 5, max = 95;
	int n, d, r;
	double umbral, suma;
	
	r = -1;
	do{
		n = getRandomNumber(100);
		if(n < min || n > max){
			r = 0;
		}
		if(r != 0){
			suma = fg1.velocity + fg2.velocity;
			umbral = (100*fg1.velocity)/suma;
			if(n <= umbral){
				d = (n * fg1.attack)/divisor;
				fg2.shield -= d;
				
			}
			if(n > umbral){
				d = ((100-n)*fg2.attack)/divisor;
				fg1.shield -= d;		
			}
		}
	}while(r != 0 && fg1.shield > 0 && fg2.shield > 0);
	r = 0;	
	if(fg2.shield <= 0 && fg1.shield > 0){
		r = -1;
	}
	else{
		if(fg1.shield <= 0 && fg2.shield > 0){
			r = 1;
		}
	}
	return r;
}

void launchSquadron(Ship &imperial,Ship &rebel){

        int i, resultado, p, cuantos;
    //-------------------
	string cadena;
	string aux;
    //----------------
        vector<int> numerosCazas; // numero cazas introducido por el usuario
	vector<Fighter> launchimp;
	vector<Fighter> launchRebel;
    //---------------
	Fighter naveImperial;
	Fighter naveRebel;
        
        
        //----------------------------
        //--- crear los escuadrones---
        //----------------------------
        
        // en caso de tener fighters para poder lanzar, sino, error(NO_FIGHTERS);
	if(rebel.fighters.size() >= 1 && imperial.fighters.size() >= 1){
		cout << "Select fighter numbers: ";
		getline(cin, cadena);
		i = 0;
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
		
		
		cuantos = 0;
                // crear el ESCUADRON IMPERIAL
                // recorrer el vector numerosCazas
		//sacarlos del imperial y meterlos en un auxiliar seleccionados.
		for(p = 0; p  < numerosCazas.size(); p++){
			if(numerosCazas[p] >= 1 && numerosCazas[p] <= imperial.fighters.size()){
				launchimp.push_back(imperial.fighters[(numerosCazas[p] - 1)]);				
				imperial.fighters.erase(imperial.fighters.begin() + (numerosCazas[p] - 1));
                                cuantos++;
			}
		}
		if(cuantos == 0){
			error(WRONG_NUMBER);
		}
		else{
			// creamos ESCUADRON REBEL de forma aleatoria
			int margin = getRandomNumber(7);
			margin = margin - 3;
			int total = launchimp.size() + margin;
			if(total <= 0)
				total = 1;
			else{
				if(total > rebel.fighters.size()){
					total = rebel.fighters.size();
				}
			}
			int randomn, rsimulation;
			// obtenidos el numero de cazas los sacamos
			for(p = 1; p <= total; p++){
				randomn = getRandomNumber(rebel.fighters.size());
				launchRebel.push_back(rebel.fighters[randomn]);
				rebel.fighters.erase(rebel.fighters.begin() + randomn);
			}
                        // inicialmente se muestras ambos escuadrones
			cout << "Squadron A" << endl;
			listFighters(launchimp);
			cout << "Squadron B" << endl;
			listFighters(launchRebel);
                        
                //-----------------------------------------
                //---------simulación del combate ---------
                //-----------------------------------------
			do{
				rsimulation = getRandomNumber(100);
                                // en caso que el numero aleatorio sea menor que 10 se termina la simulación
				if(rsimulation >= 10){
                                        // se extrae un fighter y se guarda en naveImperial
					randomn = getRandomNumber(launchimp.size());
					naveImperial = launchimp[randomn];
					launchimp.erase(launchimp.begin()+randomn);
                                        // se extrae un fighter y se guarda en naveRebel
					randomn = getRandomNumber(launchRebel.size());
					naveRebel = launchRebel[randomn];
					launchRebel.erase(launchRebel.begin()+randomn);
					
					cout << "------" << endl;
					cout << "-- begin fight" << endl;
					listFighter(naveImperial);
					cout << endl;
					listFighter(naveRebel);
					cout << endl;		
					cout << "--" << endl;
					resultado = fight(naveImperial, naveRebel);
					listFighter(naveImperial);
					cout << endl;		
					listFighter(naveRebel);
					cout << endl;
					cout << "-- end fight" << endl;
                                        cout << "------" << endl;
                                        cout << endl;
                                        
                                        if(resultado == 1){
						rebel.wins++;
						imperial.losses++;
						launchRebel.push_back(naveRebel);
						rebel.credits = naveImperial.cost + rebel.credits;
					}
					if(resultado == 0){
						launchRebel.push_back(naveRebel);
						launchimp.push_back(naveImperial);
					}
					if(resultado == -1){
						imperial.wins++;
						rebel.losses++;
						launchimp.push_back(naveImperial);
						imperial.credits = naveRebel.cost + imperial.credits;
					}
				}
			}while(rsimulation >= 10 && launchRebel.size()>0 && launchimp.size()>0);
                        
                        
			cout << "Squadron A" << endl;
			listFighters(launchimp);
			cout << "Squadron B" << endl;
			listFighters(launchRebel);
                        // los cazas supervivientes se añaden al final de los vectores
			for(int p = 0; p < launchimp.size(); p++){
				imperial.fighters.push_back(launchimp[p]);
			}
			for(int p = 0; p < launchRebel.size(); p++){
				rebel.fighters.push_back(launchRebel[p]);
			}
		}
	}
	else{
		error(NO_FIGHTERS);
	}
}

void saveDataImperial(Ship &ship){
    
	ofstream fich;
	string nombreFichero;
	ShipBin sbin;
	FighterBin fbin;
	
	cout << "Filename: ";
	getline(cin, nombreFichero);
	fich.open(nombreFichero.c_str(), ios::binary);
        
	if(fich.is_open() == false){
		error(CANT_OPEN_FILE);
	}
	else{
		sbin.maxCapacity = ship.maxCapacity;
		sbin.wins = ship.wins;
                sbin.side = ship.side;
		sbin.losses = ship.losses;
                sbin.credits = ship.credits;
		fich.write((const char *) &sbin, sizeof(sbin));
                
		for(int i = 0; i < ship.fighters.size(); i++){
                    // mantener orden que pide la practica
			fbin.velocity = ship.fighters[i].velocity;
			fbin.attack = ship.fighters[i].attack;
			fbin.shield = ship.fighters[i].shield;
			fbin.cost = ship.fighters[i].cost;
                        
                        if(ship.fighters[i].type == "TIE-Interceptor"){
				strcpy(fbin.type, "ti");
			}
			if(ship.fighters[i].type == "TIE-Advanced"){
				strcpy(fbin.type, "ta");
			}
			if(ship.fighters[i].type == "TIE-Fighter"){
				strcpy(fbin.type, "tf");
			}
			if(ship.fighters[i].type == "TIE-Bomber"){
				strcpy(fbin.type, "tb");
			}
			fich.write((const char *) &fbin, sizeof(fbin));
		}
		fich.close();
	}	
}

void saveDataRebel(Ship &ship){
    
	ofstream fich;
	string nombreFichero;
	ShipBin sbin;
	FighterBin fbin;
	
	cout << "Filename: ";
	getline(cin, nombreFichero);
	fich.open(nombreFichero.c_str(), ios::binary);
        
	if(fich.is_open() == false){
		error(CANT_OPEN_FILE);
	}
	else{
		sbin.maxCapacity = ship.maxCapacity;
		sbin.wins = ship.wins;
                sbin.side = ship.side;
		sbin.losses = ship.losses;
                sbin.credits = ship.credits;
                
		fich.write((const char *) &sbin, sizeof(sbin));
		for(int i = 0; i < ship.fighters.size(); i++){
                    // mantener orden practica
			fbin.velocity = ship.fighters[i].velocity;
			fbin.attack = ship.fighters[i].attack;
			fbin.shield = ship.fighters[i].shield;
			fbin.cost = ship.fighters[i].cost;
                        
			if(ship.fighters[i].type == "X-Wing"){
				strcpy(fbin.type, "xw");
			}
			if(ship.fighters[i].type == "Y-Wing"){
				strcpy(fbin.type, "yw");
			}
			if(ship.fighters[i].type == "A-Wing"){
				strcpy(fbin.type, "aw");
			}
			if(ship.fighters[i].type == "B-Wing"){
				strcpy(fbin.type, "bw");
			}
			fich.write((const char *) &fbin, sizeof(fbin));
		}
		fich.close();
	}	
}

bool loadDataImperial(Ship &ship, string nombreFichero){
    

	FighterBin fbin;
	Fighter f;
	char respuesta;
	bool cargado = false, preguntar;
        ifstream fich;
	ShipBin sbin;

	if(nombreFichero == ""){
		preguntar = true;
		cout << "Filename: ";
		getline(cin, nombreFichero);
	}
	else{
		respuesta = 'y';
		preguntar = false;
	}
	
	fich.open(nombreFichero.c_str(), ios::binary);
	if(fich.is_open() == false){
		error(CANT_OPEN_FILE);
	}
	else{
		cargado = true;
		if(preguntar == true){
			cout << "This operation will erase current data. Confirm? (y/n) ";
			cin >> respuesta;
			cin.get();
		}
		if(respuesta == 'y'){	
			fich.read((char *)&sbin, sizeof(sbin));
			if(sbin.side == IMPERIAL){
				ship.side = sbin.side;
				ship.maxCapacity = sbin.maxCapacity;
				ship.credits = sbin.credits;
				ship.losses = sbin.losses;
				ship.wins = sbin.wins;
				ship.fighters.clear();
				fich.read((char *)&fbin, sizeof(fbin));
                                
				while(fich.eof() == false){
					f.velocity = fbin.velocity;
					f.attack = fbin.attack;
					f.shield = fbin.shield;
					f.cost = fbin.cost;
                                        
					if(strcmp(fbin.type, "tf") == 0){
						f.type = "TIE-Fighter";
					}
					if(strcmp(fbin.type, "tb") == 0){
						f.type = "TIE-Bomber";
					}
					if(strcmp(fbin.type, "ti") == 0){
						f.type = "TIE-Interceptor";
					}
					if(strcmp(fbin.type, "ta") == 0){
						f.type = "TIE-Advanced";
					}
					ship.fighters.push_back(f);
					fich.read((char *)&fbin, sizeof(fbin));
				}
				fich.close();
			}
		}
	}
	return cargado;
}

bool loadDataRebel(Ship &ship, string nombreFichero){

	ifstream fich;
	ShipBin sbin;
	FighterBin fbin;
	Fighter f;

	char respuesta;
	bool cargado = false;
	bool preguntar;
	
	if(nombreFichero == ""){
		preguntar= true;		
		cout << "Filename: ";
		getline(cin, nombreFichero);
	}
	else{
		respuesta = 'y';	
		preguntar = false;
	}

	fich.open(nombreFichero.c_str(), ios::binary);
	if(fich.is_open() == false){
		error(CANT_OPEN_FILE);
	}
	else{
		cargado = true;
		if(preguntar == true){		
			cout << "This operation will erase current data. Confirm? (y/n) ";
			cin >> respuesta;
			cin.get();
		}
		if(respuesta == 'y'){
			fich.read((char *)&sbin, sizeof(sbin));
			if(sbin.side == REBEL){

				ship.side = sbin.side;
				ship.maxCapacity = sbin.maxCapacity;
				ship.credits = sbin.credits;
				ship.losses = sbin.losses;
				ship.wins = sbin.wins;
				ship.fighters.clear();

				fich.read((char *)&fbin, sizeof(fbin));
				while(fich.eof() == false){
					f.velocity = fbin.velocity;
					f.attack = fbin.attack;
					f.shield = fbin.shield;
					f.cost = fbin.cost;
                                        
                                        if(strcmp(fbin.type, "yw") == 0){
						f.type = "Y-Wing";
					}
					if(strcmp(fbin.type, "aw") == 0){
						f.type = "A-Wing";
					}
					if(strcmp(fbin.type, "xw") == 0){
						f.type = "X-Wing";
					}

					if(strcmp(fbin.type, "bw") == 0){
						f.type = "B-Wing";
					}
					ship.fighters.push_back(f);
					fich.read((char *)&fbin, sizeof(fbin));
				}
				fich.close();
			}
		}
	}
	return cargado;
}

void exportData(Ship ship){

	ofstream fich;
	string nombre;
	cout << "Filename: ";
	getline(cin, nombre);
	fich.open(nombre.c_str());

	if(fich.is_open() == false){
		error(CANT_OPEN_FILE);
	}
	else{
		for(int i = 0; i < ship.fighters.size(); i++){
			fich << ship.fighters[i].type << " (v=";
			fich << ship.fighters[i].velocity << ", a=";
			fich << ship.fighters[i].attack << ", s=";
			fich << ship.fighters[i].shield << ", c=";
			fich << ship.fighters[i].cost << ")";
			fich << endl;
		}
		fich.close();
	}
}

void procesarLinea(Fighter &nuevo, string linea){
	
	int i = 0;
	string aux;

	aux = "";
	while(linea[i] != ' '){
		aux = aux + linea[i];
		i++;
	}
	nuevo.type = aux;
	i = i + 4;
	aux = "";
	while(linea[i] != ','){
		aux = aux + linea[i];
		i++;	
	}
	nuevo.velocity = atoi(aux.c_str());
	
	aux = "";
	i = i + 4;
	while(linea[i] != ','){
		aux = aux + linea[i];
		i++;
	}
	nuevo.attack = atoi(aux.c_str());
	aux = "",	
	i = i + 4;
	while(linea[i] != ','){
		aux = aux + linea[i];
		i++;
	}
	nuevo.shield = atoi(aux.c_str());
	aux = "";
	i = i + 4;
	while(linea[i] != ')'){
		aux = aux + linea[i];
		i++;
	}
	nuevo.cost = atoi(aux.c_str());
}

bool importData(Ship &ship, string nombre){
	string linea;
	ifstream fich;
	Fighter leido;
	bool parar, cargado;

	if(nombre == ""){
		cout << "Filename: ";
		getline(cin, nombre);
	}
	fich.open(nombre.c_str());
	if(fich.is_open() == false){
		error(CANT_OPEN_FILE);
		cargado = false;
	}
	else{
		cargado = true;
		getline(fich, linea);
		parar = false;	
		while(!fich.eof() && parar == false){
			procesarLinea(leido, linea);
			if(leido.type != "TIE-Fighter" && leido.type != "TIE-Bomber" && leido.type != "TIE-Advanced" && leido.type != "TIE-Interceptor"){
				error(WRONG_FIGHTER_TYPE);	
				getline(fich, linea);			
			}
			else{
				if(ship.fighters.size() == ship.maxCapacity){
					error(CAPACITY_EXCEEDED);
					parar = true;
				}
				else{
					ship.fighters.push_back(leido);
					getline(fich, linea);	
				}
			}
		}
	}
	return cargado;
}

bool procesarArgumentos(Ship &imperialDestroyer, Ship &rebelShip, int argc, char *argv[]){
	bool ok = false;
	string imperialTexto = "", imperialBinario = "", rebelBinario = "";
	int i;

	if(argc == 1 || argc == 3 || argc == 5 || argc == 7){
		ok = true;
		for(i = 1; i < argc && ok == true; i = i + 2){
                    
			if(strcmp(argv[i], "-l") == 0){
				if(imperialBinario == "")
					imperialBinario = argv[i+1];
				else
					ok = false;
			}
			else{
				if(strcmp(argv[i], "-L") == 0){
					if(rebelBinario == "")
						rebelBinario = argv[i+1];
					else
						ok = false;
				}
				else{
					if(strcmp(argv[i], "-i") == 0){
						if(imperialTexto == "")						
							imperialTexto = argv[i+1];
						else
							ok = false;
					}
					else{
						ok = false;					
					}
				}
			}
		}
		if(ok == true){
			if(imperialBinario != ""){
				ok = loadDataImperial(imperialDestroyer, imperialBinario);
			}			
			if(imperialTexto != "" && ok){
				ok = importData(imperialDestroyer, imperialTexto);
			}
			if(rebelBinario != "" && ok){
				ok = loadDataRebel(rebelShip, rebelBinario);
			}

		}
		else
			error(WRONG_ARGUMENTS);
	}
	else
		error(WRONG_ARGUMENTS);
	return ok;
}

int main(int argc,char *argv[])
{
	Ship imperialDestroyer, rebelShip;
	char option;
	bool argumentosOK;

	srand(1); 
	initializeShip(imperialDestroyer, IMPERIAL);
	initializeShip(rebelShip, REBEL);
        
	// carga los ficheros una vez que 
        // son ok
	argumentosOK = procesarArgumentos(imperialDestroyer, rebelShip, argc, argv);
	if(argumentosOK == true)
	{	
		do {
			
                        cout << "1- List ship info" << endl;
                        cout << "2- Add fighters" << endl;
                        cout << "3- Repair/improve fighter" << endl;
                        cout << "4- Launch squadron" << endl;
                        cout << "5- List enemy info" << endl;
                        cout << "s- Save data" << endl;
                        cout << "S- Save enemy data" << endl;
                        cout << "l- Load data" << endl;
                        cout << "L- Load enemy data" << endl;
                        cout << "x- Export data" << endl;
                        cout << "i- Import data" << endl;
                        cout << "q- Quit" << endl;
                        cout << "Option: ";
                    
			cin >> option;
			cin.get();
			switch (option) 
			{
				case '1':
					listShip(imperialDestroyer);
					break;
				case '2':
					addFighters(imperialDestroyer);
					break;
				case '3':
					improveFighter(imperialDestroyer);
					break;
				case '4':
					launchSquadron(imperialDestroyer, rebelShip);
					break;
				case '5':
					listShip(rebelShip);
					break;
				case 's':
					saveDataImperial(imperialDestroyer);
					break;
				case 'S':
					saveDataRebel(rebelShip);
					break;
				case 'l':
					loadDataImperial(imperialDestroyer, ""); // le paso el nombre vacio para que lo pida dentro
					break;
				case 'L':
					loadDataRebel(rebelShip, "");
					break;
				case 'x':
					exportData(imperialDestroyer);
					break;
				case 'i':
					importData(imperialDestroyer, "");
					break;
				case 'q':
					break;
				default: 
					error(UNKNOWN_OPTION);
					break;
			}
		} while (option != 'q');
	}
}
