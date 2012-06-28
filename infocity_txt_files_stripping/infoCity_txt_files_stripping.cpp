// infoCity_txt_files_stripping.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost\filesystem.hpp>
#include <boost\filesystem\fstream.hpp>
#include <boost\algorithm\string\replace.hpp>

void getFileNames(std::string directoryName, std::vector<std::string>& fileNames);
std::string readFile(std::string directoryName, std::string fileName);
std::vector<std::string> parseFile(std::vector<std::string>& details, std::string& file);
std::string findDetail(std::string& file, std::string searchQuery1, std::string searchQuery2, int offset);
int stringOccurrenceCount(std::string const & str, std::string const & word);
void findBusiness(std::vector<std::string>& details, std::string& file);

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

	std::vector<std::string> details;
	parseFile(details, file);

	std::ofstream output("d:/test/out.txt");
	for(std::vector<std::string>::iterator it = details.begin(); it != details.end(); ++it)
		output << *it << std::endl;
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
std::vector<std::string> parseFile(std::vector<std::string>& details, std::string& file)
{
	// finding full name
	details.push_back(findDetail(file, "Polno ime:", "</H2>", 55));

	// finding short name
	details.push_back(findDetail(file, "Kratko ime:", "&nbsp;", 56));

	// finding address
	details.push_back(findDetail(file, "Naslov:", "</td>", 16));

	// finding town
	details.push_back(findDetail(file, "Naselje:", "</td>", 17));

	// finding post office
	details.push_back(findDetail(file, "Pošta:", "</td>", 27));

	//finding form of organization
	details.push_back(findDetail(file, "Organizacijska oblika:", "</td>", 43));

	//finding number of employees
	details.push_back(findDetail(file, "Število zaposlenih:", "</td>", 40));

	//finding phone number
	details.push_back(findDetail(file, "Telefon:", "</td>", 29));

	//finding fax number
	details.push_back(findDetail(file, "Fax:", "</td>", 25));

	//finding mobile phone number
	details.push_back(findDetail(file, "Gsm:", "</td>", 25));

	//finding website
	details.push_back(findDetail(file, "Splet:", "\">", 52));

	//finding registration number
	details.push_back(findDetail(file, "Matièna številka:", "</td>", 38));

	//finding tax code
	details.push_back(findDetail(file, "Davèna številka:", "</td>", 37));

	//finding ID code for VAT
	details.push_back(findDetail(file, "ID številka za DDV:", "</td>", 41));

	//finding transaction numbers
	std::string::size_type detailPosition = file.find("Transakcijski raèun:");
	std::string detail;
	if(detailPosition != std::string::npos)
		detail = file.substr(detailPosition + 29,  250);

	int number = stringOccurrenceCount(detail, "<br />");
	
	for(int i = 0; i <= number; ++i)
	{
		if(i == number)
		{
			details.push_back(findDetail(file, *(details.end()-1), "</td>", (details.end()-1)->size() + 6));
			break;
		}
		if(i == 0)
			details.push_back(findDetail(file, "Transakcijski raèun:", "<br />", 29));
		else
			details.push_back(findDetail(file, *(details.end()-1), "<br />", (details.end()-1)->size() + 6));
	}
	
	// finding founders
	details.push_back(findDetail(file, "Ustanovitelji:", "</td>", 35));

	// finding date
	details.push_back(findDetail(file, "Datum vpisa pri registrskem organu:", "</td>", 56));

	// finding registration authority
	details.push_back(findDetail(file, "Registrski organ:", "</td>", 38));

	// finding ID number
	details.push_back(findDetail(file, "Zaporedna številka vpisa:", "</td>", 34));

	// finding type of property
	details.push_back(findDetail(file, "Vrsta lastnine:", "</td>", 24));

	// finding origin of initial capital
	details.push_back(findDetail(file, "Poreklo ustanovitvenega kapitala:", "</td>", 42));

	// finding countries of initial capital
	details.push_back(findDetail(file, "Države ustanovitvenega kapitala:", "</td>", 41));
	
	// finding business
	findBusiness(details, file);


	return details;
}

std::string findDetail(std::string& file, std::string searchQuery1, std::string searchQuery2, int offset)
{
	std::string::size_type detailPosition;
	std::string detail;

	// finding detail
	detailPosition = file.find(searchQuery1);
	if(detailPosition != std::string::npos)
		detail = file.substr(detailPosition + offset,  100);

	detailPosition = detail.find(searchQuery2);
	if(detailPosition != std::string::npos)
		detail = detail.substr(0, detailPosition);

	std::cout << detail << std::endl;
	return detail;
}

int stringOccurrenceCount(std::string const & str, std::string const & word)
{
       int count(0);
       std::string::size_type word_pos( 0 );
       while ( word_pos!=std::string::npos )
       {
               word_pos = str.find(word, word_pos );
               if ( word_pos != std::string::npos )
               {
                       ++count;

         // start next search after this word 
                       word_pos += word.length();
               }
       }
       
       return count;
}

void findBusiness(std::vector<std::string>& details, std::string& file)
{
	std::string::size_type detailPosition;
	std::string detail;
	detail = "";
	detailPosition = file.find("Dejavnosti");
	if(detailPosition != std::string::npos)
	{
		detail = file.substr(detailPosition + 67,  500);
		file = file.substr(detailPosition + 67);
	}
	detailPosition = detail.find("\">");
	if(detailPosition != std::string::npos)
	{
		detail = detail.substr(detailPosition + 2);
		file = file.substr(detailPosition + 2);
	}
	detailPosition = detail.find("</a>");
	if(detailPosition != std::string::npos)
	{
		detail = detail.substr(0, detailPosition);
		file = file.substr(detailPosition);
	}
	boost::algorithm::replace_all(detail, "&nbsp;&gt;&nbsp;", " -> ");
	details.push_back(detail);
	std::cout << detail << std::endl << std::endl;
	while(file.find("</a></li><li><a href=\"http://www.infocity.si") != std::string::npos)
	{
		detail = "";
		detailPosition = file.find("</a></li><li><a href=\"http://www.infocity.si");
		if(detailPosition != std::string::npos)
		{
			detail = file.substr(detailPosition + 57,  500);
			file = file.substr(detailPosition + 57);
		}
		detailPosition = detail.find("\">");
		if(detailPosition != std::string::npos)
		{
			detail = detail.substr(detailPosition + 2);
			file = file.substr(detailPosition + 2);
		}
		detailPosition = detail.find("</a>");
		if(detailPosition != std::string::npos)
		{
			detail = detail.substr(0, detailPosition);
			file = file.substr(detailPosition);
		}
		boost::algorithm::replace_all(detail, "&nbsp;&gt;&nbsp;", " -> ");
		details.push_back(detail);
		std::cout << detail << std::endl << std::endl;
	}
}