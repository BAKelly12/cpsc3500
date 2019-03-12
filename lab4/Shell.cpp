// CPSC 3500: Shell
// Implements a basic shell (command line interface) for the file system

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cerrno>
#define SERVER_PORT "11243" 

using namespace std;

#include "Shell.h"

#define PACKET_MAX_SIZE 32
static const string PROMPT_STRING = "NFS> ";	// shell prompt

// Mount the network file system with server name and port number in the format of server:port
void Shell::mountNFS(string fs_loc) {
  if(is_mounted){
    cerr<<"FS Already Mounted..\n\n";
    return;
  }
  

  
  /*INIT VARS*/
  struct addrinfo hints, *p, *servInfo;
  bzero((char*) &servInfo, sizeof(servInfo));
  bzero((char*) &hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

 
  /*END OF CLIENT VARIABLE INIT*/
  
  /*GETADDRINFO*/
  int rv(0);
  cerr<<"Getting address info..\n";
  /*Get Server address information using hints and place it into servInfo*/
  if((rv = getaddrinfo(fs_loc.c_str(), SERVER_PORT, &hints, &servInfo))<0){
    cerr<<"Error resolving hostname.. Exit code: "<<gai_strerror(rv)<<"\n\n";
    return;
  }
  cerr<<"Found server..\n\n";
  /*END OF GETADDRINFO*/
  
  
  /*DNS LOOKUP*/
  for(p=servInfo; p!=NULL;p=p->ai_next){
    if((cs_sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol))<0){
      cerr<<"Error creating socket..\n\n";
      continue;
    }   
    if(connect(cs_sock, p->ai_addr, p->ai_addrlen)<0){
      cerr<<"Error 418: I'm a teapot..\n\n";
      continue;
    }
    break;
  }
  if(p == NULL){
      cerr<<"client: Unable to connect to server..\n\n";
      return;
  }
  //else
  cerr<< "client: Connection Successful..\n\n";

  /*END OF DNS LOOKUP*/
  
  is_mounted = true;
  return;
}


// Unmount the network file system if it was mounted
void Shell::unmountNFS() {
	// close the socket if it was mounted
  if(is_mounted){
    if(close(cs_sock)){
      cerr<<"Error closing socket..\n\n";
      return;
    }
  }
  is_mounted = false;
}


// Remote procedure call on mkdir
void Shell::mkdir_rpc(string dname)  
{
  
  int bytes_sent(0);
  
  string mkdir = "mkdir " + dname + "\\r\\n\r\n";
  
  mkdir.resize(PACKET_MAX_SIZE,'0');
  cout<<"mkdir size: " << sizeof(mkdir)<<endl;
	if(cs_sock != -1) 
	{
      if(( bytes_sent=send(cs_sock , mkdir.c_str() , strlen( mkdir.c_str() ) , 0)) < 0)
      {
        cout << "Send failed : " << mkdir << endl;
        return;
      }
	  getResp();
	}
    else
		
        return;
 
 
  cerr<<"send succeeded\n\n Bytes sent: "<<bytes_sent<<"\n";
  

  
  
  return;
  
}
 

// Remote procedure call on cd
void Shell::cd_rpc(string dname) {
  
  int bytes_sent(0);
  
  string cd = "cd " + dname + "\\r\\n\r\n";
  
  cd.resize(PACKET_MAX_SIZE,'0');
  cout<<"cd size: " << sizeof(cd)<<endl;
	if(cs_sock != -1) 
	{
      if(( bytes_sent=send(cs_sock , cd.c_str() , strlen( cd.c_str() ) , 0)) < 0)
      {
        cout << "Send failed : " << cd << endl;
        return;
      }
	getResp();
  }
    else
      return;
 
 
  cout<<"send succeeded\n\n Bytes sent: "<<bytes_sent<<endl;
	return;
  
  
}

