/**
 *@file: flagger.h - Header file for flagger class -- Basic thread manager
                      //Abstraction of semaphore
  *@Author: Brigid Kelly
  *@See:  "Seattle University CPSC 3500 Winter 2019"
  */
  

#include <time.h>
#include <vector>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <mutex>
  
#pragma once
 
class flagger{
    
public:
    
/*Specific Constructors*/
     flagger(void* (*ca)(void *args), std::string fln, std::string tln, char dir);
     flagger(void* (*ca)(void *args), std::string fln, std::string tln);
     
/**  Big Six **/
      flagger() = delete;
      ~flagger();
      flagger(const flagger& other) = default;
      flagger(flagger&& temp) = default;
	    flagger& operator=(const flagger& other) = default;
    	flagger& operator=(flagger&& temp) = default;
      
/** Member Functions **/

      //getters
      int getw();
      int getp();
      

      //Setters
      int fLog_init();
      int tLog_init();
      
      void* make_t(int num);

      //Go commands
      void* post();
      void* wait();
      int sleep(int seconds);
      void* create();
      void* join();
      

private:

/**Private Member Variables**/

      std::string tLogFilePath;
      std::string fLogFilePath;

      int waiting;
      int tid;
      std::vector<pthread_t> consQ;
      std::vector<pthread_t> prodQ;
      
      void* (*critSect)(void* args);
      char direction;
      sem_t sem;
      pthread_t consTest;
      
      bool tlog_enable;
      bool flog_enable;
      bool tLogHasHeader;
      bool fLogHasHeader;


/** Private Member Functions **/
 

  std::string getTime()
  {
    std::time_t tanD = std::time(nullptr);
    char buff[20];
    struct std::tm *sTm = localtime(&tanD); 
    strftime(buff, sizeof(buff), "%H:%M:%S", sTm);   
    std::string now(buff);   
    return now; 
  } 


/**Functions for logging */

  inline void fLog( std::string logMsg )
  {

    std::string now = getTime();
    
    std::ofstream ofs(fLogFilePath, std::ios_base::out | std::ios_base::app );
    ofs << now << '\t' << logMsg << std::endl;
    ofs.close();
  }

  inline void tLog( std::string logMsg )
  {

    std::string now = getTime();
    
    std::cout << logMsg << "\n";
    std::ofstream ofs(tLogFilePath, std::ios_base::out | std::ios_base::app );
    ofs << now << '\t' << logMsg << std::endl;
    ofs.close();
  }
  
  void* flog_header(){
    
      std::ofstream ofs(fLogFilePath, std::ios_base::out | std::ios_base::app );
      ofs<<"Time \t\t\t State"<<std::endl;
      ofs.close();
      fLogHasHeader = true;
    return NULL;
  }

  void* tlog_header(){
  
    std::ofstream ofs(fLogFilePath, std::ios_base::out | std::ios_base::app );
    ofs<<"Time \t\t\t State"<<std::endl;
    ofs.close();
    tLogHasHeader = true;

  return NULL;
  
  }




};//end of class definition