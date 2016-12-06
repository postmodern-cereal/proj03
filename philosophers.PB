philosophers alternate between thinking and eating, and can only eat when they have both left and right forks
Philosopher()
{
    i = getPID();
    while (1)
    {
        think();
        wait(Forks[i]);
        wait(Forks[(i+1) % 5]);
        eat();
        signal(Forks[i]);
        signal(Forks[(i + 1) % 5]);
    }
}

120002 //load R0 with 2
36R0R2 //load PID to R2
14R2ZZ //store PID to ACC
160001 //add 1 to ACC
120005 //store 5 to R0
15R3ZZ //store ACC to R3
37R3R0 //ACC = R3 % R0
15R3ZZ //store ACC to R3
120001 //1 for signal call
13R1R0 //move to R1
120000 //0 for wait call, fork id
14R2ZZ //load ACC with PID
36R0R0 //wait forks[i] <-- LOOP START
14R3ZZ //load ACC with (PID+1) % 5
36R0R0 //wait forks other
36R1R0 //signal forks other
14R2ZZ //load ACC with PID
36R1R0 //signal forks[i]
3512ZZ //branch to loop start
