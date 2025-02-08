// uni_cpp_sound-data-preprocessor_2025.cpp

// NB: requires C++20 for std::chrono::parse

#include "csv.hpp"
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <utility>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <cctype>

namespace fs = std::filesystem;

// Function declarations
void process_file(const std::string& input_filename);
std::vector<std::string> get_csv_files_in_directory(const std::string& directory_path = ".");
double get_time_secs(std::string& time_string, bool& is_date_format);
void isodate_str_to_epochtime_conv(std::string& time_string);
bool parse_iso_utc(const std::string& original, std::chrono::system_clock::time_point& tp);

static constexpr auto iso_format_no_tz = "%Y-%m-%dT%H:%M:%S.%fZ";

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

		bool is_date_format = false;
		bool is_first = true;
		double start_time = 0.0;

		for (csv::CSVRow& row : reader)
		{
			std::string time_str = row["time"].get<std::string>();
			double time = get_time_secs(time_str, is_date_format);
			if (is_first) {
				start_time = time;
				is_first = false;
			}

			time -= start_time;
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

double get_time_secs(std::string& time_string, bool& is_date_format) {
	double time = 0.0;
	size_t consumed = 0;
	try {
		time = std::stod(time_string, &consumed);
		std::cerr << "[DEBUG] " << time_string << " was interpreted as numeric: "
			<< time << std::endl;

		while (consumed < time_string.size() && std::isspace(static_cast<unsigned char>(time_string[consumed]))) {
			consumed++;
		}

		if (consumed != time_string.size()) {
			// Not all characters were part of the double => must be an ISO string or something else
			throw std::invalid_argument("Incorrect pase, trying ISO date parse.");
		}
	}
	catch (...) {
		// If conversion fails, assume the string is an ISO date.

		std::cerr << "[DEBUG] std::stod() failed for \"" << time_string
			<< "\" => trying ISO8601 parse." << std::endl;

		is_date_format = true;
		isodate_str_to_epochtime_conv(time_string);
		time = std::stod(time_string);  // Now time_string holds the numeric value.

		std::cerr << "[DEBUG] " << time_string << " => " << time << std::endl;
	}
	return time;
}


void isodate_str_to_epochtime_conv(std::string& time_string) {
	std::cerr << "[DEBUG] isodate_str_to_epochtime_conv called with: "
		<< time_string << std::endl;

	double time = 0.0;
	std::istringstream iss(time_string);
	std::chrono::system_clock::time_point tp;

	if (!parse_iso_utc(time_string, tp)) {
		throw std::runtime_error("Invalid time format: " + time_string);
	}

	auto epoch_time = tp.time_since_epoch();

	auto secs = std::chrono::duration_cast<std::chrono::seconds>(epoch_time);
	auto fractional = epoch_time - secs;

	try {
		time = secs.count() + std::chrono::duration<double>(fractional).count();
		std::cerr << "[DEBUG] Parsed epoch time: " << std::fixed << time << std::endl;
	}
	catch (std::exception& e) {
		std::cerr << "Error processing date format: " << e.what();
	}


	time_string = std::to_string(time);
	std::cerr << "[DEBUG] => epoch time: " << time_string << std::endl;
}

bool parse_iso_utc(const std::string& original, std::chrono::system_clock::time_point& tp)
{
	// make a copy and delete the Z
	std::string s = original;
	if (!s.empty() && s.back() == 'Z') {
		s.pop_back();
	}

	// try parse with fractional seconds
	{
		std::istringstream iss(s);
		iss >> std::chrono::parse("%Y-%m-%dT%H:%M:%S.%f", tp);
		if (!iss.fail()) {
			return true;
		}
	}

	// if fail
	std::cerr << "[ERROR] Could not parse '" << original
		<< "' with either fractional or no-fraction format.\n";
	return false;
}
