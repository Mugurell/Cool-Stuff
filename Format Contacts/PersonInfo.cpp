//
// Created by Lingurar Petru-Mugurel on 29.05.2015.
//

#include <sstream>
#include <fstream>
#include "PersonInfo.h"


void
PersonInfo::read_input_file2people(std::ifstream &input_file,
							std::vector<PersonInfo> &people) {
	std::string line;
	while (getline(input_file, line)) {
		std::istringstream record(line);     // will hold each line from input
		std::vector<std::string> numbers;    // will hold the numbers
		std::string tmp_name, nr;                // name and nr buffers

		record >> tmp_name;
		while (record >> nr)
			numbers.push_back((nr + " "));

		PersonInfo buff(tmp_name, numbers);
		people.push_back(buff);
	}
}


void
PersonInfo::print_save_contacts(const std::vector<PersonInfo> &people,
						 std::ofstream &output_file, bool goodC) {

	if (goodC) {
		output_file << "\nFormatted list of good contacts:" << std::endl;
		std::cout << "\nFormatted list of good contacts:" << std::endl;
	} else {
		std::cout << "\nFormatted list of contacts with problems:" << std::endl;
		output_file << "\n\nFormatted list of contacts with problems:"
		<< std::endl;
	}

	for (auto entry : people) {
		if (entry.phones.empty()) {
			continue;
		} else {
			std::cout << '\n' << entry.name << ": ";
			output_file << '\n' << entry.name << ": ";
			for (auto nums : entry.phones) {
				std::cout << nums << "\n\t";
				output_file << nums << "\n\t";
			}
		}
	}

}

std::string
PersonInfo::format(const std::string &number) {
	// This function will simply put dots between digits as to comply to the
	// general format of US phone numbers. I think...
	std::string new_format;
	unsigned count = 0;
	for (auto chr : number) {
		++count;
		if (count == 4 || count == 7)
			new_format += '.';
		new_format += chr;
	}

	return new_format;
}


bool
PersonInfo::valid(std::string &number) {
	// Careful because the numbers received as string may include spaces
	for (auto it = number.begin(); it != number.end(); it++) {
		if (isspace(*it)) {
			number.erase(it);
			break;
		}
	}

	// Check if out string now contains only digits - is a number.
	for (auto nr : number)
		if (!isdigit(nr))
			return false;

	// Be sure it has exactly 10 numbers - proper phone # format.
	if (number.size() != 10)
		return false;

		// If all the checks above failed means we have a valid phone # !
	else
		return true;
}

void
PersonInfo::format_contacts(const std::vector<PersonInfo> &people,
				std::vector<PersonInfo> &goodContacts,
				std::vector<PersonInfo> &badContacts) {

	for (const auto &entry : people) {
		PersonInfo badC, goodC;
		for (auto nums : entry.phones) {
			if (valid(nums))
				goodC.phones.push_back(format(nums));
			else
				badC.phones.push_back(format(nums));
		}
		badC.name = entry.name;
		goodC.name = entry.name;
		goodContacts.push_back(goodC);
		badContacts.push_back(badC);
	}
}