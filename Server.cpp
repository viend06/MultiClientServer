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
using namespace std ; 

mutex mtx;
vector<int> clients;

class Socket{
    private: 
        int sockfd;
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
            size_t total = 0 ; 
            size_t bytesWereSent = 0;
            size_t len = msg.size();
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
            static string buf;
            while(true){
                char buffer[1024];
                int bytesWereRecv = ::recv(sockfd, buffer,sizeof(buffer),0);
                if(bytesWereRecv == -1){
                    throw runtime_error("Recv failed");
                }
                if(bytesWereRecv == 0){
                    throw runtime_error("Disconnected!");
                }                
                buf.append(buffer, bytesWereRecv);
                size_t pos = buf.find('\n');
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
            cout << new_fd + " joined the chat." << endl;
            return Socket(new_fd);
        }


        void handle_client(){
            string msg;
            while(true){
                recv(msg);
                vector<int> tmp;
                {
                    lock_guard<mutex> lock(mtx);
                    tmp = clients;
                }
                for(int fd : tmp){
                    if(fd == sockfd) continue;
                    if(fd != -1){
                        ::send(fd, (msg + '\n').c_str(), msg.size() + 1, 0);
                    }
                }
            }

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
    string message;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

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

    server->listen(20);
    while(true){
        Socket cli = server->accept(their_addr);
        {
            lock_guard<mutex> lock(mtx);
            clients.push_back(cli.getfd());
        }
        thread t(&Socket::handle_client, move(cli));
        t.detach();

    }

    return 0;
}

