//
// Created by Lingurar Petru-Mugurel on 30.05.2015.
//

#ifndef SAVE_PHONE_NUMBERS_USING_SSTREAM_FILECHECKS_H
#define SAVE_PHONE_NUMBERS_USING_SSTREAM_FILECHECKS_H


#include <fstream>
#include <sstream>
#include "PersonInfo.h"


//class PersonInfo;


bool valid_input(int argc, char **argv, std::ifstream &input_file,
                 std::string &inputFName);
bool valid_output(int argc, char **argv, const std::string &inputFName,
                  std::ofstream &output_file, std::string &outputFName);

bool is_open(const std::ifstream&, const std::string&);
bool is_open(const std::ofstream&, const std::string&);
bool is_empty(std::ifstream&, const std::string&);

void inline clear_cin(void);



#endif //SAVE_PHONE_NUMBERS_USING_SSTREAM_FILECHECKS_H
