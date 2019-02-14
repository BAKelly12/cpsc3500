/**
 * @file p3.cpp - Basic Implementation of multithreading and synchronization
 * @Author Brigid Kelly
 * @See "Seattle Universtiy CPSC3500 Winter 2019"
 **/

#include <time.h>
#include <queue>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#define NUMBER_OF_CARS 50
#define MAX_WAITING_CARS 10

using namespace std;

/*log file names*/
string fFile = "flagPerson.log";
string tFile = "cars.log";

/*Member Functions*/
inline int pthread_sleep(int seconds);
void* criticalSection(void* args);
void* makeCar(void* args);

/*Logging functions*/
inline void* tlog_header();
inline void* flog_header();
inline void tLog(string logMsg);
inline void fLog(string logMsg);
inline void tLog_ID(int carID);
inline void tLog_MSG(string logMsg);
inline void tLog_endl();
inline void tLog_time();
inline string getTime();

/*global variable declarations*/
volatile int waiting;
volatile char direction = '-';
volatile int carCount(1);
volatile bool first_t = true;
volatile unsigned int wNorth(0);
volatile unsigned int wSouth(0);
volatile bool dSwitch = true; //True: North False: south
queue<string> arrTimes;

/*semaphores and mutex*/
sem_t flag;
pthread_mutex_t csmtx;
sem_t makerSem;
pthread_mutex_t makerMtx;

int main(){
   /*initialize logs with headers*/
	 flog_header();
	 tlog_header();
	 
   /*initialize semaphores and mutexes*/
	 sem_init(&flag, 0, 0);
	 pthread_mutex_init(&csmtx,NULL);
	 
	 sem_init(&makerSem, 0, (unsigned int)1);
	 pthread_mutex_init(&makerMtx, NULL);
 
   /*make the critical section thread*/
	 srand(time(NULL));
	    pthread_t crit_s_thread;
		if ( -1 == pthread_create(&crit_s_thread, NULL, &criticalSection, NULL))
			{
				perror("error making critical section thread");
				return -1;
			} 
		
    /*set the north's direction and make its generator*/
	 char* nArg = "N";	  
	 pthread_t makeNorth; 
	 if ( -1 == pthread_create(&makeNorth, NULL, &makeCar, (void*)nArg ))
		{
			perror("error making north car generator");
			return -1;
		} 
				
   /*set the south's direction and make its generator*/
	 char* sArg = "S";
	 pthread_t makeSouth;
	 if ( -1 == pthread_create(&makeSouth, NULL, &makeCar, &sArg )){
			perror("error making south car generator");
			return -1;
		}

   /*rejoin threads with main */
	 pthread_join(crit_s_thread, NULL);
	 // just cancel the two infinite threads
	 pthread_cancel(makeSouth);	
	 pthread_cancel(makeNorth);
	
   /* destroy semaphores */	 
	 sem_destroy(&flag);
     pthread_mutex_destroy(&csmtx);
	 pthread_mutex_destroy(&makerMtx);
     sem_destroy(&makerSem);
	 
	return 0;
}//end of main()

/*******************************************
 *
 *            FUNCTIONS
 *
 * ****************************************/            


/**             MAIN CRITICAL SECTION FUNCTION HERE
 *@brief criticalSection(void* args) -  This function is the main critical
 *			section of this program.  It uses a single semaphore and mutex
 *			to lock the section while the thread is moving through it.  
 *			It provides logic and functionality to watch for incoming 
 *			"cars" and properly orders them for processing througn the
 *			critical section choke point.
 *@param void* args - void pointer passed to function via pthread_create(), 
 *						it has no usage in this function.
 *@return - n/a
 */ 
void* criticalSection(void* args){
 
  while((carCount <= NUMBER_OF_CARS ) ){
	/* basic error catching to ensure the first thread in gets the lock*/ 
    if(first_t){
	   first_t = false; 
	   pthread_mutex_lock(&csmtx);
    }else{   
      sem_wait(&flag);  
      pthread_mutex_lock(&csmtx);  
    }

   ////////////START CRITICAL SECTION////////////////
	  		
		/*Another switching mechanism to figure out when
				to sleep & log and which side a new car is coming from */
	 if(!wNorth && !wSouth)
	 {
		fLog("Asleep");
		cout<< "flagPerson: I SLEEP...\n";
		while(!(wNorth || wSouth)){} //wait while nothing is there
		cout << "flagPerson: I HAVE AWOKEN...\n";
		fLog("Awake");
		if(wNorth)
			dSwitch = true;
		else
			dSwitch = false;
	 }
	 else if( (dSwitch && wSouth >= MAX_WAITING_CARS) || !wNorth)
		 dSwitch = !dSwitch;
	 else if( (!dSwitch && wNorth >= MAX_WAITING_CARS) || !wSouth)
		 dSwitch = !dSwitch;
	
	/* basic switching mechanism to declare a direction for logging */
	char myDir;
	if(dSwitch)//if current traveling direction is north
	{	
		myDir = 'N';
		
	}
	else
	{
		myDir = 'S';
	}
	
	/* more basic error catching to prevent 
	   unsigned vars from going to numerical limit */
	int myCarCount = carCount;
	if(myDir == 'N' && wNorth !=0)
    {
		string this_cars_arrival_time;
     	this_cars_arrival_time = arrTimes.front();
    	arrTimes.pop();
		tLog_ID(myCarCount);
		tLog_MSG("\t N \t");
		tLog_MSG(this_cars_arrival_time);
		tLog_time();
		wNorth--;
		carCount++;	
		cout<<"Car number " << myCarCount <<" pulling through..DIR: "<<myDir<<"\n";
		pthread_sleep(2);
		cout<<"Car number " << myCarCount<<" is leaving(3 seconds later)\n";
		tLog_time();//exit time
		tLog_endl();		
	}
	else if(myDir == 'S' && wSouth !=0)
    {	
        string this_cars_arrival_time;
	    this_cars_arrival_time = arrTimes.front();
	    arrTimes.pop();
		tLog_ID(myCarCount);
		tLog_MSG("\t S \t");
		tLog_MSG(this_cars_arrival_time);
		tLog_time();
		wSouth--;
	    carCount++;	
        cout<<"Car number " << myCarCount <<" pulling through..DIR: "<<myDir<<"\n";
        pthread_sleep(2);
        cout<<"Car number " << myCarCount<<" is leaving(3 seconds later)\n";
		tLog_time();//exit time
		tLog_endl();
		
	}
	
   pthread_mutex_unlock(&csmtx);
   sem_post(&flag);
   
   
	/* This is just to prevent more cars being made */
   if(carCount > NUMBER_OF_CARS && !wNorth && !wSouth)
	   break;  
  }//end of critical section while-loop
  return NULL;
}//end of crticalSection function



