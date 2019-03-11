// CPSC 3500: File System
// Implements the file system commands that are available to the shell.

#ifndef FILESYS_H
#define FILESYS_H
#include <string>
#include <string.h>
using namespace std;

#include "BasicFileSys.h"

class FileSys {
  
  public:
    // mounts the file system
    void mount(int sock);

    // unmounts the file system
    void unmount();

    // make a directory
    void mkdir(const char *name);

    // switch to a directory
    void cd(const char *name);
    
    // switch to home directory
    void home();
    
    // remove a directory
    void rmdir(const char *name);

    // list the contents of current directory
    void ls();

    // create an empty data file
    void create(const char *name);

    // append data to a data file
    void append(const char *name, const char *data);

    // display the contents of a data file
    void cat(const char *name);

    // display the first N bytes of the file
    void head(const char *name, unsigned int n);

    // delete a data file
    void rm(const char *name);

    // display stats about file or directory
    void stat(const char *name);

  private:
    BasicFileSys bfs;	// basic file system
    short curr_dir;	// current directory
	short home_dir = 1;
    int fs_sock;  // file server socket
	string toWrite;
	
void writeSock(string msg)
{    
	int bytes_sent(0);
	size_t size = msg.length();
	int x;
	void* p = (void*)msg.c_str();
  
	while(bytes_sent < size){
		if((x = write(fs_sock, p , size - bytes_sent))<0){
			cerr<<"Error writing to socket..\n";
			close(fs_sock);
		return;
		}
		p+=x;
		bytes_sent +=x;
	}
	return;
}

    // Additional private variables and Helper functions - if desired

};

#endif 
