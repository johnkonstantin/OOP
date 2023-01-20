#include <iostream>
#include "Parser.h"
#include "SoundConverter.h"

int main(int argc, char** argv) {
	auto args = Parser::parseArgs(argc, argv);
	if (args.first == "help" && args.second[0] == "help") {
		std::cout << SoundConverter::generateHelp();
		return 0;
	}
	SoundConverter converter = SoundConverter(args.first, args.second);
	converter.convert();

	return 0;
}
