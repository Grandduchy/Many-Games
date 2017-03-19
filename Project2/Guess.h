#pragma once
#include <deque>
#include <random>
#include <Windows.h>
#include <ctime> //for time()
extern void clear();
extern char check(const string& message, const std::initializer_list<char>& criteria);

#ifndef GUESSCLASS
#define GUESSCLASS

class GuessResult;

class Guess {// class will handle the actual game creation and printing
	friend GuessResult;
public:
	Guess();
	Guess(const Guess &a) : gusses(a.gusses), user_sequences(a.user_sequences), sequence(a.sequence){};
	Guess& operator--() { gusses--; return *this; }; //prefix
	~Guess() { free(); };

	void instructions(const bool all) const;
	std::ostream& previous(std::ostream&) const;
	std::ostream& all(std::ostream& os) const;
	GuessResult startGame() const;
	std::ostream& gameSequence(std::ostream& os) const;
	string gameSequence() const;
	void newGame();
	int chances() const { return gusses; };
	bool playAgain();
	std::ostream& printGameSequence(std::ostream& os) const;
private:
	int gusses; //amount of times the user is able to try to guess.
	std::deque<std::pair<string, std::pair<int,int>>>* user_sequences; //user's sequence
															// the first string represents the user's sequence, the second pair represents the amt correct/wrong place
	vector<char>* sequence; //our sequence the user will guess

	std::ostream& printEntry(std::ostream& os, const std::pair<string, std::pair<int, int>>&) const; //prints a single entry in user_sequences
	void free() { delete user_sequences; delete sequence; }; 
	void createSequence(); //creats our sequence the user will guess
};

bool Guess::playAgain() {
	cout << "Do you want to play again? y,n" << endl;
	char again = check("Play Again? (y) = yes, (n) = no", { 'y','n' });
	if (again == 'y') {
		newGame(); //make a new game
		clear();
		instructions(false);
		cin.ignore(); //check returns a newline which getline() reads.
		return true;
	}
	else {
		return false;
	}
	throw std::runtime_error("PlayAgain did not go through control paths");
	return true;
}

void Guess::createSequence() {
	vector<char> colors({ 'r','y','g','b','o','c' });
	static std::default_random_engine engine(std::time(0));
	static std::uniform_int_distribution<int> rg(0, colors.size() - 1);
	for (vector<char>::size_type sz = 0; sz != 4; sz++) {
		cout << colors[rg(engine)];
		sequence->push_back(colors[rg(engine)]);
	}
}

void Guess::newGame() {
	free();
	sequence = new vector<char>();
	createSequence();
	user_sequences = new std::deque<std::pair<string, std::pair<int, int>>>();
}

std::ostream& Guess::printGameSequence(std::ostream& os) const {
	for (auto i : *sequence) {
		if (i == 'r') {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192);
			cout << i << " " << flush;
		}
		else if (i == 'y') {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 224);
			cout << i << " " << flush;
		}
		else if (i == 'g') {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
			cout << i << " " << flush;
		}
		else if (i == 'b') {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 144);
			cout << i << " " << flush;
		}
		else if (i == 'p') {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 208);
			cout << i << " " << flush;
		}
		else if (i == 'c') {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 176);
			cout << i << " " << flush;
		}
		else {
			throw std::runtime_error("No colour matching any inputs in Guess::printGameSequence, recieved :" + i);
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	return os;
}

std::ostream& Guess::gameSequence(std::ostream& os) const {
	for (vector<char>::iterator begin = sequence->begin(); begin != sequence->end(); begin++) {
		os << *begin << " " << endl;
	}
	return os;
}
string Guess::gameSequence() const {
	string x;
	for (vector<char>::iterator begin = sequence->begin(); begin != sequence->end(); begin++) {
		x += *begin; x += ' ';
	}
	return x;
}

Guess::Guess() : user_sequences(new std::deque<std::pair<string, std::pair<int, int>>>), sequence(new vector<char>), gusses(10) {
	createSequence();
}

