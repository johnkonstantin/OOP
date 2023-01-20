#ifndef SOUND_PROCESSOR_RINGBUFFER_H
#define SOUND_PROCESSOR_RINGBUFFER_H

#include "Types.h"
#include <iostream>

class RingBuffer {
private:
	Sample* _buffer = nullptr;
	Size _bufferSize;
	Size _mask;
	Size _idxIn;
	Size _idxOut;

public:
	RingBuffer(Size = 2048);
	RingBuffer(const RingBuffer &) = delete;
	RingBuffer &operator=(const RingBuffer &) = delete;
	RingBuffer(RingBuffer &&) noexcept;
	RingBuffer &operator=(RingBuffer &&) noexcept;
	~RingBuffer();

	bool putSample(Sample);
	bool getSample(Sample &);
	Size putSamples(Sample*, Size);
	Size getSamples(Sample*, Size);
	bool isEmpty() const;
	bool isFull() const;
	Size size() const;
	Size count() const;


};


#endif
