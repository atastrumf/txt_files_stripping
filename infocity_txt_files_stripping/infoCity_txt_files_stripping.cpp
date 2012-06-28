// infoCity_txt_files_stripping.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost\filesystem.hpp>
#include <boost\filesystem\fstream.hpp>

void getFileNames(std::string directoryName, std::vector<std::string>& fileNames);
std::string readFile(std::string directoryName, std::string fileName);
std::vector<std::string> parseFile(std::string& file);

int main()
{
	std::vector<std::string> fileNames;

	std::string directoryName;
	//std::cout << "Enter directory path: ";
	//std::cin >> directoryName;
	directoryName = "d:/test/";
	getFileNames(directoryName, fileNames);

	for(std::vector<std::string>::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
		std::cout << *it << std::endl;

	std::string file = readFile(directoryName, fileNames[0]);

	std::vector<std::string> details = parseFile(file);

	std::ofstream output("d:/test/out.txt");
	output << details[0] << "\n" 
		<< details[1] << "\n" 
		<< details[2] << "\n" 
		<< details[3];
	output.close();

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

// parsing files for specific content
std::vector<std::string> parseFile(std::string& file)
{
	// vector where we store all information of the company
	std::vector<std::string> details;

	// finding full name
	size_t fullNamePosition = file.find("Polno ime:");
	std::string fullName;
	if(fullNamePosition != std::string::npos)
		fullName = file.substr(fullNamePosition+55,  100);

	fullNamePosition = fullName.find("</H2>");
	if(fullNamePosition != std::string::npos)
		fullName = fullName.substr(0, fullNamePosition);
	if(fullName.size() > 2)
	{
		//std::cout << fullName;
		details.push_back(fullName);
	}

	// finding short name
	size_t shortNamePosition = file.find("Kratko ime:");
	std::string shortName;
	if(shortNamePosition != std::string::npos)
		shortName = file.substr(shortNamePosition+56,  80);

	shortNamePosition = shortName.find("&nbsp;");
	if(shortNamePosition != std::string::npos)
		shortName = shortName.substr(0, shortNamePosition);
	if(shortName.size() > 2)
	{
		//std::cout << shortName;
		details.push_back(shortName);
	}

	// finding address
	size_t addressPosition = file.find("Naslov:");
	std::string address;
	if(addressPosition != std::string::npos)
		address = file.substr(addressPosition+16,  80);

	addressPosition = address.find("</td>");
	if(addressPosition != std::string::npos)
		address = address.substr(0, addressPosition);
	if(address.size() > 2)
	{
		//std::cout << address;
		details.push_back(address);
	}

	// finding town
	size_t townPosition = file.find("Naselje:");
	std::string town;
	if(townPosition != std::string::npos)
		town = file.substr(townPosition+17,  80);

	townPosition = town.find("</td>");
	if(townPosition != std::string::npos)
		town = town.substr(0, townPosition);
	if(town.size() > 2)
	{
		std::cout << town;
		details.push_back(town);
	}

	return details;
}