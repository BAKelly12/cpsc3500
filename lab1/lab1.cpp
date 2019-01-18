/**
 * @file lab1.cpp
 * @Authors Brigid Kelly, Sam VanNes
 * @see "Seattle University CPSC3500 Winter 2019"
 */


#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <cctype>
using namespace std;

struct command{
	char buf[50][100];
};

//char* parceFunction(string input)

int main()
{
	string input;			
	string tokenBank[10];
	int token = 0;
	unsigned stringChecker;
	//Print the shell prompt, separate the string into vectors separated by space
  cout << "MyShell$ ";
	getline(cin,input);
  cout << input << endl;
	
	//Parce Function  
	for (unsigned i = 0; i < input.length(); i++) {
 
		if (input[i] == '\'') { //Single quote parce
			stringChecker = i + 1; //So it won't break out of loop immediately
			while (input[stringChecker] != '\'')
				stringChecker++;
			tokenBank[token] = input.substr(i + 1, stringChecker - (i+1));
			token++;
			i = stringChecker;
		}
	
		else if (input[i] == '\"') { //Double quote parce
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
			while (input[stringChecker] != '|' && input[stringChecker] != ' ' &&
			stringChecker < input.length() )
				stringChecker++;
			tokenBank[token] = input.substr(i, stringChecker - i);
			token++;
			i = stringChecker - 1; //Push index back due to possible pipe
		}
	}

		
	cout << endl << endl << endl << "Now the bank:";
	cout << token;
	for (int i = 0; i < token; i++) {
		cout << endl << tokenBank[i];	
	}
	
	cout << endl << endl;

	/*	
	stringstream check1(input);
  vector <string> tokens;
 	string intermediate;
  
	while (getline(check1, intermediate, '\"')
		tokens.push_back(intermediate);

	while(getline(check1, intermediate, ' '))
  	tokens.push_back(intermediate);

  for(unsigned int i = 0; i < tokens.size(); i++)
		cout << tokens[i] << endl;
	*/	

}



