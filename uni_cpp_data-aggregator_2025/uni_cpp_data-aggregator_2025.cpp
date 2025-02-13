#include <string>
#include <map>
#include <vector>
#include "csv.hpp"

int main()
{
	try {
		const std::string inputFile = "group22-data.csv";

		csv::CSVReader reader(inputFile);

		// lookup maps for raw values to text
		std::map<std::string, std::string> locationNames = {
			{"A", "front_gate"},
			{"B", "nassau_st_gate"},
			{"C", "pearse_st_gate"}
		};
		std::map<std::string, std::string> dayNames = {
			{"1", "wednesday"},
			{"2", "thursday"},
			{"3", "friday"},
			{"4", "sunday"}
		};

		std::vector<std::string> header = { "location", "time_of_day", "day", "avg_gain" };

	}
	catch (...) {
		std::cerr << "Something went wrong. Unsuccessful parse.\n";
	}

}