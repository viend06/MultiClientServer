#ifndef QUIT_H
#define QUIT_H

#include <string>
#include "transport/send_recv.h"
using namespace std;

void checkQuitChat(int sockfd, string &msg, bool &running);

#endif