#include "SlippiGameFileLoader.h"

#include "Common/FileUtil.h"
#include "DiscIO/FileMonitor.h"

std::string getFilePath(std::string fileName)
{
	std::string dirPath = File::GetExeDirectory();
	std::string filePath = dirPath + "/Sys/GameFiles/GALE01/" + fileName; // TODO: Handle other games?

	if (File::Exists(filePath))
	{
		return filePath;
	}

	filePath = filePath + ".diff";
	if (File::Exists(filePath))
	{
		return filePath;
	}

	return "";
}

u32 SlippiGameFileLoader::LoadFile(std::string fileName, std::string &data)
{
	if (fileCache.count(fileName))
	{
		data = fileCache[fileName];
		return (u32)data.size();
	}

	std::string gameFilePath = getFilePath(fileName);
	if (gameFilePath.empty())
	{
		fileCache[fileName] = "";
		data = "";
		return 0;
	}

	std::string fileContents;
	File::ReadFileToString(gameFilePath, fileContents);

	if (gameFilePath.substr(gameFilePath.length() - 5) == ".diff")
	{
		// If the file was a diff file, load the main file from ISO and apply patch
		std::vector<u8> buf;
		FileMon::ReadFileWithName(fileName, buf);
		std::string diffContents = fileContents;
		decoder.Decode((char *)buf.data(), buf.size(), diffContents, &fileContents);
	}

	fileCache[fileName] = fileContents;
	data = fileCache[fileName];
	return (u32)data.size();
}
