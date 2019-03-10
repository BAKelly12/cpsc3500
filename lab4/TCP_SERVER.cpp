
#include "TCP_SERVER.h"
using namespace std;



TCP_SERVER::TCP_SERVER()
{ 
  /*clear Server address structure then set protocol and address*/
  bzero((char*) &serverAddress, sizeof(serverAddress));  
  serverAddress.sin_family=AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero((char*) &clientAddress, sizeof(clientAddress));
  bzero((char*) &serverAddressInfo, sizeof(serverAddressInfo));
  clientAddressLength = 0;
  sock = 0;
  newSock = 0;
  port = 0;
  clientName = "";
  
}

/*
TCP_SERVER::~TCP_SERVER()
{

  
}
*/


int TCP_SERVER::knit(int p)
{  
    /*create socket*/
    if((sock = socket(AF_INET, SOCK_STREAM, 0))<0){
      cerr<<"Socket failed to create\n\n";
      return -1;
    }
    /*set port to network byte-order*/
    serverAddress.sin_port=htons(p);
    
    /*bind socket to port*/
    if(bind(sock, (struct sockaddr*) &serverAddress, sizeof(serverAddress))<0){
      cerr<<"Error binding to socket\n\n";
      return -1;
    }
    /*Listen on socket, max 5 connection*/
    if(listen(sock, 5)<0){
      cerr<<"I'm deaf..\n\n";
      return -1;
    }
    cout<<"Server: Listening for connection on " << p <<"...\n";
    return 0;
}



int TCP_SERVER::await()
{

		clientAddressLength  = sizeof(clientAddress);
		newSock = accept(sock,(struct sockaddr*)&clientAddress,&clientAddressLength);
    if(newSock<0)
      return -1;
    /*get a string copy of client address for easy display*/
		clientName = inet_ntoa(clientAddress.sin_addr);
    cout<<"Server:  Connected to client: " << clientName << "..."<<endl;

    /*new thread creation would be implemented here*/

  return 0;
}


int TCP_SERVER::sockread(size_t len)
{
    char buf[len];
    size_t count = len;
    char  *bufptr = (char*)buf;
    cerr<<"Reading from socket..\n";  
    ssize_t bytes_received;
    while(count > 0)
   {
       bytes_received = read(newSock, bufptr, count);
       if (bytes_received <= 0)
           return bytes_received;
        /* Decrement remaining size */  
        count -= bytes_received;  
        /*increment buffer pointer*/
        bufptr += bytes_received;  
        /*tabulate total receive size*/
        len+=bytes_received;
    }
    message = buf;
    cout << endl<<"Bytes received: "<<bytes_received<<endl;
    return len;
}


int TCP_SERVER::sockwrite(void* p, int size)
{ 
  int bytes_sent(0);
  int x;
  while(bytes_sent < size){
    if(x = write(sock, p, size - bytes_sent)<0){
      cerr<<"Error writing to socket..\n";
      close(sock);
      return -1;
    }
    p+=x;
    bytes_sent +=x;
  }
  return bytes_sent;
}

void TCP_SERVER::unbind(){
  close(sock);
  close(newSock);
  cerr<<"server: Sockets unbound...\n";
}

void TCP_SERVER::wipe()
{ 
  clientName="";
}