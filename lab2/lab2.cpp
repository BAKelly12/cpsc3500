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

//******************
//compile stuff
//*******************

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
	
	int RRNum = 3;
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

	//********************
	//SRTF
	//********************

 	//Order them and push
	/*	
	systemTime = 0;
	processCounter = 0;
	processInfoStruct = shortestTimeRemaining[20];
	//Order them and push
	while (processCounter != listLength) { //Can be errors here with length

		//Push list ordered by arrival time to the queue
		for (int i  = 0; i < listLength; i++) {
			if (processList[i].arrival_time == systemTime)
				shortestTimeRemaining[..] = processList[i];
		}
	
		************
		// make the priority queue here 
		************	
				

		if (orderQueue.empty()) { 
			cout << "<system time " << systemTime << "> Idle... " << endl;
		} 

		else if (shortestTimeRemaining[0].burst_time == 0) {
			cout << "<system time " << systemTime << "> process " << 
			shortestTimeRemaining[0] << " is finished...." << endl;
			shortestTimeRemaining[0].burst_time = -1;
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
	*/

	//********************
	//RR
	//********************	
	
	//Order them and push	
	systemTime = 0;
	processCounter = 0;
	processInfoStruct tempHolder;
	while (processCounter != listLength) { //Can be errors here with length

		//Push list ordered by arrival time to the queue
		for (int i  = 0; i < listLength; i++) {
			if (processList[i].arrival_time == systemTime)
			orderQueue.push(processList[i]);
		}

		//Push pid in front of the queue to the back	
		if (systemTime % RRNum == 0) {
			tempHolder.pid = orderQueue.front().pid;
			tempHolder.arrival_time = orderQueue.front().arrival_time;
			tempHolder.burst_time = orderQueue.front().burst_time;
			orderQueue.pop();
			orderQueue.push(tempHolder);
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

}






