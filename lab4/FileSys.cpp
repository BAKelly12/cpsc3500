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
	//Load current directory
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	for (unsigned int i = 0; i < currentDir.num_entries; i++) //check to see if it exists yet
	{
		if (strcmp (currentDir.dir_entries[i].name, name) == 0)
		{
			cout << "Directory already exists" << endl;
			return;
		}
	}
	
	//Create the directory entry in current directory entries
	struct dirblock_t dir_block;
	short dirEntry = bfs.get_free_block();
	dir_block.magic = DIR_MAGIC_NUM;
	dir_block.num_entries = 0;
	for (int i = 0; i < MAX_DIR_ENTRIES; i++) {
		dir_block.dir_entries[i].block_num = 0;
	}
	
	//Update new directory in current directory entries
	strcpy (currentDir.dir_entries[currentDir.num_entries].name, name);
	currentDir.dir_entries[currentDir.num_entries].block_num = dirEntry;
	currentDir.num_entries++;
	
	//Write updates
	bfs.write_block(curr_dir, (void*) &currentDir);
	bfs.write_block(dirEntry, (void*) &dir_block);
	cout << "success" << endl;
}

// switch to a directory
void FileSys::cd(const char *name)
{
	//load the current directory
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	
	//Search current directory for the directory we want to switch to.
	for (unsigned int i = 0; i < currentDir.num_entries; i++) {
		
		if (strcmp (currentDir.dir_entries[i].name, name) == 0)
		{
			//Ensure what we are switching to is not a inode
			if (currentDir.magic == 0xFFFFFFFE)
			{
				cout << "Can't change directory to a data file." << endl;
				return;
			}
			
			curr_dir = currentDir.dir_entries[i].block_num;
			cout << endl << "success" << endl;
			return;
		}
	}
	cout << "Directory not found.";
}

// switch to home directory
void FileSys::home() 
{
	curr_dir = 1; //1 is the home directory
}

// remove a directory
void FileSys::rmdir(const char *name)
{
	//Load current directory
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	
	//Find directory to remove
	for (unsigned int i = 0; i < currentDir.num_entries; i++) {
		
		if (strcmp (currentDir.dir_entries[i].name, name) == 0)
		{
			struct dirblock_t dirBlockTemp;
			bfs.read_block(currentDir.dir_entries[i].block_num, (void*) &dirBlockTemp);
			
			//Ensure it is a directory
			if (dirBlockTemp.magic == 0xFFFFFFFE)
			{
				cout << "This file is a data file, not a directory. Exiting command." << endl;
				return;
			}
			
			if (dirBlockTemp.num_entries == 0) //Ensure it is empty
			{
				bfs.reclaim_block(currentDir.dir_entries[i].block_num);
				for (unsigned int j = i; j < currentDir.num_entries; j++)
					currentDir.dir_entries[j] = currentDir.dir_entries[j+1];
				
				currentDir.num_entries--;
				bfs.write_block(curr_dir, (void*) &currentDir);
				cout << endl << "success" << endl;
				return;
			} else {
				cout << "Directory isn't empty." << endl;
				return;
			}
		}
	}
	cout << "File does not exist" << endl;
}

// list the contents of current directory
void FileSys::ls()
{
	char printS[] = " ";
	//load the current directory contents.
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	
	//Check to see if anything exists in current directory
	if (currentDir.num_entries == 0)
		cout << "Empty folder." << endl;
	//Go through each entry, output each entry name.
	else {
		struct inode_t tempBlock;
		for (unsigned int i = 0; i < currentDir.num_entries; i++) 
		{
			bfs.read_block(currentDir.dir_entries[i].block_num, (void*) &tempBlock);
			
			if (tempBlock.magic == 0xFFFFFFFE) //Check to see if it is an inode
			{
			strcpy (printS, currentDir.dir_entries[i].name);
			cout << printS << endl;
			} else {
				strcpy (printS, currentDir.dir_entries[i].name);
				cout << printS << "/" << endl;
			}
		}
		cout << endl;
	}
}

