#include "FileManager.h"
#include <fstream>

void FileManager::ReadStringFromFile(std::string pathToFile, std::wstring& outString)
{
	std::ifstream in(pathToFile);
	/*std::string outString((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());*/

	outString.reserve(16 * 16);
	std::copy(
		std::istreambuf_iterator<char>(in),
		std::istreambuf_iterator<char>(),
		std::back_inserter(outString)
	);
}
