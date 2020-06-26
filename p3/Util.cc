
#include "Util.h"
#include <iostream>
using namespace std;

void Util::error(int n){
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
	}
}

int Util::getRandomNumber(int max){
   // Genera un número aleatorio entre 0 y maximo-1
   int na;
   na = (int)(((double)max)*rand()/(RAND_MAX+1.0));
   return na;
}
