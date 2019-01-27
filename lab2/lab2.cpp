//
//
//
//
//
//
//
//
//
//
#include <fstream>
#include <string>
#include <queue>
#include <iostream>
using namespace std;

struct processInfoStruct
{
	int pid;
	int arrival_time;
	int burst_time;
};

int main()
{
	const string INPUT_FILE = "testData.txt";
	ifstream inFile(INPUT_FILE);
	
	processInfoStruct processList[20];
	
	int processIndex = 0;
	int pidNum, arrival_t, burst_t;
	while (!inFile.eof()){
		inFile >> pidNum >> arrival_t >> burst_t;
		processList[processIndex].pid = pidNum;
		processList[processIndex].arrival_time = arrival_t;
		processList[processIndex].burst_time = burst_t;
		processIndex++;	
	}
	
	inFile.close();
	for (int i = 0; i < processIndex-1; i++) {
		cout << "processnum: " << processList[i].pid << endl;
		cout << "arrival_t: " << processList[i].arrival_time << endl;
		cout << "burst_t: " << processList[i].burst_time << endl;
		cout << endl << endl;
	}

	

}