// Remote procedure call on home
void Shell::home_rpc() {
  
  int bytes_sent(0);
  
  string home = "home\\r\\n\r\n";
  
  home.resize(PACKET_MAX_SIZE,'0');
  cout<<"home size: " << sizeof(home)<<endl;
  
	if(cs_sock != -1) 
	{
      if(( bytes_sent=send(cs_sock , home.c_str() , strlen( home.c_str() ) , 0)) < 0)
      {
        cout << "Send failed : " << home << endl;
        return;
      }
	  getResp();

  }
    else
      return;
 
 
  cout<<"send succeeded\n\n Bytes sent: "<<bytes_sent<<endl;
	return;
  
  
}

// Remote procedure call on rmdir
void Shell::rmdir_rpc(string dname) {
  
  int bytes_sent(0);
  
  string rmdir = "rmdir " + dname + "\\r\\n\r\n";
  
  rmdir.resize(PACKET_MAX_SIZE,'0');
  cout<<"rmdir size: " << sizeof(rmdir)<<endl;
	if(cs_sock != -1) 
	{
      if(( bytes_sent=send(cs_sock , rmdir.c_str() , strlen( rmdir.c_str() ) , 0)) < 0)
      {
        cout << "Send failed : " << rmdir << endl;
        return;
      }
	getResp();
  }
    else
      return;
 
 
  cout<<"send succeeded\n\n Bytes sent: "<<bytes_sent<<endl;
	return;
  
  
}

// Remote procedure call on ls
void Shell::ls_rpc() {
    
  int bytes_sent(0);
  
  string ls = "ls\\r\\n\r\n";
  
  ls.resize(PACKET_MAX_SIZE,'0');
  cout<<"ls size: " << sizeof(ls)<<endl;
	if(cs_sock != -1) 
	{
      if(( bytes_sent=send(cs_sock , ls.c_str() , strlen( ls.c_str() ) , 0)) < 0)
      {
        cout << "Send failed : " << ls << endl;
        return;
      }
	getResp();
  }
    else
      return;
 
 
  cout<<"send succeeded\n\n Bytes sent: "<<bytes_sent<<endl;
	return;

}

// Remote procedure call on create
void Shell::create_rpc(string fname) {
  
    
  int bytes_sent(0);
  
  string create = "create " + fname + "\\r\\n\r\n";
  
  create.resize(PACKET_MAX_SIZE,'0');
  cout<<"create size: " << sizeof(create)<<endl;
	if(cs_sock != -1) 
	{
      if(( bytes_sent=send(cs_sock , create.c_str() , strlen( create.c_str() ) , 0)) < 0)
      {
        cout << "Send failed : " << create << endl;
        return;
      }
	getResp();
  }
    else
      return;
 
 
  cout<<"send succeeded\n\n Bytes sent: "<<bytes_sent<<endl;
	return;
  
  
}


/******************************************/
// Remote procedure call on append
void Shell::append_rpc(string fname, string data) {
 
  int bytes_sent(0);
  
  string append = "append " + fname + " " + data + "\\r\\n\r\n";
  
  append.resize(PACKET_MAX_SIZE,'0');
  cout<<"cd size: " << sizeof(append)<<endl;
	if(cs_sock != -1) 
	{
      if(( bytes_sent=send(cs_sock , append.c_str() , strlen( append.c_str() ) , 0)) < 0)
      {
        cout << "Send failed : " << append << endl;
        return; 
      }
	getResp();
  }
    else
      return;
 
 
  cout<<"send succeeded\n\n Bytes sent: "<<bytes_sent<<endl;
	return;
  //for this:
  //send append, then on server if append, it will get ready to accept a number of bytes
   //  somehow
  //then it will create something to take in that many bytes
  //then let the client know its ready to accept
  //then client begins send
  
}


/******************************************/



