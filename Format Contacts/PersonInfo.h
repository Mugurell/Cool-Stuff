//
// Created by Lingurar Petru-Mugurel on 29.05.2015.
//

#ifndef SAVE_PHONE_NUMBERS_USING_SSTREAM_PERSONINFO_H
#define SAVE_PHONE_NUMBERS_USING_SSTREAM_PERSONINFO_H

#include <vector>
#include <iostream>
#include <string>



// This class will hold the contact info: name + phone number(s)
class PersonInfo {
    std::string name;
    std::vector<std::string> phones;
public:
    PersonInfo() = default;
    PersonInfo(std::string n, std::vector<std::string> p)
            : name(n), phones(p) { }
    static void read_input_file2people(std::ifstream&,
                                       std::vector<PersonInfo>&);
    static void print_save_contacts(const std::vector<PersonInfo>&,
                                    std::ofstream&, bool);
    static void format_contacts(const std::vector<PersonInfo> &people,
                                std::vector<PersonInfo> &goodContacts,
                                std::vector<PersonInfo> &badContacts);
    static std::string format(const std::string&);

    static bool valid(std::string &number);
};



#endif //SAVE_PHONE_NUMBERS_USING_SSTREAM_PERSONINFO_H
