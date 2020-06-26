
#ifndef _ship
#define _ship
#include "Fleet.h"
#include "Fighter.h"
#include <iostream>
using namespace std;

class Ship{
	protected:
		bool side;
		int maxCapacity;
		Fleet fleet;
	public:
		Ship(bool side);
		bool addFighters(string fighters);
		bool improveFighter();
		void fight(Ship &enemy);
		friend ostream &operator<<(ostream &os, const Ship &ship);
};
#endif 
