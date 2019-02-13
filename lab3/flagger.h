/**
 *@file: flagger.h - Header file for flagger class -- Basic thread manager
                      //Abstraction of semaphore & pthread
  *@Author: Brigid Kelly
  *@See:  "Seattle University CPSC 3500 Winter 2019"
  */
  

#include <time.h>
#include <vector>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>

  
#pragma once
 
class flagger
{
    
public:
    
     /**
      *@brief specific constructors
      *@param ca: address of critical section function
      *      fln: flagger log filename
      *      tln: thread log filename
      *      dir: char depicting a direction of thread flow
      *@note:  Including these file names is the only way to init logging
      *@example flagger flagger(&criticalSection, "flagger.log", "threads.log", 'n'); 
      */
       
     flagger(void* (*ca)(void *args), std::string fln, std::string tln, char dir);
     flagger(void* (*ca)(void *args), std::string fln, std::string tln);
     
/**  Big Six **/ 
      flagger();
      ~flagger();
      flagger(const flagger& other) = delete;
      flagger(flagger&& temp) = delete;
	    flagger& operator=(const flagger& other) = delete;
    	flagger& operator=(flagger&& temp) = delete;
      
/** Member Functions **/
    
    //getters
      /**
        *@member int getw() - returns number of currently waiting threads
        *@param none
        *@return int 
        **/
      int get_w();
      
      
       /**
        *@member int getp() - returns current value of semaphore
        *@param none
        *@return int 
        **/
      int get_p();

     
  //Thread Commands
     
     /**
      *@member void* make_t(int num) - tells flagger how many threads to will 
      *                               need to be made when create is called.
      *@modifies size of T_Q 
      *@param num: number of threads to initialize
      *@return n/a
      **/
      void* make_t(int num);
      
      
     /**
      *@member void* create_t() - creates number of threads specified by T_Q.size() and
      *                             executes code at function instruction address passed 
      *                               to constructor
      *@param n/a
      *@return n/a
      **/
      void* create_t();
      
     /**
      *@member void* push_t() - adds 1 to size of T_Q (thread queue)
      *@param n/a 
      *@return n/a
      */
      void* push_t();
      
     /**
      *@member pop_t() - subtracts 1 from size of T_Q (thread queue)
      *@param n/a 
      *@return n/a
      */
      void* pop_t();
      
    /**
      *@member isThefirst() - Checks if thread is the first one through;
      *@return 1 if is the first, 0 if not
      */
      volatile bool first_t();

     /**
      *@member join() - rejoins main process with threads
      *@param n/a 
      *@return n/a
      */
     void* join();
      
  //Mutex Commands
      
  /**
   *@member get_lock() - gets flagger mutex
   *@return n/a
   */
    
     int get_lock();
     
     
     /**
      *@memeber release_lock() - Releases flagger mutex
      *@return n/a
      */
     
     int release_lock();
     

      
  //Semaphore commands
      
      /**
       *@member void* post() - abstraction of semaphore sem_post() with log functionality
       *@param n/a
       *@return n/a
       */
      void* post();
      
    
      
      /**
       *@member void* wait() - abstraction of semaphore sem_wait() with log functionality
       *@param n/a
       *@return n/a
       */
      void* wait();
      
      
      /**
       *@member int sleep(int seconds) - Thread Sleep Timer 
       *@author Dr. Yingwu Zhu
       *@param n/a
       *@return exit status of sleep
       */
      int sleep(int seconds);
 
private:

/**Private Member Variables**/

      std::string tLogFilePath;
      std::string fLogFilePath;

      int waiting;
      int tid;
      std::vector<pthread_t> T_Q;
          
      void* (*critSect)(void* args);
      char direction;
      sem_t sem;
      pthread_t consTest;
      pthread_mutex_t mtx; 
     
      bool tlog_enable;
      bool flog_enable;
      bool tLogHasHeader;
      bool fLogHasHeader;
      bool threadsInMotion;
      volatile bool lights_off;



/** Private Member Functions **/
  
  
  /**
   *@brief get current time of day
   *@return string containing current time : HH:MM:SS output in 24 hour format
   */
  std::string getTime();


/**Functions for logging */
  
  /**
   *@brief Initializes flagger logging 
   *@return 1 if init success, -1 if already enabled or failed
   */
  int fLog_init();
 
  
  /**
   *@brief Initializes thread logging 
   *@return 1 if init success, -1 if already enabled or failed
   */
  int tLog_init();
  
 /**
   *@brief creates log entry in flagger logfile
   *@param message in string format for input to flagger logfile
   */
  inline void fLog( std::string logMsg );


  /**
   *@brief creates log entry in thread logfile
   *@param message for input to thread logfile
   */
  inline void tLog( std::string logMsg );

  /**
   *@brief creates header inside flagger logfile
   */
  void* flog_header();


   /**
   *@brief creates header inside thread logfile
   */
  void* tlog_header();


};//end of class definition