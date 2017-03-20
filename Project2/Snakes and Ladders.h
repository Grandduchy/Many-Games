#ifndef SNAKE_N_LADD_CLASS
#define SNAKE_N_LADD_CLASS
#include <map>
#include <random>
#include <ctime>
#include <windows.h>
#include "Functions.h"
class Player;

class SnakeLadder {
	friend ostream& operator<<(ostream& os, const SnakeLadder& sl);
public:
	SnakeLadder();
	ostream& instructions(ostream&,bool) const;
	ostream& print_snakes(ostream&) const;
	ostream& print_ladders(ostream&) const;
	bool play_again();
private:
	vector<vector<int>> board;
	std::map<int, int> ladders; //first i = bottom of ladd, sec i = top of ladd.
	std::map<int, int> snakes; //first i = top of snk, sec i = bottom of snk.
	void create_ladd_n_snake();
};

ostream& SnakeLadder::print_ladders(ostream& os) const {
	os << "Ladders at positions : ";
	for (auto begin = ladders.begin(); begin != ladders.end(); begin++) {
		os << begin->first << "," << begin->second << " | ";
	}
	return os;
}

ostream& SnakeLadder::print_snakes(ostream& os) const {
	os << "Snakes at positions : ";
	for (auto begin = snakes.begin(); begin != snakes.end(); begin++) {
		os << begin->first << "," << begin->second << " | ";
	}
	return os;
}

ostream& SnakeLadder::instructions(ostream& os,bool full) const  {
	if (full) {
		os << "Each player is placed at the start(1) position." << endl;
		os << "Each player then rolls a dice to determine amound of moves forwards." << endl;
		os << "Aim for ladders! At the bottom they will bring you up higher." << endl;
		os << "Watch out for snakes, At the top they will being you down." << endl;
	}
	os << "Input 'roll' to roll the dice, 'board' to view board, 'ladders'/'snakes' to know of their positions." << endl;
	return os;
}

SnakeLadder::SnakeLadder() {
	for (int x = 100; x != 0; x-=10) {
		vector<int> row;
		for (int z = 0; z != 10; z++) {
			row.push_back(x - z);
		}
		board.push_back(row);
	}
	create_ladd_n_snake();
}

ostream& operator<<(ostream& os, const SnakeLadder& sl) {
	int counter = 10;
	for (const vector<int>& row : sl.board) {
		if ((counter & 1) == 0) { // 100  99 98...
			for (vector<int>::const_iterator tile_iter = row.cbegin(); tile_iter != row.cend(); tile_iter++) {
				os << *tile_iter << "      ";
			}
		}
		else { //90 91 92 93...
			for (auto rev_tile_iter = row.crbegin(); rev_tile_iter != row.crend(); rev_tile_iter++) {
				if (*rev_tile_iter >= 10)
					os << *rev_tile_iter << "      ";
				else
					os << *rev_tile_iter << "       ";
			}
		}
		counter--;
	}
	return os;
}

void SnakeLadder::create_ladd_n_snake() {
	static std::default_random_engine engine(std::time(0));
	static std::uniform_int_distribution<unsigned> rng_amt(5, 7);
	static std::uniform_int_distribution<unsigned> rng(0, 100);
	int amt = rng_amt(engine);
	while (amt != 0) {
		int x = rng(engine), y = rng(engine);
		if (x > y)
			ladders.insert(std::make_pair(y, x));
		else
			ladders.insert(std::make_pair(x, y));
		amt--;
	}
	amt = rng_amt(engine);
	while (amt != 0) {
		int x = rng(engine), y = rng(engine);
		if (x > y)
			snakes.insert(std::make_pair(x, y));
		else
			snakes.insert(std::make_pair(y, x));
		amt--;
	}
}



class SNL_Player_Structure {
public:
	SNL_Player_Structure(const int& i = 0, bool b = false) : position(i),instant(b) {};
	virtual void roll_dice() = 0;
	bool won = false;
protected:
	int position;
	bool instant;
};

class SNI_Ai : public SNL_Player_Structure {
public:
	SNI_Ai(const string& player,const int& pos = 0, bool inst = true) : SNL_Player_Structure(pos,inst), name(player){};
	virtual void roll_dice() override;
private:
	string name;
};

void SNI_Ai::roll_dice() {
	static std::default_random_engine engine;
	static std::uniform_int_distribution<unsigned> rng(1, 6);
	int roll = rng(engine);
	position += roll;
	if (position >= 100) {
		won = true;
		position = 100;
	}
	cout << name << " has rolled a " << roll << " and is now on tile " << position << endl;
	if (!instant) 
		Sleep(500);
}

class SNI_Player : public SNL_Player_Structure {
public:
	SNI_Player(const int& i = 0, bool b = false) : SNL_Player_Structure(i, b) {};
	virtual void roll_dice() override;
};

void SNI_Player::roll_dice() {
	static std::default_random_engine engine;
	static std::uniform_int_distribution<unsigned> rng(1, 6);
	int roll = rng(engine);
	cout << "It's now your turn. " << endl;
	if (!instant) {
		string input = check("Invalid input, enter 'roll' to roll the dice, or 'instantaneous' for the game to roll it for you.", { "roll","instantaneous" });
		if (input == "instantaneous")
			instant = true;
	}
	position += roll;
	if (position >= 100) {
		won = true;
		position = 100;
	}
	cout << "You have rolled a " << roll << " and you are now at tile " << position << endl;
	
	if (!instant)
		Sleep(500);
}

#endif // !SNAKE_N_LADD_CLASS

#ifndef SNAKE_N_LADD_GAME
#define SNAKE_N_LADD_GAME

void run_snake_n_ladd() {

}

#endif // !SNAKE_N_LADD_GAME
