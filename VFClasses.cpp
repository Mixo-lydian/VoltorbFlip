#include "vf.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using std::cout;
using std::srand;
using std::endl;
using std::setw;

int WIDTH = 7;

Card::Card() {
	value = rand() % 4;
	flipped = false;
}

void Card::print() {
	if (!flipped) cout << setw(WIDTH) << "?";
	else cout << setw(WIDTH) << value;
	return;
}

int Card::get_value() const {
	return value;
}

Indicator::Indicator() {
	value = 0;
	voltorbs = 0;
}

Indicator::Indicator(int value, int voltorbs) {
	this->value = value;
	this->voltorbs = voltorbs;
}

void Indicator::print(int width = WIDTH) {
	cout << setw(width) << value << "/" << voltorbs;
}

GameBoard::GameBoard() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			Card c;
			board[i][j] = c;
		}
	}

	// Row indicator
	for (int i = 0; i < 5; i++) {
		int sum = 0;
		int counter = 0;
		for (int j = 0; j < 5; j++) {
			if (board[i][j].get_value() == 0) counter++;
			sum += board[i][j].get_value();
		}
		rowSums.push_back(Indicator(sum, counter));
	}

	// Column indicator
	for (int i = 0; i < 5; i++) {
		int sum = 0;
		int counter = 0;
		for (int j = 0; j < 5; j++) {
			if (board[j][i].get_value() == 0) counter++;
			sum += board[j][i].get_value();
		}
		columnSums.push_back(Indicator(sum, counter));
	}
}

void GameBoard::print() {
	cout << "==||=====||=====||=====||=====||=====||=====||" << endl;
	for (int i = 0; i < 5; i++) {
		int j = 0;
		while (j < 6) {
			if (j < 5) {
				board[i][j].print();
			}
			else if (j == 5) rowSums[i].print();
			j++;
		}
		cout << endl << "==||=====||=====||=====||=====||=====||=====||" << endl;
	}
	cout << " ";
	for (int i = 0; i < 5; i++) {
		columnSums[i].print(WIDTH - 2);
	}
	cout << endl;
}