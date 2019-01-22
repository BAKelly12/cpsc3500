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


int main()
{


	///////////////////////////////////////////////////////////////////
	char buf[50][100];


  printf("MyShell$ ");
  
	//Getting the command/arguments from user
	int max = 100;
	char getPlz[max];
	fgets(getPlz, max, stdin);
	printf("string is: %s\n", getPlz);	


	//////////////////////////////
	//Parce here
	//////////////////////////////	

	
	int rowBuf = 0;
	int colBuf = 0;


	int strLength = strlen(getPlz) - 1;	
	printf ("length of stirng: %d \n\n", strLength);
	int stringIndex = 0; //Counter for the entire input line
	int i = 0; //Used to count within each argument

	while (stringIndex <= strLength ) {
		if (getPlz[stringIndex] == '\'') { //Single quote parce	
  		stringIndex++;
			while (getPlz[stringIndex] != '\'') {
				buf[rowBuf][colBuf] = getPlz[stringIndex];
				colBuf++;
				stringIndex++;
			}
		
			//Reset for next command

			colBuf = 0;
			rowBuf++;
			stringIndex++; //This means its on a special char, so go past it.
		}	
    
		/*            
		if (getPlz[stringIndex] == '\"') { //Single quote parce
  		i++;
			while (getPlz[i] != '\"') {
				buf[rowBuf][colBuf] = getPlz[i];
				colBuf++;
				i++;
			}
			
			//Reset for next command
			colBuf = 0;
			stringIndex = stringIndex + i + 1;
			rowBuf++;
			i = 0;
		}
		*/
	
		else {
			stringIndex++;
		}
	}
	
	////////////////////////////////////////////////////////////////
	
	
	char* args[50];

	i = 0;
	for (i = 0; i < rowBuf; i++)
		args[i] = (char*)buf[i];
	args[rowBuf] = (char*)NULL;
		
	execvp(args[0], args);	
	
}

/*************
 * FUNCTIONS *
 *************
 */

/*
int lp(char** args){

	
	pid_t cp,w;
  cp = fork();
  if(!cp){                  
          //This is the child process
          if(execvp(args[0], args)==-1)
                  perror("error executing");
  }else if(cp<0){
          perror("There was an error with fork");
  }else{
          //This is the parent process, need to wait for child to finish
          //this code is adapted from Dr. Zhu's example in the additional
          //reading section of canvas.
          int child_status;
          do{
                  w = wait(&child_status);
                  if(w != cp) process_terminated(w);
          }while(w != cp);
  }
  return 1;

}
*/

/*
void process_terminated(pid_t& pid){
     
        //I think pipe should be closed here as well - Bri
        //
        //
        //
        kill(pid, SIGKILL);
        cout<< "Killed child.: "  << pid << endl;

}
*/

/*
char* parcer(string& input){
	//Parce Function  
        int token(0);
        static string tokenBank[50];
        unsigned int stringChecker(0);
        for (unsigned i = 0; i < input.length(); i++) {
                else if (input[i] == '|') { //Pipe
                        tokenBank[token] = input[i];
                        token++;
                }
                else if (input[i] != ' ') { //Non space characters
                        stringChecker = i;
                        while (input[stringChecker] != '|' && input[stringChecker] != ' ' &&  stringChecker < input.length() )
                                stringChecker++;
                        tokenBank[token] = input.substr(i, stringChecker - i);
                        token++;
                        i = stringChecker - 1; //Push index back due to possible pipe
                }
        }
	
				char charArray[50];
				int charArrayIndex = 0;
				for (int i = 0; i < token; i++) {
					for (unsigned j = 0; j < tokenBank[i].length(); j++) {
						charArray[charArrayIndex] = tokenBank[i][j];
						charArrayIndex++;
					}
					charArray[charArrayIndex] = ' ';
					charArrayIndex++;
				}
				charArray[charArrayIndex-1] = NULL;				
	
				//Troubleshooting char array
				for (int i = 0; i < charArrayIndex-1; i++)
					cout << charArray[i] << endl;
	
    return charArray;
}
*/	
