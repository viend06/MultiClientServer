#ifndef COMMAND_H
#define COMMAND_H
#include <string>
using namespace std;

enum CommandType
{
    LOGIN,
    MSG,
    PRIVATE_MSG,
    JOIN,
    LEAVE,
    LIST,
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
    string room;
};

#endif