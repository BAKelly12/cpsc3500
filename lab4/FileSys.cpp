// CPSC 3500: File System
// Implements the file system commands that are available to the shell.
#include <string>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "FileSys.h"
#include "BasicFileSys.h"
#include "Blocks.h"
#include <netdb.h>

using namespace std;
#define PACKET_MAX_SIZE 32
#include <queue>//for a cheesy way to send packets

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
			sendMsg(to_string(500) + "File exists\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
			return;
		}
	}
	
	if (strlen(name) > MAX_FNAME_SIZE)
	{
		sendMsg(to_string(500) + "File name is too long\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
		return;	
	}
	
	//Dont go beyond the max num of entries
	if (currentDir.num_entries + 1 > MAX_DIR_ENTRIES)
	{
		sendMsg(to_string(506)+"Directory is full\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
		return;		
	}
	
	//Create the directory entry in current directory entries
	struct dirblock_t dir_block;
	short dirEntry = bfs.get_free_block();
	if (dirEntry == 0)
	{
		sendMsg(to_string(500) + "Disk is full\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
		return;		
	}
	
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
	sendMsg(to_string(200) + "OK\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
	return;
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
				sendMsg(to_string(500) + "File is not a directory\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
				return;
			}
			
			curr_dir = currentDir.dir_entries[i].block_num;
			sendMsg(to_string(200) + "OK\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
			return;
		}
	}
	sendMsg(to_string(500) + "File does not exist\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
	return;
}

// switch to home directory
void FileSys::home() 
{
	curr_dir = 1; //1 is the home directory
	sendMsg(to_string(200) + "OK\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
	return;
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
				sendMsg(to_string(500) + "File is not a directory\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
				return;
			}
			
			if (dirBlockTemp.num_entries == 0) //Ensure it is empty
			{
				bfs.reclaim_block(currentDir.dir_entries[i].block_num);
				for (unsigned int j = i; j < currentDir.num_entries; j++)
					currentDir.dir_entries[j] = currentDir.dir_entries[j+1];
				
				currentDir.num_entries--;
				bfs.write_block(curr_dir, (void*) &currentDir);
				sendMsg(to_string(200) + "OK\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
				return;
			} else {
				sendMsg("Directory is not empty\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
				return;
			}
		}
	}
	sendMsg(to_string(500) + "File does not exist\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
	return;
}

// list the contents of current directory
void FileSys::ls()
{
	char printS[] = "";
	string s1;
	string tempStr = to_string(200) + "OK\\r\\n\r\nLength:";
	string strInt, strComplete;
	string strEnding = "";
	char* str1;
	//load the current directory contents.
	struct dirblock_t currentDir;
	bfs.read_block(curr_dir, (void*) &currentDir);
	
	//Check to see if anything exists in current directory
	if (currentDir.num_entries == 0) {
		sendMsg(to_string(200) + "OK\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
		return;
	}
	//Go through each entry, output each entry name.
	else {
		struct inode_t tempBlock;
		for (unsigned int i = 0; i < currentDir.num_entries; i++) 
		{
			bfs.read_block(currentDir.dir_entries[i].block_num, (void*) &tempBlock);
			if (tempBlock.magic == 0xFFFFFFFE) //Check to see if it is an inode
			{
				string s1 = currentDir.dir_entries[i].name;
				strEnding = strEnding + s1 + "\n";
			} else {
				string s1 = currentDir.dir_entries[i].name;
				strEnding = strEnding + s1 + "/\n";
			}
		}
		strComplete = tempStr + to_string(strInt.length()) + "\\r\\n\r\n\\r\\n\r\n" + strEnding;
		sendMsg(strComplete);
		return;
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
			sendMsg(to_string(500) + "File exists\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
			return;
		}
	}
	
	if (strlen(name) > MAX_FNAME_SIZE)
	{
		sendMsg(to_string(500) + "File name is too long\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
		return;	
	}
	
	if (currentDir.num_entries + 1 > MAX_DIR_ENTRIES)
	{
		sendMsg(to_string(500) + "Directory is full\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
		return;		
	}

	short inodeEntry = bfs.get_free_block(); //inode
	if (inodeEntry == 0)
	{
		sendMsg(to_string(500) + "Disk is full\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
		return;		
	}
	
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
	sendMsg(to_string(200) + "OK\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
	return;
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
			
			//Check if it is a dir
			if (tempInode.magic == 0xFFFFFFFF)
			{
				sendMsg(to_string(501) + "File is a directory\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
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
				
				if (tempInode.size > MAX_FILE_SIZE)
				{
					sendMsg(to_string(500) + "Append exceeds maximum file size\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
					return;
				}
					
				if(dataIndex == BLOCK_SIZE) 
				{
					//create new data block as the current block has been filled
					dataIndex = 0;
					short newDataBlock = bfs.get_free_block();
					if (newDataBlock == 0)
					{
						sendMsg(to_string(500) + "Disk is full\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
						return;		
					}
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
			sendMsg(to_string(200) + "OK\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
			return;
		}
	}
	sendMsg(to_string(500) + "File does not exist\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
	return;
}

// display the contents of a data file
void FileSys::cat(const char *name)
{
	string tempStr = to_string(200) + "OK\\r\\n\r\nLength:";
	string strInt, strComplete;
	string strEnding = "";
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
			
			//Ensure it is a file
			if (tempInode.magic == 0xFFFFFFFF)
			{
				sendMsg(to_string(500) + "File is a directory\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
				return;
			}
			
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
					{
						strInt = to_string(htonl(counter));
						strComplete = tempStr + strInt + "\\r\\n\r\n\\r\\n\r\n" + strEnding;
						sendMsg(strComplete);
						return;
					}
					strEnding = strEnding + tempData.data[j];
					counter++;
				}				
			}
			
		}
	}
	sendMsg(to_string(500) + "File does not exist\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
	return;
}

// display the first N bytes of the file
void FileSys::head(const char *name, unsigned int n)
{	
	n = ntohl(n);
	string tempStr = to_string(200) + "OK\\r\\n\r\nLength:";
	string strInt, strComplete;
	string strEnding = "";
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
			
			//Ensure it is a file
			if (tempInode.magic == 0xFFFFFFFF)
			{
				sendMsg(to_string(500) + "File is a directory\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
				return;
			}
				
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
					{
						strInt = to_string(counter);
						strComplete = tempStr + strInt + "\\r\\n\r\n\\r\\n\r\n" + strEnding;
						sendMsg(strComplete);
						return;
					}
					strEnding = strEnding + tempData.data[j];
					counter++;
				}				
			}
		}
	}
	sendMsg(to_string(500) + "File does not exist\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
	return;
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
				sendMsg(to_string(500) + "File is a directory\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
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
			sendMsg(to_string(200) + "OK\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
			return;
		}
	}
	sendMsg(to_string(500) + "File does not exist\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
	return;
}

// display stats about file or directory
void FileSys::stat(const char *name)
{

	string strBegin = to_string(200) + "OK\\r\\n\r\nLength:";
	string strInt, strComplete;
	string strEnd, str1, str2, str3, str4, nameStr;
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

				str1 = "\nInode block: " + std::to_string(currentDir.dir_entries[i].block_num);
				str2 = "\nBytes in file: " + std::to_string(tempBlock.size);
			
				//Number of blocks
				int tempNumBlocks;
				tempNumBlocks = tempBlock.size / BLOCK_SIZE; // Count how many blocks are full
				if (tempBlock.size % BLOCK_SIZE > 0) //Check to see if any are partially full, if so increment var.
					tempNumBlocks++;
				tempNumBlocks++; // Account for inode being part of it.
				str3 = "\nNumber of blocks: " + to_string(tempNumBlocks);
			
				//First block
				short tempFirstBlock;
				if (tempBlock.size == 0)
					tempFirstBlock = 0;
				else
					tempFirstBlock = tempBlock.blocks[0];
			
				str4 = "\nFirst block: " + to_string(tempFirstBlock) + "\n";
				strEnd = str1 + str2 + str3 + str4;
				//Ending part
				
				strComplete = strBegin + to_string(strEnd.length()) + "\\r\\n\r\n\\r\\n\r\n" + strEnd;
				sendMsg(strComplete);
				return;
			}
			else 
			{
				//THE FOLLOWING IS DIRECTORY INFORMATION
				string str3 = currentDir.dir_entries[i].name;
				string str1 = "Directory name: " + str3 + "/\n";
				string str4 = to_string(currentDir.dir_entries[i].block_num);
				str2 = "Directory block: " + str4 + "\n";
				strEnd = str1 + str2;
				
				unsigned hnum = htonl(strEnd.length());
				
				strComplete = strBegin +  to_string(hnum) + "\\r\\n\r\n\\r\\n\r\n" + strEnd;
				sendMsg(strComplete);
				return;
			}

		}
	}
	sendMsg(to_string(500) + "File does not exist\\r\\n\r\nLength:0\\r\\n\r\n\\r\\n\r\n");
	return;
}


void FileSys::sendMsg(string msg){ 
	
	int bytes_sent(0);
	int pos(0);
	size_t size = msg.length();
	string temp;
	queue<string> packetQueue;
	int x(0);
	int count(0);
	//for cleaning up the message into proper packets
	int mod = (size % PACKET_MAX_SIZE);
	
	//if this isnt a multiple of packet size, we need to append 0's to the end
	
	int totalByteSent = 0;
	int totalPackets = size / PACKET_MAX_SIZE;
	
	
	//resize so the message is a multiple of packet size//
	if(mod){
		if(size < PACKET_MAX_SIZE){
			msg.resize(PACKET_MAX_SIZE, '0');
			packetQueue.push(msg);
		}
		else//bigger than a single packet
		{
			while(pos + PACKET_MAX_SIZE < size){
				temp = msg.substr(pos, (pos + PACKET_MAX_SIZE));
				packetQueue.push(temp);
				pos+=PACKET_MAX_SIZE;
			}
			temp = msg.substr(pos);
			temp.resize(PACKET_MAX_SIZE, '0');
			packetQueue.push(temp);
		}
	}
	
	while(!packetQueue.empty()){
		string package = packetQueue.front();
		packetQueue.pop();
		void* p = (void*)package.c_str();
		while (bytes_sent < PACKET_MAX_SIZE) {
			if((x = send(fs_sock, p , PACKET_MAX_SIZE,0))<0){
				cerr<<"Error writing to socket..\n";
				close(fs_sock);
				return;
			}
			p+=x;
			bytes_sent +=x;
		}
		totalByteSent += bytes_sent;
		bytes_sent = 0;

	}
	
	return;
}

