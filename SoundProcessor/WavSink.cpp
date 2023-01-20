#include "WavSink.h"

WavSink::WavSink(const std::string &path, Size bufferSize) {
	_fout = new std::ofstream(path, std::ios_base::out | std::ios_base::binary);
	_header.chunkId[0] = 'R';
	_header.chunkId[1] = 'I';
	_header.chunkId[2] = 'F';
	_header.chunkId[3] = 'F';
	_header.chunkSize = 36;
	_header.format[0] = 'W';
	_header.format[1] = 'A';
	_header.format[2] = 'V';
	_header.format[3] = 'E';
	_header.subchunk1Id[0] = 'f';
	_header.subchunk1Id[1] = 'm';
	_header.subchunk1Id[2] = 't';
	_header.subchunk1Id[3] = 0x20;
	_header.subchunk1Size = 16;
	_header.audioFormat = 1;
	_header.numChannels = 1;
	_header.sampleRate = 44100;
	_header.byteRate = 44100 * 2;
	_header.blockAlign = 2;
	_header.bitsPerSample = 16;
	_header.subchunk2Id[0] = 'd';
	_header.subchunk2Id[1] = 'a';
	_header.subchunk2Id[2] = 't';
	_header.subchunk2Id[3] = 'a';
	_header.subchunk2Size = 0;
	_fout->write((char*)&_header, sizeof(_header));
	_buffer = RingBuffer(bufferSize);
}

WavSink::~WavSink() {
	flush();
	_fout->seekp(0, std::ios_base::beg);
	_header.subchunk2Size = _numWroteSamples;
	_fout->write((char*)&_header, sizeof(_header));
	_fout->close();
	delete _fout;
}

void WavSink::writeSample(Sample sample) {
	if (_fout->is_open()) {
		if (_buffer.isFull()) {
			flush();
		}
		_buffer.putSample(sample);
	}
}

void WavSink::flush() {
	if (_fout->is_open()) {
		Size samplesCount = _buffer.count();
		Sample* samples = new Sample[samplesCount];
		_buffer.getSamples(samples, samplesCount);
		char* buff = new char[samplesCount * 2];
		for (Size i = 0; i < samplesCount; ++i) {
			buff[i * 2] = samples[i] & 0xFF;
			buff[i * 2 + 1] = samples[i] >> 8;
		}
		_fout->write(buff, samplesCount * 2);
		_numWroteSamples += samplesCount;
		delete[] samples;
		delete[] buff;
	}
}
