#ifndef GOL_FIELD_H
#define GOL_FIELD_H

#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include "Rule.h"

class Field {
private:
	class Cell {
	private:
		bool _alive;
		size_t _posX;
		size_t _posY;
		int _numNeighbors;
		Field &_field;

	public:
		Cell(Field &, size_t, size_t);

		bool isAlive();
		void setAlive(bool);
		int getNumNeighbors();
		void updateNeighbors();
		size_t relToAbsPosX(ssize_t);
		size_t relToAbsPosY(ssize_t);
	};

	size_t _sizeX;
	size_t _sizeY;
	Cell** _field;
	Rule _rule;
	std::string _name;
	size_t _iterationNum;

public:
	Field(std::string, Rule, size_t, size_t);
	~Field();

	size_t getSizeX() const;
	size_t getSizeY() const;
	void nextGeneration();
	void setCells(std::vector<std::pair<size_t, size_t>>);
	void genocide();
	std::string getName();
	size_t getIterationNum();

	class FieldPrinter {
	private:
		bool _consoleMode;
		std::ostream &_os;
		Field* _field;

	public:
		FieldPrinter(std::ostream &, Field*, bool);
		void print();
		void clearScreen();
		void dumpWorld(std::ostream &);
	};

};

#endif