// Remote procesure call on cat
void Shell::cat_rpc(string fname) {
  
  int bytes_sent(0);
  
  string cat = "cat " + fname + "\\r\\n\r\n";
  
  cat.resize(PACKET_MAX_SIZE,'0');
  cout<<"cat size: " << sizeof(cat)<<endl;
	if(cs_sock != -1) 
	{
      if(( bytes_sent=send(cs_sock , cat.c_str() , strlen( cat.c_str() ) , 0)) < 0)
      {
        cout << "Send failed : " << cat << endl;
        return;
      }
	getResp();
  }
    else
      return;
 
 
  cout<<"send succeeded\n\n Bytes sent: "<<bytes_sent<<endl;
	return;
  
}

// Remote procedure call on head
void Shell::head_rpc(string fname, int n) {
  
   int nbo = htonl(n);
   string nbos = to_string(nbo);
   int bytes_sent(0);
  
  string head = "head " + fname + " " + nbos + "\\r\\n\r\n";
  
  head.resize(PACKET_MAX_SIZE,'0');
  cout<<"head size: " << sizeof(head)<<endl;
	if(cs_sock != -1) 
	{
      if(( bytes_sent=send(cs_sock , head.c_str() , strlen( head.c_str() ) , 0)) < 0)
      {
        cout << "Send failed : " << head << endl;
        return;
      }
	getResp();
  }
    else
      return;
 
 
  cout<<"send succeeded\n\n Bytes sent: "<<bytes_sent<<endl;
	return;
  
  //this still needs to be able to send the int
  
}

// Remote procedure call on rm
void Shell::rm_rpc(string fname) {
  int bytes_sent(0);
  
  string rm = "rm " + fname + "\\r\\n\r\n";
  
  rm.resize(PACKET_MAX_SIZE,'0');
  cout<<"rm size: " << sizeof(rm)<<endl;
	if(cs_sock != -1) 
	{
      if(( bytes_sent=send(cs_sock , rm.c_str() , strlen( rm.c_str() ) , 0)) < 0)
      {
        cerr << "Send failed : " << rm << endl;
        return;
      }
	getResp();
  }
    else
      return;
 
 
  cout<<"send succeeded\n\n Bytes sent: "<<bytes_sent<<endl;
	return;
  
}

// Remote procedure call on stat
void Shell::stat_rpc(string fname) {
    int bytes_sent(0);
  
  string stat = "stat " + fname + "\\r\\n\r\n";
  
  stat.resize(PACKET_MAX_SIZE,'0');
  cout<<"stat size: " << sizeof(stat)<<endl;
	if(cs_sock != -1) 
	{
      if(( bytes_sent=send(cs_sock , stat.c_str() , strlen( stat.c_str() ) , 0)) < 0)
      {
        cerr << "Send failed : " << stat << endl;
        return;
      }
	getResp();
  }
    else
      return;
 
 
  cout<<"send succeeded\n\n Bytes sent: "<<bytes_sent<<endl;
	return;
  
}

// Executes the shell until the user quits.
void Shell::run()
{
  // make sure that the file system is mounted
  if (!is_mounted)
 	return; 
  
  // continue until the user quits
  bool user_quit = false;
  while (!user_quit) {

    // print prompt and get command line
    string command_str;
    cin.clear();
    cout << PROMPT_STRING;
    getline(cin, command_str);
  

    // execute the command
    user_quit = execute_command(command_str);
  }

  // unmount the file system
  unmountNFS();
}

// Execute a script.
void Shell::run_script(char *file_name)
{
  // make sure that the file system is mounted
  if (!is_mounted)
  	return;
  // open script file
  ifstream infile;
  infile.open(file_name);
  if (infile.fail()) {
    cerr << "Could not open script file" << endl;
    return;
  }


  // execute each line in the script
  bool user_quit = false;
  string command_str;
  getline(infile, command_str, '\n');
  while (!infile.eof() && !user_quit) {
    cout << PROMPT_STRING << command_str << endl;
    user_quit = execute_command(command_str);
    getline(infile, command_str);
  }

  // clean up
  unmountNFS();
  infile.close();
}


