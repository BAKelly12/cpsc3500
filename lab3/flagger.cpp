/**
 *@file flagger.cpp - Implementation of flagger class
  *@authors Brigid Kelly
  *@see "Seattle University CPSC-3500 WINTER 2019"
  */
  
  
#include "flagger.h"

using namespace std; 

flagger::flagger(){
  waiting = 0; 
  sem_init(&sem, 0, 0);
  tLogHasHeader = false;
  fLogHasHeader = false;  
  threadsInMotion = false;
  lights_off = true; 
  flog_enable = false;
  tlog_enable = false;
}
 
 
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


flagger::~flagger(){
  sem_destroy(&sem);
  pthread_mutex_destroy(&mtx);
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
 
  time_t t = time(NULL);
  
 
   time_t seed[T_Q.size()]; 
 
   
  if(!threadsInMotion){ 
  
    for(size_t i(0); i<T_Q.size(); i++){
      
      seed[i] = t+i;
      pthread_create(&T_Q[i], NULL, critSect, &seed[i]);  
    }
    
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
  ++waiting;
  sem_wait(&sem);

	return NULL;
}

int flagger::get_lock(){
  if(pthread_mutex_init(&mtx,NULL))
      return -1;       
   pthread_mutex_lock(&mtx);
   return 0; 
}


int flagger::release_lock(){
  pthread_mutex_unlock(&mtx);  
  return 0;
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


///////////////////////////
/**Functions for logging */
////////////////////////////

string flagger::getTime()
{
    time_t tanD = time(nullptr);
    char buff[20];
    struct tm *sTm = localtime(&tanD); 
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

