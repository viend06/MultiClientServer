#include "CommandDispatcher.h"
#include "quit/Quit.h"
#include "transport/send_recv.h"
#include <iostream>

using namespace std;

void dispatchCommand(Command &cmd, bool &running)
{
    switch (cmd.type)
    {
    case LOGIN:
        handleLoginResponse(cmd.socketFD, cmd.user, cmd.password);
        break;

    case MSG:
        break;

    case LIST:
        break;

    case QUIT:
        checkQuitChat(cmd.socketFD, cmd.message, running);
        break;

    default:
        break;
    }
}