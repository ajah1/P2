
#ifndef _Fighter
#define _Fighter
#include <iostream>
using namespace std;

// Fighters table 
const int MAXFIGHTERS=8;
// abbreviations (to use in option "Add fighter")
const string FIGHTERABR[]= { "tf", "tb", "ti", "ta", 
                             "xw", "yw", "aw", "bw"
};
class Fighter{
	protected:
		string type;
		int velocity;
		int attack;
		int shield;
		int cost;
	public:
		Fighter(string abr = "");
		string getType() const;
		int getVelocity() const;
		int getAttack() const;
		int getShield() const;
		int getCost() const;
		void increaseVelocity(int amount);
		void increaseAttack(int amount);
		void increaseShield(int amount);
		void increaseCost(int amount);
		bool isImperial() const;
		int fight(Fighter &enemy);
		friend ostream &operator<<(ostream &os, const Fighter &f);
};
#endif


