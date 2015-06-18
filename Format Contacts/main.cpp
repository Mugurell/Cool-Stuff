/******************************************************************************
*******************************************************************************
**
**  Author:           Lingurar Petru-Mugurel
**  Written:          29.05.2015 19:53
**  Last updated:     18.06.2015 12:02
**
**  Compilation:  g++ -std=c++11 -Wall -Werror -Wextra -pedantic -Wshadow
**   (g++ 5.1)        -Woverloaded-virtual -Winvalid-pch -Wcast-align
**                    -Wformat=2 -Wformat-nonliteral -Wmissing-declarations
**                    -Wmissing-format-attribute -Wmissing-include-dirs
**                    -Wredundant-decls -Wswitch-default -Wswitch-enum
**
**  Execution:    ./...
**
**  Description:
**  Will read a text file with contacts info (name (+) phone number(s)), format
**  them (insert dot separators, check for the 10 digit fixed lenght), and save
**  them to another text file in formatted version.
**
**  Bugs:
**  --- None ---
**
**  TODO:
**  --- None ---
**
**  Notes:
**  Simple program but in which I used most of what I've learned until now.
**  Quite pleased with the end result.
**
*******************************************************************************
******************************************************************************/



#include "PersonInfo.h"
#include "FileChecks.h"

int main(int argc, char **argv)
{
    std::cout << std::unitbuf;      // flush the buffer after every output
    std::string dashes (40, '-');   // will be used to delimit prog sections

    std::cout << "\nI will read a txt file in search of person contacts.\n"
              << "Which will be formatted for easier viewing.\n"
              << dashes << std::endl;


/*
 * Create and initialize fstreams for IO files.
 */
    std::ifstream input_file;        // input file for person contacts
    std::string inputFName;          // name of the input file
    std::ofstream output_file;       // output file for the formatted contacts
    std::string outputFName;         // name of the output file


/*
 * Ask the user to pick the IO files and check if they are valid.
 */
    if (!valid_input(argc, argv, input_file, inputFName)) return 0;
    if (!valid_output(argc, argv, inputFName, output_file, outputFName))
        return 0;


/*
 * Process the data.
 */
    // People will hold the info for every contact.
    std::vector<PersonInfo> people;
    PersonInfo::read_input_file2people(input_file, people);


    // Sort out the numbers - good or bad. Format them.
    // Store good/bad contacts in different classes.
    std::vector<PersonInfo> goodContacts;
    std::vector<PersonInfo> badContacts;
    PersonInfo::format_contacts(people, goodContacts, badContacts);



    // Print & save the formatted contacts.
    // 1st write a short introduction to the output file.
    output_file << "\n\t|| File created " __DATE__ " at " __TIME__ " ||\n\n"
                << "This contains a list of formatted contacts from \""
                << inputFName << "\".";
    output_file << '\n' << dashes << '\n' << std::endl;

    PersonInfo::print_save_contacts(goodContacts, output_file, true);

    std::cout << '\n' << dashes << std::endl;
    output_file << '\n' << dashes << std::endl;

    PersonInfo::print_save_contacts(badContacts, output_file, false);

    std::cout << '\n' << std::endl;

    return 0;
}