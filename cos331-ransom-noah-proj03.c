//By Noah Ransom

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Funcs.h"
#include "Vars.h"

main(int argc, char *argv[])
{
    int timeSlice = 1;
    char *fileName;
    //argv[1] is what version to return
    //0 is guaranteed no deadlock
    //1 is guaranteed deadlock
    //2 is random between 1 and 10
    //3 is random between 1 and 10 with doorman
    //timeslice is length of program
    int variant = (int)(*argv[1] - 48);
    srand(time(NULL));
    switch(variant)
    {
        case 0: timeSlice = 20;
            fileName = "Philosophers.PB";
            break;

        case 1: timeSlice = 5;
            fileName = "Philosophers.PB";
            break;

        case 2: timeSlice = (rand() % 10) + 1;
            fileName = "Philosophers.PB";
            break;

        case 3: timeSlice = 7;//(rand() % 10 + 1);
            fileName = "PhilosophersWithDoorman.PB";
            break;

        default: timeSlice = 20;
            fileName = "Philosophers.PB";
            break;
    }

    int i;
    PC = 0;
    //must set PC to 0 because headers are bitching
    //if queue empty then we reached a deadlock
    //step 1: read files into memory
    struct PCB *Current, *tmp;
    Doorman = (semaphore *) malloc (sizeof (semaphore));
    Doorman->count = 4;
    for(i = 0; i < 5; i++)
    {
        Forks[i] = (semaphore *) malloc (sizeof (semaphore));
        Forks[i]->count = 1;
        Forks[i]->Sem_Queue = NULL;
    }

    RQ = (struct PCB *) malloc (sizeof (struct PCB));
    RQ->PID = 0;
    RQ->IC = timeSlice;
    tmp = RQ;

    for(i = 1; i < 5; i++)
    {
        tmp->Next_PCB = (struct PCB *) malloc (sizeof (struct PCB));
        tmp->Next_PCB->PID = i;
        tmp->Next_PCB->IC = timeSlice;  //rand returns 0 .. MAX
        tmp->Next_PCB->Next_PCB = NULL;
        tmp = tmp->Next_PCB;
    }

    RQT = tmp;
    RQT->Next_PCB = NULL;

	tmp = RQ;
	for (i = 0; i < 5; i++)
	{
	    LoadProgram( i, fileName, &tmp);
		printf("LimitReg = %d. BaseReg = %d. IC = %d\n",tmp->LimitReg, tmp->BaseReg, tmp->IC);
		//tmp->IC = i + 2;
		tmp = tmp->Next_PCB;
	}

	printf("Now let's EXECUTE IT! WOOOEEEEEE!!!\n");

	while(1)
    {
        //PrintQ(RQ);
        Current = GetNextProcess(&RQ);
		RestoreState(Current);
        printf("\n\nCURRENT PID %d, IC %d, PC is %d\n", Current->PID, Current->IC, Current->PC);
        //Current->IC--;

        printf("Fork Counts:\n");
        for(i = 0; i < 5; i++)
        {
            printf("%d\n", Forks[i]->count);
        }
        printf("Doorman: %d\n\n", Doorman->count);

		int Completed = ExecuteProc(Current);
        if (Completed == 1)
        {
            printf("Removing PID %d\n", Current->PID);
            DeletePCB(Current);
            Current = NULL;
        }

        else if (Completed == 0)
        {
            SaveState(&Current);
			printf("Moving PID %d to TAIL\n", Current->PID);
            MvToTail(Current, &RQT);
            printf("RQT is %d\n", RQT->PID);
			//if(RQ == NULL)
                //RQ = RQT;
        }
        if(RQ == NULL)
        {
            //if head is null then nothing remains in the queue
            printf("No philosophers remain unblocked. Deadlock achieved.\n");
            return;
            /*if(Current == NULL)
            {
                //now print the entirety of memory
                //Commented out because it is obnoxiously long
                //LimitRegister = 1000;
	            //printf("Full contents of main memory:\n");
                //printMEM(0);

            }
            else
            {
                RQ = Current;
                RQ->Next_PCB = NULL;
            }*/
        }

        PrintQ(RQ);
        sleep(1);

    }
}
