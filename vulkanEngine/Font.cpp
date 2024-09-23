#include "Font.hpp"

#include <sstream>
#include <iostream>


void FontData::parse(const std::string& data) {
	std::istringstream stream(data);
	std::string line;

	while (std::getline(stream, line)) {
		if (line.find("info") == 0 || line.find("common") == 0) {
			parseInfo(line);
		}
		else if (line.find("char") == 0) {
			parseChar(line);
		}
		else if (line.find("kernings") == 0) {
			// Skip the kernings count line
			std::getline(stream, line);
			while (std::getline(stream, line) && !line.empty()) {
				parseKerning(line);
			}
		}
	}
}

void FontData::parseInfo(const std::string& line)
{
	{
		std::istringstream iss(line);
		std::string token;
		while (iss >> token) {
			if (token.find("face=") == 0) {
				face = token.substr(6, token.size() - 8); // Extract face name
			}
			else if (token.find("size=") == 0) {
				size = std::stoi(token.substr(5));
			}
			else if (token.find("scaleW=") == 0) {
				std::cout << "sad" << std::endl;
				scale = std::stof(token.substr(7));
			}
		}
	}
}

void FontData::parseChar(const std::string& line)
{
	{
		CharInfo charInfo;
		std::istringstream iss(line);
		std::string token;

		while (iss >> token) {
			if (token.find("id=") == 0) {
				charInfo.id = std::stoi(token.substr(3));
			}
			else if (token.find("x=") == 0) {
				charInfo.x = std::stoi(token.substr(2));
			}
			else if (token.find("y=") == 0) {
				charInfo.y = std::stoi(token.substr(2));
			}
			else if (token.find("width=") == 0) {
				charInfo.width = std::stoi(token.substr(6));
			}
			else if (token.find("height=") == 0) {
				charInfo.height = std::stoi(token.substr(7));
			}
			else if (token.find("xoffset=") == 0) {
				charInfo.xoffset = std::stoi(token.substr(8));
			}
			else if (token.find("yoffset=") == 0) {
				charInfo.yoffset = std::stoi(token.substr(8));
			}
			else if (token.find("xadvance=") == 0) {
				charInfo.xadvance = std::stoi(token.substr(9));
			}
		}
		chars.push_back(charInfo);
	}
}

void FontData::parseKerning(const std::string& line)
{
	{
		Kerning kerning;
		std::istringstream iss(line);
		std::string token;

		while (iss >> token) {
			if (token.find("first=") == 0) {
				kerning.first = std::stoi(token.substr(6));
			}
			else if (token.find("second=") == 0) {
				kerning.second = std::stoi(token.substr(7));
			}
			else if (token.find("amount=") == 0) {
				kerning.amount = std::stoi(token.substr(7));
			}
		}
		kernings.push_back(kerning);
	}
}
