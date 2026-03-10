#include "FileHandler.h"
#include "login/Login.h"
using namespace std;

void saveToFile(const string &name, const string &pw){
    {
        lock_guard<mutex> lock(mtx);
        ofstream file("user.txt", ios::app);
        file << name << " " << pw << endl;
        file.close();
    }
}

void readFromFile(){
    {
        lock_guard<mutex> lock(mtx);
        ifstream file("user.txt");
        string information;
        while(getline(file, information)){
            stringstream ss(information);
            string name, password;
            ss >> name >> password;
            list_user[name] = password;
        }
        file.close();
        // ofstream clear("user.txt", ios::trunc);
        // clear.close();
    }
}