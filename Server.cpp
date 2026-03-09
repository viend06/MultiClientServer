#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <unistd.h>
#include <optional>
#include <mutex>
#include <vector>
#include <thread>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include "transport/send_recv.h"
using namespace std ; 

mutex mtx;

struct InfoOfUsers{
    string name;
    string password;
};

unordered_map<int, InfoOfUsers> clients;

class Socket{
    private: 
        int sockfd;
        string buf;
    public:
        //Constructor
        Socket(int domain, int type, int protocol){
            sockfd = ::socket(domain, type, protocol);
            if(sockfd == -1){
                throw runtime_error("getting fd failed");
            }
            cout << "Socket created successfully" << endl;
        }

        int getfd() const{
            return sockfd;
        }

        Socket(int new_fd){
            sockfd = new_fd;
        }

        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

        //Move constructor
        Socket(Socket&& other) noexcept : sockfd(other.sockfd){
            other.sockfd = -1;
        }

        //Move assignment
        Socket& operator=(Socket&& other){
            if(this != &other){
                if(sockfd != -1){
                    close(sockfd);
                }
                sockfd = other.sockfd;
                other.sockfd = -1;
            }
            return *this;
        }

        // Is this address valid?
        void bind(sockaddr *my_addr, size_t addrlen){
            if(::bind(sockfd, my_addr, addrlen) == -1){
                throw runtime_error("bind failed");
            }
            cout << "Successfully assigned the port and address to the server." << endl;
        }

        // Create stack
        void listen(int backlog){
            if(::listen(sockfd, backlog) == -1){
                throw runtime_error("Can not create stack for server");
            }
            cout<< "Successfully created a listening stack." << endl;
        }

        //send message(actually send bytes)
        void send(int client_fd, const string &msg){
            int total = 0 ; 
            ssize_t bytesWereSent = 0;
            int len = msg.size();
            while(total < len){
                bytesWereSent = ::send(client_fd, msg.c_str() + total, len - total, 0);
                if(bytesWereSent < 0){
                    throw runtime_error("Sending message failed");
                }
                if(bytesWereSent == 0){
                    throw runtime_error("Connection closed while sending");
                }
                total += bytesWereSent;
            }
        }

        // Recv message(actually recv bytes)
        void recv(string &message){
            message.clear();
            while(true){
                char buffer[1024];
                ssize_t bytesWereRecv = ::recv(sockfd, buffer,sizeof(buffer),0);
                if(bytesWereRecv == -1){
                    throw runtime_error("Recv failed");
                }
                if(bytesWereRecv == 0){
                    throw runtime_error("Disconnected!");
                }                
                buf.append(buffer, bytesWereRecv);
                int pos = buf.find('\n');
                if(pos != string::npos){
                    message = buf.substr(0,pos);
                    buf.erase(0,pos +1);
                    break;
                }
            }
        }

        //Accept. Return a new socket to send and recv
        Socket accept(sockaddr_storage &their_addr){
            socklen_t addr_size = sizeof(their_addr);
            int new_fd = ::accept(sockfd,(sockaddr *)&their_addr, &addr_size);
            if(new_fd == -1){
                throw runtime_error("Accepting failed");
            }
            return Socket(new_fd);
        }


        void handle_client(){
            try{
                sendMessage(sockfd, "Moi nhap ten : ");
                string user_name;
                recvMessage(sockfd, user_name);

                sendMessage(sockfd, "Moi nhap mat khau : ");
                string pass;
                recvMessage(sockfd, pass);

                cout << user_name << " joined the chat." << endl;
                {
                    lock_guard<mutex> lock(mtx);
                    clients[sockfd].name = user_name;
                    clients[sockfd].password = pass;
                }
                string msg;
                while(true){
                    recv(msg);
                    unordered_map<int, InfoOfUsers> tmp;
                    {
                        lock_guard<mutex> lock(mtx);
                        tmp = clients;
                    }
                    for(auto &cli : tmp){
                        if(cli.first == sockfd) continue;
                        if(cli.first != -1){
                            send(cli.first,user_name + " : " +  msg + '\n') ;
                        }
                    }
                }
            }catch(...){
                lock_guard<mutex> lock(mtx);
                cout << clients[sockfd].name << " left the chat." << endl;
                saveToFile(clients[sockfd].name, clients[sockfd].password);
                clients.erase(sockfd);
            }
        }

        

        void saveToFile(const string &name, const string pw){
            ofstream file("user.txt", ios::app);
            file << name << " " << pw << endl;
        }

        //Deconstructor
        ~Socket(){
            if(sockfd != -1){
                close(sockfd);
            }
        }


};

int main(){
    int status;
    struct addrinfo hints{}; 
    struct addrinfo *res, *p;
    struct sockaddr_storage their_addr;
    int yes = 1;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, "2026", &hints, &res);
    if(status != 0){
        throw runtime_error("Getting address information failed");
    }

    optional<Socket> server;
    for(p = res; p != NULL; p = p->ai_next){
        try{
            Socket s = Socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if(setsockopt(s.getfd(), SOL_SOCKET, SO_REUSEADDR,&yes, sizeof(yes)) == -1){
                throw runtime_error("Cannot reuse");
            }
            s.bind(p->ai_addr, p->ai_addrlen);
            server = move(s);
            break;
        }catch(...){
            continue;
        }
    }
    freeaddrinfo(res);
    server->listen(20);
    while(true){
        Socket cli = server->accept(their_addr);
        thread t(&Socket::handle_client, move(cli));
        t.detach();
    }

    return 0;
}

