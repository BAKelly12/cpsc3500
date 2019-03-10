//

#include "TCP_CLIENT.h"

using namespace std;

TCP_CLIENT::TCP_CLIENT(){

  bzero((char*) &servInfo, sizeof(servInfo));
  bzero((char*) &hints, sizeof(hints));
  
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  
  servInfo->ai_family = AF_UNSPEC;
  servInfo->ai_socktype = SOCK_STREAM;
}

//dns lookup
int TCP_CLIENT::lookup(string hostname, string po){
  int rv(0);
  /*Get Server address information using hints and place it into servInfo*/
  if((rv = getaddrinfo(hostname.c_str(), po.c_str(), &hints, &servInfo))<0){
    cerr<<"Error resolving hostname.. Exit code: "<<gai_strerror(rv)<<"\n\n";
    return -1;
  }
  cerr<<"Found server..\n\n";
  return 0;
}

int TCP_CLIENT::call(){ 
  /*Loop to try until connection or failure*/
  for(p=servInfo; p!=NULL;p=p->ai_next){
    if((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol))<0){
      cerr<<"Error creating socket..\n\n";
      continue;
    }   
    if(connect(sock, p->ai_addr, p->ai_addrlen)<0){
      cerr<<"Error 418: I'm a teapot..\n\n";
      continue;
    }
    break;
  }
    if(p == NULL){
      cerr<<"client: Unable to connect to server..\n\n";  
      return -1;
    }
    //else
    cerr<< "client: Connection Successful..\n\n";
    freeaddrinfo(servInfo);
    freeaddrinfo(p);
    return 0;
}

int TCP_CLIENT::hangup(){
  close(sock);
  return 0;
}


int TCP_CLIENT::sockwrite(void *m, int size){
  int bytes_sent(0);
  int x;
  while(bytes_sent < size){
    if((x = write(sock, p, size - bytes_sent))<0){
      cerr<<"Error writing to socket..\n";
      close(sock);
      return -1;
    }
    p+=x;
    bytes_sent +=x;
  }
  return bytes_sent;
}


string TCP_CLIENT::sockread(int size){
  
    int len(0);
    buf = new char[size];
    char  *bufptr = (char*) buf;
    size_t count(size);
    while (count > 0)
    {
      //will need to add something here to account for messaging format
        ssize_t readSize = read(sock, bufptr, count);
        if (readSize <= 0){
           //Error or other end closed connection 
            cerr<<"Error reading from socket...";
            return "-1";
        }           
        /* Decrement remaining size */  
        count -= readSize;  
        /*increment buffer pointer*/
        bufptr += readSize;  
        /*tabulate total receive size*/
        len+=readSize;
    }
    message = buf;
    //MAybe need to free memory here
    return message; 
}
