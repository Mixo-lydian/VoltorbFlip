#include <iostream>
#include <vector>
#include <string>

#ifndef VF_H
#define VF_H

using std::vector;
using std::string;

class Tile {
public:
	virtual void print(int width) = 0;
};

class Card : public Tile {
public:
	Card(int level);
	void print(int width);
	int get_value() const;
	bool get_flipped() const;
private:
	int value;
	bool flipped;
	bool flagged;
	friend class GameBoard;
};

class Indicator : public Tile {
public:
	Indicator();
	Indicator(int value, int voltorbs);
	void print(int width);
private:
	int value;
	int voltorbs;
	friend class GameBoard;
};

class GameBoard {
public:
	GameBoard(int playerLevel);
	Card board[5][5];
	void print();
	int interact(int row, int column, int type);
	bool check_victory() const;
private:
	vector<Indicator> rowSums;
	vector<Indicator> columnSums;
	int unneededFlips;
	int flippedCards;
	int playerLevel;
	bool victory;
};

class Player {
public:
	Player();
	int get_coins() const;
	int get_level() const;
	void update_coins(int amount);
	void level_up();
private:
	int coins;
	int level;
};

#endif