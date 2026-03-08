#ifndef SEND_RECV_H
#define SEND_RECV_H

#include <string>
using namespace std;

void sendMessage(int sockfd, const string &msg);
void recvMessage(int sockfd, string &msg);

#endif