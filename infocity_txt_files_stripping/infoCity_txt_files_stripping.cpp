// infoCity_txt_files_stripping.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>
#include <boost\filesystem.hpp>
#include <boost\filesystem\fstream.hpp>

void getFileNames(std::string directoryName, std::vector<std::string>& fileNames);
std::string readFile(std::string directoryName, std::string fileName);

int main()
{
	std::vector<std::string> fileNames;

	std::string directoryName;
	std::cout << "Enter directory path: ";
	//std::cin >> directoryName;
	directoryName = "d:/test/";
	getFileNames(directoryName, fileNames);

	for(std::vector<std::string>::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
		std::cout << *it << std::endl;

	std::string file = readFile(directoryName, fileNames[0]);

	std::cin.get();
	return 0;
}

// search directory for txt files with boost
void getFileNames(std::string directoryName, std::vector<std::string>& fileNames)
{
	// transform directoryName to path
	boost::filesystem::path path(directoryName);

	boost::filesystem::directory_iterator end_it;
	for(boost::filesystem::directory_iterator it(path); it != end_it; ++it)
	{
		// skip if not a file
		if(!boost::filesystem::is_regular_file(it->status())) continue;

		// if file is txt than it is stored
		if(it->path().extension() == ".txt")
			fileNames.push_back(it->path().filename().string());
	}
}

// reading file and returning it as a std::string
std::string readFile(std::string directoryName, std::string fileName)
{
	// using boost ifstream, but with std::ifstream would be the same
	boost::filesystem::ifstream inputFile(directoryName + fileName);
	if(!inputFile.good())
		return "ERROR: cannot open file " + fileName;
	else
	{
		std::string file(std::istreambuf_iterator<char>( inputFile ), (std::istreambuf_iterator<char>()));
		return file;
	}
}