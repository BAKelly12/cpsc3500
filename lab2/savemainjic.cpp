
/************************************
*             MAIN BODY             *
*************************************/
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
