#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <exception>
#include <string>
#include <set>
#include <map>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;


class Date {
public:

	Date(int day_, int month_, int year_) {
		day = day_;
		month = month_;
		year = year_;
	}

	int GetYear() const {
		return year;
	}

	int GetMonth() const {
		return month;
	}

	int GetDay() const {
		return day;
	}

private:
	int year;
	int month;
	int day;
};


ostream& operator<<(ostream& out, const Date& lhs) {
	out << setw(4) << setfill('0') << lhs.GetYear();
	out << '-' << setw(2) << setfill('0') << lhs.GetMonth();
	out << '-' << setw(2) << setfill('0') << lhs.GetDay();
	return out;
}

istream& operator>>(istream& in, Date& lhs) {
	string toParse;
	in >> toParse;

	stringstream parse(toParse);
	string message;
	int year, month, day;
	char def1, def2;
	

	if (!(parse >> year)) {
		message = "Wrong date format: " + toParse;
		throw invalid_argument(message);
	}

	if (parse.peek() == EOF) {
		message = "Wrong date format: " + toParse;
		throw invalid_argument(message);
	}

	if (!(parse >> def1)) {
		message = "Wrong date format: " + toParse;
		throw invalid_argument(message);
	}

	if (parse.peek() == EOF) {
		message = "Wrong date format: " + toParse;
		throw invalid_argument(message);
	}

	if (!(parse >> month)) {
		message = "Wrong date format: " + toParse;
		throw invalid_argument(message);
	}

	if (parse.peek() == EOF) {
		message = "Wrong date format: " + toParse;
		throw invalid_argument(message);
	}

	if (!(parse >> def2)) {
		message = "Wrong date format: " + toParse;
		throw invalid_argument(message);
	}

	if (parse.peek() == EOF) {
		message = "Wrong date format: " + toParse;
		throw invalid_argument(message);
	}

	if (!(parse >> day)) {
		message = "Wrong date format: " + toParse;
		throw invalid_argument(message);
	}

	if (parse.peek() == ' ' || parse.peek() == EOF) {
		if (month < 1 || month > 12) {
			message = "Month value is invalid: " + to_string(month);
			throw invalid_argument(message);
		}

		if (day < 1 || day > 31) {
			message = "Day value is invalid: " + to_string(day);
			throw invalid_argument(message);
		}
		lhs = Date(day, month, year);
		return in;
	}
	else {
		message = "Wrong date format: " + toParse;
		throw invalid_argument(message);
	}
}

bool operator==(const Date& lhs, const Date& rhs) {
	return (lhs.GetYear() == lhs.GetYear() && lhs.GetMonth() == rhs.GetMonth() && lhs.GetDay() == rhs.GetDay());
}

bool operator<(const Date& lhs, const Date& rhs) {
	return (lhs.GetYear() < lhs.GetYear() || (lhs.GetYear() == lhs.GetYear() && lhs.GetMonth() < rhs.GetMonth())
		|| (lhs.GetYear() == lhs.GetYear() && lhs.GetMonth() == rhs.GetMonth() && lhs.GetDay() < rhs.GetDay()));
}

class Database {
public:

	void AddEvent(const Date& date, const string& event) {
		history[date].insert(event);
	}

	bool DeleteEvent(const Date& date, const string& event) {
		if (history[date].count(event)) {
			history[date].erase(event);

			if (history[date].size() == 0) {
				history.erase(date);
			}

			return true;
		}
		else {
			return false;
		}
	}

	int DeleteDate(const Date& date) {
		int n = history[date].size();
		history.erase(date);
		return n;
	}

	void Find(const Date& date) const {
		for (auto const& pair : history)
		{
			if (pair.first == date) {
				for (auto const& episode : pair.second) {
					cout << episode << endl;
				}
			}
		}
	}

	void Print() const {
		for (auto const& pair : history)
		{
			for (auto episode : pair.second) {
				cout << pair.first << " " << episode << endl;
			}
		}
	}

private:
	map<Date, set<string>> history;
};

int main() {

	//freopen("input.txt", "r", stdin);

	Database db;
	string operation, episode;
	Date lhs = Date(0, 0, 0);

	string command;
	while (getline(cin, command)) {
		if (command == "\n" || command.size() == 0) {
			continue;
		}
		else {
			stringstream what_to_do(command);
			what_to_do >> operation;

			if (operation == "Add") {
				try {
					what_to_do >> lhs;
					what_to_do >> episode;
					db.AddEvent(lhs, episode);
				}
				catch (invalid_argument& ex) {
					cout << ex.what();
					exit(0);
				}
				catch (exception& ex) {
					cout << ex.what();
					exit(0);
				}
			}

			else if (operation == "Del") {
				try {
					what_to_do >> lhs;
					if (what_to_do.peek() == EOF || what_to_do.peek() == '\n') {
						cout << "Deleted " << to_string(db.DeleteDate(lhs)) << " events\n";
					}
					else {
						what_to_do >> episode;
						if (db.DeleteEvent(lhs, episode)) {
							cout << "Deleted successfully\n";
						}
						else {
							cout << "Event not found\n";
						}
					}
				}
				catch (invalid_argument & ex) {
					cout << ex.what();
					exit(0);
				}
				catch (exception & ex) {
					cout << ex.what();
					exit(0);
				}

			}

			else if (operation == "Find") {
				try {
					what_to_do >> lhs;
					db.Find(lhs);
				}
				catch (invalid_argument & ex) {
					cout << ex.what();
					exit(0);
				}
				catch (exception & ex) {
					cout << ex.what();
					exit(0);
				}
			}

			else if (operation == "Print") {
				db.Print();
			}

			else {
				cout << "Unknown command: " + operation;
				exit(0);
			}

		}
	}
	return 0;
}
