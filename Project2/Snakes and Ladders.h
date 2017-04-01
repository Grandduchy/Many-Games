
#include <map>
#include <random>
#include <ctime>
#include <windows.h>
#include <cassert>
#include "Functions.h"
#ifndef SNAKE_N_LADD_CLASS
#define SNAKE_N_LADD_CLASS
/* Forward Declarations */
class SnakeLadder;
/* Classes */

class SNL_Player_Structure { // the general structure of the player and ai classes
public:
	SNL_Player_Structure(const int& i = 0) :position(i) {};
	virtual int roll_dice() { assert(0); return 0; };
	bool won = false;
	int get_position() const { return position; }
	virtual SNL_Player_Structure& operator=(const int& a) { position = a; return *this; }
protected:
	int position;
};

class SNI_Ai : public SNL_Player_Structure {
public:
	SNI_Ai(const string& player, bool i = false, const int& pos = 0);
	virtual int roll_dice() override;
	string name;
	bool instant;
	int colour;
	SNI_Ai& operator=(const int& x) { position = x; return *this; }
};

class SNI_Player : public SNL_Player_Structure {
public:
	SNI_Player(const int& i = 0) : SNL_Player_Structure(i) {};
	virtual int roll_dice() override;
	int color = 240;
};

class SnakeLadder {
	friend std::pair<bool, int> on_snake(const SnakeLadder& sl, const int& i);
	friend std::pair<bool, int> on_ladder(const SnakeLadder& sl, const int& i);
	friend ostream& operator<<(ostream& os, const SnakeLadder& sl);
public:
	SnakeLadder(int&,bool);
	void change_inst();
	ostream& instructions(ostream&,bool) const;
	ostream& print_snakes(ostream&) const;
	ostream& print_ladders(ostream&) const;
	vector<SNI_Ai> ais;
	SNI_Player player;
	std::pair<bool, string> won;
	int change_colour(int) const;
private:
	vector<vector<int>> board;
	std::map<int, int> ladders; //first i = bottom of ladd, sec i = top of ladd.
	std::map<int, int> snakes; //first i = top of snk, sec i = bottom of snk.
	void create_ladd_n_snake();
};


/*** Snake Ladder Member Functions ***/

int SnakeLadder::change_colour(int tile_num) const {
	if (player.get_position() == tile_num) {
		return player.color;
	}
	else {
		auto iter = std::find_if(ais.begin(), ais.end(), [&](const SNI_Ai& ai) {
			return ai.get_position() == tile_num ? true : false;
		});
		if (iter != ais.end())
			return iter->colour;
	}
	return 7;
}

void SnakeLadder::change_inst() {
	for (auto begin = ais.begin(); begin != ais.end(); begin++) {
		begin->instant = !begin->instant;
	}
}

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
	os << "Input 'roll' to roll the dice, 'board' to view board, 'ladders'/'snakes' to know of their positions,'new' for new game,'inst' to change instantaneous," <<
		" 'quit','clear' to clear screen, and '(full)help' for help.\n You are colour white." << endl;
	return os;
}

SnakeLadder::SnakeLadder(int& player_count,bool inst = false) : player(0) {
	for (int x = 100; x != 0; x-=10) {
		vector<int> row;
		for (int z = 0; z != 10; z++) { //creating the rows in the board
			row.push_back(x - z);
		}
		board.push_back(row);
	}
	create_ladd_n_snake(); //creates the snakes and ladder positions
	while (player_count != 0) { //creates the ais
		ais.emplace_back("Player " + std::to_string(player_count--), inst);
	}
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

/*** SNI_AI Member Functions ***/

SNI_Ai::SNI_Ai(const string& player, bool i = false, const int& pos = 0) : SNL_Player_Structure(pos), name(player), instant(i) {
	static std::default_random_engine engine(std::time(0));
	static std::uniform_int_distribution<unsigned> rng(1, 239);
	int c = rng(engine);
	colour = c != 240 ? c : rng(engine);
}

int SNI_Ai::roll_dice() { // explanation in SNI_Player implentation
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

/*** SNI_Player Member Functions***/

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

/*** Public Overloaded Operators ***/

ostream& operator<<(ostream& os, const SnakeLadder& sl) {
	int counter = 10; // there is only 10 rows, used to determine which end we should count from.
	for (const vector<int>& row : sl.board) {
		if ((counter & 1) == 0) { //is even // 100  99 98...
			for (vector<int>::const_iterator tile_iter = row.cbegin(); tile_iter != row.cend(); tile_iter++) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sl.change_colour(*tile_iter)); //change colour based off user's positions.
				os << *tile_iter;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);// makes the background not the same colour as the user's color
				os << "      ";
			}
		}
		else { //90 91 92 93...
			for (auto rev_tile_iter = row.crbegin(); rev_tile_iter != row.crend(); rev_tile_iter++) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sl.change_colour(*rev_tile_iter));
				if (*rev_tile_iter >= 10) {
					os << *rev_tile_iter;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					os << "      ";
				}
				else {
					os << *rev_tile_iter;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					os << "       ";
				}
					
			}
		}
		counter--;
	}
	return os; // no need to change back the colour, we did it in the loop.
}

#endif // !SNAKE_N_LADD_CLASS

#ifndef SNAKE_N_LADD_GAME
#define SNAKE_N_LADD_GAME

/* Public Memeber functions */
std::pair<bool, int> on_snake(const SnakeLadder& sl, const int& i) { //determines if the user is on a snake
	auto iter = sl.snakes.find(i); //find the int that is position of the snake
	std::pair<bool, int> match({ false,0 });
	if (iter != sl.snakes.end()) { //we found a match
		match = std::make_pair( true,iter->second ); //player is now on *iter position
	}
	return match;
}

