#include "SoundConverter.h"

SoundConverter::SoundConverter(const std::string &configPath, const std::vector<std::string> &files) {
	std::ifstream config(configPath);
	if (!config.is_open()) {
		throw std::runtime_error("Error while setting Sound Converter: wrong config file path!");
	}
	while (!config.eof()) {
		std::string line;
		std::getline(config, line);
		if (line.empty()) {
			continue;
		}
		std::vector<std::string> args;
		{
			std::string tempStr;
			for (auto c: line) {
				if (c == ' ') {
					args.emplace_back(tempStr);
					tempStr = "";
				}
				else {
					tempStr += c;
				}
			}
			args.emplace_back(tempStr);
		}
		if (args[0] == "mute") {
			if (args.size() != 3) {
				for (auto t: _converters) {
					delete t;
				}
				for (const auto &t: _streams) {
					for (auto tt: t) {
						delete tt;
					}
				}
				throw std::runtime_error("Error while parsing config file: too few mute arguments!");
			}
			float begin, end;
			begin = std::stof(args[1]);
			end = std::stof(args[2]);
			auto muter = new Muter(begin, end);
			_converters.emplace_back(muter);
			_streams.emplace_back(std::vector<WavSource*>{});
		}
		else if (args[0] == "mix") {
			if (args.size() < 2 || args.size() > 3) {
				for (auto t: _converters) {
					delete t;
				}
				for (const auto &t: _streams) {
					for (auto tt: t) {
						delete tt;
					}
				}
				throw std::runtime_error("Error while parsing config file: too few mix arguments!");
			}
			int numStream;
			float insertionPoint;
			if (args[1][0] != '$') {
				for (auto t: _converters) {
					delete t;
				}
				for (const auto &t: _streams) {
					for (auto tt: t) {
						delete tt;
					}
				}
				throw std::runtime_error("Error while parsing config file: wrong mix argument!");
			}
			std::string numStreamStr(args[1].c_str() + 1);
			numStream = std::stoi(numStreamStr);
			if (numStream < 1 || numStream > files.size() - 1) {
				for (auto t: _converters) {
					delete t;
				}
				for (const auto &t: _streams) {
					for (auto tt: t) {
						delete tt;
					}
				}
				throw std::runtime_error("Error while parsing config file: wrong mix argument!");
			}
			if (args.size() == 2) {
				insertionPoint = 0;
			}
			else {
				insertionPoint = std::stof(args[2]);
			}
			auto mixer = new Mixer(insertionPoint);
			_converters.emplace_back(mixer);
			_streams.emplace_back(std::vector<WavSource*>{new WavSource(files[numStream])});
		}
		else if (args[0] == "mult") {
			if (args.size() < 2 || args.size() > 4) {
				for (auto t: _converters) {
					delete t;
				}
				for (const auto &t: _streams) {
					for (auto tt: t) {
						delete tt;
					}
				}
				throw std::runtime_error("Error while parsing config file: too few mult arguments!");
			}
			float k, begin, end;
			k = std::stof(args[1]);
			begin = 0;
			end = 0;
			if (args.size() == 3) {
				begin = std::stof(args[2]);
			}
			else if (args.size() == 4) {
				begin = std::stof(args[2]);
				end = std::stof(args[3]);
			}
			auto multiplier = new Multiplier(k, begin, end);
			_converters.emplace_back(multiplier);
			_streams.emplace_back(std::vector<WavSource*>{});
		}
		else if (args[0][0] == '#') {
			continue;
		}
		else {
			for (auto t: _converters) {
				delete t;
			}
			for (const auto &t: _streams) {
				for (auto tt: t) {
					delete tt;
				}
			}
			throw std::runtime_error("Error while parsing config file: wrong config file format!");
		}
	}
	_mainInput = new WavSource(files[1]);
	_output = new WavSink(files[0]);
}

void SoundConverter::convert() {
	while (!_mainInput->isEnd()) {
		Sample res = _mainInput->readSample();
		for (Size i = 0; i < _converters.size(); ++i) {
			std::vector<Sample> samples;
			samples.emplace_back(res);
			for (auto stream: _streams[i]) {
				samples.emplace_back(stream->readSample());
			}
			res = _converters[i]->handle(samples);
		}
		_output->writeSample(res);
	}
}

SoundConverter::~SoundConverter() {
	for (auto converter: _converters) {
		delete converter;
	}
	for (const auto &streamVec: _streams) {
		for (auto stream: streamVec) {
			delete stream;
		}
	}
	delete _mainInput;
	delete _output;
}

std::string SoundConverter::generateHelp() {
	Muter muter = Muter(0, 1);
	Mixer mixer = Mixer(0);
	Multiplier multiplier = Multiplier(0, 0, 0);

	std::string helpStr;
	helpStr += muter.help();
	helpStr += '\n';
	helpStr += mixer.help();
	helpStr += '\n';
	helpStr += multiplier.help();
	helpStr += '\n';

	return helpStr;
}
