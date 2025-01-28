// uni_cpp_sound-data-preprocessor_2025.cpp

#include "csv.hpp"

#include <iostream>
#include <string>
#include <map>
#include <fstream>

// Function declarations
void process_file(const std::string& input_filename);


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

void process_file(const std::string& input_filename)
{
	std::string output_filename = input_filename.substr(0, input_filename.find(".csv")) + "_preprocessed.csv");
	try {
		csv::CSVReader reader(input_filename);

		// Map to store sum of gains for each 5-second bucket
		// Key: bucket number (0 for 0-5, 5 for 5-10, etc)
		// Value: sum of gains

		std::map<int, double> gain_buckets;

		for (csv::CSVRow& row : reader)
		{
			double time = row["time"].get<double>();
			double gain = row["gain"].get<double>();

			// ignore data after 13 minutes == 780 seconds
			if (time > 780) break;

			// Calculate bucket number
			int bucket = static_cast<int>(time / 5.0) * 5;

			// Add to bucket
			gain_buckets[bucket] += gain;
		}

		// Write to output file
		std::ofstream output_file(output_filename);
		
		auto writer = csv::make_csv_writer(output_file);

		writer << std::vector<std::string> {"time", "avg_gain"};

		for (const auto& [bucket, sum] : gain_buckets)
		{
			double time = (bucket + 1) * 5;
			double avg_gain = sum / 5.0;

			writer << std::make_tuple(time, avg_gain);
		}
		
		std::cout << "File " << input_filename << " processed ..." << std::endl;
		std::cout << "Output written to " << output_filename << std::endl;
		
	}
	catch (const std::exception& e) {
		std::cerr << "Error processing file " << input_filename << ": " << e.what() << std::endl;
	}
}