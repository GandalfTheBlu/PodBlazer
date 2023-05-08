#include "file_reader.h"
#include <fstream>
#include <sstream>

namespace Engine
{
	bool ReadTextFile(const std::string& path, std::string& text)
	{
		std::ifstream file;
		file.open(path);

		if (!file.is_open()) {
			printf("[ERROR] failed to open file '%s'\n", path.c_str());
			return false;
		}

		std::stringstream stringStream;
		stringStream << file.rdbuf();
		text = stringStream.str();
		file.close();

		return true;
	}
}