#ifndef SOUND_PROCESSOR_SOUNDCONVERTER_H
#define SOUND_PROCESSOR_SOUNDCONVERTER_H

#include "Types.h"
#include "WavSource.h"
#include "WavSink.h"
#include "Muter.h"
#include "Mixer.h"
#include "Multiplier.h"

class SoundConverter {
private:
	std::vector<Converter*> _converters;
	WavSource* _mainInput;
	WavSink* _output;
	std::vector<std::vector<WavSource*>> _streams;

public:
	SoundConverter(const std::string &, const std::vector<std::string> &);
	~SoundConverter();

	void convert();

	static std::string generateHelp();
};


#endif
