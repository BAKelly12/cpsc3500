/**
 * @file lab1.cpp
 * @Authors Brigid Kelly, Sam VanNes
 * @see "Seattle University CPSC3500 Winter 2019"
 */


#include <iostream>
#include <bits/stdc++.h>
using namespace std;


int main(){

        //Print the shell prompt, separate the string into vectors separated by space
        cout << "MyShell$ ";
        string input;
        getline(cin,input);
        cout << input << endl;
        stringstream check1(input);
        vector <string> tokens;
        string intermediate;
        while(getline(check1, intermediate, ' ')){
                tokens.push_back(intermediate);
        } 
        for(unsigned int i = 0; i < tokens.size(); i++)
                cout << tokens[i] << endl;
}



