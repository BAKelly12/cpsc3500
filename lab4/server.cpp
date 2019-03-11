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
#include "FileSys.h"
#include "TCP_SERVER.h"
#define MAX_INC_READ_SIZE 4096
#define PACKET_MAX_SIZE 32

using namespace std;

void cleanExit(){exit(0);}
void parseAndCall(string s);
void getCmd();

FileSys fs;
int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Usage: ./nfsserver port#\n";
        return -1;
    }
    
    int port = atoi(argv[1]);
    TCP_SERVER server;
    
    if(server.knit(port)==-1)
      exit(EXIT_FAILURE); 

    //mount the file system
    fs.mount(server.newSock); //assume that sock is the new socket created 
                    //for a TCP connection between the client and the server.   
 
    //loop: get the command from the client and invoke the file
    //system operation which returns the results or error messages back to the clinet
    //until the client closes the TCP connection.
    

    server.await();
    for(int i(0); i<5;i++){
      server.message.erase();
      int x = server.sockread(PACKET_MAX_SIZE);
      cout << server.message<<endl;
    }
    
    //close the listening socket
    
    server.unbind();
    

    //unmout the file system
    fs.unmount();
    
    /*Close the socket*/

    return 0;
}

void getCmd(){
    /*Wild and crazy search function*/
    string msg;
    int bytes_read(0);
    bool eom = false;
    size_t pos = npos;
    while(!eom){
        if(bytes_read > MAX_INC_READ_SIZE){
            cerr<<"Incoming command exceeds max allowable size\n";
            break;
        }
        size_t readSize = server.sockread(PACKET_MAX_SIZE);
        msg = msg + server.message;
        if(bytes_read>3)
            pos = msg.find("\r\n", bytes_read-4);
        bytes_read+= readSize;
        if(pos != npos){
            eom = true;
            size_t offset = bytes_read - (bytes_read-(pos+3));
            msg.resize(offset);
        }
    }
    parseAndCall(msg);    
}

void parseAndCall(string message)
{
	int sizeOfString = message.length();
	string command;
	const int ending = 4;
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
		command = message.substr (7, counter - 7);
		command2 = message.substr(counter + 1, sizeOfString - (counter + 1) - ending);
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
		command = message.substr (5, counter - 5);
		command2 = message.substr(counter + 1, sizeOfString - (counter + 1) - ending);
		fs.head(command.c_str(), stoul (command2.c_str(), nullptr, 0));
	}
	else if (message[0] == 'r' && message[1] == 'm' && message[3] == ' ') {
		command = message.substr (3, sizeOfString - 3 - ending);
		fs.rm(command.c_str());
	}
	else if (message[0] == 's') {
		command = message.substr (5, sizeOfString - 5 - ending);
		fs.stat(command.c_str());
	}
	else 
		cout << "ERROR EXITING HELP";	
}