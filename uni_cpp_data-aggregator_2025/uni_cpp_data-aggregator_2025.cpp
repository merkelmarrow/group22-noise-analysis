#include <string>
#include <map>
#include <vector>
#include <stdexcept>
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

		std::vector<std::string> header = { "Location", "Time of Day", "Day", "Average Sound Level" };

		for (csv::CSVRow& row : reader) {
			// skip first row
			if (trim(row[0].get<std::string>()) == "Location") continue;

			for (size_t i = 0; i < row.size(); i += 5) {
				std::string location_raw = trim(row[i].get<std::string>());
				std::string time_of_day = trim(row[i + 1].get<std::string>());
				std::string day_raw = trim(row[i + 2].get<std::string>());
				std::string avg_sound = trim(row[i + 3].get<std::string>());

				std::string location;
				std::string day;

				try {
					location = location_names[location_raw];
					day = day_names[day_raw];
				}
				catch (...) {
					throw std::runtime_error("Location (A,B,C) or time of day (1,2,3) not recognised.");
				}

				std::vector<std::string> record = { location, time_of_day, day, avg_sound };
			}


		}


	}
	catch (...) {
		std::cerr << "Something went wrong. Unsuccessful parse.\n";
	}

}