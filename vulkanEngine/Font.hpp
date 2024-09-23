#pragma once

#include "Util.hpp"


class FontData {
public:
	std::string face;
	int size;
	float scale;
	std::vector<CharInfo> chars;
	std::vector<Kerning> kernings;

	void parse(const std::string& data);

private:
	void parseInfo(const std::string& line);

	void parseChar(const std::string& line);

	void parseKerning(const std::string& line);
};