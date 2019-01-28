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
	
	int systemTime = 0;
	int processCounter = 0;
	while (processCounter != listLength) { //Can be errors here with length

		//Push list ordered by arrival time to the queue
		for (int i  = 0; i < listLength; i++) {
			if (processList[i].arrival_time == systemTime)
			orderQueue.push(processList[i]);
		}
	
		if (orderQueue.empty()) { 
			cout << "<system time " << systemTime << "> Idle... " << endl;
		} 

		else if (orderQueue.front().burst_time == 0) {
			cout << "<system time " << systemTime << "> process " << 
			orderQueue.front().pid << " is finished...." << endl;
			orderQueue.pop();
			processCounter++;
			//Go to next process since the previous is finished
			if (processCounter == listLength) {
				cout << "<system time " << systemTime << "> All processes finished" 
				<< "..............." << endl;
			} else {
				cout << "<system time " << systemTime << "> process " << 
				orderQueue.front().pid << " is running" << endl;
				orderQueue.front().burst_time--;
			}
		} 
	
		else {
			cout << "<system time " << systemTime << "> process " << 
			orderQueue.front().pid << " is running" << endl;
			orderQueue.front().burst_time--;
		}
		
		systemTime++;
	}


	/* Open this to see the arrival time order
	for (int i = 0; i < listLength; i++) {
		cout << endl << orderQueue.front().arrival_time;
		orderQueue.pop();
	}
	*/

	// Every second... Need to look into using <crono> or <ctime> for this.
	

	/*Ordering List
	while (!completedProcessList) {
	for (int i = 0; i < listLength; i++) {
		for (int j = 0; j < listLength - 1; j++) {
			if (processList[j].arrival_time > processList[j+1].arrival_time)
				swap(processList[j], processList[j+1]);
		}
	}
	*/


	//********************
	//SRTF
	//********************

 
	queue<processInfoStruct> priorityOrderQueue;
	
	//Order them and push
	
	systemTime = 0;
	processCounter = 0;
	int tempArrayIndex = 0;
	processInfoStruct tempArray[20];
	while (processCounter != listLength) { //Can be errors here with length

		//Push list ordered by arrival time to the queue
		for (int i  = 0; i < listLength; i++) {
			if (processList[i].arrival_time == systemTime)
			priorityOrderQueue.push(processList[i]);
			tempArrayIndex++;
		}

		for (int i = 0; i < tempArrayIndex; i++) //err possible
			tempArray[i] = priorityOrderQueue.pop();

		
	
		for (int i = 0; i < tempArrayIndex; i++) {
			for (int j = 0; j < tempArrayIndex - 1; j++) {
				if (processList[j].burst_time > processList[j+1].burst_time)
					swap(processList[j], processList[j+1]);
			}
		}

		for (int i = 0; i < tempArrayIndex; i++)
			priorityOrderQueue.push(tempArray[i]);

		if (priorityOrderQueue.empty()) { 
			cout << "<system time " << systemTime << "> Idle... " << endl;
		} 

		else if (priorityOrderQueue.front().burst_time == 0) {
			cout << "<system time " << systemTime << "> process " << 
			priorityOrderQueue.front().pid << " is finished...." << endl;
			priorityOrderQueue.pop();
			processCounter++;
			//Go to next process since the previous is finished
			if (processCounter == listLength) {
				cout << "<system time " << systemTime << "> All processes finished" 
				<< "..............." << endl;
			} else {
				cout << "<system time " << systemTime << "> process " << 
				priorityOrderQueue.front().pid << " is running" << endl;
				priorityOrderQueue.front().burst_time--;
			}
		} 
	
		else {
			cout << "<system time " << systemTime << "> process " << 
			priorityOrderQueue.front().pid << " is running" << endl;
			priorityOrderQueue.front().burst_time--;
		}
		
		systemTime++;
	}



	




}






