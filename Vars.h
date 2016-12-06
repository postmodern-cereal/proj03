#ifndef VARS_H_INCLUDED
#define VARS_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct PCB
{
    char *programName;
    struct PCB *Next_PCB;
    int PID;
    short int PC, P0, P1, P2, P3;
    int IC; //number of instructions before preemption (i.e., time slice)
    int R0, R1, R2, R3;
    char PSW[2];
    int ACC;
    int BaseReg, LimitReg;
    /* Note: You cannot store the process' memory */
}PCB;

typedef struct semaphore
{
    int count;
    PCB *Sem_Queue;
    PCB *Sem_Queue_Tail;
} semaphore;

/*These are variables representing the VM itself*/

char IR[6];
short int PC;

PCB *RQ, *RQT;

char input_line [7];

short int P0; //these are the pointer registers
short int P1;
short int P2;
short int P3;

int R0; //GP regs
int R1;
int R2;
int R3;

int BaseRegister, LimitRegister;

char *name; //string that holds the program name
int ACC;
char PSW[2];
char memory [1000][6]; 	//this is the program memory for all programs
short int opcode;

semaphore *Forks [5];
semaphore *Doorman;

#endif
