#ifndef GOL_PARSER_H
#define GOL_PARSER_H

#include <stdexcept>
#include <vector>
#include "Field.h"
#include <conio.h>

class Parser {
public:
	enum Command {
		DUMP,
		TICK,
		EXIT,
		HELP,
		NO,
		ERR
	};

	static Field* parseField(std::istream &);
	static std::vector<std::string> parseArgs(int, char**);
	static std::vector<std::string> split(std::string, char);
	static std::pair<Parser::Command, std::string> checkCommand(Field*);
};


#endif
