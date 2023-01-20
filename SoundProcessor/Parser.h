#ifndef SOUND_PROCESSOR_PARSER_H
#define SOUND_PROCESSOR_PARSER_H

#include "Types.h"
#include <vector>
#include <stdexcept>

class Parser {
public:
	static std::pair<std::string, std::vector<std::string>> parseArgs(int, char**);
};


#endif
