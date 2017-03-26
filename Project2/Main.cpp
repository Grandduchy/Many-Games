#include <iostream>
#include "Hangman.h"
#include "Guess.h"
#include "Snakes and Ladders.h"
using std::cout; using std::cin; using std::endl; using std::flush;

int main() {
	char input;
	cout << "Games available are :\nHangman(H), Guess(G), Snakes and Ladders(S) \nto play input the character in brackets. otherwise press any other character." << endl;
	cin >> input;
	if (input == 'H') {//Hangman
		clear();
		Hangman Hm(cin, cout);
	}
	else if (input == 'G') {//Guess
		runGuess();
	}
	else if (input == 'S') {
		run_snake_n_ladd();
	}
	cout << "Press enter...";
	cin.get();
	return cin.good() ? 0 : -1;
}
