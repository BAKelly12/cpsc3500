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

char** parcer(void);
void process_terminated(pid_t &pid);
int lp(char** args);
char** startochar(string str[]);

int main(int argc, char** argv)
{

   //Print the shell prompt, get line, split into tokens and convert 
  //to char** for input into execvp

  cout << "MyShell$ ";
  
  char** input = parcer();

  //cout << "args[0]: " << args[0] << endl;
  //for(int i = 0; i<n; i++)
    //      cout << args[i];
 
  cout <<"in main"<< endl;


  //this is a dirty way of doing this
  //char** argi = new char*;
  //*argi = args;
  //cout << "Tokens: " << *tokens << endl;
  //cout << "result of strcpy: " << args << endl;
  //cout <<"argin: " <<  argi<<endl;
  cout << "Exit Status of exec: " << lp(input)<<endl;
  //free(argi);
}

/*************
 * FUNCTIONS *
 *************
 */


int lp(char** args){
  cout << "in lp" << endl;

  //for(unsigned int i = 0; i<sizeof(args)-1;i++)
          cout << *args;
  cout<<endl;

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

char** parcer(void){
	//Parse Function
        string input;
        getline(cin,input); 
        cout << endl;
        int token(0);
        static string tokenBank[10];
        unsigned int stringChecker(0);
    for (unsigned i = 0; i < input.length(); i++) {   
            if (input[i] == '\'') { //Single quote parce            
                    stringChecker = i + 1; //So it won't break out of loop immediately  
                    while (input[stringChecker] != '\'')      
                            stringChecker++;                    
                    tokenBank[token] = input.substr(i + 1, stringChecker - (i+1));
                    token++;                                                                    
                    i = stringChecker;                
            }else if (input[i] == '\"') { //Double quote parce
                    stringChecker = i + 1;                                        
                    while (input[stringChecker] != '\"')                            
                            stringChecker++;                    

                    tokenBank[token] = input.substr(i + 1, stringChecker - (i+1));                   
                    token++;                                                            
                    i = stringChecker;
                
            }else if (input[i] == '|') { //Pipe             
                    tokenBank[token] = input[i];                    
                    token++;                                    
                    cout << "spaghetti";
            
            }else if (input[i] != ' ') { //Non space characters           
                    stringChecker = i;                    
                    while (input[stringChecker] != '|' && input[stringChecker] != ' ' && stringChecker < input.length() )                            
                            stringChecker++;                                            
                    tokenBank[token] = input.substr(i, stringChecker - i);  
                    token++;
                    i = stringChecker - 1; //Push index back due to possible pipe           
            }
    }


    
    cout << endl << endl << endl << "Now the bank:"; 
    cout << "Token?: " << token<< endl<<endl;
/**
    for (int i = 0; i < token; i++) {                  
            cout << endl << tokenBank[i];
            args[i] = tokenBank[i]; 
    }
*/


    cout << "In parser: " << endl <<"Input: " <<input << endl;
    cout << "tokenBank: "<< *tokenBank << endl<<endl;

    char** args = startochar(tokenBank);
//    int n = tokenBank.length()+1;

 
    return args;


}

//@brief Takes tokenized string array and converts to char** array
char** startochar(string str[]){

        
    char** args[str->length()+1];
    //int pos=0;
    string tmp;
    cout << "In startochar: " << endl;

//    int pos;
  //  char space = ' ';
    for(unsigned i = 0; i <str->length();i++) {
            cout << str[i];
            tmp = str[i];
            for(unsigned j = 0;j<tmp.length();j++){
                  //  **args[pos]=tmp[j];
                  //  pos++;
                    cout<<tmp[j]<<endl;
            }
//            **args[pos]=space;
  //          pos++;
               
        }
        
    return *args;

}


