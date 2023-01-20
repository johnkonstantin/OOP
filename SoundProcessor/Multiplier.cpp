#include "Multiplier.h"

Multiplier::Multiplier(float k, float begin, float end) {
	_k = k;
	if (begin >= 0 && end >= 0 && (end == 0 || end > begin)) {
		_begin = begin;
		_end = end;
	}
	else {
		throw std::runtime_error("Error while setting Multiplier block: wrong bounds!");
	}
}

Sample Multiplier::handle(std::vector<Sample> samples) {
	if (!samples.empty()) {
		Sample res = samples[0];
		if (_end != 0) {
			if (_numReadSamples / 44100.0 >= _begin && _numReadSamples / 44100.0 <= _end) {
				double t = res * _k;
				if (t > 32767) {
					t = 32767;
				}
				if (t < -32768) {
					t = -32768;
				}
				res = t;
			}
		}
		else {
			if (_numReadSamples / 44100.0 >= _begin) {
				float t = res * _k;
				if (t > 32767) {
					t = 32767;
				}
				if (t < -32768) {
					t = -32768;
				}
				res = t;
			}
		}
		++_numReadSamples;
		return res;
	}
	return 0;
}

std::string Multiplier::help() {
	return std::string("Multiplying the interval by a constant(mult). "
					   "Three parameters: multiplier, start and isEnd of the time interval (in seconds). "
					   "The start and isEnd are 0 by default. "
					   "If the end of the interval is the isEnd of the file, then the corresponding parameter should be 0. "
					   "In the output stream in this time interval - the multiplied interval with a cutoff, the rest - as in the input stream.");
}
