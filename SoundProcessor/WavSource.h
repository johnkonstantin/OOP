#ifndef SOUND_PROCESSOR_WAVSOURCE_H
#define SOUND_PROCESSOR_WAVSOURCE_H

#include "Converter.h"
#include "WavHeader.h"
#include "Types.h"
#include "RingBuffer.h"
#include <string>
#include <fstream>

class WavSource {
private:
	std::ifstream* _fin;
	WavHeader _header;
	Size _numReadSamples;
	Size _fileSize;
	Size _numSamples;
	RingBuffer _buffer;

	void updateBuffer();

public:
	WavSource(const std::string &, Size = 2048);
	~WavSource();
	bool isEnd();
	Sample readSample();
};


#endif
