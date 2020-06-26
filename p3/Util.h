
#ifndef _util
#define _util
#include <cstdlib>

const int UNKNOWN_OPTION = 1;
const int WRONG_FIGHTER_TYPE = 2;
const int CAPACITY_EXCEEDED = 3;
const int WRONG_NUMBER = 4;
const int NO_FUNDS = 5;
const int NO_FIGHTERS = 6;

class Util{
	public:
		static int getRandomNumber(int max); 
		static void error(int n);
};
#endif