std::ostream& Guess::printEntry(std::ostream& os,const std::pair<string,std::pair<int,int>>& entry) const {// both all and previous will use this, they simply print the user's sequences and their results from that sequence.
	/*Color types
	* 192 - red
	* 224 - yellow
	* 160 - green
	* 144 - blue
	* 208 - purple
	* 176 - cyan // there is no orange ):
	* 240 - white
	* 128 - grey
	* ^^^^ all affect background colours 
	* 7 - default(gray)
	*/
	vector<char> ic;//indivusal colours
	std::stringstream sst(entry.first);
	string x;
	while (sst >> x) {
		if (x[0] == 'r') {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192);
			cout << x[0] << " " << flush;
		}
		else if (x[0] == 'y') {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 224);
			cout << x[0] << " " << flush;
		}
		else if (x[0] == 'g') {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
			cout << x[0] << " " << flush;
		}
		else if (x[0] == 'b') {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 144);
			cout << x[0] << " " << flush;
		}
		else if (x[0] == 'p') {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 208);
			cout << x[0] << " " << flush;
		}
		else if (x[0] == 'c') {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 176);
			cout << x[0] << " " << flush;
		}
		else {
			throw std::runtime_error("No colour matching any inputs in Guess::printEntry, recieved '" + x + "'");
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	os << " " << entry.second.first << flush; //correct colour correct place
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 128);
	os << " " << entry.second.second << flush; //wrong places correct colour
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	return os;
}

std::ostream& Guess::previous(std::ostream& os) const {
	if (this->user_sequences->empty()) {
		os << "No previous sequences has been entered." << flush;
	}
	else {
		printEntry(os, this->user_sequences->back());
	}
	return os;
}

std::ostream& Guess::all(std::ostream& os) const {
	if (this->user_sequences->empty()) {
		os << "No previous sequences has been entered." << flush;
	}
	else {
		for (auto rBegin = user_sequences->crbegin(); rBegin != user_sequences->crend(); rBegin++) {
			printEntry(os, *rBegin) << endl;
		}
	}
	return os;
}

void Guess::instructions(const bool all) const {
	if (all) {
		cout << "You will be given a set of character representing a color, and have to reproduce a predetermined sequence of colors within a cetain number of guesses"
				<< " Each guess gets marked with the number of correctly-coloured pegs in a correct place(in white)"
				<< " and also the number of correctly-coloured pegs in the wrong places(in gray)"
				<< " Enter all four colors eg, 'r y r g'" << endl;
		}
	cout << "(r) = red, (y) = yellow, (g) = green, (b) = blue, (p) = purple, (c) = cyan" << endl;
	cout << "'previous' for last move,'all' for all moves, 'Help' for detailed instructions, 'help' for procedural instructions,\n'quit' to quit,'clear' to clear the screen, 'new' to make a new game." << endl;
}

class GuessResult {//class will handle processing and user input
	friend Guess;
public:
	GuessResult(std::deque<std::pair<string, std::pair<int, int>>>* a, vector<char>* b) : user_sequences(a),sequence(b){};
	GuessResult() {}; //No work is done because Guess and GuessResult will not ever become apart from each other, and GuessResult will not manage any resources.
	auto& previous() const;//returns last entry from the std::pair
	std::pair<int, int> compare(const vector<char>&) const;
	void add(const std::pair<string, std::pair<int, int>>& user_result);
	bool won()const;
private:
	unsigned short guesses;
	std::deque<std::pair<string, std::pair<int, int>>>* user_sequences;
	vector<char>* sequence;
	
	
};

auto&  GuessResult::previous() const {
	return this->user_sequences->back();
}

void GuessResult::add(const std::pair<string, std::pair<int, int>>& user_result) {
	user_sequences->push_back(user_result);
}

bool GuessResult::won() const {
	std::stringstream s(user_sequences->back().first);
	string m;
	vector<char> l;
	while (s >> m) {
		l.push_back(m[0]);
	}
	return *sequence == l;
}

