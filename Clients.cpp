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
    
    for(p = res, p != NULL, p = p->ai_next){
        try{

        }
    }

}
