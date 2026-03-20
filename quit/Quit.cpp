#include "Quit.h"

void checkQuitChat(int sockfd, string &msg, bool &running)
{
    sendMessage(sockfd, "QUIT_OK\n");
    running = false;
}