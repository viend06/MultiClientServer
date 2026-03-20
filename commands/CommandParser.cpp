#include "CommandParser.h"
#include <sstream>

using namespace std;

Command parseCommand(string &input, int &socketFD)
{
    Command cmd;
    cmd.type = UNKNOWN;
    cmd.socketFD = socketFD;
    cmd.type = UNKNOWN;
    cmd.user.clear();
    cmd.message.clear();
    cmd.room.clear();

    stringstream ss(input);

    string command;
    ss >> command;

    if (command == "/login")
    {
        cmd.type = LOGIN;
        ss >> cmd.user;     // username
        ss >> cmd.password; // password
    }
    else if (command == "/msg")
    {
        cmd.type = MSG;
        getline(ss, cmd.message);
        if (!cmd.message.empty() && cmd.message.front() == ' ')
        {
            cmd.message.erase(0, 1);
        }
    }
    else if (command == "/join")
    {
        cmd.type = JOIN;
        ss >> cmd.room;
    }
    else if (command == "/leave")
    {
        cmd.type = LEAVE;
        ss >> cmd.room;
    }
    else if (command == "/list")
    {
        cmd.type = LIST;
    }
    else if (command == "/quit")
    {
        cmd.type = QUIT;
    }
    else
    {
        cmd.type = UNKNOWN;
    }

    return cmd;
}