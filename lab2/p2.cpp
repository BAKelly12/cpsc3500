/**
 * @file p2.cpp - Simulation of 3 CPU scheduling algorithms:
 *                  1.  First Come First Serve
 *                  2.  Shortest Time Remaining First
 *                  3.  Round Robin fix
 * @authors Brigid Kelly / Sam Van Nes
 * @see "Seattle University CPSC3500 Winter 2019"
 */

#include <fstream>
#include <string.h>
#include <queue>     //FCFS Handling
#include <iostream>
#include <utility>   //for swap()
#include <map>      //for pcb management (SRTF/RR)
#include <vector>   //for queue manipulation
#include <iomanip>  //For precision output
//#include <algorithm> //for command line parse

using namespace std;

static const char percent = '%';

/* PCB */
struct processInfoStruct
{	
	int pid=0;
	int arrival_time=0;
	int burst_time=0;
	int respT=0;
	int waitT=0;
	int cpuT=0;
	int turnT=0;
	bool running = false;
	bool activated = false;
	bool done = false;	
}; 


/*
 *@struct stats - Container for placing all gathered statistical 
 *				in functions to perform calculations.
 */				
struct stats
{
	
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
		avgTurn = (double)sum/(double)vsz;
		return avgTurn;
	}
	
	double getAvgWait()
	{
		size_t vsz = waitTimes.size();
		int sum(0);
		for(size_t i(0);i<vsz;i++)
			sum+=waitTimes[i];
		avgWait = (double)sum/(double)vsz;
		return avgWait;
	}
	
	double getAvgResp()
	{
		size_t vsz = respTimes.size();
		int sum(0);
		for(size_t i(0);i<vsz;i++)
			sum+=respTimes[i];
		avgResp = (double)sum/(double)vsz;
		return avgResp;
	}	
	
	double getCpUtilzation(int sysTime)
	{
		double out = (double)cpUsage / (double)sysTime;
		out = out*100;
		return out;
	}
	
};//end of stat structure
stats stats;  //global object instantiation.

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

/** Scheduler Algorithm Function Declarations */
void fcfs(processInfoStruct processList[], int listLength);
void roundRobin(processInfoStruct processList[], int listLength, int quantum);
void srtf(processInfoStruct processList[], int listLength);


/************************************
*             MAIN BODY             *
*************************************/
int main(int argc, char** argv)
{	
	int processIndex = 0;
	int listLength;
	int pidNum, arrival_t, burst_t;
	processInfoStruct processList[20];
	
	//Get the file from the command line
	string INPUT_FILE(argv[1]);
	ifstream inFile(INPUT_FILE);

	//Load into array
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
	
	//Check to see which algorithm was selected	
	if (argc == 3) {
		if ((strcmp(argv[2], "FCFS")) == 0) {
			cout << "\n" << "Scheduling algorithm: FCFS \n";
			cout << "Total of " << listLength << " tasks are read from input file."
			<< " Press 'enter' to start. \n";
			cin.get();
			fcfs(processList, listLength);	
		} else if ((strcmp(argv[2], "SRTF") == 0)) {
			cout << "\n" << "Scheduling algorithm: SRTF \n";
			cout << "Total of " << listLength << " tasks are read from input file."
			<< " Press 'enter' to start. \n";
			cin.get();
			srtf(processList, listLength);
		} else {
			cout << "Invalid command." << endl;
		}
	} else if (argc == 4) {
			cout << "\n" << "Scheduling algorithm: RR \n";
			cout << "Total of " << listLength << " tasks are read from input file."
			<< " You selected a quantum time of " << atoi(argv[3]) << 
			". Press 'enter' to start. \n";
			cin.get();
			roundRobin(processList, listLength, atoi(argv[3]));
	} else {
		cout << endl << "You didn't enter a proper scheduling algorithm." << endl;
	}
}

/*************************************
 *       SCHEDULING FUNCTIONS        *
 ************************************/
 
 /******************************
  *   FIRST COME FIRST SERVE	*
  ******************************/
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
				break;	
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
	stats.cpUsage = systemTime;
	printf("\nFirst Come First Serve Results: \n");
	printf("\nTotal CPU utilization:  %5.2f%c.\n", stats.getCpUtilzation(systemTime),percent);
	cout << "Average turnaround time: " << stats.getAvgTurn() <<"\n";
	cout << "Average wait time: " << stats.getAvgWait() <<"\n";
	cout << "Average response time: " << stats.getAvgResp() <<"\n\n\n\n";
}//end of fcfs signature

 /*************************************
  *   SHORTEST TIME REMAINING FIRST	  *
  *************************************/
