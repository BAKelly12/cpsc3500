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
#define PACKET_MAX_SIZE 32

using namespace std;

void cleanExit(){exit(0);}

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
    FileSys fs;
    fs.mount(server.newSock); //assume that sock is the new socket created 
                    //for a TCP connection between the client and the server.   
 
    //loop: get the command from the client and invoke the file
    //system operation which returns the results or error messages back to the clinet
    //until the client closes the TCP connection.
    

    server.await();
    for(int i(0); i<10;i++){
      server.message.erase();
      int x = server.sockread(PACKET_MAX_SIZE-1);
      cout << server.message <<endl;
    }
    

    
    //close the listening socket
    
    server.unbind();
    

    //unmout the file system
    fs.unmount();
    
    /*Close the socket*/

    return 0;
}
