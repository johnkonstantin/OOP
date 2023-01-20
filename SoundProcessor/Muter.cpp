#include "Muter.h"

Sample Muter::handle(std::vector<Sample> samples) {
	if (!samples.empty()) {
		Sample res = samples[0];
		if (_numReadSamples / 44100.0 >= _beginMute && _numReadSamples / 44100.0 <= _endMute) {
			res = 0;
		}
		++_numReadSamples;
		return res;
	}
	return 0;
}

Muter::Muter(float begin, float end) {
	if (begin >= 0 && end > begin) {
		_beginMute = begin;
		_endMute = end;
	}
	else {
		throw std::runtime_error("Error while setting Muter block: wrong bounds!");
	}
}

std::string Muter::help() {
	return std::string("Mute the interval(mute). Two parameters: the beginning and the isEnd of the time interval (in seconds). "
					   "There is silence in the output stream in this time interval, the rest is like in the input stream.");
}
