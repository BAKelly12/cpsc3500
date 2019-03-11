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
//#include <pthread.h> to be implemented later
//#define ENABLE_MULTITHREADING 0


#pragma once


class TCP_SERVER
{
  public:
    int port;
    int sock, newSock;
    socklen_t clientAddressLength;
    std::string clientName; 
    std::string message;
    struct addrinfo serverAddressInfo;
    struct sockaddr_in serverAddress, clientAddress; 
 
    /***********************************
     *        MEMBER FUNCTIONS
     ***********************************/
     
     
     /**BIG SIX*/
     TCP_SERVER();
     ~TCP_SERVER()=default;
     TCP_SERVER(const TCP_SERVER& other) = delete;
     TCP_SERVER(TCP_SERVER&& temp) = delete;
	 TCP_SERVER& operator=(const TCP_SERVER& other) = delete;
     TCP_SERVER& operator=(TCP_SERVER&& temp) = delete;

 
    /**
      *@brief for creating socket and binding to port
      */
    int knit(int p);

    /**
      *@brief reads packet from socket, puts bits read in buf
      *@return total length of read
      */
    int sockread(size_t len); 
    
    
    /**
     *@brief concatenates message
     */
    int buildMSG();
    
 
    /**
     *@brief waits for and accepts connection when client joins
     *@return success(0) or failure(-1)
     */
    int await();
    
    
    /**
     *@brief writes msg to client
     *Code adapted, Courtesy of Dr. Yingwu Zhu, Seattle University
     */
    int sockwrite(void *p, int size);
    
    
    /**
     *@brief closes sockets, unbinds port
     */
    void unbind();
    
    
    /**
      *@brief Clears client stuff
      */
    void wipe();
    
    
    
  private:
    
  struct Packet
    {
      std::string data;
      int size;
    };
    
    
};
