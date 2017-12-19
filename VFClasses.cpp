#include "vf.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <algorithm>

using std::cout;
using std::srand;
using std::endl;
using std::setw;
using std::string;
using std::for_each;

int WIDTH = 7;

Card::Card(int level = 1) {
	if (level > 7) level = 7;
	flipped = false;
	flagged = false;
	int rng = rand() % 100;
	switch (level) {
	case 1:
		if (rng < 25) value = 0; // 25%
		else if (rng < 80 && rng >= 25) value = 1; // 55%
		else if (rng < 95 && rng >= 80) value = 2; // 15%
		else if (rng < 100 && rng >= 95) value = 3; // 5%
		else throw;
		break;
	case 2:
		if (rng < 25) value = 0; // 25%
		else if (rng < 70 && rng >= 25) value = 1; // 45%
		else if (rng < 90 && rng >= 70) value = 2; // 20%
		else if (rng < 100 && rng >= 90) value = 3; // 10%
		else throw;
		break;
	case 3:
		if (rng < 25) value = 0; // 25%
		else if (rng < 65 && rng >= 25) value = 1; // 40%
		else if (rng < 85 && rng >= 65) value = 2; // 20%
		else if (rng < 100 && rng >= 85) value = 3; // 15%
		else throw;
		break;
	case 4:
		if (rng < 20) value = 0; // 20%
		else if (rng < 60 && rng >= 20) value = 1; // 40%
		else if (rng < 85 && rng >= 60) value = 2; // 25%
		else if (rng < 100 && rng >= 85) value = 3; // 15%
		else throw;
		break;
	case 5:
		if (rng < 20) value = 0; // 20%
		else if (rng < 50 && rng >= 20) value = 1; // 30%
		else if (rng < 85 && rng >= 50) value = 2; // 35%
		else if (rng < 100 && rng >= 85) value = 3; // 15%
		else throw;
		break;
	case 6:
		if (rng < 20) value = 0; // 20%
		else if (rng < 45 && rng >= 20) value = 1; // 25%
		else if (rng < 80 && rng >= 45) value = 2; // 35%
		else if (rng < 100 && rng >= 80) value = 3; // 20%
		else throw;
		break;
	case 7:
		if (rng < 20) value = 0; // 20%
		else if (rng < 35 && rng >= 20) value = 1; // 15%
		else if (rng < 75 && rng >= 35) value = 2; // 40%
		else if (rng < 100 && rng >= 75) value = 3; // 25%
		else throw;
		break;
	}
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

GameBoard::GameBoard(int playerLevel = 1) {
	this->playerLevel = playerLevel;
	flippedCards = 0;
	victory = false;
	bool regenerateCheck = true;

	while (regenerateCheck) {
		unneededFlips = 0;
		int voltorbs = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				Card c(playerLevel);
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
					voltorbs++;
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

		if (unneededFlips != 25 && voltorbs != 0) regenerateCheck = false;
	}
}

void GameBoard::print() {
	// Debugging to show # of unneeded flips and flipped cards
	// cout << unneededFlips << " " << flippedCards << " " << playerLevel << endl;

	cout << "===== VOLTORB FLIP =====" << endl;
	cout << "Your level: " << playerLevel << endl << endl;
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
	for_each(columnSums.begin(), columnSums.end(), [](Indicator x) { x.print(WIDTH - 2); });
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

Player::Player() {
	coins = 0;
	level = 1;
}

int Player::get_coins() const {
	return coins;
}

int Player::get_level() const {
	return level;
}

void Player::update_coins(int amount) {
	coins += amount;
	return;
}

void Player::level_up() {
	level++;
	return;
}