#include "csv.hpp"
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <utility>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

// Function declarations
void process_file(const std::string& input_filename);
std::vector<std::string> get_csv_files_in_directory(const std::string& directory_path = ".");

int main()
{
	try {
		auto csv_files = get_csv_files_in_directory();
		if (csv_files.empty()) {
			std::cout << "No csv files found in current directory." << std::endl;
			return 1;
		}

		std::cout << "Found " << csv_files.size() << " csv files in current directory." << std::endl;
		for (const auto& file : csv_files) {
			std::cout << "- " << file << std::endl;
		}
		std::cout << std::endl;

		// Process each file
		for (const auto& filename : csv_files) {
			process_file(filename);
		}

		std::cout << "\nAll files processed successfully!" << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	std::cout << "Press Enter to continue..." << std::endl;
	std::cin.get();

	return 0;
}

std::vector<std::string> get_csv_files_in_directory(const std::string& directory_path) {
	std::vector<std::string> csv_files;

	for (const auto& entry : fs::directory_iterator(directory_path)) {
		if (entry.is_regular_file()) {
			std::string filename = entry.path().filename().string();
			// Check if file ends with .csv (case-insensitive)
			if (filename.length() >= 4) {
				std::string extension = filename.substr(filename.length() - 4);
				std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
				if (extension == ".csv" && filename.find("_preprocessed") == std::string::npos) {
					csv_files.push_back(filename);
				}
			}
		}
	}

	return csv_files;
}

void process_file(const std::string& input_filename)
{
	std::string output_filename = input_filename.substr(0, input_filename.find(".csv")) + "_preprocessed.csv";
	try {
		csv::CSVReader reader(input_filename);

		// Map to store sum of gains for each 5-second bucket
		// Key: bucket number (0 for 0-5, 5 for 5-10, etc)
		// Value: pair-> sum of gains, number of data points in bucket
		std::map<int, std::pair<double, int>> gain_buckets;

		for (csv::CSVRow& row : reader)
		{
			double time = row["time"].get<double>();
			double gain = row["gain"].get<double>();

			// ignore data after 13 minutes == 780 seconds
			if (time > 780) break;

			// Calculate bucket number
			int bucket = static_cast<int>(time / 5);

			// Add to bucket
			gain_buckets[bucket].first += gain;
			gain_buckets[bucket].second++;
		}

		// Write to output file
		std::ofstream output_file(output_filename);
		if (!output_file.is_open()) {
			throw std::runtime_error("Could not open output file: " + output_filename);
		}

		auto writer = csv::make_csv_writer(output_file);
		writer << std::vector<std::string> {"time", "avg_gain"};

		for (const auto& [bucket, pair] : gain_buckets)
		{
			auto sum = pair.first;
			auto bucket_count = pair.second;

			double time = (bucket + 1) * 5;
			double avg_gain = sum / bucket_count;

			writer << std::make_tuple(time, avg_gain);
		}

		std::cout << "File " << input_filename << " processed ..." << std::endl;
		std::cout << "Output written to " << output_filename << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error processing file " << input_filename << ": " << e.what() << std::endl;
		throw; // Re-throw to be caught by main
	}
}