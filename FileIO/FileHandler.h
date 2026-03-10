#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>
#include "login/Login.h"

using namespace std;

void saveToFile(const string &name, const string &pw);
void readFromFile();

#endif