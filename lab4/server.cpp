#include <iostream>
#include <signal.h>
#include <string>
#include <strings.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sstream>
#include <unistd.h> 
#include "FileSys.h"
#include <arpa/inet.h>

#define MAX_INC_READ_SIZE 4096
#include <netinet/in.h>
#define PACKET_MAX_SIZE 32

using namespace std;

void cleanExit(){exit(0);}
void parseAndCall(string s);
void getCmd();
int knit(int p);
int await();
int sockread(size_t len);
void unbind();

FileSys fs;


int sock, newSock;
socklen_t clientAddressLength;
std::string clientName; 
std::string message;
sockaddr_in serverAddress, clientAddress;




int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Usage: ./nfsserver port#\n";
        return -1;
    }
    
    int port = atoi(argv[1]);

    
    if(knit(port)==-1)
      exit(EXIT_FAILURE); 
  
    await();
    //mount the file system
    fs.mount(newSock); //assume that sock is the new socket created 
                    //for a TCP connection between the client and the server.   
 
    //loop: get the command from the client and invoke the file
    //system operation which returns the results or error messages back to the clinet
    //until the client closes the TCP connection.
    

    

	
	for(int i(0); i<10;i++){
		cerr<<"Getting command\n\n";
	
	getCmd();
	}
    //close the listening socket
    
    unbind();
    

    //unmout the file system
    fs.unmount();
    
    /*Close the socket*/

    return 0;
}



void getCmd(){
    /*Wild and crazy search function*/

	string asubstr;
	string s1;
	string s2 = "\\r\\n";
    string msg;
	size_t found;
    int bytes_read(0);

    while( (found = msg.find(s2) )== std::string::npos){

		/**keep this stuff*/
		cerr<<"in getCmd...\n\n";
        if(bytes_read > 32){
            cerr<<"Incoming command exceeds max allowable size\n";
            break;
        }
		
        size_t readSize = sockread(PACKET_MAX_SIZE);
		
		cerr<<"readsize in getCmd = " << readSize<<"\n";
		
		if(bytes_read==0)
			bytes_read = readSize;
		else
			bytes_read+= readSize;
		
		cerr<<"Bytes read in getCmd = "<<bytes_read<<"\n";
		
        msg = msg + message;
		
		cerr<<"found = " << found << "\n";
	}
		
	s1 = msg.substr(0, found);
		
	cout<<"\n"<<s1<<endl;
 
	
    parseAndCall(s1);  
	
}


void unbind(){
  close(sock);
  close(newSock);
  cerr<<"server: Sockets unbound...\n";
}


void parseAndCall(string message)
{
	int sizeOfString = message.length();
	string command;
	const int ending = 0;
	int numSpaces, counter; //For head and append parsing
	if (message[0] == 'm') {
		command = message.substr (6, sizeOfString - 6 - ending);
		fs.mkdir(command.c_str());
	}
	else if (message[0] == 'c' && message[1] == 'd') {
		command = message.substr (3, sizeOfString - 3 - ending);
		fs.cd(command.c_str());
	}
	else if (message[0] == 'h' && message[1] == 'o'){
		fs.home();
	}
	else if (message[0] == 'r' && message[1] == 'm' && message[2] == 'd') {
		command = message.substr (6, sizeOfString - 6 - ending);
		fs.rmdir(command.c_str());
	}
	else if (message[0] == 'l') {
		fs.ls();
	}
	else if (message[0] == 'c' && message[1] == 'r') {
		command = message.substr (7, sizeOfString - 7 - ending);
		fs.create(command.c_str());
	}
	else if (message[0] == 'a') {
		numSpaces = 0;
		counter = 0;
		string command2;
		while(numSpaces != 2)
		{
			if (message[counter] == ' ')
				numSpaces++;
			counter++;
		}
		command = message.substr (7, counter - 8);
		command2 = message.substr(counter, sizeOfString - (counter) - ending);
		fs.append(command.c_str(), command2.c_str());
	}
	else if (message[0] == 'c' && message[1] == 'a') {
		command = message.substr (4, sizeOfString - 4 - ending);
		fs.cat(command.c_str());
	}
	else if (message[0] == 'h' && message[1] == 'e') {
		numSpaces = 0;
		counter = 0;
		string command2;
		while(numSpaces != 2)
		{
			if (message[counter] == ' ')
				numSpaces++;
			counter++;
		}
		command = message.substr (5, counter - 6);
		command2 = message.substr(counter, sizeOfString - (counter) - ending);
		unsigned hnum = stoul(command2.c_str(), nullptr, 0);
		hnum = ntohl(hnum);
		fs.head(command.c_str(), stoul(command2.c_str(), nullptr, 0));
	}
	else if (message[0] == 'r' && message[1] == 'm') {
		command = message.substr (3, sizeOfString - 3 - ending);
		fs.rm(command.c_str());
	}
	else if (message[0] == 's') {
		command = message.substr (5, sizeOfString - 5 - ending);
		fs.stat(command.c_str());
	}
	else 
		cout << "ERROR EXITING HELP\n";	
	
}


int sockread(size_t len)
{
    char buf[len];
    size_t count = len;
    char  *bufptr = (char*)buf;
    cerr<<"Reading from socket..\n";  
    ssize_t bytes_received(0);
    while(count > 0)
   {
       bytes_received = read(newSock, bufptr, count); 
       if (bytes_received <= 0) 
           return bytes_received;
        /* Decrement remaining size */  
        count -= bytes_received;  
        /*increment buffer pointer*/
        bufptr += bytes_received;   
    }
    message = buf;
	cerr<<message;
    cerr<< endl<<"Bytes received: "<<bytes_received<<"\n";
    return len;
}



int knit(int p)
{  
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(p); 
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); 

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

int await()
{

	socklen_t clientAddressLength  = sizeof(clientAddress);
	newSock = accept(sock,(struct sockaddr*)&clientAddress,&clientAddressLength);
    if(newSock<0)
      return -1;
    /*get a string copy of client address for easy display*/
	clientName = inet_ntoa(clientAddress.sin_addr);
    cout<<"Server:  Connected to client: " << clientName << "..."<<endl;
 

    /*new thread creation would be implemented here*/
  return 0;
}
