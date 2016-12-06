120002 //load R0 with 2
36R0R2 //load PID to R2
14R2ZZ //store PID to ACC
160001 //add 1 to ACC
120005 //store 5 to R0
15R3ZZ //store ACC to R3
37R3R0 //ACC = R3 % R0
15R3ZZ //store ACC to R3
120001 //load R0 with 1
13R1R0 //move to R1
120000 //0 for wait call, fork id
14R2ZZ //load ACC with PID <-- LOOP START
36R0R1 //wait on doorman
36R0R0 //wait forks[i]
14R3ZZ //load ACC with (PID+1) % 5
36R0R0 //wait forks other
14R2ZZ //load ACC with PID
36R1R0 //signal forks[i]
14R3ZZ //load acc with other
36R1R0 //signal forks other
36R1R1 //call signal on doorman
3500ZZ //branch to loop start

TQ of 9 guarantees no deadlock, but causes starvation?
