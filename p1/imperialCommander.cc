#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

typedef struct {
     string type;  // TIE Fighter, X-Wing, ...
     int velocity;
     int attack;    // attack power
     int shield;    // current shield status. if <=0, the fighter is considered destroyed
     int cost;      // cost in credits
} Fighter;


typedef struct {
    bool side;       // IMPERIAL or REBEL
    int maxCapacity; // maximum capacity of fighters allowed in the ship
    int credits;      // credits remaining to buy fighters, make repairs or improvements
    int wins;         // enemy fighters destroyed
    int losses;        // fighters lost
    vector<Fighter>fighters;  // fighters inside the ship
} Ship;


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

const int initialImperialShipDotation[] = {10,5,5,5,0,0,0,0};
// 10 TIE-Fighters, 5 TIE-Bombers, 5 TIE-Interceptors, 5 TIE-Advanced, 0 X-Wing, ....

const int initialRebelShipDotation[] = {0,0,0,0,10,5,8,5};
// 0 TIE-Fighters ..., 10 X-Wing, 5 Y-Wing, 8 A-Wing, 5 B-Wing

// ship initial credits
const int CREDITS_INITIAL=2000;

// error numbers
const int UNKNOWN_OPTION=1;
const int WRONG_FIGHTER_TYPE=2;
const int CAPACITY_EXCEEDED=3;
const int WRONG_NUMBER=4;
const int NO_FUNDS=5;
const int NO_FIGHTERS=6;

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
      break;
  }
}

//--------------------------------------------------- 1. list-info - 5. list enemy
int getRandomNumber(int max)
{
   // Genera un número aleatorio entre 0 y maximo-1
   int na;
   
   na = (int)(((double)max)*rand()/(RAND_MAX+1.0));
   return na;
}

void initializeShip(Ship &ship,bool side)
{

        int i, j;
        
        ship.wins = 0;
        ship.losses = 0;
        ship.side = side;
        ship.credits = CREDITS_INITIAL;
        
        switch(side){
            case IMPERIAL:
                ship.maxCapacity = IMPSHIPCAPACITY;
                
                for(i=0; i < MAXFIGHTERS; i++){
                    for(j=0; j < initialImperialShipDotation[i]; j++){
                        ship.fighters.push_back(FIGHTERTABLE[i]);
                        ship.credits = ship.credits -  FIGHTERTABLE[i].cost;
                    }
                }
                break;
            
            case REBEL: 
                ship.maxCapacity = REBSHIPCAPACITY;
                
                for(i=0; i < MAXFIGHTERS; i++){
                    for(j=0; j < initialRebelShipDotation[i]; j++){ 
                        ship.fighters.push_back(FIGHTERTABLE[i]);
                        ship.credits = ship.credits - FIGHTERTABLE[i].cost;
                    }
                }
                break;
        }
}

void listFighter(const Fighter &f)
{
    cout << f.type << " (v=" << f.velocity;
    cout << " ,a=" << f.attack;
    cout << " ,s=" << f.shield;
    cout << " ,c=" << f.cost << ")";
}

void listFighters(const vector<Fighter> &vf)
{
    int i;
    
    for(i=0; i < vf.size(); i++){
        cout << "[" << i+1 << "]";
        listFighter(vf[i]);
        cout << endl;
    }
}

