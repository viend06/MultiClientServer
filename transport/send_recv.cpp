#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <unistd.h>
#include <vector>
#include <string>
#include <algorithm>
using namespace std ; 

void sendMessage(int sockfd, const string &msg){
    ssize_t bytesWereSent = ::send(sockfd, msg.c_str(), msg.size(), 0);
    if(bytesWereSent == -1){
        cout << sockfd << " disconnected!" << endl;
        return;
    }
    if(bytesWereSent == 0){
        cout << "The connection was closed while sending." << endl;
        return;
    }
}

void recvMessage(int sockfd, string &msg){
    msg.clear();
    char buf[1024];
    ssize_t bytes = ::recv(sockfd, buf, sizeof(buf), 0);
    if(bytes == -1){
        cout <<"recv failed" << endl;
        return;
    }
    if(bytes == 0){
        cout <<"Disconnected!" << endl;
        return;
    }
    buf[bytes] = '\0';
    msg = buf;
}