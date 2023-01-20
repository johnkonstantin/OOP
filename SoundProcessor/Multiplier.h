#ifndef SOUND_PROCESSOR_MULTIPLIER_H
#define SOUND_PROCESSOR_MULTIPLIER_H

#include "Converter.h"
#include "Types.h"

class Multiplier : public Converter {
private:
	float _k;
	float _begin;
	float _end;

public:
	Multiplier(float, float = 0, float = 0);
	~Multiplier() override = default;

	Sample handle(std::vector<Sample>) override;
	std::string help() override;
};


#endif