void srtf(processInfoStruct ps[], int listLength)
{	
	int systemTime = 0;
	map<int, processInfoStruct> rq;
	int finished = 0;
	vector<pair<int,processInfoStruct>> tempq;
	pair<int, processInfoStruct> nextToRun;
	bool doNotIdle = false;
	while(finished < listLength)
	{	

	/*	Check for new arrivals	*/
		for (int i  = 0; i < listLength; i++) 
		{
			if (ps[i].arrival_time == systemTime)
			{
				rq[ps[i].pid] = ps[i];
				rq[ps[i].pid].activated = true;
			}
		}
		
	/* Check if any active processes, change flag accordingly */
		for(auto &it: rq)
		{
			if(rq[it.first].activated && !rq[it.first].done)
			{
				doNotIdle = true;
				break;
			}
		}
		
	/* Idle output for empty queue */
		if(!doNotIdle)
			cout<< "Idle..\n";
		else 
		{
	/* Find lowest burst time */	
			for(auto &it: rq)
			{	
				if(it.first == rq.begin()->first)
				{
					nextToRun = it;
				}
				else
				{	
					int irburst = rq[it.first].burst_time - rq[it.first].cpuT;
					int nrburst = rq[nextToRun.first].burst_time - rq[nextToRun.first].cpuT;
					if(irburst < nrburst && !rq[it.first].done)
						nextToRun = it;						
				}
			}
		}	
		int nrburst = rq[nextToRun.first].burst_time - rq[nextToRun.first].cpuT;	
	
	/* If duplicate bt, fcfs */
		for(auto &it: rq)
		{
			int irburst = rq[it.first].burst_time - rq[it.first].cpuT;
			if(irburst == nrburst)
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
		rq[nextToRun.first].running = true;
		
	/* If this is first time running, record response time */
		if(!rq[nextToRun.first].cpuT)
		{
			rq[nextToRun.first].respT = systemTime - rq[nextToRun.first].arrival_time;
		}
		
		/* increment cputime */	
		rq[nextToRun.first].cpuT++;
		
		/* Tell others to wait */
		for(auto &itr: rq)
		{
			if(rq[itr.first].activated && !rq[itr.first].done && !rq[itr.first].running)
				rq[itr.first].waitT++;
		}
	
	/* Check for job completion */
		if(rq[nextToRun.first].cpuT == rq[nextToRun.first].burst_time)
		{
			cout<<"<system time " << systemTime << "> Process " << rq[nextToRun.first].pid;
			cout << " is finished...\n";	
			rq[nextToRun.first].done = true;
			rq[nextToRun.first].turnT = systemTime - rq[nextToRun.first].arrival_time;
			finished++;
		}
		rq[nextToRun.first].running = false;
		systemTime++;
	}	

/* Stat Collection */
	for(auto &it: rq)
	{
		stats.waitTimes.push_back(rq[it.first].waitT);
		stats.respTimes.push_back(rq[it.first].respT);
		stats.turnTimes.push_back(rq[it.first].turnT);
		stats.cpUsage+=rq[it.first].cpuT;
	}
	
	printf("\nShortest Remaining Time First Results: \n");
	printf("\nTotal CPU utilization:  %5.2f%c.\n", stats.getCpUtilzation(systemTime),percent);
	printf("Average wait time: %6.2f.\n",  stats.getAvgWait());
	printf("Average response time: %5.2f.\n", stats.getAvgResp());
	printf("Average turnaround time: %6.2f.\n\n\n\n",stats.getAvgTurn());
	
}//end of func signature

 /******************************
  *       ROUND ROBIN	      *
  ******************************/
void roundRobin(processInfoStruct ps[], int listLength, int quantum)
{
	int sysTime(0);
	int finished(0);
	bool doNotIdle = false;
	
	map<int, processInfoStruct> rrq;
	
/* Populate dictionary */
	for(int i = 0; i<listLength;i++)
	{
		rrq[ps[i].pid] = ps[i];	
	}	
	while(finished < listLength)
	{//main loop	
		for(auto &it: rrq)//this is to go through all items in map
		{
		/* Check arrival time and check completion status */
			if(rrq[it.first].arrival_time <= sysTime && !rrq[it.first].done) 
			{
			/*  Running the process */
				rrq[it.first].running = true;
				cout<< "<System Time " << sysTime << "> Process "<< it.first << " running.  \n";
				doNotIdle = true;
				for(int i = 0 ; i<quantum;i++)//if arrived, run the process for quantum
				{
				/* if this is the first time, set to active and record response time */
					if(!rrq[it.first].activated)
					{
						rrq[it.first].activated = true;
						rrq[it.first].respT = sysTime - rrq[it.first].arrival_time;
					}					
					rrq[it.first].cpuT++;//increment cputime
					sysTime++;//increment sysTime
					
				/* Incrememnted sysTime, all activated processes not running increment wait time */
					for(auto &itr: rrq)
					{
						if(rrq[itr.first].activated && !rrq[itr.first].done && !rrq[itr.first].running)
							rrq[itr.first].waitT++;
					}
						
				/* if its run for total burst time flag as done and record turnaround */
					if(rrq[it.first].cpuT == rrq[it.first].burst_time)
					{
						rrq[it.first].done = true;
						rrq[it.first].turnT = sysTime - rrq[it.first].arrival_time;
						finished++;
						cout<< "<System Time " << sysTime << "> Process "<< it.first << " finished..\n";
					}
				/* move on if process is done */
					if(rrq[it.first].done)
						break;
					
				}//end of running section	
			/* Interrupt, flag as waiting */
				rrq[it.first].running = false;
			}
		}//end of map iteration
		if(!doNotIdle)
		{
			sysTime++;
			cout<< "<System Time " << sysTime <<"> Idle..\n";
		}			
		/* Reset idle flag */
		doNotIdle = false;
	}//end of main while loop sig
/* Compile stats */
	for(auto &it: rrq)
	{
		stats.respTimes.push_back(rrq[it.first].respT);
		stats.waitTimes.push_back(rrq[it.first].waitT);
		stats.turnTimes.push_back(rrq[it.first].turnT);
		stats.cpUsage+=rrq[it.first].cpuT;
	}
	
	printf("\nRound Robin Results: \n");
	printf("\nTotal CPU utilization:  %5.2f%c.\n", stats.getCpUtilzation(sysTime),percent);
	printf("Average wait time: %6.2f.\n",  stats.getAvgWait());
	printf("Average response time: %5.2f.\n", stats.getAvgResp());
	printf("Average turnaround time: %6.2f.\n\n\n\n",stats.getAvgTurn());

}//end of function signature
