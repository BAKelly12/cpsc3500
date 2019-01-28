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
#include <ctime>
#include <utility> //for swap()
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
	int listLength;
	int pidNum, arrival_t, burst_t;
	while (!inFile.eof()){
		inFile >> pidNum >> arrival_t >> burst_t;
		processList[processIndex].pid = pidNum;
		processList[processIndex].arrival_time = arrival_t;
		processList[processIndex].burst_time = burst_t;
		processIndex++;	
	}
	listLength = processIndex - 1;
	
	inFile.close();
	for (int i = 0; i < listLength; i++) {
		cout << "processnum: " << processList[i].pid << endl;
		cout << "arrival_t: " << processList[i].arrival_time << endl;
		cout << "burst_t: " << processList[i].burst_time << endl;
		cout << endl << endl;
	}

	//********************
	//FCFS
	//********************

	queue<processInfoStruct> orderQueue;
	
	//Order them and push
	for (int i = 0; i < listLength; i++) {
		for (int j = 0; j < listLength - 1; j++) {
			if (processList[j].arrival_time > processList[j+1].arrival_time)
				swap(processList[j], processList[j+1]);
		}
	}

	//Push list ordered by arrival time to the queue
	for (int i = 0; i <  listLength; i++)
		orderQueue.push(processList[i]);

	//When one second passed...
	cout << "<system time " << systemTime << "> process " << 
	orderQueue.front().pid << " is running" << endl;
	
	if (orderQueue.front().burst_time == 0) {
		orderQueue.pop();
		//cout finishing..
	}
	orderQueue.front().burst_time--;

	


	/* Open this to see the arrival time order
	for (int i = 0; i < listLength; i++) {
		cout << endl << orderQueue.front().arrival_time;
		orderQueue.pop();
	}
	*/

	// Every second... Need to look into using <crono> or <ctime> for this.
	








	

}
