#include <iostream>
#include <initializer_list>
#include <algorithm>
#include "Hangman.h"
#include "Guess.h"
#include "Snakes and Ladders.h"
using std::cout; using std::cin; using std::endl; using std::flush;

void clear() { //clears the screen.
	cout << string(100, '\n') << flush;
}

char check(const string& message,const std::initializer_list<char>& criteria) {//checking inputs recieved, if the stream is good and if they match what characters were wanted.
	char x;
	while (cin.good()) {
		cin >> x;
		if (cin.bad()) {
			cout << message << endl;
			cin.clear();
			continue;
		}
		else {//cin is in a good condition
			bool good = false;
			for (auto begin = criteria.begin(); begin != criteria.end(); begin++) {//now we check if user's input matches any criteria.
				if (*begin == x) {
					good = true;
				}
			}//for
			if (good) {
				return x;
			}
			else {
				continue;
			}
		}

	}//while
}

int main() {
	/*char input;
	cout << "Games available are :\nHangman(H), Guess(G), \nto play input the character in brackets. otherwise press any other character." << endl;
	cin >> input;
	if (input == 'H') {//Hangman
		clear();
		Hangman Hm(cin, cout);
	}
	else if (input == 'G') {//Guess
		runGuess();
	}*/
	SnakeLadder a;
	a.print_ladders(cout) << endl;
	a.print_snakes(cout) << endl;
	system("pause");
	return cin.good() ? 0 : -1;
}
