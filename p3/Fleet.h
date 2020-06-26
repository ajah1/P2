
#ifndef _fleet
#define _fleet
#include <iostream>
#include "Fighter.h"
#include <vector>
using namespace std;

const bool IMPERIAL = false;
const bool REBEL = true;
const int CREDITS_INITIAL = 2000;

class Fleet{
	protected:
		vector<Fighter> fighters;
		int credits;
		int wins;
		int losses;
	private:
		// módulo adicional, reutilizado de la practica 2
		// para recorrer el vector de fighters y mostrarlo
		// por pantalla
		void listFighters(const vector<Fighter> &vf) const;
	public:
		Fleet();
		Fighter getFighter(int n) const;
		int getNumFighters() const;
		int getCredits() const;
		bool fight(Fleet &enemy);
		bool addFighters(string type, int number, bool side);
		void improveFighter(int nf, char feature, int amount, int cost);
		friend ostream &operator<<(ostream &os, const Fleet &flota);
};

#endif
