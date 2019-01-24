#Makefile:  use to compaile, tar and submit P1,
#CPSC3500, Winter 2019
#Brigid Kelly/Sam Van Nes

CC = gcc
DEBUG =  -g 
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

p1:
	$(CC) $(LFLAGS) -o p1 p1.c

tar: 
	tar -cvf  p1.tar  README  p1.c  Makefile 

submit: 
	/home/fac/zhuy/class/SubmitHW/submit3500 p1 p1.tar



