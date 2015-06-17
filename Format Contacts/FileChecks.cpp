//
// Created by Lingurar Petru-Mugurel on 30.05.2015.
//

#include <iostream>
#include <vector>
#include "FileChecks.h"


bool
is_open(const std::ifstream &pFile, const std::string &pName) {
	if (! pFile.is_open()) {
		std::cout << "\nError: Can't open \"" << pName
				  << "\".\n\t\tExiting...\n" << std::endl;
		return false;
	} else
		return true;
}


bool
is_open(const std::ofstream &pFile, const std::string &pName) {
	if (! pFile.is_open()) {
		std::cout << "\nError: Can't open \"" << pName
				  << "\".\n\tExiting...\n" << std::endl;
		return false;
	} else
		return true;
}

// this returns true if there is not a single character in the file
// also returns true if the file couldn't be opened, but in that case
// we want to print another message.
bool
is_empty(std::ifstream  &pFile, const std::string &pName) {
	 if (pFile.peek() == std::ifstream::traits_type::eof()) {
		 std::cerr << "\nError: \"" << pName << "\" is empty."
		           << "\n\t\tExiting... \n" << std::endl;
		 return true;
	 }
    return false;
}


// clear any flags for std::cin, consume any other input
void inline
clear_cin(void) {
	std::cin.clear();
	while(std::cin.get() != '\n');
}


bool
valid_input(int argc, char **argv, std::ifstream &input_file,
          std::string &inputFName) {
	bool valid_choice = false;
	do {
		std::cout << "\nPlease select input txt file:\n"
		<< "\t1) - Use the first argument given to this program;\n"
		<< "\t2) - Use a default location - \"contacts.txt\" "
        << "in the program's folder;\n"
		<< "\t3) - Input the complete path to file;\n"
		<< "\t4) - Quit.\n"
		<< "Your choice? [1] / [2] / [3] / [4]: ";
		unsigned choice;
		std::cin >> choice;
		switch (choice) {
			case 1: // first check if any argument was passed to the program
				if (argc == 1) {
					std::cerr << "\tError: No arguments were passed to the "
					<< "program\n" << std::endl;

				} else {
					inputFName = argv[1];
					valid_choice = true;
				}
		        break;
			case 2:
				inputFName = "contacts.txt";
		        valid_choice = true;
		        break;
			case 3:
				std::cout << "Enter the full path to the file:\n\t";
				std::cin >> inputFName;
				valid_choice = true;
				break;
			case 4:
				std::cout << "\n\tBBye!\n" << std::endl;
				return false;
			default:
				std::cerr << "\tError: No valid input.\n" << std::endl;
		        clear_cin();
		        break;
		}
	} while (!valid_choice);

    input_file.open(inputFName);
	clear_cin();

	return valid_choice;
}


bool
valid_output(int argc, char **argv, const std::string &inputFName,
           std::ofstream &output_file, std::string &outputFName) {
	bool valid_choice = false;
	do {
		std::cout << "\n\n\nPlease select output txt file:\n"
		<< "\t1) - Use the first argument given to this program;\n"
		<< "\t2) - Use the second argument given to this program;\n"
		<< "\t3) - Use a default location - \"contacts_formatted.txt\" "
		<< "in the program's folder;\n"
		<< "\t4) - Input the complete path to file;\n"
		<< "\t5) - Quit.\n"
		<< "Your choice? [1] / [2] / [3] / [4] / [5]: ";
		unsigned choice;
		std::cin >> choice;
		switch (choice) {
			case 1: // first check the # of arguments passed to the program
				if (argc == 1)
					std::cerr << "\tError: No arguments were passed to the "
					<< "program" << std::endl;
				else if (argc >= 2) {
                    if (inputFName == argv[1] && argc == 2)
                        std::cerr << "\tOnly one argument was passed to the "
                                  << "program which is used as input file."
                                  << std::endl;
                    else if (inputFName == argv[1])
                        std::cerr << "\tError: " << *(argv + 1) << " allready"
                                  << " selected to be used as input file."
                                  << std::endl;
                    else {
						outputFName = argv[1];
						valid_choice = true;
					}
				}
		        break;
			case 2:
				if (argc == 1)
                    std::cerr << "\tError: No arguments were passes to the "
                                 "program" << std::endl;
                else if (argc == 2)
                    std::cerr << "\tError: Only one argument was passed to "
                                 "the program!" << std::endl;
                else {
                    outputFName = argv[2];
                    valid_choice = true;
                }
		        break;
			case 3:
				outputFName = "contacts_formatted.txt";
		        valid_choice = true;
		        break;
			case 4: {
				std::cout << "Enter the full path to the file:\n\t";
				std::cin >> outputFName;
                if (inputFName == outputFName)
                    std::cerr << "Error " << inputFName << " allready selected "
                              << "to be used as input file.\n\n";
				else
                    valid_choice = true;
				break;
			}
			case 5:
				std::cout << "\n\tBBye!\n" << std::endl;
		        return false;
			default:
				std::cerr << "\tError: No valid input." << std::endl;
		        clear_cin();
		        break;
		}
	} while (!valid_choice);

    output_file.open(outputFName);
	clear_cin();

	return valid_choice;
}