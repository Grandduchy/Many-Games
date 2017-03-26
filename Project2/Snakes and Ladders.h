#include <map>
#include <random>
#include <ctime>
#include <windows.h>
#include "Functions.h"
#ifndef SNAKE_N_LADD_CLASS
#define SNAKE_N_LADD_CLASS
/* Forward Declarations */
class SNI_Ai; 
class SNI_Player;
/* Classes */
class SnakeLadder {
	friend std::pair<bool, int> on_snake(const SnakeLadder& sl, const int& i);
	friend std::pair<bool, int> on_ladder(const SnakeLadder& sl, const int& i);
	friend ostream& operator<<(ostream& os, const SnakeLadder& sl);
public:
	SnakeLadder();
	ostream& instructions(ostream&,bool) const;
	ostream& print_snakes(ostream&) const;
	ostream& print_ladders(ostream&) const;
private:
	vector<vector<int>> board;
	std::map<int, int> ladders; //first i = bottom of ladd, sec i = top of ladd.
	std::map<int, int> snakes; //first i = top of snk, sec i = bottom of snk.
	void create_ladd_n_snake();
};
/*Snake Ladder Member functions*/
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
		os << "Watch out for snakes, At the top they will bring you down." << endl;
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
		if ((counter & 1) == 0) { //is even // 100  99 98...
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
	static std::uniform_int_distribution<unsigned> rng(10, 90);
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
/* Snakes and Ladders players/user classes and methods */
class SNL_Player_Structure { // the general structure of the player and ai classes
public:
	SNL_Player_Structure(const int& i = 0) :position(i) {};
	virtual int roll_dice() = 0;
	bool won = false;
	int get_position() const { return position; }
	virtual SNL_Player_Structure& operator=(const int& a) { position = a; return *this; }
protected:
	int position;
};

class SNI_Ai : public SNL_Player_Structure {
public:
	SNI_Ai(const string& player,bool i = false,const int& pos = 0) : SNL_Player_Structure(pos), name(player),instant(i){};
	virtual int roll_dice() override;
	string name;
	bool instant;
	SNI_Ai& operator=(const int& x) { position = x; return *this; }
};

int SNI_Ai::roll_dice() {
	static std::default_random_engine engine;
	static std::uniform_int_distribution<unsigned> rng(1, 6);
	int roll = rng(engine);
	position += roll;
	if (position >= 100) {
		won = true;
		position = 100;
	}
	return roll;
}

class SNI_Player : public SNL_Player_Structure {
public:
	SNI_Player(const int& i = 0) : SNL_Player_Structure(i) {};
	virtual int roll_dice() override;
};

int SNI_Player::roll_dice() {
	static std::default_random_engine engine;
	static std::uniform_int_distribution<unsigned> rng(1, 6);
	int roll = rng(engine);
	position += roll; //move his position by the random roll
	if (position >= 100) { // checking if player has won
		won = true;
		position = 100;
	}
	return roll;
}


#endif // !SNAKE_N_LADD_CLASS

#ifndef SNAKE_N_LADD_GAME
#define SNAKE_N_LADD_GAME

/* Public Memeber functions */
std::pair<bool, int> on_snake(const SnakeLadder& sl, const int& i) {
	auto iter = sl.snakes.find(i); //find the int that is position of the snake
	std::pair<bool, int> match({ false,0 });
	if (iter != sl.snakes.end()) { //we found a match
		match = std::make_pair( true,iter->second ); //player is now on *iter position
	}
	return match;
}

std::pair<bool, int> on_ladder(const SnakeLadder& sl, const int& i) {
	auto iter = sl.ladders.find(i);//find the int that is position of the snake
	std::pair<bool, int> match({ false,0 });
	if (iter != sl.ladders.end()) {//we found a match
		match = std::make_pair(true, iter->second);//player is now on *iter position
	}
	return match;
}

/* Our Actual gameplay */
void run_snake_n_ladd() {
	SnakeLadder game;
	bool inst = true;
	int player_count = 3;
	SNI_Player user;
	std::vector<SNI_Ai> players;
	for (int x = 1; x <= player_count+2; x++) {
		players.emplace_back("Player " + std::to_string(x),inst);
		player_count--;
	}
	std::pair<bool, string> ai_win;
	int rounds = 0;//we'll count how many rounds there were.
	while (true) {
		rounds++;
		string input;
		cout << "It is now your turn!" << endl;
		cin >> input;
		if (input == "roll") {
			//roll processing
			int user_roll = user.roll_dice(); // where the user landed after he rolled the dice.
			cout << "You have rolled a " << user_roll << " and landed on " << user.get_position() << endl; //print where he landed
			std::pair<bool, int> snake = on_snake(game, user_roll); // check if the position is on a snake
			std::pair<bool, int> ladder = on_ladder(game, user_roll); // check if the position is on a snake
			if (snake.first) { //landed on snake
				cout << "You landed on a snake on position " << user.get_position()  << " to " << snake.second << "!" << endl;
				user = snake.second; //changes position to the bottom of the snake
				cout << "You are now on position" << snake.second << endl;
			}
			else if (ladder.first) { //landed on ladder
				cout << "You landed on a ladder on position " << user.get_position() << " to " << ladder.second << "!" << endl;
				user = ladder.second;//changes position to the top of the ladder
				cout << "You are now on position " << snake.second << endl;
			}

			if (!inst)
				Sleep(500);
			for (vector<SNI_Ai>::iterator begin = players.begin(); begin != players.end(); begin++) {
				int roll = begin->roll_dice();
				cout << begin->name << " has rolled a " << roll << " and is now on tile " << begin->get_position() << endl;
				std::pair<bool, int> ai_snake = on_snake(game, roll); // check if the position is on a snake
				std::pair<bool, int> ai_ladder = on_ladder(game, roll); // check if the position is on a snake
				if (ai_snake.first) { //landed on snake
					cout << begin->name << " has landed on a snake at positions " << begin->get_position() << " to " << ai_snake.second << endl;
					(*begin) = ai_snake.second;
					cout << begin->name << " is now on position " << begin->get_position();
				}
				else if (ai_ladder.first) { //landed on ladder
					cout << begin->name << " has landed on a ladder at positions " << begin->get_position() << " to " << ai_ladder.second << endl;
					(*begin) = ai_ladder.second;
					cout << begin->name << " is now on position " << begin->get_position() << endl;
				}

				if (begin->won) {
					ai_win.first = true;
					ai_win.second = begin->name;
				}
				if (!begin->instant)// if user has put non instantneous, we make the console pause for 0.5s.
					Sleep(500);
			}
			//checking wins

			if (user.won) {
				cout << "You won in" << rounds << " turns! ";
				cout << "Do you want to play again?" << endl;
				char play_again = check("Input only (y) or (n), do you want to play again?", { 'y','n' });
				if (play_again == 'y')

				break;
			}
			if (ai_win.first) {
				cout << ai_win.second << " won in " << rounds << " turns!";
				break;
			}
		}
		else if (input == "snakes") { game.print_snakes(cout) << endl; }
		else if (input == "ladders") { game.print_ladders(cout) << endl; }
		else if (input == "new") { 
			game = SnakeLadder();
			
		}
		else if (input == "board") { cout << game; }
		else if (input == "inst") {
			inst = inst == true ? false : true;
			for (vector<SNI_Ai>::iterator begin = players.begin(); begin != players.end(); begin++) {
				begin->instant = inst;
			}
		}
		else if (input == "change") {}
		else if (input == "quit") { break; }
		else if (input == "clear") { clear(); }
		else {// cin failed
			cout << "Only enter characters," << endl;
			cin.clear();
		}

		

	}

}

#endif // !SNAKE_N_LADD_GAME
/*cout << "Welcome to Snakes and Ladders, do you know how to play? y\n" << endl;
	char help = check("Invalid input, input (y) for yes or (n) for no.", { 'y','n' });
	help == 'y' ? game.instructions(cout, true) : game.instructions(cout, false);
	cout << "Do you want Ai moves to be instantaneous? y\n" << endl;
	char inst_check = check("Invalid input, input (y) for yes or (n) for no.", { 'y','n' });
	bool ai_inst = inst_check == 'y' ? true : false;
	cout << "Enter amount of players 1-8" << endl;
	int player_count = check("Invalid input, enter single number 1-8.", { 1,2,3,4,5,6,7,8 });*/