std::pair<bool, int> on_ladder(const SnakeLadder& sl, const int& i) { // determines if the user is on a ladder
	auto iter = sl.ladders.find(i);//find the int that is position of the snake
	std::pair<bool, int> match({ false,0 });
	if (iter != sl.ladders.end()) {//we found a match
		match = std::make_pair(true, iter->second);//player is now on *iter position
	}
	return match;
}

void landed_ladd_or_snak(SNL_Player_Structure& user, string&& tile_name, string player_name,std::pair<bool, int>& ladd_or_snake) {
	if (ladd_or_snake.first) { // if he did land on one of these objects
	cout << player_name <<" landed on a " << tile_name << " on position " << user.get_position() << " to " << ladd_or_snake.second << "!" << endl;
	user = ladd_or_snake.second; //changes position to bottom or top of the object.
	cout << (player_name == "You" ? "You are now on position " : player_name + " is on position " )// grammar changing
		<< ladd_or_snake.second << endl;
	}
}

/* Our Actual gameplay */
void run_snake_n_ladd() {
	cout << "Welcome to Snakes and Ladders, do you know how to play? (y)/(n)" << endl;
	char help = check("Invalid input, input (y) for yes or (n) for no.", { 'y','n' }); //if we should print the instructions
	cout << "Do you want moves to be instantaneous? (y)/(n)" << endl;
	char inst_check = check("Invalid input, input (y) for yes or (n) for no.", { 'y','n' }); // if we should make it instant
	cout << "Enter amount of players 1-8" << endl;
	int player_count = check("Invalid input, enter single number 1-8.", { 1,2,3,4,5,6,7,8 }); // asks how many players the gamer wants.
	bool instantaneous = inst_check == 'y' ? true : false;// if we should make console sleep for 0.5s
	bool need_help = help == 'y' ? false : true; //if we should print the instructions
	/* Start of Game */
	SnakeLadder game(player_count, instantaneous);
	game.instructions(cout, need_help);
	int rounds = 0;
	while (true) {
		++rounds;
		string input;
		cout << "It is now your turn!" << endl;
		cin >> input;
		/* Has entered to play the game */
		if (input == "roll") {
			/* Player processing */
			int player_roll = game.player.roll_dice();
			cout << "You have rolled a "<< player_roll << " and landed on "<< game.player.get_position() << endl;
			std::pair<bool, int> snake = on_snake(game, game.player.get_position()); // check if the position is on a snake
			std::pair<bool, int> ladder = on_ladder(game, game.player.get_position()); // check if the position is on a snake
			landed_ladd_or_snak(game.player, "snake", "You" ,snake);
			landed_ladd_or_snak(game.player, "ladder", "You",ladder);
			if (game.player.get_position() == 100) {//user has won!
				game.won = std::make_pair(true, "You");
				break; //exit the loop, no more work to be done processing.
			}
			/* Ai Processing */
			if (!game.ais.begin()->instant)// waits 0.5 seconds before ais start playing
				Sleep(500);
			for (auto ai_begin = game.ais.begin(); ai_begin != game.ais.end(); ++ai_begin) {
				int ai_roll = ai_begin->roll_dice();
				cout << ai_begin->name << " has rolled a " << ai_roll << " and is now on tile " << ai_begin->get_position() << endl;
				std::pair<bool, int> ai_snake = on_snake(game, ai_begin->get_position()); // check if the position is on a snake
				std::pair<bool, int> ai_ladder = on_ladder(game, ai_begin->get_position()); // check if the position is on a snake
				landed_ladd_or_snak(*ai_begin, "snake", ai_begin->name,ai_snake);
				landed_ladd_or_snak(*ai_begin, "ladder", ai_begin->name,ai_ladder);
				if (ai_begin->get_position() == 100) {//ai has won!
					game.won = std::make_pair(true, ai_begin->name);
				}
				if (!ai_begin->instant)// makes console pause for 0.5s.
					Sleep(500);
			}
		}
		/* Has entered a command */
		else if (input == "snakes") { game.print_snakes(cout) << endl; }
		else if (input == "ladders") { game.print_ladders(cout) << endl; }
		else if (input == "new") {
			run_snake_n_ladd();
			break;
		}
		else if (input == "board") { cout << game; }
		else if (input == "inst") { game.change_inst(); }
		else if (input == "quit") { break; }
		else if (input == "clear") { clear(); }
		else if (input == "help") { game.instructions(cout, false) << endl; }
		else if (input == "fullhelp") { game.instructions(cout, true) << endl; }
		else {// cin failed
			cout << "Only enter characters, input 'help' for more information." << endl;
			cin.clear();
		}
		/* Checking wins */
		if (game.won.first) {//someone has won
			if (game.won.second == "You") {// player has won
				cout << "Congratulations! \n"+game.won.second+" won in " << rounds << " turns! " << endl;
				cout << "Do you want to play again? " << endl;
				char x = check("Invalid input, do you want to play again? (y),(n)", { 'y','n' });
				if (x == 'y')
					run_snake_n_ladd();
				else
					break;
			}
			else { // the ai has won
				cout << "You have lost! \n" + game.won.second + " won in " << rounds << " turns! " << endl;
				cout << "Do you want to play again?" << endl;
				char x = check("Invalid input, do you want to play again? (y),(n)", { 'y','n' });
				if (x == 'y')
					run_snake_n_ladd();
				else
					break;
			}
		}// !checking wins

	}// !while loop

}// !function

#endif // !SNAKE_N_LADD_GAME

