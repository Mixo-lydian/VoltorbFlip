#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
#include <iomanip>
#include "vf.h"

using std::cout;

int main() {
	srand((int)time(0));

	GameBoard g;
	g.print();
	return 0;
}