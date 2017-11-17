#include "vf.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cassert>

using std::cout;
using std::srand;
using std::endl;
using std::setw;
using std::string;

int WIDTH = 7;

Card::Card() {
	value = rand() % 4;
	flipped = false;
	flagged = false;
}

void Card::print(int width = WIDTH) {
	if (flipped) cout << setw(width) << value;
	else if (flagged) cout << setw(width) << "F";
	else cout << setw(width) << "?";
	return;
}

int Card::get_value() const {
	return value;
}

bool Card::get_flipped() const {
	return flipped;
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
	unneededFlips = 0;
	flippedCards = 0;
	victory = false;

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
			if (board[i][j].get_value() == 0) {
				counter++;
				unneededFlips++;
			}
			else if (board[i][j].get_value() == 1) unneededFlips++;
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
	cout << unneededFlips << " " << flippedCards << endl << "==||=====||=====||=====||=====||=====||=====||" << endl;
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
	cout << endl << endl;
}

int GameBoard::interact(int row, int column, int type = 0) {
	assert(row >= 1 && row <= 5 && column >= 1 && column <= 5);
	int result = 1;
	switch (type) {
	case 0:
		if (board[row - 1][column - 1].get_flipped()) {
			cout << "You already flipped this card!" << endl << endl;
		}
		else if (!board[row - 1][column - 1].get_flipped()) {
			board[row - 1][column - 1].flipped = true;
			if (board[row - 1][column - 1].get_value() > 1) flippedCards++;
			result = board[row - 1][column - 1].get_value();
		}
		break;
	case 1:
		for (int i = 0; i < 5; i++) {
			if (!board[row - 1][i].get_flipped()) result *= board[row - 1][i].get_value();
			if ((board[row - 1][i].get_value() > 1) && !board[row - 1][i].get_flipped()) flippedCards++;
			board[row - 1][i].flipped = true;
		}
		break;
	case 2:
		for (int i = 0; i < 5; i++) {
			if (!board[i][column - 1].get_flipped()) result *= board[i][column - 1].get_value();
			if ((board[i][column - 1].get_value() > 1) && !board[i][column - 1].get_flipped()) flippedCards++;
			board[i][column - 1].flipped = true;
		}
		break;
	case 3:
		bool temp = !board[row - 1][column - 1].flagged;
		board[row - 1][column - 1].flagged = temp;
		result = -1;
		break;
	}
	if ((flippedCards == 25 - unneededFlips) && board[row - 1][column - 1].get_value() != 0) victory = true;
	return result;
}

bool GameBoard::check_victory() const {
	return victory;
}