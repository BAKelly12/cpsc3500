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

// CAT IS READY FOR TESTING
// display the contents of a data file
void FileSys::cat(const char *name)
{
	//load current directory
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	
	int counter = 0;
	//find inode info
	for (int i = 0; i < currentDir.num_entries; i++)
		if (currentDir.dir_entries[i].name == name)
		{
			//load inode in.
			struct inode_t tempInode;
			bfs.read_block(currentDir.dir_entries[i].block_num, (void*) &tempInode);
			//We are in inode now. Look through data.
			
			//See how many blosk we will be reading from
			int totalReadBlocks = tempInode.size / 128;
			if (n % BLOCK_SIZE > 0) 
					totalReadBlocks++;
			
			for (int i = 0; i < totalReadBlocks; i++)
			{
				//Load a block of data into memory
				struct datablock_t tempData;
				bfs.read_block(tempInode.blocks[i], (void*) &tempData);
				
				for (int i = 0; i < 128; i++);
				{
					//If it hits the correct number of bytes..
					if (counter == tempInode.size)
						return NULL;
					cout << tempData.data[i] 
					counter++;
				}				
			}
		}
	}
}

// HEAD IS READY FOR TESTING
// display the first N bytes of the file
void FileSys::head(const char *name, unsigned int n)
{
	//load current directory
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	
	//find inode info
	int counter = 0;
	for (int i = 0; i < currentDir.num_entries; i++)
		if (currentDir.dir_entries[i].name == name)
		{
			//load inode in.
			struct inode_t tempInode;
			bfs.read_block(currentDir.dir_entries[i].block_num, (void*) &tempInode);
			//We are in inode now. Look through data.
			
			//Ensure you can display n bytes and that it doesnt exceed the bytes a file has.
			if (n > tempInode.size)
			{
				perror("The n bytes entered exceeds the size of the file");
				return NULL;			
			}
			
			
			//See how many blosk we will be reading from
			//Unnecessary, could just do some large number, but this keeps things logical.
			int totalReadBlocks = n / 128;
			if (n % BLOCK_SIZE > 0) 
					totalReadBlocks++;
			
			for (int i = 0; i < totalReadBlocks; i++)
			{
				//Load a block of data into memory
				struct datablock_t tempData;
				bfs.read_block(tempInode.blocks[i], (void*) &tempData);
				
				for (int i = 0; i < 128; i++);
				{
					//If it hits the correct number of bytes..
					if (counter == n)
						return NULL;
					cout << tempData.data[i] 
					counter++;
				}				
			}
		}
	}
}

// delete a data file
void FileSys::rm(const char *name)
{
	
}

// STAT IS READY FOR TESTING
// display stats about file or directory
void FileSys::stat(const char *name)
{
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	for (int i = 0; i < currentDir.num_entries; i++)
	{	
		if (currentDir.dir_entries[i].name == name)
		{
			//load inode/directory in
			struct inode_t tempInode;
			bfs.read_block(currentDir.dir_entries[i].block_num, (void*) &tempBlock)
			
			if (tempBlock.magic == 0xFFFFFFFE) //Check to see if it is an inode
			{
				//THE FOLLOWING IS INODE INFORMATION
				// Inode block and bytes in file.
				cout << "Inode block: " << currentDir.dir_entries[i].block_num << endl;
				cout << "Bytes in file: " << tempBlock.size << endl;
			
				//Number of blocks
				int tempNumBlocks;
				tempNumBlock = tempBlock.size / BLOCK_SIZE; // Count how many blocks are full
				if (tempBlock.size % BLOCK_SIZE > 0) //Check to see if any are partially full, if so increment var.
					tempNumBlocks++;
				tempNumBlocks++; // Account for inode being part of it.
				cout << "Number of blocks: " << tempNumBlocks << endl;
			
				//First block
				int tempFirstBlock;
				if (tempBlock.size == 0)
					tempFirstBlock = 0;
				else
					tempFirstBlock = tempInode.blocks[0]
			
				cout << "First block: " << tempFirstBlock << endl;
				return NULL;
			}
			
			else if (tempBlock.magic == 0xFFFFFFFF) //check to see if it is a directory
			{
				//THE FOLLOWING IS DIRECTORY INFORMATION
				cout << "Directory name: " << currentDir.dir_entries[i].name << endl;
				cout << "Directory block: " << currentDir.dir_entries[i].block_num << endl;
				return NULL;
			}
			
			else //Something isnt right.
				perror("There is an issue in stats.");
		}
	}
}

// HELPER FUNCTIONS (optional)