// create an empty data file
void FileSys::create(const char *name)
{
	//see if file already exists.
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	for (unsigned int i = 0; i < currentDir.num_entries; i++)
	{
		if (strcmp (currentDir.dir_entries[i].name, name) == 0)
		{
			cout << "File already exists." << endl;
			return;
		}
	}
	

	short inodeEntry = bfs.get_free_block(); //inode
	
	//create inode to hold metadata
	struct inode_t newInode;
	newInode.magic = INODE_MAGIC_NUM;
	newInode.size = 0;
	bfs.write_block(inodeEntry, (void*) &newInode); //write inode to block
	
	//Index new file in directory
	strcpy (currentDir.dir_entries[currentDir.num_entries].name, name);
	currentDir.dir_entries[currentDir.num_entries].block_num = inodeEntry;
	currentDir.num_entries++;
	
	bfs.write_block(curr_dir, (void*) &currentDir);
	//this seems like an append to currentFolder.dir? need to come back to this
	cout << "success" << endl;
}

// append data to a data file
void FileSys::append(const char *name, const char *data)
{
	//Load current directory info
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	
	//find inode info
	for (unsigned int i = 0; i < currentDir.num_entries; i++)
	{
		if (strcmp (currentDir.dir_entries[i].name, name) == 0)
		{
			//load inode in.
			struct inode_t tempInode;
			bfs.read_block(currentDir.dir_entries[i].block_num, (void*) &tempInode);
			//We are in inode now. Look through data.
			
			if (tempInode.magic == 0xFFFFFFFF)
			{
				cout << "Can't append a directory. Exiting command." << endl;
				return;
			}
			
			//Append data in any case
			unsigned int dataCounter = 0; //Selects the bytes in the data being passed in
			int inodeBlockNum = tempInode.size / BLOCK_SIZE; //See what block num to start at.
			int dataIndex = tempInode.size % BLOCK_SIZE; //Controls location in data block
			
			if (tempInode.size == 0)
			{
				short startEntry = bfs.get_free_block();
				tempInode.blocks[0] = startEntry;
				bfs.write_block(currentDir.dir_entries[i].block_num, (void*) &tempInode);
			}
			
			
			while (strlen(data) != dataCounter) //Go until all data is written
			{
				if(dataIndex == BLOCK_SIZE) 
				{
					//create new data block as the current block has been filled
					dataIndex = 0;
					short newDataBlock = bfs.get_free_block();
					struct datablock_t newData;
					inodeBlockNum++;
					newData.data[dataIndex] = data[dataCounter];
					dataIndex++;
					dataCounter++;
					tempInode.size++;
					tempInode.blocks[inodeBlockNum] = newDataBlock;
					bfs.write_block(tempInode.blocks[inodeBlockNum], (void*) &newData);
				}
				else //Dont need to make a new block
				{
					struct datablock_t oldData;
					bfs.read_block(tempInode.blocks[inodeBlockNum], (void*) &oldData);
					oldData.data[dataIndex] = data[dataCounter]; //Write one byte to data spot.
					dataIndex++;
					dataCounter++;
					tempInode.size++;
					bfs.write_block(tempInode.blocks[inodeBlockNum], (void*) &oldData); //Write new char to disk
				}
				//Update each with the new information
				bfs.write_block(currentDir.dir_entries[i].block_num, (void*) &tempInode);
			}
			cout << "success" << endl;
			return;
		}
	}
	cout << "Data file not found." << endl;
}

// display the contents of a data file
void FileSys::cat(const char *name)
{
	//load current directory
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	
	unsigned int counter = 0;
	//find inode info
	for (unsigned int i = 0; i < currentDir.num_entries; i++)
	{
		if (strcmp (currentDir.dir_entries[i].name, name) == 0)
		{
			//load inode in.
			struct inode_t tempInode;
			bfs.read_block(currentDir.dir_entries[i].block_num, (void*) &tempInode);
			//We are in inode now. Look through data.
			
			//See how many blosk we will be reading from
			int totalReadBlocks = tempInode.size / BLOCK_SIZE;
			if (tempInode.size % BLOCK_SIZE > 0) 
					totalReadBlocks++;
			
			for (int i = 0; i < totalReadBlocks; i++)
			{
				//Load a block of data into memory
				struct datablock_t tempData;
				bfs.read_block(tempInode.blocks[i], (void*) &tempData);
				
				for (int j = 0; j < BLOCK_SIZE; j++)
				{
					//If it hits the correct number of bytes..
					if (counter == tempInode.size + 1)
						return;
					cout << tempData.data[j];
					counter++;
				}				
			}
		}
	}
	cout << endl << "File does not exist." << endl;
}