// Executes the command. Returns true for quit and false otherwise.
bool Shell::execute_command(string command_str)
{
  // parse the command line
  struct Command command = parse_command(command_str);

  // look for the matching command
  if (command.name == "") {
    return false;
  }
  else if (command.name == "mkdir") {
    mkdir_rpc(command.file_name);
  }
  else if (command.name == "cd") {
    cd_rpc(command.file_name);
  }
  else if (command.name == "home") {
    home_rpc();
  }
  else if (command.name == "rmdir") {
    rmdir_rpc(command.file_name);
  }
  else if (command.name == "ls") {
    ls_rpc();
  }
  else if (command.name == "create") {
    create_rpc(command.file_name);
  }
  else if (command.name == "append") {
    append_rpc(command.file_name, command.append_data);
  }
  else if (command.name == "cat") {
    cat_rpc(command.file_name);
  }
  else if (command.name == "head") {
    errno = 0;
    unsigned long n = strtoul(command.append_data.c_str(), NULL, 0);
    if (0 == errno) {
      head_rpc(command.file_name, n);
    } else {
      cerr << "Invalid command line: " << command.append_data;
      cerr << " is not a valid number of bytes" << endl;
      return false;
    }
  }
  else if (command.name == "rm") {
    rm_rpc(command.file_name);
  }
  else if (command.name == "stat") {
    stat_rpc(command.file_name);
  }
  else if (command.name == "quit") {
    return true;
  }

  return false;
}

// Parses a command line into a command struct. Returned name is blank
// for invalid command lines.
Shell::Command Shell::parse_command(string command_str)
{
  // empty command struct returned for errors
  struct Command empty = {"", "", ""};

  // grab each of the tokens (if they exist)
  struct Command command;
  istringstream ss(command_str);
  int num_tokens = 0;
  if (ss >> command.name) {
    num_tokens++;
    if (ss >> command.file_name) {
      num_tokens++;
      if (ss >> command.append_data) {
        num_tokens++;
        string junk;
        if (ss >> junk) {
          num_tokens++;
        }
      }
    }
  }

  // Check for empty command line
  if (num_tokens == 0) {
    return empty;
  }
    
  // Check for invalid command lines
  if (command.name == "ls" ||
      command.name == "home" ||
      command.name == "quit")
  {
    if (num_tokens != 1) {
      cerr << "Invalid command line: " << command.name;
      cerr << " has improper number of arguments" << endl;
      return empty;
    }
  }
  else if (command.name == "mkdir" ||
      command.name == "cd"    ||
      command.name == "rmdir" ||
      command.name == "create"||
      command.name == "cat"   ||
      command.name == "rm"    ||
      command.name == "stat")
  {
    if (num_tokens != 2) {
      cerr << "Invalid command line: " << command.name;
      cerr << " has improper number of arguments" << endl;
      return empty;
    }
  }
  else if (command.name == "append" || command.name == "head")
  {
    if (num_tokens != 3) {
      cerr << "Invalid command line: " << command.name;
      cerr << " has improper number of arguments" << endl;
      return empty;
    }
  }
  else {
    cerr << "Invalid command line: " << command.name;
    cerr << " is not a command" << endl; 
    return empty;
  } 

  return command;
}



