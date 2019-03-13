// CPSC 3500: Shell
// Implements a basic shell (command line interface) for the file system

/**Code adapted by Brigid Kelly and Sam VanNes 
 *  Framework and shell implementation courtesy of Dr Yingwu Zhu, Seattle University
 */

#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <queue>


// Shell
class Shell {

  public:
    //constructor, do not change it!!
    Shell() : cs_sock(-1), is_mounted(false) {
      
    }

    // Mount a network file system located in host:port, set is_mounted = true if success
    void mountNFS(string fs_loc);  //fs_loc must be in the format of server:port

    //unmount the mounted network file syste,
    void unmountNFS();

    // Executes the shell until the user quits.
    void run();

    // Execute a script.
    void run_script(char *file_name);

  private:

    int cs_sock; //socket to the network file system server

    bool is_mounted; //true if the network file system is mounted, false otherise
    
    /*Added this for sending data in a fixed structure*/
    struct Packet
    {
      string data;
      int size;
    };
    
    // data structure for command line
    struct Command
    {
      string name;		// name of command
      string file_name;		// name of file
      string append_data;	// append data (append only)
    };

    // Executes the command. Returns true for quit and false otherwise.
    bool execute_command(string command_str);

    // Parses a command line into a command struct. Returned name is blank
    // for invalid command lines.
    struct Command parse_command(string command_str);

    // Remote procedure call on mkdir 
    void mkdir_rpc(string dname);

    // Remote procedure call on cd
    void cd_rpc(string dname);

    // Remote procedure call on home
    void home_rpc();

    // Remote procedure call on rmdir
    void rmdir_rpc(string dname);

    // Remote procedure call on ls
    void ls_rpc();

    // Remote procedure call on create
    void create_rpc(string fname);

    // Remote procedure call on append
    void append_rpc(string fname, string data);
   
    // Remote procesure call on cat
    void cat_rpc(string fname);

    // Remote procedure call on head
    void head_rpc(string fname, int n);

    // Remote procedure call on rm
    void rm_rpc(string fname);

    // Remote procedure call on stat
    void stat_rpc(string fname); 
    
    private:
	
	string msgBuffer;//
	
	//waits for resposne from server and calls sockread
	void getResp();
	
	//reads input size from buffer, places in msgBuffer
	int sockread(size_t len);
	
	//handles outgoing messages by building packets of predefined length
	void sendMsg(string msg);
  
    
};

#endif




