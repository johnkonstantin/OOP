#include "Parser.h"

Field* Parser::parseField(std::istream &is) {
	std::string str;
	std::getline(is, str);
	if (str != "#Life 1.06") {
		throw std::runtime_error("Wrong file format!!!!!!!!!!");
	}
	bool haveName = false;
	bool haveRule = false;
	bool haveSize = false;
	bool surviveRule[9] = {false, false, true, true, false, false, false, false, false};
	bool birthRule[9] = {false, false, false, true, false, false, false, false, false};
	std::string name = "Standard Universe";
	size_t sizeX = 0;
	size_t sizeY = 0;
	std::vector<std::pair<size_t, size_t>> cells;
	while (!is.eof()) {
		std::getline(is, str);
		if (str.size() == 0) {
			continue;
		}
		if (str[0] == '#') {
			switch (str[1]) {
				case 'N': {
					if (str.length() < 4) {
						throw std::runtime_error("Wrong name format!!!!!!");
					}
					name = std::string(str.c_str() + 3);
					haveName = true;
					break;
				}
				case 'R': {
					surviveRule[2] = false;
					surviveRule[3] = false;
					birthRule[3] = false;
					char* t = (char*)(str.c_str() + 3);
					std::string str = std::string(t);
					std::vector<std::string> vec = split(str, '/');
					if (vec.size() != 2) {
						throw std::runtime_error("Wrong rule format!!!!!!");
					}
					if (vec[0][0] != 'B' || vec[1][0] != 'S') {
						throw std::runtime_error("Wrong rule format!!!!!!");
					}
					for (int i = 1; i < vec[0].length(); ++i) {
						if (std::isdigit(vec[0][i])) {
							birthRule[vec[0][i] - '0'] = true;
						}
						else {
							throw std::runtime_error("Wrong rule format!!!!!!");
						}
					}
					for (int i = 1; i < vec[1].length(); ++i) {
						if (std::isdigit(vec[1][i])) {
							surviveRule[vec[1][i] - '0'] = true;
						}
						else {
							throw std::runtime_error("Wrong rule format!!!!!!");
						}
					}
					haveRule = true;
					break;
				}
				case 'S': {
					if (str.length() < 3) {
						throw std::runtime_error("Wrong size format!!!!!!");
					}
					for (size_t i = 3; i < str.length() - 3; ++i) {
						if (!std::isdigit(str[i]) && str[i] != ' ') {
							throw std::runtime_error("Wrong size format!!!!!!");
						}
					}
					char* t = (char*)(str.c_str());
					t += 3;
					char* end = nullptr;
					sizeX = strtoull(t, &end, 10);
					t = end;
					sizeY = strtoull(t, &end, 10);
					if ((sizeX == 0 && sizeY == 0 && str.length() != 3) ||
					    (sizeX == ULLONG_MAX || sizeY == ULLONG_MAX)) {
						throw std::runtime_error("Wrong size format!!!!!!");
					}
					haveSize = true;
					break;
				}
				default: {
					throw std::runtime_error("Wrong file format!!!!!!");
				}
			}
		}
		else {
			if (str.length() < 3) {
				throw std::runtime_error("Wrong cell format!!!!!!");
			}
			char* t = (char*)(str.c_str());
			char* end = nullptr;
			size_t posX = strtoull(t, &end, 10);
			t = end;
			size_t posY = strtoull(t, &end, 10);
			if ((posX == 0 && posY == 0 && str.length() != 3) || (posX == ULLONG_MAX || posY == ULLONG_MAX)) {
				throw std::runtime_error("Wrong cell format!!!!!!");
			}
			cells.emplace_back(posX, posY);
		}
	}

	if (!haveSize) {
		size_t maxX = 0;
		size_t maxY = 0;
		for (auto c: cells) {
			if (c.first > maxX) {
				maxX = c.first;
			}
			if (c.second > maxY) {
				maxY = c.second;
			}
		}
		maxX += 5;
		maxY += 5;
		sizeX = maxX;
		sizeY = maxY;
	}
	if (!haveRule) {
		std::cerr << "No rule, used standard instead." << std::endl;
	}
	Rule rule = Rule(surviveRule, birthRule);
	if (!haveName) {
		std::cerr << "No name, used standard instead." << std::endl;
	}
	Field* field = new Field(name, rule, sizeX, sizeY);
	field->setCells(cells);

	return field;
}