void Shell::getResp(){
	
	
  	size_t found;
	int fcount(0);
    int bytes_read(0);
	string thisMsg;
	int counter = 0;
	int offset=0;
	int counterString = 0;
	int curPos(0);

    string s2 = "\\r\\n";
	/*This is for getting the header file and length thereafter*/
    while( ( ( (found = thisMsg.find(s2,curPos) ) == std::string::npos) ) && fcount <3) {
		
        if(bytes_read > 4096){
            cerr<<"Incoming command exceeds max allowable message size\n";
            break;
        }
		
		size_t readSize = sockread(PACKET_MAX_SIZE);
			
		if(bytes_read==0)
			bytes_read = readSize;
		else
			bytes_read+= readSize;
		
        thisMsg = thisMsg + msgBuffer;
		if(found != std::string::npos && fcount < 3){
			fcount++;
			curPos = found+1;
			found = std::string::npos;
		}	
	}
  cerr<<"\n\nThis message: "<<thisMsg<<"\n"<<endl;
  cerr<<"is the stoul here??\n";

	//quick logic to escape function if the operation wasn't successful*/
	unsigned ezcheck = stoul( thisMsg.substr(0,3) );
	cerr<<"EZCHECK : "<<ezcheck<<endl;
	if(ezcheck!=200){
		//there was a problem
		int upper=thisMsg.find(s2);
		cerr<<thisMsg.substr(3,upper-3)<<endl;
		return;		
	}
	
	/*find the length of the message after header*/

	while (thisMsg[counter] != ':')
		counter++;
	counter++; //Go just after length
	if (thisMsg[counter] == 0)
		return;
	else
	{
		while (thisMsg[counter + offset] != '\\')
			offset++;    
	}
cerr<<"or the second one?";
	string command2 = thisMsg.substr(counter, offset);
	unsigned hnum = stoul(command2.c_str(), nullptr, 0);
	
	
	/*hnum is the length of the message from server after header*/
	/*it will also be counter for remaining characters to be read*/
	//hnum = ntohl(hnum);
	
	/*if hnum is zero, lets just get out of here*/
	if(!hnum){
		cerr<<"Operation successful\n\n";
		return;
	}
	
	
	/*This is the end of the header*/
	size_t headEnd = thisMsg.rfind(s2)+4;
	size_t readSize; 
	
	size_t bytes_remain = bytes_read + hnum - 1;
	

	while( bytes_read < bytes_remain){
		cerr<<"Number of bytes remiaining to read: " << bytes_remain << "\n";
		if((readSize=sockread(PACKET_MAX_SIZE))<0){
			cerr<<"Error reading message after header..\n\n";
			return;
		}
		bytes_read+=readSize;
		thisMsg = thisMsg + msgBuffer;
		/*if we've already read enough data to find the end, lets just get the end*/
		
	}
	cout<<thisMsg.substr(headEnd, hnum)<<"\n";
}
	


int Shell::sockread(size_t len)
{
    char buf[len];
    size_t count = len;
    char  *bufptr = (char*)buf;
    cerr<<"Reading from socket..\n";  
    ssize_t bytes_received(0);
    while(count > 0)
   {
	   cerr<<"\nShell:  sockread: In read loop\n";
       bytes_received = read(cs_sock, bufptr, count); 
	   cerr<<"Did we get anything? " << bytes_received<<endl;
       if (bytes_received <= 0) 
           return bytes_received;
        /* Decrement remaining size */  
        count -= bytes_received;  
        /*increment buffer pointer*/
        bufptr += bytes_received;  
        ///*tabulate total receive size*/
       // len+=bytes_received;
    }
    msgBuffer = buf;
	cerr<<msgBuffer;
    cerr<< endl<<"Bytes received: "<<bytes_received<<"\n";
    return len;
}

void Shell::printMessage(string msg)
{
	
	int counter = 0;
	int offset = 0;
	string command1,command2;
	cerr<<"In print\n\n";
	command1= msg.substr(0, 3);
	unsigned hnum = stoul(command1.c_str(), nullptr, 0);
	hnum = ntohl(hnum);
	//Error message
	if (hnum != 200)
	{
		cout << msg << "\n";
		return;
	}
	
	while (msg[counter] != ':')
		counter++;
	counter++; //Go just after length
	if (msg[counter] == 0)
		return;
	else
	{
		while (counter + offset != '\\')
		offset++;    
	}
	command2 = msg.substr(counter, offset);
	hnum = stoul(command2.c_str(), nullptr, 0);
	hnum = ntohl(hnum);
	
	
	//From the length of string minus the length of message to the end.
}


  

void Shell::sendMsg(string msg){ 
	
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
			if((x = send(cs_sock, p , PACKET_MAX_SIZE,0))<0){
				cerr<<"Error writing to socket..\n";
				close(cs_sock);
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


