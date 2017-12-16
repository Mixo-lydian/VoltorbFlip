#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include "vf.h"

using std::cout;
using std::endl;
using std::cin;
using std::stringstream;
using std::string;
using std::system;

void playGame(GameBoard &g, Player &p) {
	int row = 1;
	int column = 1;
	int coins = 0;
	int cardvalue = 0;
	bool keepPlaying = true;
	int typecheck = 0;
	g.print();
	while (keepPlaying) {
		cout << "0 = Flip; 1 = Flip row; 2 = Flip column; 3 = Flag; 4+ = Quit" << endl << "Choose an option: ";
		cin >> typecheck;
		if (typecheck >= 4) keepPlaying = false;
		if (keepPlaying) {
			if (typecheck == 0 || typecheck == 1 || typecheck == 3) {
				cout << "Enter row (1-5): ";
				cin >> row;
			}
			if (typecheck == 0 || typecheck == 2 || typecheck == 3) {
				cout << "Enter column (1-5): ";
				cin >> column;
			}
			cardvalue = g.interact(row, column, typecheck);
			system("CLS");
			g.print();
			if (cardvalue >= 1) {
				if (coins == 0) coins += cardvalue;
				else coins *= cardvalue;
				cout << "Coins: " << coins << endl;
			}
			else if (cardvalue == 0) {
				coins = 0;
				cout << "You have lost the game!" << endl;
				keepPlaying = false;
			}
			if (g.check_victory()) {
				cout << "You have won the game!" << endl;
				keepPlaying = false;
			}
		}
	}
	p.update_coins(coins);
	return;
}

int main() {
	srand((int)time(0));
	Player p;
	bool repeat = true;
	int repeatCheck = 0;
	while (repeat) {
		GameBoard g;
		playGame(g, p);
		cout << "Would you like to play again? (0- = Yes, 1+ = No): ";
		cin >> repeatCheck;
		if (repeatCheck >= 1) repeat = false;
		else system("CLS");
		p.level_up();
	}

	cout << "Final total: " << p.get_coins() << endl;

	return 0;
}