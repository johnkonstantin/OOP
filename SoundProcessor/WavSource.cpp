#include "WavSource.h"

WavSource::WavSource(const std::string &path, Size bufferSize) {
	_fin = new std::ifstream(path, std::ios_base::in | std::ios_base::binary);
	if (!_fin->is_open()) {
		throw std::runtime_error("Error while setting Wav Source block: wrong file path!");
	}
	_fin->seekg(0, std::ios_base::end);
	_fileSize = _fin->tellg();
	_fin->seekg(0, std::ios_base::beg);
	_numSamples = (_fileSize - sizeof(_header)) / 2;
	if (_fileSize < sizeof(_header)) {
		throw std::runtime_error("Error while setting Wav Source block: wrong file format!");
	}
	_fin->read((char*)&_header, sizeof(_header));
	if (_header.subchunk1Size != 16 || _header.audioFormat != 1 || _header.numChannels != 1 || _header.bitsPerSample != 16) {
		throw std::runtime_error("Error while setting Wav Source block: wrong file format!");
	}
	_buffer = RingBuffer(bufferSize);
	_numReadSamples = 0;
}

Sample WavSource::readSample() {
	if (_fin->is_open()) {
		if (_buffer.isEmpty()) {
			updateBuffer();
		}
		Sample sample;
		if (_buffer.getSample(sample)) {
			++_numReadSamples;
			return sample;
		}
	}
	return 0;
}

WavSource::~WavSource() {
	_fin->close();
	delete _fin;
}

bool WavSource::isEnd() {
	return _numReadSamples == _numSamples;
}

void WavSource::updateBuffer() {
	if (!_fin->is_open() || _buffer.isFull()) {
		return;
	}
	Size samplesCount = _buffer.size() - _buffer.count();
	char* t = new char[samplesCount * 2];
	_fin->read(t, samplesCount * 2);
	samplesCount = _fin->gcount() / 2;
	for (Size i = 0; i < samplesCount; ++i) {
		Sample sample = (Sample)(((uint8_t)t[i * 2 + 1] << 8) | (uint8_t)t[i * 2]);
		_buffer.putSample(sample);
	}
	delete[] t;
}
