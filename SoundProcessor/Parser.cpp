#include "Parser.h"

std::pair<std::string, std::vector<std::string>> Parser::parseArgs(int argc, char** argv) {
	if (argc == 2) {
		if (argv[1][0] == '-' && argv[1][1] == 'h') {
			return std::pair<std::string, std::vector<std::string>>{"help", std::vector<std::string>{"help"}};
		}
		else {
			throw std::runtime_error("Error while parsing arguments: wrong arguments!");
		}
	}
	if (argc < 5) {
		throw std::runtime_error("Error while parsing arguments: too few arguments!");
	}

	std::pair<std::string, std::vector<std::string>> res;

	if (argv[1][0] != '-' && argv[1][1] != 'c') {
		throw std::runtime_error("Error while parsing arguments: wrong arguments!");
	}

	res.first = argv[2];
	for (Size i = 3; i < argc; ++i) {
		res.second.emplace_back(argv[i]);
	}

	return res;
}