void listShip(const Ship &ship)
{
    cout << "Ship info: max. capacity= " << ship.maxCapacity << ", side=";
    
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

//--------------------------------------------------- 2. Add
bool addFighter(Ship &ship)
{
    int num, cost, i, pos;
    bool insertar;
    string tipo;
    
    pos  = -1;
    insertar = false;
    
    cout << "Enter fighter type: (tf/tb/ti/ta): ";
    getline(cin, tipo);
    
    for(i = 0; i < 4; i++){
        if(FIGHTERABR[i] == tipo){
            pos = i;
        }
    }
    
    if(pos == -1){
        error(WRONG_FIGHTER_TYPE);
    }    
    else{
        cout << "Number of fighters: ";
        cin >> num;
        
        cost = FIGHTERTABLE[pos].cost*num;
        if(cost > ship.credits){
            error(NO_FUNDS);
        }
        else{
            
            if(ship.fighters.size() + num >= ship.maxCapacity){
                error(CAPACITY_EXCEEDED);
            }
            else{
                insertar = true;
                ship.credits = ship.credits - cost;
                for(i=1; i < num; i++){
                    ship.fighters.push_back(FIGHTERTABLE[pos]);
                }
            }
        }
    }
    return insertar;
}

//--------------------------------------------------- 3. Repair/improve
bool improveFighter(Ship &ship)
{
        bool mejorado;
        int numero;
        char mejora, respuesta;
        int unidad, coste;
        mejorado = false;
        cout << "Select fighter number: ";
        cin >> numero;
        if(numero >= 1 && numero <= ship.fighters.size() ){
                cout << "What to improve (v/a/s)?";
                cin >> mejora;
                if(mejora == 'v'|| mejora == 'a' || mejora == 's' ){
                    cout << "Amount: ";
                    cin >> unidad;
                    switch (mejora) 
                    {
                    case 'v': 
                        coste = unidad * 2;
                        break;
                    case 'a':
                        coste = unidad * 3;
                        break;
                    case 's':
                        coste = (unidad+1)/2;
                        break;
                    }
                    if(coste < ship.credits){
                        cout << "That will cost you " << coste << " credits. Confirm? (y/n) ";
                        cin >> respuesta;
                        if(respuesta == 'y'){
                            switch (mejora) 
                            {
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
                            ship.credits = ship.credits - coste;
                            mejorado = true;
                    }
                    else{
                       error(NO_FUNDS);
                    }
                }
                else{
                    error(UNKNOWN_OPTION);
                }
            }
        }
        else{
                error(WRONG_NUMBER);
        }
        return mejorado;
}

//--------------------------------------------------- 4- Launch fighter
int fight(Fighter &fg1,Fighter &fg2)
{
        
        int resultado;
        double umbral;
        int n; // aleatorio
        int d; // daño
        
        resultado = -1;
        do{
                n = getRandomNumber(100);
                if(n < 5 || n > 95){
                        resultado = 0;
                }
                else{
                            umbral = (100*fg1.velocity)/(fg1.velocity + fg2.velocity);
                        if(n <= umbral){
                                // daño al imperial
                                d = (n * fg1.attack)/300;
                                fg2.shield -= d;
                                
                        }
                        else{
                                // daño al rebelde
                                d = ((100-n)*fg2.attack)/300;
                                fg1.shield -= d;		
                        }
                }
        }while(resultado != 0 && fg1.shield > 0 && fg2.shield > 0);
        
        if(fg2.shield <= 0){
                resultado = -1;
        }
        else{
                if(fg1.shield <= 0){
                        resultado = 1;
                }
        }
        return resultado;
    
    
    
}

void launchFighter(Ship &imperial,Ship &rebel)
{
    
    int resultadoPelea, numRebelde, num;
    Fighter lanzadoImperial, lanzadoRebelde;
    
    if(imperial.fighters.size() >= 1 && rebel.fighters.size() >= 1){
        
        // guardar los fighters lanzados en los vectores auxiliares y borrar
        // de los disponibles tanto como de rebel como de imperial
            
            cout << "Select fighter number: ";
            cin >> num;
            
            numRebelde = getRandomNumber(rebel.fighters.size());
            // guardado y borrado del fighter rebelde
            lanzadoRebelde = rebel.fighters[numRebelde];
            rebel.fighters.erase(rebel.fighters.begin() + numRebelde);
            
            // guardado y borado del fighter imperial
            lanzadoImperial = imperial.fighters[num - 1];
            imperial.fighters.erase(imperial.fighters.begin() + num - 1);
            
            cout << "-- begin fight" << endl;
            listFighter(lanzadoImperial);
            cout << endl;
            listFighter(lanzadoRebelde);
            cout << endl;		
            cout << "--" << endl;
            
            // se le pasa al modulo 'fight' los fighters donde se calculará el ganador
            resultadoPelea = fight(lanzadoImperial, lanzadoRebelde);
            listFighter(lanzadoImperial);
            cout << endl;		
            listFighter(lanzadoRebelde);
            cout << endl;
            cout << "-- end fight" << endl;
            
            if(resultadoPelea == 1){
                
                rebel.fighters.push_back(lanzadoRebelde);
                rebel.credits =  rebel.credits + lanzadoImperial.cost;
                rebel.wins++;
                imperial.losses++;

                }
                else{
                        if(resultadoPelea == -1){
                            
                            imperial.fighters.push_back(lanzadoImperial);
                            imperial.credits = imperial.credits + lanzadoRebelde.cost;
                            imperial.wins++;
                            rebel.losses++;
                            

                        }
                        else{
                                rebel.fighters.push_back(lanzadoRebelde);
                                imperial.fighters.push_back(lanzadoImperial);
                        }
                }
    }
    else{
        error(NO_FIGHTERS);
    }
}
//--------------------------------------------------- 

int main(int argc,char *argv[])
{
   Ship imperialDestroyer, rebelShip;
   char option;

   srand(1); // inicializar secuencia de números aleatorios: NO TOCAR   
   initializeShip(imperialDestroyer, IMPERIAL);
   initializeShip(rebelShip, REBEL);

   do {

        cout << "1- List ship info" << endl;
        cout << "2- Add fighters" << endl;
        cout << "3- Repair/improve fighter" << endl;
        cout << "4- Launch fighter" << endl;
        cout << "5- List enemy info" << endl;
        cout << "q- Quit" << endl;
        cout << "Option: ";
        cin >> option;
        
                switch (option) 
                {
                        case '1':
                                listShip(imperialDestroyer);
                                break;
                        case '2':
                                addFighter(imperialDestroyer);
                                break;
                        case '3':
                                improveFighter(imperialDestroyer);
                                break;
                        case '4':
                                launchFighter(imperialDestroyer, rebelShip);
                                break;
                        case '5':
                                listShip(rebelShip);
                                break;
                        case 'q':
                                break;
                        default: 
                                error(UNKNOWN_OPTION);
                                break;
                }
        }while (option != 'q');
}
