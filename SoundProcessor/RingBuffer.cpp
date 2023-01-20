#include "RingBuffer.h"

RingBuffer::RingBuffer(Size bufferSize) {
	if (bufferSize == 0) {
		std::cerr << "Buffer size must not be 0! It will be 2048." << std::endl;
		bufferSize = 2048;
	}
	if ((bufferSize & (bufferSize - 1)) != 0) {
		Size power = 1;
		while (bufferSize != 1) {
			bufferSize /= 2;
			++power;
		}
		bufferSize <<= power;
	}
	_bufferSize = bufferSize;
	_buffer = new Sample[_bufferSize];
	_mask = _bufferSize - 1;
	_idxIn = 0;
	_idxOut = 0;
}

RingBuffer::~RingBuffer() {
	delete[] _buffer;
}

Size RingBuffer::size() const {
	return _bufferSize;
}

bool RingBuffer::isEmpty() const {
	return _idxIn == _idxOut;
}

bool RingBuffer::isFull() const {
	return ((Size)(_idxIn - _idxOut) & (Size)~(_mask)) != 0;
}

Size RingBuffer::count() const {
	if (isFull()) {
		return _bufferSize;
	}
	return (_idxIn - _idxOut) & _mask;
}

bool RingBuffer::putSample(Sample sample) {
	if (isFull()) {
		return false;
	}
	_buffer[_idxIn++ & _mask] = sample;
	return true;
}

bool RingBuffer::getSample(Sample &sample) {
	if (isEmpty()) {
		return false;
	}
	sample = _buffer[_idxOut++ & _mask];
	return true;
}

Size RingBuffer::getSamples(Sample* samples, Size count) {
	Size countReadSamples = 0;
	while (!isEmpty() && countReadSamples < count) {
		getSample(samples[countReadSamples++]);
	}
	return countReadSamples;
}

Size RingBuffer::putSamples(Sample* samples, Size count) {
	Size countWroteSamples = 0;
	while (!isFull() && countWroteSamples < count) {
		putSample(samples[countWroteSamples++]);
	}
	return countWroteSamples;
}

RingBuffer::RingBuffer(RingBuffer &&buffer) noexcept {
	delete[] _buffer;
	_buffer = buffer._buffer;
	_bufferSize = buffer._bufferSize;
	_mask = buffer._mask;
	_idxIn = buffer._idxIn;
	_idxOut = buffer._idxOut;
	buffer._buffer = nullptr;
}

RingBuffer &RingBuffer::operator=(RingBuffer &&buffer) noexcept {
	if (&buffer == this) {
		return *this;
	}
	delete[] _buffer;
	_buffer = buffer._buffer;
	_bufferSize = buffer._bufferSize;
	_mask = buffer._mask;
	_idxIn = buffer._idxIn;
	_idxOut = buffer._idxOut;
	buffer._buffer = nullptr;
	return *this;
}
