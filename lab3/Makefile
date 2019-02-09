#Makefile:  use to compaile, tar and submit P3,
#CPSC3500, Winter 2019
#Brigid Kelly/Sam Van Nes

CC = g++
STD = -std=c++11
DEBUG =  -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

p3:
	$(CC) $(LFLAGS) -o p3 p3.cpp flagger.h flagger.cpp $(STD) -pthread
  
lab3s:
	$(CC) $(LFLAGS) -o lab3s lab3s.cpp flagger.h flagger.cpp $(STD) -pthread
  
lab3b:
	$(CC) $(LFLAGS) -o lab3b lab3b.cpp flagger.h flagger.cpp $(STD) -pthread

p3_tar:
	tar -cvf  p3.tar README p3.cpp flagger.h flagger.cpp flagger.log cars.log Makefile

p3_submit:
	/home/fac/zhuy/class/SubmitHW/submit3500 p3 p3.tar
