// CPSC 3500: File System
// Implements the file system commands that are available to the shell.

#include <cstring>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
using namespace std;

#include "FileSys.h"
#include "BasicFileSys.h"
#include "Blocks.h"

// mounts the file system
void FileSys::mount(int sock) {
  bfs.mount();
  curr_dir = 1; //by default current directory is home directory, in disk block #1
  fs_sock = sock; //use this socket to receive file system operations from the client and send back response messages
}

// unmounts the file system
void FileSys::unmount() {
  bfs.unmount();
  close(fs_sock);
}

// make a directory
void FileSys::mkdir(const char *name)
{
	if (mkdir(name) == 0)
		//Directory has been made
	else
		printf("Error in make directory.");
}

// switch to a directory
void FileSys::cd(const char *name)
{
	if (chdir(name) == 0)
		//Directory changed
	else
		printf("Error in switch directory.");
}

// switch to home directory
void FileSys::home() 
{
	//Save home directory from the start?
}

// remove a directory
void FileSys::rmdir(const char *name)
{
	if(rmdir(name) == 0)
		//File removed..
	else	
		printf("Error in remove directory");
}

// list the contents of current directory
void FileSys::ls()
{
	
}

// create an empty data file
void FileSys::create(const char *name)
{
	
}

// append data to a data file
void FileSys::append(const char *name, const char *data)
{
}

// display the contents of a data file
void FileSys::cat(const char *name)
{
	
}

// display the first N bytes of the file
void FileSys::head(const char *name, unsigned int n)
{
	//This likely isnt write, but this is close? Might need to read from the file in terms of bytes.
	head -c n name
}

// delete a data file
void FileSys::rm(const char *name)
{
	if (remove(name) == 0)
		//Data file removed/deleted...
	else	
		printf("Error in deleting a data file..");
}

// display stats about file or directory
void FileSys::stat(const char *name)
{
}

// HELPER FUNCTIONS (optional)

