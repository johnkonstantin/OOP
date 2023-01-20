#include "Field.h"

Field::Cell::Cell(Field &field, size_t posX, size_t posY) : _field(field) {
	if (posX > field._sizeX || posY > field._sizeY) {
		throw std::runtime_error("Out of bounds!!!!!!!!");
	}
	_posX = posX;
	_posY = posY;
	_alive = false;
	_numNeighbors = 0;
}

bool Field::Cell::isAlive() {
	return _alive;
}

void Field::Cell::setAlive(bool alive) {
	_alive = alive;
}

int Field::Cell::getNumNeighbors() {
	return _numNeighbors;
}

void Field::Cell::updateNeighbors() {
	_numNeighbors = 0;
	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			size_t X = relToAbsPosX(j);
			size_t Y = relToAbsPosY(i);
			_numNeighbors += (_field._field[_field._sizeY * Y + X]->_alive) ? 1 : 0;
		}
	}
	_numNeighbors -= (_alive) ? 1 : 0;
}

size_t Field::Cell::relToAbsPosX(ssize_t posX) {
	return (_field._sizeX + _posX + posX) % _field._sizeX;
}

size_t Field::Cell::relToAbsPosY(ssize_t posY) {
	return (_field._sizeY + _posY + posY) % _field._sizeY;
}

size_t Field::getSizeX() const {
	return _sizeX;
}

size_t Field::getSizeY() const {
	return _sizeY;
}

void Field::nextGeneration() {
	for (size_t i = 0; i < _sizeX * _sizeY; ++i) {
		_field[i]->updateNeighbors();
	}

	for (size_t i = 0; i < _sizeX * _sizeY; ++i) {
		if (_field[i]->isAlive()) {
			_field[i]->setAlive(_rule.willSurvive(_field[i]->getNumNeighbors()));
		}
		else {
			_field[i]->setAlive(_rule.willBorn(_field[i]->getNumNeighbors()));
		}
	}
	++_iterationNum;
}

Field::Field(std::string name, Rule rule, size_t sizeX, size_t sizeY) : _name(name) {
	if (sizeX == 0 || sizeY == 0) {
		throw std::runtime_error("Field size cannot be zero!!!!!!");
	}
	_sizeX = sizeX;
	_sizeY = sizeY;
	_rule = rule;

	_field = new Cell* [_sizeX * _sizeY];
	for (size_t i = 0; i < _sizeY; ++i) {
		for (size_t j = 0; j < _sizeX; ++j) {
			_field[i * _sizeY + j] = new Cell(*this, j, i);
		}
	}
	_iterationNum = 0;
}

Field::~Field() {
	for (size_t i = 0; i < _sizeX * _sizeY; ++i) {
		delete _field[i];
	}
	delete[] _field;
}

void Field::setCells(std::vector<std::pair<size_t, size_t>> cells) {
	if (cells.size() > _sizeX * _sizeY) {
		std::cerr << "Too much Cells!!!!!!!" << std::endl;
	}
	genocide();
	for (size_t i = 0; i < std::min(cells.size(), _sizeX * _sizeY); ++i) {
		size_t posX, posY;
		posX = cells[i].first;
		posY = cells[i].second;
		if (posX > _sizeX || posY > _sizeY) {
			throw std::runtime_error("Out of bounds!!!!!!!!");
		}
		_field[posY * _sizeY + posX]->setAlive(true);
	}
}

void Field::genocide() {
	for (size_t i = 0; i < _sizeX * _sizeY; ++i) {
		_field[i]->setAlive(false);
	}
}

std::string Field::getName() {
	return _name;
}

size_t Field::getIterationNum() {
	return _iterationNum;
}

void Field::FieldPrinter::print() {
	if (_consoleMode) {
		clearScreen();
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD point;
		point.X = 0;
		DWORD l;
		for (size_t i = 1; i < _field->_sizeY + 1; ++i) {
			point.Y = i;
			SetConsoleCursorPosition(console, point);
			FillConsoleOutputAttribute(console, BACKGROUND_GREEN, _field->_sizeX * 2, point, &l);
		}
		point.Y = 0;
		SetConsoleCursorPosition(console, point);
		_os << _field->getName() << " Iterations: " << _field->getIterationNum() + 1 << std::endl;
		SetConsoleTextAttribute(console, BACKGROUND_RED);
		for (size_t i = 0; i < _field->_sizeY; ++i) {
			for (size_t j = 0; j < _field->_sizeX; ++j) {
				if (_field->_field[i * _field->_sizeY + j]->isAlive()) {
					point.X = j * 2;
					point.Y = i + 1;
					SetConsoleCursorPosition(console, point);
					_os << "  ";
				}
			}
		}
	}
	else {
		_os << _field->getName() << " Iteration: " << _field->getIterationNum() + 1 << std::endl;
		for (size_t i = 0; i < _field->_sizeY; ++i) {
			for (size_t j = 0; j < _field->_sizeX; ++j) {
				if (_field->_field[i * _field->_sizeY + j]->isAlive()) {
					_os << 'x';
				}
				else {
					_os << 'o';
				}
			}
			_os << std::endl;
		}
	}
}

Field::FieldPrinter::FieldPrinter(std::ostream &os, Field* field, bool consoleMode = false)
		: _os(os), _field(field), _consoleMode(consoleMode) {
	if (_field == nullptr) {
		throw std::runtime_error("Field is null!!!!!!!!!!!");
	}
	if (_consoleMode) {
		clearScreen();
	}
}

void Field::FieldPrinter::clearScreen() {
	COORD tl = {0, 0};
	CONSOLE_SCREEN_BUFFER_INFO s;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	GetConsoleScreenBufferInfo(console, &s);
	DWORD written, cells = s.dwSize.X * s.dwSize.Y;
	FillConsoleOutputCharacter(console, ' ', cells, tl, &written);
	FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(console, tl);
}

void Field::FieldPrinter::dumpWorld(std::ostream &os) {
	os << "#Life 1.06" << std::endl;
	os << "#N " << _field->getName() << std::endl;
	os << "#R B";
	for (int i = 0; i < 9; ++i) {
		if (_field->_rule.willBorn(i)) {
			os << i;
		}
	}
	os << "/S";
	for (int i = 0; i < 9; ++i) {
		if (_field->_rule.willSurvive(i)) {
			os << i;
		}
	}
	os << std::endl;
	os << "#S " << _field->_sizeX << " " << _field->_sizeY << std::endl;

	for (size_t i = 0; i < _field->_sizeY; ++i) {
		for (size_t j = 0; j < _field->_sizeX; ++j) {
			if (_field->_field[i * _field->_sizeY + j]->isAlive()) {
				os << j << " " << i << std::endl;
			}
		}
	}
}
