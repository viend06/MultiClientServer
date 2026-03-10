#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>
#include <sstream>
#include <string>
#include <mutex>
#include <unordered_map>
#include "transport/send_recv.h"

using namespace std;

extern mutex mtx;
extern unordered_map<string, string> list_user;

void userLogin(int socketFD);
void handleLoginResponse(int socketFD, string &name, string &pass);
bool check(const string &name, const string &password);

#endif