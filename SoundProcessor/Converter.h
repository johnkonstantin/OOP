#ifndef SOUND_PROCESSOR_CONVERTER_H
#define SOUND_PROCESSOR_CONVERTER_H

#include <cstdint>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "Types.h"

class Converter {
public:
	Converter();
	virtual ~Converter();

	virtual Sample handle(std::vector<Sample>) = 0;
	virtual std::string help() = 0;

protected:
	size_t _numReadSamples;
};


#endif