std::pair<int,int> GuessResult::compare(const vector<char>& user) const {
	std::pair<int, int> w({0,0}); // first int is correct colours, second is ones in wrong places
	vector<char> comp = *sequence;
	for (vector<char>::const_iterator userBegin = user.cbegin(); userBegin != user.cend(); userBegin++) {//first finding ones in the wrong places
		auto g = std::find(comp.begin(), comp.end(), *userBegin);
		if (g != comp.end()) {
			w.second++;
			comp.erase(g);
		}
	}
	vector<char>::const_iterator userBegin = user.cbegin();
	vector<char>::const_iterator compBegin = sequence->cbegin();
	while (user.cbegin() != user.cend() && compBegin != sequence->cend()) {//then finding ones in right places
		if (*(userBegin++) == *(compBegin++)) {
			w.first++;
			w.second--;
		}

	}
	return w;
}

//Guess methods
GuessResult Guess::startGame() const {
	return GuessResult(this->user_sequences, this->sequence);
}


#endif // !GUESSCLASS




#ifndef GUESSGAME
#define GUESSGAME


string colorInput(const vector<string>& b) {
	bool is_Valid = true;
	if (b.size() == 4) {
		string ourFormat;
		std::for_each(b.begin(), b.end(), [&is_Valid, &ourFormat](const string& part) {
			if (part.size() > 1) { //check if any inputs are bigger than (r)
				is_Valid = false;
			}
			ourFormat.append(" " + part); //add it to our local string in our format of |r g m n|
		});
		ourFormat.erase(ourFormat.begin()); //erase in the beginning we added.
		std::stringstream sst(ourFormat); //now we must check if the entered chars are valid colours.
		char x;
		while (sst >> x) {
			if (x == 'r' || x == 'y' || x == 'b' || x == 'c' || x == 'g' || x == 'p') {
				;
			}
			else {
				return "";
			}
		}
		return is_Valid ? ourFormat : "";
	}
	else {
		return ""; //empty string meaning invaled.
	}
}

void runGuess() {
	clear();
	cout << "Do you know how to play? y or n" << endl;
	char n = check("Invalid input, input only a single character.", { 'y','n' });
	Guess game;//create game.
	clear();
	n == 'n' ? game.instructions(true) : game.instructions(false);
	cin.ignore();
	while (true) {
		GuessResult current = game.startGame();
		string input;
		getline(cin, input);
		if (cin.good()) {
			vector<string> ind; //indivisual inputs
			string i;
			std::stringstream ss(input);
			while (ss >> i) {
				ind.push_back(i);
			}
			string stringColors = colorInput(ind);
			if (stringColors != "") { // if the format is in our format we want, we'll put it into the user's sequences
				int correct_places = 0, correct_colors = 0;
				vector<char> indColrs;
				std::for_each(ind.begin(), ind.end(), [&indColrs](const string& a) { indColrs.push_back(a[0]); }); //create a vector<char> with each color
				std::pair<int, int> b = current.compare(indColrs); //get the comparing numbers
				std::pair<string, std::pair<int, int>> total(input, b);//now we have the final format.
				current.add(total); //now we add it to the user sequence in our class.
				bool won = current.won(); // now we check if the user won
				game.previous(cout) << endl;//regardless we print the last entry.
				--game;
				bool lost = game.chances() <= 0;
				if (won) {//now we put out the end game messages if the user won.
					cout << "Congratulations!\nSequence was : " << flush;
					game.printGameSequence(cout) << endl;
					if (!game.playAgain()) {
						break;
					}
				}
				else if (lost) {
					cout << "Game over!\nSequence was : " << flush;
					game.printGameSequence(cout) << endl;
					if (!game.playAgain()) {
						break;
					}
				}
				else { cout << "Guesses left -> " << game.chances() << endl; }
			}
			else { //entering a command
				if (input == "previous") { game.previous(cout); }
				else if (input == "all") { game.all(cout); }
				else if (input == "Help") { game.instructions(true); }
				else if (input == "help") { game.instructions(false); }
				else if (input == "quit") { break; }
				else if (input == "clear") { clear(); }
				else if (input == "new") { game.newGame(); clear(); game.instructions(false); }
				else {
					cout << "Invalid input, inputs are case sensitive. type 'Help' for detail." << endl;
					cout << "Your input = '" << input << "'" << endl;
				}
			}
		}//cin.good
		else {
			cout << "Only enter characters separated by spaces" << endl;
			cin.clear();
		}
	}
}

#endif // !GUESSGAME
