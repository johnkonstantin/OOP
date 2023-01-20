#include <iostream>
#include <ctime>
#include <sstream>
#include "Parser.h"
#include "Preset.h"

int main(int argc, char** argv) {
	std::vector<std::string> args = Parser::parseArgs(argc, argv);
	if (args.size() == 3) {
		std::ifstream fin(args[0]);
		std::ofstream fout(args[1]);
		size_t iterations = strtoull(args[2].c_str(), nullptr, 10);
		Field* field = Parser::parseField(fin);
		fin.close();
		Field::FieldPrinter printer = Field::FieldPrinter(fout, field, false);
		for (size_t i = 0; i < iterations; ++i) {
			field->nextGeneration();
		}
		printer.dumpWorld(fout);
		fout.close();
		delete field;
	}
	else {
		Field* field = nullptr;
		time_t tLast = 0;
		if (args.empty()) {
			std::stringstream in;
			in << golPreset::preset;
			field = Parser::parseField(in);
		}
		else {
			std::ifstream fin(args[0]);
			field = Parser::parseField(fin);
			fin.close();
		}
		Field::FieldPrinter printer = Field::FieldPrinter(std::cout, field, true);
		while (true) {
			std::pair<Parser::Command, std::string> p = Parser::checkCommand(field);
			switch (p.first) {
				case Parser::DUMP: {
					std::ofstream fout(p.second);
					printer.dumpWorld(fout);
					fout.close();
					break;
				}
				case Parser::TICK: {
					size_t ticks = strtoull(p.second.c_str(), nullptr, 10);
					for (size_t i = 0; i < ticks; ++i) {
						field->nextGeneration();
					}
					printer.print();
					break;
				}
				case Parser::EXIT:
					delete field;
					printer.clearScreen();
					return 0;
				case Parser::HELP:
					std::cout << std::endl;
					std::cout << "dump <filename> - save current universe to the file\n"
					             "tick <n=1> - skip n (default 1) iterations and print universe\n"
					             "exit - finish the game\n"
					             "help - print the help about the command";
					getch();
					break;
				case Parser::NO:
					break;
				case Parser::ERR:
					std::cerr << "Wrong command!!!!!!!!" << std::endl;
					getch();
					break;
			}
			if ((float)(clock() - tLast) / CLOCKS_PER_SEC >= 0.1) {
				printer.print();
				field->nextGeneration();
				tLast = clock();
			}
		}
	}
	return 0;
}
