#ifndef HANGMAN
#define HANGMAN
#include <string>
#include <iostream>
#include <cctype>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <set>

// this code is a mess, don't look at it.

using std::string; using std::cin; using std::endl; using std::flush; using std::istream; using std::ostream; using std::cout; using std::vector; 
vector<string> split(const string& container);
string myToLower(const string& s);

class Hangman {
public:
	Hangman(std::istream& i, std::ostream& o) { start(i, o); };
	void getDiff(istream& i, ostream& o);
	void start(istream& i, ostream& o);
	void startGame(istream & i, ostream & o);
	void playAgain(istream& i, ostream& o);
	int getDiff();
	string getWord();
	vector<string> getWords(const int& i);
	vector<string> words;
private:
	int tries = 10;
	std::set<char> input_words;
	string word;//the word that we defined.
	int difficulty;
};

int Hangman::getDiff() {
	if (difficulty) {
		return this->difficulty;
	}
	else {
		throw std::runtime_error("No difficulty");
	}
	
}
string Hangman::getWord() {
	if (this->word.size() >1) {
		return this->word;
	}
	else {
		throw std::runtime_error("No Word");
	}
}

void Hangman::start(istream& i,ostream& o) {
	o << "Welcome to HangMan! \nDo you know how to play? y/n" << endl;
	string x;
	bool help = false;
	do {
		i.clear();
		i >> x;
		if ((tolower(x[0]) == 'n' || tolower(x[0]) == 'y' )&& x.size() == 1) {
			help = (x[0] == 'n' ? true : false);
		}
		else {
			o << "Enter 'Y' or 'N' " << endl;
			i.ignore();
			continue;
		}
	} while (  i.fail() );
	if (help) {
		o << "Enter a single character (No spaces) to guess the full word, if you know the full word simply type it in, words are in lowercase." << endl;
	}
	o << "Choose difficulty 1-5" << endl;
	getDiff(i,o);
	this->words = getWords(this->difficulty);
	//this->word = words[ (rand() % this->words.size()) ];
	this->word = words[0 + (rand() % (int)(words.size() - 0 + 1))];
	startGame(i,o);
}

void Hangman::getDiff(istream& i,ostream& o) {
	int difficult;
	do {
		i.clear();
		i.ignore();
		i >> difficult;
		if (difficult < 1 || difficult > 5) {
			o << "Enter a single integer thats 1-5." << endl;
			i.ignore();
		}
	} while (i.fail());
	this->difficulty = difficult;
}


vector<string> Hangman::getWords(const int& i) {
	std::ifstream ifs("data.txt");
	string line;
	int diff;
	while (ifs) {
		ifs >> diff;
		getline(ifs, line);
		if (diff == i) {
			break;
		}
	}
	vector<string> words = split(line);
	return words;
}

void Hangman::startGame(istream& i, ostream& o) {
	o << "Your word is " << this->word.size() << " letters long." << endl;
	string line;
	while (i.good()) {
		i >> line;
		if (i.good() && line.size() <=1) {
			char character = tolower(line[0]);
			this->input_words.insert(character);
			bool isNew = false;
			for (const char& m : this->word) {
				if (character == m) {
					isNew = true;
				}
			}

			if (!isNew) {
				this->tries--;
			}

			for (string::iterator begin = this->word.begin(); begin != this->word.end(); begin++) {
				if (this->input_words.find(*begin) != this->input_words.end()) {
					o << *begin << flush;
				}
				else {
					o << "_" << flush;
				}
			}
			if (this->tries) {
				o << "Tries remaining : " << this->tries << endl;
			}
			else {
				o << " No more tries left, word was : " << this->word << endl;
				break;
			}
			
		}
		else if (line == this->word) {
			o << "Correct!" << endl;
			break;
		}
		else {
			o << "Enter only a single character. " << endl;
			i.ignore();
			i.clear();
		}
	}
	playAgain(i,o);
}

void Hangman::playAgain(istream& i, ostream& o) {
	o << "Do you want to play again? insert 'y' if so, otherwise press any character." << endl;
	string t;
	bool playAgain = false;
	while (i.good()) {
		i >> t;
		if (t.size() == 1) {
			t[0] = tolower(t[0]);
			playAgain = t[0] == 'y' ? true : false;
			break;
		}
		else {
			break;
			cin.ignore();//incase of non string inputs.
			cin.clear();
		}
		
	}
	if (playAgain) {
		std::set<char> x;
		o << "Choose difficulty 1-5" << endl;
		getDiff(i, o);
		this->words = getWords(this->difficulty);
		this->word = words[(rand() % this->words.size())];
		this->tries = 10;
		this->input_words = x;
		startGame(i, o);
	}
}

vector<string> split(const string& container) {
	std::stringstream sst(container);
	string l;
	vector<string> vec;
	while (sst >> l) {
		l = myToLower(l);//we will also lowercase the letters here.
		vec.push_back(l);
	}
	return vec;
}

string myToLower(const string& s) {
	string x = s;
	for (char& m : x) {
		m = tolower(m);
	}
	return x;
}

#endif