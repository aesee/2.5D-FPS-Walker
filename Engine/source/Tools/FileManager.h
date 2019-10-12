#pragma once
#include <string>

class FileManager
{
public:
	static void ReadStringFromFile(std::string pathToFile, std::wstring& outString);
};