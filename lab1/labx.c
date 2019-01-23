/**
 * @file lab1.cpp
 * @description this is the driver file for the myShell class.
 * @Authors Brigid Kelly, Sam VanNes
 * @see "Seattle University CPSC3500 Winter 2019"
 */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//char* parcer(string& input);
//void process_terminated(pid_t &pid);
//int lp(char** args);
struct command
{
	const char **argv;
};

int parseIntoCommands();

int main()
{

	//New program
	
	parseIntoCommands();

}

int fork_pipes (int n, struct command *cmd)
{
	int i;
	pid_t pid;
	int in, fd[2];

	in = 0;
	for (i = 0; i < n-1; i++) {
		pipe (fd);
		spawn_proc (in, fd[1], cmd + i);
		close (fd[1]);
		in = fd [0];
	}
	if (in != 0)
		dup2 (in, 0);
	return execvp( cmd[i].argv[0], (char* const*) cmd[i].argv);
}

int spawn_proc (int in, int out, struct command *cmd)
{
	pid_t pid;
	if ((pid = fork()) == 0) {
		if (in != 0) {
			dup2 (in, 0);
			close (in);
		} if (out != 1) {
			dup2 (out, 1);
			close (out);
		}
		return execvp( cmd->argv[0], (char* const *)cmd->argv);
	}
	return pid;	
}

int parseIntoCommands()
{
	
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
			stringIndex++; //This means its on a special char, so go past it.
	
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
			stringIndex++; //This means its on a special char, so go past it.
		
		} else if (getPlz[stringIndex] == '|') {
			numPipes++;
			buf[rowBuf][colBuf] = '|';
			buf[rowBuf][colBuf+1] = '\0';
			rowBuf++;;
			colBuf = 0;
			stringIndex++;

		} else if (getPlz[stringIndex] != ' ') { //Some character parce	
			while (getPlz[stringIndex] != ' ' && stringIndex < strLength) {
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
	
	//const char* 

	struct command cmd[] = {command[0], command[1]};
	
	return fork_pipes (numPipes + 1, cmd);
	
	//return 1;


}












