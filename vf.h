#include <iostream>
#include <vector>
#include <string>

#ifndef VF_H
#define VF_H

using std::vector;

class Card {
public:
	Card();
	void print();
	int get_value() const;
private:
	int value;
	bool flipped;
};

class Indicator {
public:
	Indicator();
	Indicator(int value, int voltorbs);
	void print(int width);
private:
	int value;
	int voltorbs;
};

class GameBoard {
public:
	GameBoard();
	void print();
	Card board[5][5];
private:
	vector<Indicator> rowSums;
	vector<Indicator> columnSums;
};

#endif