// display the first N bytes of the file
void FileSys::head(const char *name, unsigned int n)
{
	//load current directory
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	
	
	//find inode info
	unsigned int counter = 0;
	for (unsigned int i = 0; i < currentDir.num_entries; i++)
	{
		if (strcmp (currentDir.dir_entries[i].name, name) == 0)
		{
			//load inode in.
			struct inode_t tempInode;
			bfs.read_block(currentDir.dir_entries[i].block_num, (void*) &tempInode);
			//We are in inode now. Look through data.
				
			//See how many blosk we will be reading from
			//Unnecessary, could just do some large number, but this keeps things logical.
			int totalReadBlocks = n / BLOCK_SIZE;
			if (n % BLOCK_SIZE > 0) 
					totalReadBlocks++;
			
			for (int i = 0; i < totalReadBlocks; i++)
			{ 
				//Load a block of data into memory
				struct datablock_t tempData;
				bfs.read_block(tempInode.blocks[i], (void*) &tempData);
				
				for (int j = 0; j < BLOCK_SIZE; j++)
				{
					//If exceeds, it will just exit like cat once itts it the total number.
					//If it hits the correct number of bytes..
					if (counter == n || counter > tempInode.size)
						return;
					cout << tempData.data[j];
					counter++;
				}				
			}
		}
	}
	cout << endl << "File does not exist." << endl;
}


// delete a data file
void FileSys::rm(const char *name)
{
	//Load current directory
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);

	//Find file to remove
	for (unsigned int i = 0; i < currentDir.num_entries; i++) 
	{
		
		if (strcmp (currentDir.dir_entries[i].name, name) == 0)
		{
			//Load inode info.
			struct inode_t inodeBlockTemp;
			bfs.read_block(currentDir.dir_entries[i].block_num, (void*) &inodeBlockTemp);
			
			//Ensure it is an inode and not a directory
			if (inodeBlockTemp.magic == 0xFFFFFFFF)
			{
				cout << "This file is a directory, not a data file. Exiting command." << endl;
				return;
			}
			
			
			//Get total number of blocks
			int totalReadBlocks = inodeBlockTemp.size / BLOCK_SIZE;
			if (inodeBlockTemp.size % BLOCK_SIZE > 0) 
					totalReadBlocks++;
				
			for (int j = 0; j < totalReadBlocks; j++)
			{
				//Remove data file
				bfs.reclaim_block(inodeBlockTemp.blocks[j]);
			} 
			//Remove from directory entries
			
			//push every entry to the left to the right of the entry that was deleted.
			for (unsigned int j = i; j < currentDir.num_entries; j++)
				currentDir.dir_entries[j] = currentDir.dir_entries[j+1];
			
			currentDir.num_entries--;
			bfs.reclaim_block(currentDir.dir_entries[i].block_num);
			
			bfs.write_block(curr_dir, (void*) &currentDir);
			cout << endl << "success" << endl;
			return;
		}
	}
	cout << "File does not exist." << endl;
}

// display stats about file or directory
void FileSys::stat(const char *name)
{
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	for (unsigned int i = 0; i < currentDir.num_entries; i++)
	{	
		if (strcmp (currentDir.dir_entries[i].name, name) == 0)
		{
			//load inode/directory in
			struct inode_t tempBlock;
			bfs.read_block(currentDir.dir_entries[i].block_num, (void*) &tempBlock);
			if (tempBlock.magic == 0xFFFFFFFE) //Check to see if it is an inode
			{
				//THE FOLLOWING IS INODE INFORMATION
				// Inode block and bytes in file.
				cout << "Inode block: " << currentDir.dir_entries[i].block_num << endl;
				cout << "Bytes in file: " << tempBlock.size << endl;
			
				//Number of blocks
				int tempNumBlocks;
				tempNumBlocks = tempBlock.size / BLOCK_SIZE; // Count how many blocks are full
				if (tempBlock.size % BLOCK_SIZE > 0) //Check to see if any are partially full, if so increment var.
					tempNumBlocks++;
				tempNumBlocks++; // Account for inode being part of it.
				cout << "Number of blocks: " << tempNumBlocks << endl;
			
				//First block
				short tempFirstBlock;
				if (tempBlock.size == 0)
					tempFirstBlock = 0;
				else
					tempFirstBlock = tempBlock.blocks[0];
			
				cout << "First block: " << tempFirstBlock << endl;
				return;
			}			
			else 
			{
				//THE FOLLOWING IS DIRECTORY INFORMATION
				cout << "Directory name: " << currentDir.dir_entries[i].name << "/" << endl;
				cout << "Directory block: " << currentDir.dir_entries[i].block_num << endl;
				return;
			}

		}
	}
}

// HELPER FUNCTIONS (optional)