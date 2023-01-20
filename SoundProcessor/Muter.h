#ifndef SOUND_PROCESSOR_MUTER_H
#define SOUND_PROCESSOR_MUTER_H

#include "Converter.h"

class Muter : public Converter {
private:
	float _beginMute;
	float _endMute;

public:
	Muter(float, float);
	~Muter() override = default;
	Sample handle(std::vector<Sample>) override;
	std::string help() override;
};


#endif
