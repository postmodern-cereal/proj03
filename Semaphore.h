//where all of the semaphore magic happens
#ifndef SEMAPHORE_H_INCLUDED
#define SEMAPHORE_H_INCLUDED

#include "Vars.h"

void wait(semaphore *sem, PCB *process);

void signal(semaphore *sem, int PID);

#endif
