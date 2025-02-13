#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <utility>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "csv.hpp"

// helper func to remove spaces
std::string trim(const std::string& s) {
	size_t start = s.find_first_not_of(" ");
	size_t end = s.find_last_not_of(" ");
	if (start == std::string::npos) return "";
	return s.substr(start, end - start + 1);
}

int main()
{
	try {
		const std::string inputFile = "group22-data.csv";

		csv::CSVReader reader(inputFile);

		// lookup maps for raw values to text
		std::map<std::string, std::string> location_names = {
			{"A", "front_gate"},
			{"B", "nassau_st_gate"},
			{"C", "pearse_st_gate"}
		};
		std::map<std::string, std::string> day_names = {
			{"1", "wednesday"},
			{"2", "thursday"},
			{"3", "friday"},
			{"4", "sunday"}
		};
		std::map<std::string, std::string> TOD_names = {
			{"Morn", "morning"},
			{"Noon", "afternoon"},
			{"Afternoon", "evening"}
		};

		std::map<std::pair<std::string, std::string>, std::vector<double>> groups;

		for (csv::CSVRow& row : reader) {
			// skip first row
			if (trim(row[0].get<std::string>()) == "Location") continue;

			for (size_t i = 0; i < row.size(); i += 5) {
				if (i + 3 >= row.size()) continue;
				std::string location_raw = trim(row[i].get<std::string>());
				std::string time_of_day_raw = trim(row[i + 1].get<std::string>());
				std::string day_raw = trim(row[i + 2].get<std::string>());
				std::string gain_str = trim(row[i + 3].get<std::string>());

				std::string location;
				std::string time_of_day;

				try {
					if (!location_names.count(location_raw)) throw;
					if (!TOD_names.count(time_of_day_raw)) throw;
					location = location_names[location_raw];
					time_of_day = TOD_names[time_of_day_raw];
				}
				catch (...) {
					throw std::runtime_error("Location, day, or time of day not recognised.");
				}

				double gain = std::stod(gain_str);

				std::pair<std::string, std::string> key = { location, time_of_day };
				groups[key].push_back(gain);
			}
		}


		for (const auto& entry : groups) {
			const auto& key = entry.first; // location, time of day
			const auto& gains = entry.second;

			if (gains.size() % 4 != 0) {
				throw std::runtime_error("Missing or unexpected extra measurements");
			}

			size_t time_slots = 150;

			std::vector<std::vector<std::string>> output_rows;
			output_rows.push_back({ "time_slot", "averaged_gain" });

			// average the measurements in each time slot
			for (size_t slot = 0; slot < time_slots; slot++) {
				double sum = 0.0;
				for (size_t i = 0; i < 4; i++) {
					sum += gains[slot * 4 + i];
				}
				double avg = sum / 4.0;
				output_rows.push_back({ std::to_string(slot + 1), std::to_string(avg) });
			}

			// write the file for this group
			std::string filename = "dataset_" + key.first + "_" + key.second + ".csv";
			std::ofstream ofs(filename);
			auto writer = csv::make_csv_writer(ofs);
			for (const auto& row : output_rows) {
				writer << row;
			}
			ofs.close();
			std::cout << "Wrote file: " << filename << std::endl;
		}

	}
	catch (std::exception& e) {
		std::cerr << "Something went wrong. Unsuccessful parse: " << e.what() << std::endl;
		return 1;
	}
	std::cout << "Success.\n";
	std::cout << "Press Enter to continue..." << std::endl;
	std::cin.get();
	return 0;
}