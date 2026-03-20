#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include "Command.h"

using namespace std;

Command parseCommand(string &input, int &socketFD);

#endif