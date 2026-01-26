#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
using namespace std ; 

class Socket{
    private: 
        int sockfd;
    public:
        //Constructor
        Socket(int &domain, int &type, int &protocol){
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
    struct addrinfo *res;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfor(NULL, "2026", &hints, &res);
    if(status != 0){
        throw runtime_error("Getting address information failed");
    }



    return 0;
}

