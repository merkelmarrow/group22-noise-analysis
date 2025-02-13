#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <utility>
#include <fstream>
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

		std::vector<std::string> header = { "location", "time_of_day", "day", "avg_gain" };

		// maps key pair (ie. front_gate, morning) to a vector of records
		// ie. one record could look like
		// location, time_of_day, day, avg_gain
		// pearse_st_gate, morning, friday, 73.3892
		// pearse_st_gate, morning, friday, 63.9092
		// etc...
		std::map<std::pair<std::string, std::string>, std::vector<std::vector<std::string>>> grouped_data;

		// just stores all the records together to create an aggregated csv
		std::vector<std::vector<std::string>> aggregated_data;
		aggregated_data.push_back(header);

		for (csv::CSVRow& row : reader) {
			// skip first row
			if (trim(row[0].get<std::string>()) == "Location") continue;

			for (size_t i = 0; i < row.size(); i += 5) {
				if (i + 3 >= row.size()) continue;
				std::string location_raw = trim(row[i].get<std::string>());
				std::string time_of_day_raw = trim(row[i + 1].get<std::string>());
				std::string day_raw = trim(row[i + 2].get<std::string>());
				std::string avg_sound = trim(row[i + 3].get<std::string>());

				std::string location;
				std::string day;
				std::string time_of_day;

				try {
					location = location_names[location_raw];
					day = day_names[day_raw];
					time_of_day = TOD_names[time_of_day_raw];
				}
				catch (...) {
					throw std::runtime_error("Location (A,B,C) or day (1,2,3,4) not recognised.");
				}

				std::vector<std::string> record = { location, time_of_day, day, avg_sound };

				grouped_data[{location, time_of_day}].push_back(record);
				aggregated_data.push_back(record);
			}
		}

		// write out each grouped data set into its own csv file
		for (const auto& entry : grouped_data) {
			const auto& key = entry.first;
			const auto& records = entry.second;
			std::string filename = "dataset_" + key.first + "_" + key.second + ".csv";

			std::ofstream ofs(filename);
			auto writer = csv::make_csv_writer(ofs);
			writer << header;
			for (const auto& record : records) {
				writer << record;
			}
			ofs.close();
			std::cout << "Wrote file: " << filename << std::endl;
		}

		// write out the aggregated file
		const std::string agg_filename = "aggregated.csv";
		std::ofstream ofs(agg_filename);
		auto writer = csv::make_csv_writer(ofs);
		for (const auto& record : aggregated_data) {
			writer << record;
		}
		ofs.close();
		std::cout << "Wrote aggregated file: " << agg_filename << std::endl;
	}
	catch (...) {
		std::cerr << "Something went wrong. Unsuccessful parse.\n";
		return 1;
	}
	return 0;
}