cmake_minimum_required(VERSION 3.22)
project(sound_processor)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(sound_processor main.cpp Converter.cpp Converter.h WavSource.cpp WavSource.h WavHeader.h WavSink.cpp WavSink.h Muter.cpp Muter.h Mixer.cpp Mixer.h Multiplier.cpp Multiplier.h Parser.cpp Parser.h Types.h RingBuffer.cpp RingBuffer.h SoundConverter.cpp SoundConverter.h)
