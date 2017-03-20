#pragma once
#include <iostream>
#include <string>
using std::cout; using std::string; using std::flush;
void clear() { //clears the screen.
	cout << string(100, '\n') << flush;
}
template<typename T>
T check(const string& message, const std::initializer_list<T>& criteria) {//checking inputs recieved, if the stream is good and if they match what characters were wanted.
	T x;
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