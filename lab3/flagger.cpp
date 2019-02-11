/**
 *@file flagger.cpp - Implementation of flagger class
  *@authors Brigid Kelly
  *@see "Seattle University CPSC-3500 WINTER 2019"
  */
  
  
#include "flagger.h"

using namespace std; 
  
flagger::flagger(void* (*ca)(void* args), string fln, string tln){
  waiting = 0;
  tLogFilePath = tln;
  critSect = ca;  
  sem_init(&sem, 0, 0);
  direction = ' ';
  fLogFilePath = fln;
  fLog_init();
  tLog_init(); 
  tLogHasHeader = false;
  fLogHasHeader = false;  
  threadsInMotion = false;
  lights_off = true;
}


flagger::flagger(void* (*ca)(void* args), string fln, string tln, char dir){ 
  waiting = 0;
  tLogFilePath = tln;
  critSect = ca;  
  sem_init(&sem, 0, 0);
  direction = ' ';
  fLogFilePath = fln;
  fLog_init();
  tLog_init(); 
  tLogHasHeader = false;
  fLogHasHeader = false; 
  threadsInMotion = false;
  lights_off = true;
  
}

flagger::~flagger(){
  sem_destroy(&sem);
}

void* flagger::make_t(int num){
  if(!threadsInMotion)
    T_Q.resize(num);
  else
    cerr << "Cannot change thread queue size while threads are open\n";
  return NULL;
  
}

void* flagger::push_t(){
  
  if(!threadsInMotion)
    T_Q.resize(T_Q.size() + 1 );
  else
    cerr << "Cannot change thread queue size while threads are open\n";
  
  return NULL;
}
void* flagger::pop_t(){
  
  if(!threadsInMotion)
    T_Q.pop_back(); 
  else
    cerr << "Cannot change thread queue size while threads are open\n";
  
  return NULL;
}
  

void* flagger::create_t(){
  
  if(!threadsInMotion){ 
  
    for(size_t i(0); i<T_Q.size(); i++)
      pthread_create(&T_Q[i], NULL, critSect, NULL); 
    
    threadsInMotion = true;    
  }
  else
    cerr << "Cannot create more threads while threads are in motion";
  
  return NULL; 
}


volatile bool flagger::first_t(){
  if(lights_off){
    lights_off = false;  //if theyre off, turn them on
    return !lights_off;
  }else
    return lights_off;
}

void* flagger::wait(){
  //log arrival time here
  ++waiting;
  sem_wait(&sem);

	return NULL;
}

void* flagger::get_lock(){
  ++waiting;
  return NULL; 
}

void* flagger::post(){
  //log critical section entry time here
  sem_post(&sem);
  --waiting;
  if(0>=waiting)
    lights_off = true;  //last one out turn the lights off
 
	return NULL;
}

void* flagger::join(){ 

  for(size_t i(0); i<T_Q.size(); i++)
    pthread_join(T_Q[i], NULL); 
  //logging for vehicle exit here
  
  return NULL;
}

//whenever you need the logic
  
int flagger::get_w(){
  return waiting;
}


int flagger::get_p(){ 
  int n(0);
  sem_getvalue(&sem, &n); 
	return n;
}


int flagger::sleep(int seconds){
  
  fLog("Sleep");
  pthread_mutex_t mutex;
  pthread_cond_t conditionvar;
  
  struct timespec timetoexpire;
  
  if(pthread_mutex_init(&mutex,NULL))
    {
      return -1;       
    }
  if(pthread_cond_init(&conditionvar,NULL))
    {
      return -1;
    }   
    
  //When to expire is an absolute time, so get the current time and add     
  //it to our delay time 
  timetoexpire.tv_sec = (unsigned int)time(NULL) + seconds;
  timetoexpire.tv_nsec = 0;
  
  fLog("Awake");
  
  return pthread_cond_timedwait(&conditionvar, &mutex, &timetoexpire);
 
} 

/**Functions for logging */

std::string flagger::getTime()
{
    std::time_t tanD = std::time(nullptr);
    char buff[20];
    struct std::tm *sTm = localtime(&tanD); 
    strftime(buff, sizeof(buff), "%H:%M:%S", sTm);   
    std::string now(buff);   
    return now; 
} 

int flagger::fLog_init(){
    if(!fLogHasHeader){
      flog_header();
      flog_enable = true;
      return 1;
    }else
      return -1;
}
  
int flagger::tLog_init(){
    if(!tLogHasHeader){
      tlog_enable = true;
      tlog_header();
      return 1;
    }else
      return -1; 
}

inline void flagger::fLog( std::string logMsg ){

    std::string now = getTime();
    
    std::ofstream ofs(fLogFilePath, std::ios_base::out | std::ios_base::app );
    ofs << now << '\t' << logMsg << std::endl;
    ofs.close();
}

inline void flagger::tLog( std::string logMsg ){

    std::string now = getTime();
    
    std::cout << logMsg << "\n";
    std::ofstream ofs(tLogFilePath, std::ios_base::out | std::ios_base::app );
    ofs << now << '\t' << logMsg << std::endl;
    ofs.close();
}
  

void* flagger::flog_header(){
    
      std::ofstream ofs(fLogFilePath, std::ios_base::out | std::ios_base::app );
      ofs<<"Time \t\t\t State"<<std::endl;
      ofs.close();
      fLogHasHeader = true;
    return NULL;
}


void* flagger::tlog_header(){
  
    std::ofstream ofs(fLogFilePath, std::ios_base::out | std::ios_base::app );
    ofs<<"Time \t\t\t State"<<std::endl;
    ofs.close();
    tLogHasHeader = true;

  return NULL;
  
}

