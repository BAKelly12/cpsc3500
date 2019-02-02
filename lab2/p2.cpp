/**
 * @file p2.cpp - Simulation of 3 CPU scheduling algorithms:
 *                  1.  First Come First Serve
 *                  2.  Shortest Time Remaining First
 *                  3.  Round Robin
 * @authors Brigid Kelly / Sam Van Nes
 * @see "Seattle University CPSC3500 Winter 2019"
 */


#include <fstream>
#include <string>
#include <queue>
#include <iostream>
#include <utility> 
#include <algorithm>
#include <map> 
#include <vector> 

using namespace std;

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
 * Function Declarations
 */
void fcfs(processInfoStruct processList[], int listLength);
void roundRobin(processInfoStruct processList[], int listLength, int quantum);
void srtf(processInfoStruct processList[], int listLength);

int main()
{
	string INPUT_FILE = "testData.txt";
	ifstream inFile(INPUT_FILE.c_str());
	
	processInfoStruct processList[20];
	
	int processIndex = 0;
	int listLength;
	int pidNum, arrival_t, burst_t;
	int quantum = 2;
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
		
	//fcfs(processList, listLength);	
	//srtf(processList, listLength);
	roundRobin(processList, listLength, quantum);
	return 0;

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
	/*	Check for new arrivals	*/
		for (int i  = 0; i < listLength; i++) 
		{
			if (processList[i].arrival_time == systemTime)
				rq[processList[i].pid] = processList[i];
		}
		
		if(rq.empty())
			cout<< "Idle..\n";
		else 
		{
			for(auto &it: rq)
			{
			/* Get lowest burst time */
				if(it.first == rq.begin()->first)
				{
					nextToRun = it;
				}
				else
				{				
					if(rq[it.first].burst_time < rq[nextToRun.first].burst_time)
						nextToRun = it;						
				}
			}
		}	
	/* If duplicate bt, fcfs */
		for(auto &it: rq)
		{
			if(rq[it.first].burst_time == rq[nextToRun.first].burst_time)
				tempq.push_back(it);
		}		
		if(tempq.size() > 1)
		{			
			for(auto &it: tempq)
			{
				if(rq[it.first].arrival_time < rq[nextToRun.first].arrival_time)
					nextToRun = it;	
			}
		}		
		tempq.clear();
				
	/* Run Process */
		cout<< "<system time "<< systemTime << ">";
		cout<< "Process " << rq[nextToRun.first].pid << " is running\n";
		rq[nextToRun.first].burst_time--;
		
		if(rq[nextToRun.first].burst_time == 0)
		{
			cout<< "Process " << rq[nextToRun.first].pid << " is finished...\n";	
			rq.erase(nextToRun.first);
			finished++;
		}		
		systemTime++;
	}		
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
		

/* take off front, when done running, place at end of queue */
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
		/* process  finished? */
			if(0 == curProc.burst_time)
			{
				cout<< "<System time " << systemTime << ">"<< " Process " << curProc.pid << " is finished..\n";
				finished++;
				break;
			}
		}
	/* if not finished, put back in queue */
		if(0 != curProc.burst_time)
			rrq.push_back(curProc);
		
		systemTime++;
	}
}


