/**
*@file: lab2.cpp - Cpu scheduling algos with comments (rough draft)
*@authors:  Brigid Kelly/Sam Van Nes
*@see "Seattle University CPSC3500 Winter 2019"
*/


#include <fstream>
#include <string>
#include <queue>
#include <iostream>
#include <cstring>
#include <utility> //for swap()
#include <algorithm>
#include <map> // creating the pcb
#include <vector> //for srtf


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

bool pl_comp(const processInfoStruct &a, const processInfoStruct &b)
{
	return a.arrival_time < b.arrival_time;
}


/**
 *Function Declarations
 */
void fcfs(processInfoStruct processList[], int listLength);
void roundRobin(processInfoStruct processList[], int listLength, int quantum);
void srtf(processInfoStruct processList[], int listLength);

int main(int argc, char** argv)
{
	string INPUT_FILE = "testData.txt";
	//string INPUT_FILE = (char*)argv[0];
	ifstream inFile(INPUT_FILE.c_str());
	
	processInfoStruct processList[20];
	
	int processIndex = 0;
	int listLength;
	int pidNum, arrival_t, burst_t;
	//int quantum = 2;
	while (!inFile.eof())
	{
		inFile >> pidNum >> arrival_t >> burst_t;
		processList[processIndex].pid = pidNum;
		processList[processIndex].arrival_time = arrival_t;
		processList[processIndex].burst_time = burst_t;
		processIndex++;	
	}
	listLength = processIndex - 1;
	
	inFile.close();
	for (int i = 0; i < listLength; i++) 
	{
		cout << "processnum: " << processList[i].pid << endl;
		cout << "arrival_t: " << processList[i].arrival_time << endl;
		cout << "burst_t: " << processList[i].burst_time << endl;
		cout << endl << endl;
	}

	//put logic here for choosing which algorithm to use
		//and then all we have to do is call function with arg process list
		
	
	cout << "type: " << argv[2] << endl << endl;
	//fcfs(processList, listLength);

	
	if (argc == 3) {
		if (strcmp(argv[2], "FCFS"))
			fcfs(processList, listLength);	
		else if (strcmp(argv[2], "SRTF"))
			srtf(processList, listLength);
		else {
			cout << "Invalid command." << endl;
		}
	} else if (argc == 4) {
			roundRobin(processList, listLength, atoi(argv[3]));
	} else {
		cout << endl << "You didn't enter a proper scheduling algorithm." << endl;
	}
	

	return 1;
}

void fcfs(processInfoStruct processList[], int listLength)
{
	
	queue<processInfoStruct> orderQueue;
	
	//Order them and push
	
	int systemTime = 0;
	int processCounter = 0;
	while (processCounter != listLength) 
	{ //Can be errors here with length
		//Push list ordered by arrival time to the queue
		for (int i  = 0; i < listLength; i++) 
		{
			if (processList[i].arrival_time == systemTime)
			orderQueue.push(processList[i]);
		}	
		if (orderQueue.empty()) 
		{ 
			cout << "<system time " << systemTime << "> Idle... " << endl;
		} 
		else if (orderQueue.front().burst_time == 0) 
		{
			cout << "<system time " << systemTime << "> process " << 
			orderQueue.front().pid << " is finished...." << endl;
			orderQueue.pop();
			processCounter++;
			//Go to next process since the previous is finished
			if (processCounter == listLength) 
			{
				cout << "<system time " << systemTime << "> All processes finished" 
				<< "..............." << endl;
			} 
			else 
			{
				cout << "<system time " << systemTime << "> process " << 
				orderQueue.front().pid << " is running" << endl;
				orderQueue.front().burst_time--;
			}
		} 	
		else 
		{
			cout << "<system time " << systemTime << "> process " << 
			orderQueue.front().pid << " is running" << endl;
			orderQueue.front().burst_time--;
		}
		systemTime++;
	}
}

void srtf(processInfoStruct processList[], int listLength)
{	
	int systemTime = 0;
	map<int, processInfoStruct> rq;
	int finished = 0;
	vector<pair<int,processInfoStruct>> tempq;
	pair<int, processInfoStruct> nextToRun;
		
	while(finished < listLength)
	{
		//find arrival times that match system time and place in queue
		for (int i  = 0; i < listLength; i++) 
		{
			if (processList[i].arrival_time == systemTime)
				rq[processList[i].pid] = processList[i];
		}
		
		//print idle if readyqueue is empty
		if(rq.empty())
			cout<< "Idle..\n";
		else //rq isnt empty, find shortest time
		{
			for(auto &it: rq)
			{
				//check case if we are at start of q
				if(it.first == rq.begin()->first)//this is checking pid's
				{
					nextToRun = it;
				}
				else // check rest against nextToRun and if lower burst times replace nextToRun
				{				
					if(rq[it.first].burst_time < rq[nextToRun.first].burst_time)
						nextToRun = it;						
				}
			}
		}//minimum burst time has been found		
		//now need to check for duplicate burst times and then compare arrival times
		for(auto &it: rq)
		{
			if(rq[it.first].burst_time == rq[nextToRun.first].burst_time)
				tempq.push_back(it);
		}		
		//all common burst times have been placed in tempq
		if(tempq.size() > 1)//do this only if we actually have multiple common burst times
		{
			//FIRST COME FIRST SERVE
			for(auto &it: tempq)
			{
				if(rq[it.first].arrival_time < rq[nextToRun.first].arrival_time)
					nextToRun = it;
				
			}
		}//we now have the first to arrive with the shortest burst time
		
		tempq.clear();//memory management
		
		//LETS RUN THE PROCESS 
		cout<< "<system time "<< systemTime << ">";
		cout<< "Process " << rq[nextToRun.first].pid << " is running\n";
		rq[nextToRun.first].burst_time--;
		
		if(rq[nextToRun.first].burst_time == 0)
		{
			cout<< "Process " << rq[nextToRun.first].pid << " is finished...\n";	
			rq.erase(nextToRun.first);
			finished++;
		}		
		//I love object oriented programming 8)
		systemTime++;
	}//end of while loop		
}//end of func signature


void roundRobin(processInfoStruct ps[], int listLength, int quantum)
{
	vector<processInfoStruct> rrq;
	int systemTime(0), finished(0);
	processInfoStruct curProc;
	
	for(int i = 0; i<listLength; i++)
	{
		rrq.push_back(ps[i]);
	}
	sort(rrq.begin(), rrq.end(), pl_comp);
		
	//processes now sorted by arrival time
	
	while(finished < listLength)
	{
		curProc = *rrq.begin();
		rrq.erase(rrq.begin() );
		if(rrq.empty() && finished != listLength -1)
			cout<< "<System time " << systemTime << ">" << " Idle..\n";
		
		for(int i = 0; i < quantum; i++)
		{
			cout<< "<System time " << systemTime << ">" << " Process " << curProc.pid << " is running.\n";
			curProc.burst_time--;
			if(0 == curProc.burst_time)
			{
				cout<< "<System time " << systemTime << ">"<< " Process " << curProc.pid << " is finished..\n";
				finished++;
				break;
			}
		}
		
		if(0 != curProc.burst_time)
			rrq.push_back(curProc);
		
		systemTime++;
	}
}
