/**
 * @file p2.cpp - Simulation of 3 CPU scheduling algorithms:
 *                  1.  First Come First Serve
 *                  2.  Shortest Time Remaining First
 *                  3.  Round Robin fix
 * @authors Brigid Kelly / Sam Van Nes
 * @see "Seattle University CPSC3500 Winter 2019"
 */

/**
 * TODO:
 *      1. STATS
 *      2. ARG PARSING/TAKING INPUT & ASSOCIATED LOGIC
 *      3. COMMENTS
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
	bool activated = false;
}; 


struct stats
{
	
	int idleTimes = 0;
 	int systemTimes;
	double avgTurn, avgWait, avgResp, cpUsage;
	stats(): avgTurn(0), avgWait(0), avgResp(0), cpUsage(0) {} 	
	vector<int> respTimes;
	vector<int> waitTimes;
	vector<int> turnTimes;

	double getAvgTurn()
	{
		size_t vsz = turnTimes.size();
		int sum(0);
		for(size_t i(0);i<vsz;i++)
			sum+=turnTimes[i];
		avgTurn = sum/vsz;
		return avgTurn;
	}
	
	double getAvgWait()
	{
		size_t vsz = waitTimes.size();
		int sum(0);
		for(size_t i(0);i<vsz;i++)
			sum+=waitTimes[i];
		avgWait = sum/vsz;
		return avgWait;
	}
	
	double getAvgResp()
	{
		size_t vsz = respTimes.size();
		int sum(0);
		for(size_t i(0);i<vsz;i++)
			sum+=respTimes[i];
		avgResp = sum/vsz;
		
		return avgResp;
	}

	double getCPUusage()
	{
		return (((systemTimes - idleTimes)/systemTimes) * 100);
	}
	
};//end of stat structure

stats stats;

/**
 *@brief comparison function for sort<processInfoStruct>
 *@param a:  lhs from sort comp output arg
 *		b:  rhs from sort comp output arg
 *@return lhs arrived earlier than rhs
 */
bool pl_comp(const processInfoStruct &a, const processInfoStruct &b)
{
	return a.arrival_time < b.arrival_time;
}

/** Function Declarations */
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

}//end of main signature

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
			if (processList[i].arrival_time == systemTime) {
			orderQueue.push(processList[i]);
			}
		}
	
		if (orderQueue.empty()) 
		{ 
			cout << "<system time " << systemTime << "> Idle... " << endl;
		} 

		else if (orderQueue.front().burst_time == 0) 
		{
			cout << "<system time " << systemTime << "> process " << 
			orderQueue.front().pid << " is finished...." << endl;
			stats.turnTimes.push_back(systemTime - orderQueue.front().arrival_time);
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
				//New process is on, push info
				stats.respTimes.push_back(systemTime - orderQueue.front().arrival_time);
				stats.waitTimes.push_back(systemTime - orderQueue.front().arrival_time);
			}
			cout << "<system time " << systemTime << "> process " << 
			orderQueue.front().pid << " is running" << endl;
			orderQueue.front().burst_time--;
		}
		else 
		{
			//Bad loop, but basically checks to see if this is process one and returns info.
			for (int i = 0; i < listLength; i++) 
				{
					if (processList[i].pid == orderQueue.front().pid) 
					{
						if (processList[i].burst_time == orderQueue.front().burst_time)
						{
							stats.respTimes.push_back(systemTime - orderQueue.front().arrival_time);
							stats.waitTimes.push_back(systemTime - orderQueue.front().arrival_time);
						}
					} 
				}	
			cout << "<system time " << systemTime << "> process " << 
			orderQueue.front().pid << " is running" << endl;
			orderQueue.front().burst_time--;
		}
		systemTime++;
	}
	cout << "CPU usage: " << stats.getCPUusage() << "\n";
	cout << "Average turnaround time: " << stats.getAvgTurn() <<"\n";
	cout << "Average wait time: " << stats.getAvgWait() <<"\n";
	cout << "Average response time: " << stats.getAvgResp() <<"\n";
}//end of fcfs signature

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
	cout << "CPU usage: " << stats.getCPUusage() << "\n";
	cout << "Average turnaround time: " << stats.getAvgTurn() <<"\n";
	cout << "Average wait time: " << stats.getAvgWait() <<"\n";
	cout << "Average response time: " << stats.getAvgResp() <<"\n";
}//end of func signature


void roundRobin(processInfoStruct ps[], int listLength, int quantum)
{
	
	sort(ps, ps+listLength, pl_comp);
	int finished(0), systemTime(0), waitCount(0);
	while(finished < listLength)
	{
		for(size_t i=0; i<(size_t)listLength;i++)
		{	
			//Go through and see how many active processes there are (for waitTime)
			for (size_t i = 0; i < (size_t)listLength; i++){
				if(ps[i].arrival_time <= systemTime && ps[i].burst_time>0){
					waitCount++;
				if (ps[i].activated == false) {
					ps[i].activated = true;
					stats.respTimes.push_back(systemTime);
				}
				}
			}

			//Check to see if element has been touched/altered yet. (for respTime)

			if(ps[i].arrival_time <= systemTime && ps[i].burst_time>0){
				
				
				for(int j = 0; j<quantum;j++)
				{
					cout<<"<System time "<<systemTime<<"> Process "<<ps[i].pid<<" is running.\n";
					ps[i].burst_time--;
					stats.waitTimes.push_back(waitCount - 1 - finished);
					systemTime++;
					if(0 == ps[i].burst_time)
					{
						finished++;
						ps[i].burst_time = -1;
						cout<<"<System time "<<systemTime<<"> Process "<<ps[i].pid<<" is finished....\n";
						stats.turnTimes.push_back(systemTime - ps[i].arrival_time);
						break;
					}
					
				}
			}
			else if(ps[i].arrival_time > systemTime)
			{
				cout<<"<System time "<<systemTime<<"> Idle..\n";
				stats.idleTimes++;
				systemTime++;
				break;
			}
			else{
				cout<<"<System time "<<systemTime<<"> Idle..\n";
				stats.idleTimes++;
				systemTime++;				
			}
		}		
	}
	stats.systemTimes = systemTime;	

	cout << "CPU usage: " << stats.getCPUusage() << "\n";
	cout << "Average turnaround time: " << stats.getAvgTurn() <<"\n";
	cout << "Average wait time: " << stats.getAvgWait() <<"\n";
	cout << "Average response time: " << stats.getAvgResp() <<"\n";
}

