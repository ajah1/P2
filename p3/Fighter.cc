
#include "Fighter.h"
#include "Util.h"
#include <stdexcept>

struct FighterData {
string type;
int velocity,attack,shield,cost;
};
const FighterData FIGHTERTABLE[] = {
{ "TIE-Fighter",      150,  75, 30,  45 },
{ "TIE-Bomber",        80, 150, 45,  75 },
{ "TIE-Interceptor",  180,  65, 30,  55 },
{ "TIE-Advanced",     160,  80, 90,  95 },
{ "X-Wing",           175,  90, 75,  65 },
{ "Y-Wing",            90, 150, 90,  90 },
{ "A-Wing",           200,  60, 50,  45 },
{ "B-Wing",           120, 200, 90, 100 }
};

Fighter::Fighter(string abr){
	int pos;
	bool encontrado = false;
	if(abr == ""){
		type = "";
		shield = 0;
		cost = 0;
		velocity = 0;
		attack = 0;
	}
	else{
		// buscamos la abreviatura
		for(int i = 0; i < 8 && encontrado == false; i++){
			if(FIGHTERABR[i] == abr){
				encontrado = true;
				pos = i;
			}	
		}
		// si encontrado = false finaliza el programa
		if(encontrado == false){
			throw invalid_argument("wrong fighter type");
		}
		type = FIGHTERTABLE[pos].type;
		velocity = FIGHTERTABLE[pos].velocity;
		attack = FIGHTERTABLE[pos].attack;
		shield = FIGHTERTABLE[pos].shield;
		cost = FIGHTERTABLE[pos].cost;
	}
}

string Fighter::getType() const{
	return type;
}
int Fighter::getVelocity() const{
	return velocity;
}
int Fighter::getAttack() const{
	return attack;
}
int Fighter::getShield() const{
	return shield;
}
int Fighter::getCost() const{
	return cost;
}
void Fighter::increaseVelocity(int amount){
	velocity += amount;
}
void Fighter::increaseAttack(int amount){
	attack += amount;
}
void Fighter::increaseShield(int amount){
	shield += amount;
}
void Fighter::increaseCost(int amount){
	cost += amount;
}

bool Fighter::isImperial() const{
	bool es = false;
	for(int i = 0; i < 4 && !es; i++){
		if(type == FIGHTERTABLE[i].type){
			es = true;
		}
	}
	return es;
}

// codigo utilizado de la practica 2
int Fighter::fight(Fighter &enemy){
	const int divisor = 300;
	const int min = 5, max = 95;
	int n, d, r;
	double umbral, suma;
	
	r = -1;
	do{
		n = Util::getRandomNumber(100);
		if(n < min || n > max){
			r = 0;
		}
		if(r != 0){
			suma = velocity + enemy.velocity;
			umbral = (100*velocity)/suma;
			if(n <= umbral){
				d = (n * attack)/divisor;
				enemy.shield -= d;
				
			}
			if(n > umbral){
				d = ((100-n)*enemy.attack)/divisor;
				shield -= d;		
			}
		}
	}while(r != 0 && shield > 0 && enemy.shield > 0);
	r = 0;	
	if(enemy.shield <= 0 && shield > 0){
		r = -1;
	}
	else{
		if(shield <= 0 && enemy.shield > 0){
			r = 1;
		}
	}
	return r;
}

ostream &operator<<(ostream &os, const Fighter &f){
	os << f.type << " (v=";
	os << f.velocity << ", a=";
	os << f.attack << ", s=";
	os << f.shield << ", c=";
	os << f.cost << ")";
	return os;
}





