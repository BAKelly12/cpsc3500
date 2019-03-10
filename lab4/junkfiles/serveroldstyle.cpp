#include <iostream>
#include <signal.h>
#include <string>//
#include <strings.h>//
#include <string.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h> //
#include <arpa/inet.h>

#include "FileSys.h"

#define Q_SIZE 5

using namespace std;

/**
 *@brief provided function for clean exit on SIGINT or SIGTERM
 */
void cleanExit(int rv){exit(rv);}


// get sockaddr, IPv4 or IPv6:  (from beej.us)
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Usage: ./nfsserver port#\n";
        return -1;
    }
    
   int port = atoi(argv[1]);

    //networking part: create the socket and accept the client connection
   int sfd, newsfd,rv,count;
   struct addrinfo hints, *servInfo, *p;
   struct sockaddr_storage cliAddr;
   socklen_t cliLen;
   char s[INET_ADDRSTRLEN];
   char buf[1024];
   int on = 1;

   bzero(&hints, sizeof(hints));
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE;
   
   if((rv = getaddrinfo(NULL, to_string(port).c_str(), &hints, &servInfo))!= 0){
     cerr<<"getaddrinfo: "<< gai_strerror(rv);
     exit(EXIT_FAILURE);
   }
   
   for(p=servInfo;p!=NULL;p=p->ai_next){
     if((sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))!=0){
       cerr<<"Trying next socket..\n";
       continue;
     }
     
     if(setsockopt(sfd, SOL_SOCKET,SO_REUSEADDR, &on, sizeof(int))!=0){
       cerr<<"Error setting reuse address option..\n";
       exit(EXIT_FAILURE);
     }
     
     /*Bind socket*/
     if(bind(sfd, p->ai_addr, p->ai_addrlen)!=0){
       close(sfd);
       cerr<<"Trying next address..\n";
       continue;
     }
     break;
   }
   
   /*memory cleanup*/
   freeaddrinfo(servInfo);
   if(p==NULL){
     cerr<<"Failed to bind..\n";
     exit(EXIT_FAILURE);
   }
   /*listen for connection*/
   if(listen(sfd, Q_SIZE)!=0){
     cerr<<"I'm deaf..\n";
     exit(EXIT_FAILURE);
   }
  
   /*Set Up cleanup if abrupt exit*/
   signal(SIGTERM, cleanExit);
   signal(SIGINT, cleanExit);
   
   cliLen = sizeof(cliAddr);
   if((newsfd = accept(sfd, (struct sockaddr *)&cliAddr, &cliLen))!=0){
     cerr<<"Error accepting connection..\n";
     exit(EXIT_FAILURE);
   }
   
   /*got connection, print that we got it and address of client*/
   inet_ntop(cliAddr.ss_family, get_in_addr((struct sockaddr *)&cliAddr), s, sizeof(s));
   cout<<"Connected to "<<s<<"..."<<endl;

   
   /*test communications*/
   if((count = read(newsfd, buf, 1023))>0){
     cerr<<"I can't read..\n";
     exit(EXIT_FAILURE);
   }
   
   char *out_message = "Message Recieved\r\n";
   cout<<"Incoming message: "<<buf<<endl;
   if((count = write(newsfd, out_message, strlen(out_message)))<0){
     cerr<<"Error writing to socket.."<<endl;
     exit(EXIT_FAILURE);
   }
   
   close(sfd);
   close(newsfd);

 
    
    return 0;
}
