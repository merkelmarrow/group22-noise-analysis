// uni_cpp_sound-data-preprocessor_2025.cpp

#include "csv.hpp"

#include <iostream>
#include <string>


int main()
{
	std::string filename;
	std::vector<std::string> filenames;

	std::cout << "Enter csv filenames (one per line, empty line when done): " << std::endl;
	while (std::getline(std::cin, filename) && !filename.empty())
	{
		filenames.push_back(filename);
	}

	for (const auto& filename : filenames)
	{
		// Process files here
	}

	return 0;
}