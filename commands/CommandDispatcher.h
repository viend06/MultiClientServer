#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H

#include "Command.h"
#include "login/Login.h"

void dispatchCommand(Command &cmd, bool &running);

#endif