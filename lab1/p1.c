 /**
 *@file p1.c
 *@description implemntation of a rudimentary command
 *               line shell
 *@Authors Sam Van Nes, Brigid Kelly
 *@see "Seattle University CPSC3500 Winter 2019
 */
  
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct command{
	const char **argv;
};

//Function declarations
void parseIntoCommandTable();
pid_t spawn_proc(int in, int out, struct command *cmd);
void fork_pipes (int n, struct command *cmd);


int main(){	
	    parseIntoCommandTable();
        return 1;
}

/**
 * @brief Called by main to take in command from command-line, parses 
 *          line into tokens, creates command array structure and calls
 *          fork_pipes function
 * @param N/A
 * @return N/A 
 */

void parseIntoCommandTable(){
	
	int max = 100; //User string length
	char getPlz[max]; //Actual string given by user
  
	//Getting the command/arguments from user
	printf("MyShell$ ");
	fgets(getPlz, max, stdin);

	//Parsing Section Below
	char buf[50][100];	
	int rowBuf = 0;
	int colBuf = 0;
	int numPipes = 0;
	int strLength = strlen(getPlz) - 1;
	int stringIndex = 0; //Counter for the entire input line
	const char* command[10][10];
	
	while (stringIndex < strLength) {
		if (getPlz[stringIndex] == '\'') { //Single quote parce	
  		stringIndex++;
			while (getPlz[stringIndex] != '\'') {
				buf[rowBuf][colBuf] = getPlz[stringIndex];
				colBuf++;
				stringIndex++;
			}
			//Add null to end of argument		
			buf[rowBuf][colBuf] = '\0';
			//Reset for next command
			colBuf = 0;
			rowBuf++;
			stringIndex++; //itr on Special char, so go past it.
	
		} else if (getPlz[stringIndex] == '\"') { //Double quote parce	
  		stringIndex++;
			while (getPlz[stringIndex] != '\"') {
				buf[rowBuf][colBuf] = getPlz[stringIndex];
				colBuf++;
				stringIndex++;
			}
			//Add null to end of argument		
			buf[rowBuf][colBuf] = '\0';
			//Reset for next command
			colBuf = 0;
			rowBuf++;
			stringIndex++; //itr on Special char, so go past it.
		
		} else if (getPlz[stringIndex] == '|') {
			numPipes++;
			buf[rowBuf][colBuf] = '|';
			buf[rowBuf][colBuf+1] = '\0';
			rowBuf++;;
			colBuf = 0;
			stringIndex++;

		} else if (getPlz[stringIndex] != ' ') { //Some character parse	
			while (getPlz[stringIndex] != ' ' && stringIndex < strLength && getPlz[stringIndex] != '|'){
				buf[rowBuf][colBuf] = getPlz[stringIndex];
				colBuf++;
				stringIndex++;
			}
			//Add null to end of argument		
			buf[rowBuf][colBuf] = '\0';
			//Reset for next command
			colBuf = 0;
			rowBuf++;

		} else
			stringIndex++;
	}
 
	char* args[50];
	int i;	
	for (i = 0; i < rowBuf; i++)
		args[i] = (char*)buf[i];
	args[rowBuf] = (char*)NULL;

	//Buffer to command list	
	i = 0;
	int commandNumber = 0;
	int counter = 0;
	for (i = 0; i < rowBuf; i++){
		if (buf[i][0] == '|'){
			command[commandNumber][counter] = NULL;
			commandNumber++;
			counter = 0;		
		} else {
			command[commandNumber][counter] = args[i];
			counter++;
		}
	}
	command[commandNumber][counter] = NULL;
	
	//Assuming max of 10 commands, make structure array that contains 
	//all possible commands. If less that the max, the forkPipes tells
	//how many 
	struct command cmd[] = {command[0], command[1], command[2], command[3],
							command[4], command[5], command[6], command[7],
							command[8], command[9]};
    
	 fork_pipes(numPipes + 1, cmd);
}

/**
 * @brief Creates pips for all processes being executed
 * @param n: Number of pipes
 *        cmd:  Pointer to command structure containing all 
 *              character arrays for input to execvp
 * @return N/A
 */

void fork_pipes (int n, struct command *cmd){
	int i;
	pid_t pid;
	int in, fd[2];
	in = 0;
	for (i = 0; i < n-1; i++) {
		pipe (fd);
		pid = spawn_proc (in, fd[1], cmd + i);
		close (fd[1]);
		in = fd [0];
	}
	if (in != 0)
		dup2 (in, 0);

     unsigned es = execvp(cmd[i].argv[0], (char* const*) cmd[i].argv);
     if(!es)
             perror("Failed to execute.");
} 


/**
 * @brief Creates child processes, links pipes, closes parent input file 
 *          descriptors and executes next command in child processes
 * @param in:   Input file descriptor
 *        out:  Output file descriptor
 * @return pid: process ID of child process (does not turn if
 *              process is child process).
 */

pid_t spawn_proc (int in, int out, struct command *cmd){
	pid_t pid;
    pid = fork(); 
	if (!pid) {//This is the child process
		if (in != 0) {
			dup2 (in, 0);
			close (in);
		} if (out != 1) {
			dup2 (out, 1);
			close (out);
		}
        execvp( cmd->argv[0], (char* const *)cmd->argv);
    }else if(pid<0)
            perror("Something went wrong 8).. ");

   return pid;	
}

