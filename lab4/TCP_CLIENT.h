#include <iostream>
#include <signal.h>
#include <string>
#include <strings.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h> 
#include <arpa/inet.h>



class TCP_CLIENT{
  public:
  
  
    TCP_CLIENT();
    
    struct addrinfo hints, *p, *servInfo;
    
    char *buf;
    int port, sock;
    std::string message;
    
    /****************************
     *MEMBER FUNCTIONS
     ****************************/
    
    int lookup(std::string hostname, std::string port);
    int call();
    std::string sockread(int size);
    int sockwrite(void* m, int size);
    int hangup();

};