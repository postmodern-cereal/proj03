//Semaphore methods
#include "Semaphore.h"
#include "Funcs.h"

PCB *pop(semaphore **sem)
{
    if((*sem)->Sem_Queue->Next_PCB == NULL)
    {
        PCB *tmp = (*sem)->Sem_Queue;
        (*sem)->Sem_Queue = NULL;
        return tmp;
    }
    else
    {
        PCB *tmp = (*sem)->Sem_Queue;
        (*sem)->Sem_Queue = (*sem)->Sem_Queue->Next_PCB;
        return tmp;

    }
}

void push(semaphore **sem, PCB *process)
{
    if((*sem)->Sem_Queue != NULL)
    {
        PCB *tmp = (*sem)->Sem_Queue;
        while(tmp->Next_PCB != NULL)
        {
            tmp = tmp->Next_PCB;
        }
        tmp->Next_PCB = process;
        tmp = tmp->Next_PCB;
    }
    else
    {
        (*sem)->Sem_Queue = process;
        (*sem)->Sem_Queue->Next_PCB = NULL;
    }
}

void wait(semaphore *sem, PCB *process)
{
    sem->count--;
    if(sem == Doorman)
    {
        printf("Philosopher %d called wait on the doorman semaphore", process->PID);
        //print the doorman stuffonot blockednot blocked
        if(sem->count >= 0)
        {
            printf(", and was allowed to enter the dining room.\n\n");
            return;
        }
        printf(", and was not allowed to enter the dining room.\n\n");
    }
    else
    {
        printf("Philosopher %d called wait on fork semaphore %d", process->PID, ACC);
        if(sem->count >= 0)
        {
            printf(", and was not blocked.\n\n");
            return;
        }
        printf(", and was blocked.\n\n");
    }

    //place PCB at tail of semaphore queue

    SaveState(&process);
    push(&sem, process);
}

void signal(semaphore *sem, int PID)
{
    if(sem == Doorman)
    {
        //print the doorman stuff
        Doorman->count++;
        printf("Philosopher %d called signal on the doorman semaphore", PID);
        if(sem->count <= 0)
        {
            //take next thing off sem queue and put on ready queue
            PCB *tmp = pop(&sem);
            printf(", and Philosopher %d was allowed to enter the dining room.\n", tmp->PID);
            MvToTail(tmp, &RQT); //puts it on the tail of the ready queue
        }
        else
        {
            printf(".\n\n");
        }
    }
    else
    {
        Forks[ACC]->count++;
        printf("Philosopher %d called signal on fork semaphore %d", PID, ACC);

        if(sem->count <= 0)
        {
            //take next thing off sem queue and put on ready queue
            PCB *tmp = pop(&sem);
            printf(", and Philosopher %d was unblocked.\n\n", tmp->PID);
            MvToTail(tmp, &RQT); //puts it on the tail of the ready queue
        }

        else
        {
            printf(".\n\n");
        }
    }
}

//just some test code
/*
main()
{
    printf("Doing ok\n");
    PCB *p1, *p2, *p3;
    p1 = (PCB *) malloc (sizeof ( PCB));
    p1->PID = 1;
    p2 = (PCB *) malloc (sizeof ( PCB));
    p2->PID = 2;
    p3 = (PCB *) malloc (sizeof ( PCB));
    p3->PID = 3;
    printf("Doing ok\n");
    semaphore *sem;
    sem = (semaphore *) malloc (sizeof ( semaphore));
    sem->count = 0;
    printf("Doing ok\n");
    sem->Sem_Queue = p3;
    sem->Sem_Queue->Next_PCB = p2;
    sem->Sem_Queue_Tail = sem->Sem_Queue->Next_PCB;
    printf("Still good\n");
    wait(sem, p1);
    PrintQ(sem->Sem_Queue);
    printf("%d\n", sem->Sem_Queue_Tail->PID);

    signal(sem);
    signal(sem);
    signal(sem);
    if(sem->Sem_Queue_Tail != NULL)
    {
    printf("%d\n", sem->Sem_Queue_Tail->PID);
    PrintQ(sem->Sem_Queue);
    }

}*/