/**   CAR GENERATOR 
 *@brief threaded function to generate incoming "cars"
 *@param void* args - void pointer to input from pthread_create(), in 
						in this function, args contains the address
						to a char** containing the thread's directional seed 
						and is then dereferenced in order to increment the
						proper direction's "queue" integer.  Two threads
						operate within those code independently, but
						concurrently and in different directions.
 *@return n/a
*/ 
void* makeCar(void* args){
	
	int i(0);
	while(1){	
		pthread_sleep(2);//just to prevent hundreds of thousands of cars
    	char* direction;
    	direction = (char*)args;//typecasting & asignment
    	  srand(time(NULL) + i);//seed rand with new seed
    	  if(8 >= rand() % 10 + 1){ //80% chance
			sem_wait(&makerSem);//get lock and sem so cars are in time order
			pthread_mutex_lock(&makerMtx);			
    	    	if(*direction == 'N')
				{
    	    		++wNorth;
    	    	}
				else{
    	    		++wSouth;	
				}
			arrTimes.push(getTime());	
			pthread_mutex_unlock(&makerMtx);
			sem_post(&makerSem);			
    	  }
		  else
    		pthread_sleep(20);
	
	    i++;	
	}	
	return NULL;
}



/**
 *@brief thread sleep function provided by Dr. Yingwu Zhu, Seattle Universtiy
 */
inline int pthread_sleep(int seconds){    
    pthread_mutex_t mutex;      
	pthread_cond_t conditionvar; 
	struct timespec timetoexpire;
	if(pthread_mutex_init(&mutex,NULL))
		{               return -1;
	}
	if(pthread_cond_init(&conditionvar,NULL))
		{               return -1;        
	}//When to expire is an absolute time, so get the current time and add 
	//it to our delay time 
	timetoexpire.tv_sec = (unsigned int)time(NULL) + seconds;
	timetoexpire.tv_nsec = 0;
	return pthread_cond_timedwait(&conditionvar, &mutex, &timetoexpire);
} 



/******************************************
 *
 *          LOGGING FUNCTIONS
 *
 ******************************************/
 
/*flagger's log header*/
inline void* flog_header(){   
      ofstream ffs(fFile.c_str(), ios_base::out | ios_base::app );
      ffs<<"Time \t\t State\n";
      ffs.close();
    return NULL;
}

/*thread log header*/
inline void* tlog_header(){ 
    std::ofstream tfs(tFile.c_str(), ios_base::out | ios_base::app );
    tfs<<"carID\tDir\t  Arrival-Time Start-Time\tEnd-Time\n";
    tfs.close();
  return NULL; 
}

/*flagger message logging*/
inline void fLog( std::string logMsg ){
    std::string now = getTime(); 
    std::ofstream ofs(fFile.c_str(), std::ios_base::out | std::ios_base::app );
    ofs << now << "\t" << logMsg << std::endl;
    ofs.close();
}

/*thread Log-Time insertion Function*/
inline void tLog_time(){

    std::string now = getTime();
    std::ofstream ofs(tFile.c_str(), std::ios_base::out | std::ios_base::app );
    ofs << now << "\t";
    ofs.close();
	
}
/*thread carID logging insertion function*/
inline void tLog_ID(int carID){
	std::ofstream ofs(tFile.c_str(), std::ios_base::out | std::ios_base::app );
    ofs << carID << "\t";
    ofs.close();
}
/*general thread message logging insertion*/
inline void tLog_MSG(string logMsg){
	std::ofstream ofs(tFile.c_str(), std::ios_base::out | std::ios_base::app );
    ofs << logMsg <<"\t";
    ofs.close();
	
}

/*thread logging line end*/
inline void tLog_endl(){  
    std::ofstream ofs(tFile.c_str(), std::ios_base::out | std::ios_base::app );
    ofs<<endl;
    ofs.close();
}

	
/*using time.h to get time for logging*/
string getTime()
{
    time_t tanD = time(nullptr);
    char buff[20];
    struct tm *sTm = localtime(&tanD); 
    strftime(buff, sizeof(buff), "%H:%M:%S", sTm);   
    std::string now(buff);   
    return now; 
} 

