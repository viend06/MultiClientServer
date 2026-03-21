#ifndef COMMAND_H
#define COMMAND_H
#include <string>
using namespace std;

enum CommandType
{
    LOGIN,
    MSG,
    QUIT,
    UNKNOWN
};

struct Command
{
    CommandType type;
    int socketFD;
    string user;
    string password;
    string message;
};

#endif