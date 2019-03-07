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
  home_dir = 1; //Ensures home directory has a value
  fs_sock = sock; //use this socket to receive file system operations from the client and send back response messages
}

// unmounts the file system
void FileSys::unmount() {
  bfs.unmount();
  close(fs_sock);
}

// MKDIR IS READY TO BE TESTED
// make a directory
void FileSys::mkdir(const char *name)
{
	//Load current directory
	struct dirblock_t currentDir;
	read_block(curr_dir, (void*) &currentDir)
	
	//Create inode in memory	
	short inodeEntry = bfs.get_free_block(); //inode
	
	//initialize inode
	inode_t newInode;
	newInode.magic = INODE_MAGIC_NUM;
	newInode.size = 0;
	bfs.write_block(inodeEntry, (void*) &newInode); //write inode to block
	
	//Update inode spot in current directory
	currentDir.num_entries++;
	currentDir.dir_entries[currentFolder.num_entries].name = name;
	currentDir.dir_entries[currentFolder.num_entries].block_num = inodeEntry;
	
	//Create the directory entry in current directory entries
	short dirEntry = bfs.get_free_block();
	
	struct dirblock_t newDir;
	newDir.magic = DIR_MAGIC_NUM;
	newDir.num_entries = 2;
	//directory entry to the parent
	newDir.dir_entries[0].block_num = curr_dir;
	newDir.dir_entries[0].name = "..";
	//directory entry to itself
	newDir.dir_entries[1].block_num = dirEntry;
	newDir.dir_entries[1].name = '.';

	//Update new directory in current directory entries
	currentDir.num_entries++;
	currentDir.dir_entries[currentFolder.num_entries].name = name;
	currentDir.dir_entries[currentFolder.num_entries].block_num = dirEntry;
	
	//Write updates
	bfs.write_block(curr_dir, (void*) &currentDir); //this is wrong but right concept?
	bfs.write_block(dirEntry, (void*) &newDir);
	
}

// CD IS READY TO BE TESTED
// switch to a directory
void FileSys::cd(const char *name)
{
	//load the current directory
	struct dirblock_t currentDir;
	read_block(curr_dir, (void*) &currentDir)
	
	//Search current directory for the directory we want to switch to.
	for (int i = 0; i < currentDir.num_entries; i++) {
		
		if (currentDir.dir_entries[i].name == name)
		{
			curr_dir = currentDir.dir_entries[i].block_num;
			return NULL;
		}
	}
	
	perror("Directory not found.");
}

// HOME IS READY TO BE TESTED
// switch to home directory
void FileSys::home() 
{
	curr_dir = home_dir;
}

//This will need a lot of work..
// remove a directory
void FileSys::rmdir(const char *name)
{
	//reduce number of directories in the
	short victimDir
	
	for (int i = 0; i < currentFolder.num_entries; i++) {
		if (currentFolder.dir_entries[i].name == name)
		{
			currentFolder.num_entries--;
			victimDir = currentFolder.dir_entries[i].block_num;
			
			bfs.reclaim_block(victimDir);
			return NULL;
		}
	}
	
	
	perror("Directory not found.");
}

// LS IS READY FOR TESTING
// list the contents of current directory
void FileSys::ls()
{
	//load the current directory contents.
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	
	//Check to see if anything exists in current directory
	if (currentDir.num_entries == 0)
		cout << "empty folder" << endl;
	//Go through each entry, output each entry name.
	else {
		for (int i = 0; i < currentFolder.num_entries; i++) {
			//Loop here to go through the whole name, just a single character right now.
			cout << currentFolder.dir_entries[i].name << " ";
		}
		cout << endl;
	}
}

// CREATE IS READY FOR TESTING
// create an empty data file
void FileSys::create(const char *name)
{
	short inodeEntry = bfs.get_free_block(); //inode
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	
	//create inode to hold metadata
	inode_t newInode;
	newInode.magic = INODE_MAGIC_NUM;
	newInode.size = 0;
	bfs.write_block(inodeEntry, (void*) &newInode); //write inode to block
	
	//Index new file in directory
	currentDir.num_entries++;
	currentDir.dir_entries[currentFolder.num_entries].name = name;
	currentDir.dir_entries[currentFolder.num_entries].block_num = inodeEntry;
	
	bfs.write_block(curr_dir, (void*) &currentDir);
	//this seems like an append to currentFolder.dir? need to come back to this
}

// append data to a data file
void FileSys::append(const char *name, const char *data)
{
	short dataEntry = bfs.get_free_block();
	
	struct datablock_t newData;
}

// display the contents of a data file
void FileSys::cat(const char *name)
{
	
}

// display the first N bytes of the file
void FileSys::head(const char *name, unsigned int n)
{
	
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

