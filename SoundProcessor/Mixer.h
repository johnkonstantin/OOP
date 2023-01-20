#ifndef SOUND_PROCESSOR_MIXER_H
#define SOUND_PROCESSOR_MIXER_H

#include "Converter.h"
#include "Types.h"
#include <iostream>

class Mixer : public Converter{
private:
	float _insertionPoint;

public:
	Mixer(float = 0);
	~Mixer() override = default;

	Sample handle(std::vector<Sample>) override;
	std::string help() override;
};


#endif
