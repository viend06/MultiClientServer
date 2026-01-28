#include <iostream>
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

        int getfd() const{
            return sockfd;
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
        }

        // Create stack
        void listen(int backlog){
            if(::listen(sockfd, backlog) == -1){
                throw runtime_error("Can not create stack for server");
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
    int yes = 1;

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

            break;
        }catch(...){
            continue;
        }
    }

    return 0;
}

