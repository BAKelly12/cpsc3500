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
#include <iostream>
#include <string>
#include <cctype>
#include <bits/stdc++.h> 
using namespace std;

struct command{
	char buf[50][100];

};

string* parcer(string& input);
void process_terminated(pid_t &pid);
int lp(char** args);


int main(int argc, char** argv)
{

  string input;			

  //Print the shell prompt, get line, split into tokens and convert 
  //to char** for input into execvp

  cout << "MyShell$ ";
  getline(cin,input); 
  string* tokens;
  tokens = parcer(input);
  int n = tokens->length()+1;
  char args[n];
  strcpy(args, tokens->c_str());
 
  //cout << "args[0]: " << args[0] << endl;
  //for(int i = 0; i<n; i++)
    //      cout << args[i];
 
  cout << endl;


  //this is a dirty way of doing this
  char** argi = new char*;
  *argi = args;
  cout << "Exit Status of exec: " << lp(argi)<<endl;
  free(argi);
}

/*************
 * FUNCTIONS *
 *************
 */


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
void process_terminated(pid_t& pid){
     
        //I think pipe should be closed here as well - Bri
        //
        //
        //
        kill(pid, SIGKILL);
        cout<< "Killed child.: "  << pid << endl;

}

string* parcer(string& input){
	//Parce Function  
        int token(0);
        static string tokenBank[50];
        unsigned int stringChecker(0);
        for (unsigned i = 0; i < input.length(); i++) {
                if (input[i] == '\'') { //Single quote parce
		                        stringChecker = i + 1; //So it won't break out of loop immediately
                       while (input[stringChecker] != '\'')
                                stringChecker++;
                       tokenBank[token] = input.substr(i + 1, stringChecker - (i+1));
                       token++;
                       i = stringChecker;
                }	
                else if (input[i] == '\"') { //Double quote  
                        stringChecker = i + 1;
                        while (input[stringChecker] != '\"')
                                stringChecker++;
                        tokenBank[token] = input.substr(i + 1, stringChecker - (i+1));
                        token++;
                        i = stringChecker;
                }
                else if (input[i] == '|') { //Pipe
                        tokenBank[token] = input[i];
                        token++;
                        cout << "spaghetti";
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
	
    return tokenBank;
}	