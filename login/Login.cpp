#include "Login.h"
using namespace std;

mutex mtx;
unordered_map<string, string> list_user;

void userLogin(int socketFD){
    string user_name;
    string password;
    string msg;
         
    recvMessage(socketFD, msg);
    cout << msg ; 
    getline(cin, user_name);
    sendMessage(socketFD, user_name);

    while(true){
        recvMessage(socketFD, msg);
            
        if(msg == "LOGIN_OK"){
            cout << "Dang nhap thanh cong!\n";
            break;
        }
            
        if(msg == "LOGIN_FAIL"){
            cout << "Sai mat khau.Thu lai.\n";
        }
        cout <<"Nhap password: ";
        getline(cin, password);
        sendMessage(socketFD, password);
    }
}

void handleLoginResponse(int socketFD, string &name, string &pass){
    name.clear();
    pass.clear();
    sendMessage(socketFD, "USERNAME:");
    recvMessage(socketFD, name);
                
    while(true){
        sendMessage(socketFD, "PASSWORD:");
        recvMessage(socketFD, pass);
                    
        if(check(name, pass)){
            sendMessage(socketFD, "LOGIN_OK");
            cout << name << " joined the chat." << endl;
            break;
        } else {
            sendMessage(socketFD,"LOGIN_FAIL");
        }
    }

}

bool check(const string &name, const string &password){
    lock_guard<mutex> lock(mtx);

    auto it = list_user.find(name);
    if(it == list_user.end()) return false;
    return it->second == password;
}
