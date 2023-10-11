#include "Filesystem.h"

std::string GetSourceFileName(const char* path)
{
	std::string str_path = path;
	std::string base_filename = str_path.substr(str_path.find_last_of("/\\") + 1);

	return base_filename;
}

std::string GetSourceFileText(const char* path)
{
	std::ifstream Input(path);

	std::string buff = std::string();
	std::string result = std::string();

	while (std::getline(Input, buff))
	{
		//std::printf(result.c_str());
		result.append(buff).append("\n");
	}

	//std::printf(result.c_str());

	Input.close();

	return result;
}