#include "Mixer.h"

Mixer::Mixer(float insertionPoint) {
	if (insertionPoint >= 0) {
		_insertionPoint = insertionPoint;
	}
}

Sample Mixer::handle(std::vector<Sample> samples) {
	if (samples.size() >= 2) {
		Sample res;
		if (_numReadSamples / 44100.0 >= _insertionPoint) {
			res = (Sample)(((double)samples[0] + (double)samples[1]) / 2.0);
		}
		else {
			res = samples[0];
		}
		++_numReadSamples;
		return res;
	}
	return 0;
}

std::string Mixer::help() {
	return std::string("Mixer(mix). Two parameters: an additional stream and the insertion location (in seconds, 0 by default). "
					   "Output stream: Each sample is the average of the corresponding samples of the input streams. "
					   "The length of the output stream corresponds to the length of the main input stream. "
					   "If the additional stream is longer than the main one, then part of it is cut off (not used), "
					   "and if it is shorter, then the output stream outside it is equal to the main input stream.");
}
