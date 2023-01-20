#ifndef SOUND_PROCESSOR_WAVSINK_H
#define SOUND_PROCESSOR_WAVSINK_H

#include "Converter.h"
#include "RingBuffer.h"
#include <fstream>
#include "WavHeader.h"
#include "Types.h"

class WavSink {
private:
	std::ofstream* _fout;
	WavHeader _header;
	Size _numWroteSamples;
	RingBuffer _buffer;

	void flush();

public:
	WavSink(const std::string&, Size = 2048);
	~WavSink();
	void writeSample(Sample);
};


#endif