std::vector<std::string> Parser::parseArgs(int argc, char** argv) {
	if (argc == 1) {
		return std::vector<std::string>{};
	}
	if (argc == 2) {
		char* str = argv[1];
		if (str[0] != '-' || str[1] != '-') {
			throw std::runtime_error("Wrong argument format!!!!!!!!");
		}
		str += 2;
		std::vector<std::string> vec = split(str, '=');
		if (vec[0] != "input") {
			throw std::runtime_error("Wrong argument format!!!!!!!!");
		}
		std::ifstream fin(vec[1]);
		if (!fin.is_open()) {
			throw std::runtime_error("Wrong input path!!!!!!");
		}
		fin.close();
		return std::vector<std::string>{vec[1]};
	}
	if (argc == 4) {
		std::string input, output, iterations;
		for (int i = 1; i < 4; ++i) {
			char* str = argv[i];
			if (str[0] != '-' || str[1] != '-') {
				throw std::runtime_error("Wrong argument format!!!!!!!!");
			}
			str += 2;
			std::vector<std::string> vec = split(str, '=');
			if (vec[0] == "input") {
				input = vec[1];
				std::ifstream fin(input);
				if (!fin.is_open()) {
					throw std::runtime_error("Wrong argument!!!!!!!!");
				}
				fin.close();
			}
			else if (vec[0] == "output") {
				output = vec[1];
			}
			else if (vec[0] == "iterations") {
				iterations = vec[1];
				if (strtoull(iterations.c_str(), nullptr, 10) == 0 ||
				    strtoull(iterations.c_str(), nullptr, 10) == ULLONG_MAX) {
					throw std::runtime_error("Wrong argument!!!!!!!!");
				}
			}
			else {
				throw std::runtime_error("Wrong arguments!!!!!!!!");
			}
		}
		return std::vector<std::string>{input, output, iterations};
	}
	throw std::runtime_error("Wrong arguments!!!!!!!!");
}

std::vector<std::string> Parser::split(std::string str, char sep = ' ') {
	std::vector<std::string> res;
	std::string t = "";
	for (char c: str) {
		if (c != sep) {
			t += c;
		}
		else {
			res.emplace_back(t);
			t = "";
		}
	}
	res.emplace_back(t);
	return res;
}

std::pair<Parser::Command, std::string> Parser::checkCommand(Field* field) {
	if (kbhit()) {
		getch();
		COORD point = {0, static_cast<SHORT>(field->getSizeY() + 1)};
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(console, point);
		SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		std::cout << "Enter command: ";
		std::string str;
		std::getline(std::cin, str);
		std::vector<std::string> vec;
		std::string t;
		for (char c: str) {
			if (c == ' ') {
				vec.emplace_back(t);
				t = "";
			}
			else {
				t += c;
			}
		}
		vec.emplace_back(t);
		switch (vec.size()) {
			case 1:
				if (vec[0] == "exit") {
					return std::pair<Parser::Command, std::string>{Parser::Command::EXIT, ""};
				}
				else if (vec[0] == "help") {
					return std::pair<Parser::Command, std::string>{Parser::Command::HELP, ""};
				}
				else if (vec[0] == "tick") {
					return std::pair<Parser::Command, std::string>{Parser::Command::TICK, "1"};
				}
				else {
					return std::pair<Parser::Command, std::string>{Parser::Command::ERR, ""};
				}
			case 2:
				if (vec[0] == "dump") {
					return std::pair<Parser::Command, std::string>{Parser::Command::DUMP, vec[1]};
				}
				else if (vec[0] == "tick") {
					size_t ticks = strtoull(vec[1].c_str(), nullptr, 10);
					if (ticks == 0 || ticks == ULLONG_MAX) {
						std::cerr << "Wrong command!!!!" << std::endl;
						return std::pair<Parser::Command, std::string>{Parser::Command::ERR, ""};
					}
					return std::pair<Parser::Command, std::string>{Parser::Command::TICK, vec[1]};
				}
				else {
					return std::pair<Parser::Command, std::string>{Parser::Command::ERR, ""};
				}
			default:
				return std::pair<Parser::Command, std::string>{Parser::Command::ERR, ""};
		}
	}
	return std::pair<Parser::Command, std::string>{Parser::Command::NO, ""};
}
