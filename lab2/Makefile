#Makefile:  use to compaile, tar and submit P2,
#CPSC3500, Winter 2019
#Brigid Kelly/Sam Van Nes

CC = g++
STD = -std=c++11
DEBUG =  -g 
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

p2:
	$(CC) $(LFLAGS) -o p2 p2.cpp $(STD)

p2_tar: 
	tar -cvf  p2.tar README p2.cpp testData.txt Makefile 

p2_submit: 
	/home/fac/zhuy/class/SubmitHW/submit3500 p2 p2.tar



