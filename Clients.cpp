#include<iostream> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <unistd.h>
#include <optional>
using namespace std ; 

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
        }

        ~Socket(){
            if(sockfd != -1){
                close(sockfd);
            }
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
        void recv(int client_fd, string &message){
            message.clear();
            char buffer[1024];
            while(true){
                size_t pos = buf.find('\n');
                if(pos != string::npos){
                    message = buf.substr(0,pos);
                    buf.erase(0,pos +1);
                    break;
                }
                int bytesWereRecv = ::recv(client_fd, buffer,sizeof(buffer),0);
                if(bytesWereRecv == -1){
                    throw runtime_error("Recv failed");
                }
                if(bytesWereRecv == 0){
                    throw runtime_error("Disconnected!");
                }                
                buf.append(buffer, bytesWereRecv);
            }
        }

        void connect(sockaddr *serv_addr, int addrlen){
            int n = ::connect(sockfd,serv_addr, addrlen);
            if(n == -1){
                throw runtime_error("Connecting failed");
            }
        }
};

int main(){
    int status;
    addrinfo  hints{}, *res, *p;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo("127.0.0.1", "2026",&hints, &res);
    if(status != 0){
        throw runtime_error("Getting address information failed");
    }

    optional<Socket> client;
    for(p = res, p != NULL, p = p->ai_next){
        try{
            Socket s(p->ai_family, p->ai_socktype, p->ai_protocol);
            s.connect(p->ai_addr, p->ai_addrlen);
            client = move(s);
            break;
        }catch(...){
            continue;
        }
    }

}
