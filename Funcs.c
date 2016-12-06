//Noah Ransom

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Funcs.h"
#include "Semaphore.h"

int i;

void LoadProgram(int PID, PCB **tmp)
{
    int i, fp;
	int program_line = 100 * PID;
	(*tmp)->BaseReg  = program_line;
	(*tmp)->LimitReg = program_line + 100;
    fp = open("phil.PB", O_RDONLY); //always check the return value.
    //name = "Fib.PB";

    //(*tmp)->programName = name;

    printf("Open is %d\n", fp);

    if (fp < 0) //error in read
    {
        printf("Could not open file\n");
        exit(0);
    }

 	int ret = read (fp, input_line, 7 ); //returns number of characters read`

	while (1)
    {
        if (ret <= 0) //indicates end of file or error
            break; //breaks out of infinite loop

        printf("Copying Program line %d into memory\n", program_line);
        for (i = 0; i < 6; i++)
        {
            memory[program_line][i] = input_line[i];
            printf("%c ", memory[program_line][i]);
        }
        printf("\n");

        ret = read (fp, input_line, 7 );

        	//if the firat character is a 'Z' then you are reading data.
        	//No more program code so break out of loop

		if(input_line[0] == 'Z')
            break;

        program_line++; //now at a new line in the prog
    }

    printf("Read in Code. Closing File\n");
    close(fp);
}

void PrintQ(struct PCB *Head)
{
    struct PCB *tmp = Head;
    while (tmp != NULL)
    {
        printf("PID is %d and IC is %d\n", tmp->PID, tmp->IC);
        tmp = tmp->Next_PCB;
    }
}

struct PCB *GetNextProcess(struct PCB **Head)
{
    //remove pcb at head, returns pointer to it, updates head to point to next thing in list
    //the returned PCB will not be connected to the rest of the list in any way
    if(Head == NULL)
    {
        return NULL;
    }
    struct PCB *tmp = *Head;
    *Head = (*Head)->Next_PCB;
    tmp->Next_PCB = NULL;
    return tmp;
}

void MvToTail(struct PCB *Current, struct PCB **RQT)
{
    //since current is not connected to the rest of the queue in any way, we can just stick it on the end
    (*RQT)->Next_PCB = Current;
    *RQT = (*RQT)->Next_PCB;
    (*RQT)->Next_PCB = NULL;

}

void DeletePCB(struct PCB *Current)
{
    //use free to deallocate the PCB
    //we are assuming that nobody did anything silly like forget to assign some other pointer to point to Next_PCB already
    free(Current);
}

/*	This function executes a process for the number of instructions in its "time slice",
	which is held in the IC register of its PCB. It returns a 1 if the process has completed
	its execution (i.e., it has terminated), or a 0 if it has not yet terminated.
*/

int ExecuteProc(struct PCB *CurrentProc)
{
    //if nothing in ready queue, we have deadlocked
    //effective address = PC + BAR
    //time slice = IC
    //return 1 if process terminates, 0 if not yet terminated
    printf("Process with PID %d ready to begin execution of program ", CurrentProc->PID);
    printf("%s", CurrentProc->programName);
    printf(" and has time slice of %d\n", CurrentProc->PID, CurrentProc->IC);
    int IC = CurrentProc->IC;
    while (IC > 0)
	{

        for (i = 0; i < 6; i++)
            IR[i] = memory[PC][i];

		opcode = ((int) (IR[0])- 48) * 10;
		opcode += ((int) (IR[1])- 48);
        printf("PID: %d Instruction: %d IC: %d\n", CurrentProc->PID, opcode, IC);

		switch(opcode)
		{
            case 0: OP0(IR); PC++; break;

            case 1: OP1(IR); PC++; break;

            case 2: OP2(IR); PC++; break;

            case 3: OP3(IR); PC++; break;

            case 4: OP4(IR); PC++; break;

            case 5: OP5(IR); PC++; break;

            case 6: OP6(IR); PC++; break;

            case 7: OP7(IR); PC++; break;

            case 8: OP8(IR); PC++; break;

            case 9: OP9(IR); PC++; break;

            case 10: OP10(IR); PC++; break;

            case 11: OP11(IR); PC++; break;

            case 12: OP12(IR); PC++; break;

            case 13: OP13(IR); PC++; break;

            case 14: OP14(IR); PC++; break;

            case 15: OP15(IR); PC++; break;

            case 16: OP16(IR); PC++; break;

            case 17: OP17(IR); PC++; break;

            case 18: OP18(IR); PC++; break;

            case 19: OP19(IR); PC++; break;

            case 20: OP20(IR); PC++; break;

	        case 21: OP21(IR); PC++; break;

            case 22: OP22(IR); PC++; break;

            case 23: OP23(IR); PC++; break;

		    case 24: OP24(IR, PSW); PC++; break;

            case 25: OP25(IR, PSW); PC++; break;

            case 26: OP26(IR, PSW); PC++; break;

            case 27: OP27(IR, PSW); PC++; break;

            case 28: OP28(IR, PSW); PC++; break;

            case 29: OP29(IR,PSW); PC++; break;

		    case 30: OP30(IR, PSW); PC++; break;

		    case 31: OP31(IR, PSW); PC++; break;

            case 32: OP32(IR, PSW); PC++; break;

            case 33: OP33(IR, PSW, &PC); break;

            case 34: OP34(IR, PSW, &PC); break;

            case 35: OP35(IR, &PC); break;

            case 36: OP36(IR, &IC, CurrentProc); break;

            case 37: OP37(IR); PC++; break;

            case 99: printf("Process %d completed execution.\n", CurrentProc->PID);
                    printMEM(BaseRegister);
                    return 1;

            default: printf("Instruction %d not found!~\n", opcode);
			         exit(0);
		}
        IC--;
        printf("\n");
    }
    printf("\n\n\n");
    if(IC == 0)
    {
        printf("Process %d completed time slice. Placing at tail of ready queue.\n", CurrentProc->PID);
        //place at tail
        return 0;
    }
    else //we were blocked
    {
        return -1;
    }
}

/*	This function restores the state of the process that is set to begin its
	execution
*/

void RestoreState(struct PCB *NextProc)
{
    //give random time slice here
    //NextProc->IC = (rand() % 10) + 1; //random value between 1 and 10
    BaseRegister = NextProc->BaseReg;

    LimitRegister = NextProc->LimitReg;
    //now convert the PC from a logical to an effective address by adding
    //the value of the base register
    PC = NextProc->PC + BaseRegister;
    //PC = getEffectiveAddress(PC);
    P0 = NextProc-> P0;
    P1 = NextProc-> P1;
    P2 = NextProc-> P2;
    P3 = NextProc-> P3;
    //IC = NextProc-> IC;
    R0 = NextProc-> R0;
    R1 = NextProc-> R1;
    R2 = NextProc-> R2;
    R3 = NextProc-> R3;
    PSW[0] = NextProc-> PSW[0];
    PSW[1] = NextProc-> PSW[1];
    ACC = NextProc-> ACC;
}

/*	This function saves the state of the VM into the PCB of the process that
	just completed its "time slice"
*/

void SaveState(struct PCB **PrevProc)
{
    (*PrevProc)-> P0 = P0;
    (*PrevProc)-> P1 = P1;
    (*PrevProc)-> P2 = P2;
    (*PrevProc)-> P3 = P3;
    (*PrevProc)-> R0 = R0;
    (*PrevProc)-> R1 = R1;
    (*PrevProc)-> R2 = R2;
    (*PrevProc)-> R3 = R3;
    (*PrevProc)-> R0 = R0;
    (*PrevProc)-> ACC = ACC;
    //since PC is an effective address, we need to convert it back to
    //a logical address before saving it
    (*PrevProc)->PC = (PC-BaseRegister);
    (*PrevProc)-> PSW[0] = PSW[0];
    (*PrevProc)-> PSW[1] = PSW[1];
}

int parseOp1 (char *IR)
{
    //middle two chars are the digits
    //multiply one closes to left by 10 because is most significant digit
    int tens = (int) (IR[2] - 48) * 10;
    int ones = (int) (IR[3] - 48);
    return (tens + ones);
}

// returns the integer value of operand 2 when this operand is a two-byte integer.
int parseOp2 (char *IR)
{
    //last two chars are the digits
    //multiply one closes to left by 10 because is most significant digit
    int tens = (int) (IR[4] - 48) * 10;
    int ones = (int) (IR[5] - 48);
    return (tens + ones);
}

//returns the integer value of operands 1 and 2 combined to form a 4-byte integer.
int parseOP1andOP2Imm(char *IR)
{
    //last 4 chars are digits
    //multiply one closest to left by 1000 because is most significant digit
    int thousands = (int) (IR[2] - 48) * 1000;
    int hundreds = (int) (IR[3] - 48) * 100;
    int tens = (int) (IR[4] - 48) * 10;
    int ones = (int) (IR[5] - 48);
    return(thousands + hundreds + tens + ones);

}

// returns the register number of the register used as operand  1 of an instruction.
// Can be either Pointer or General-Purpose register.
int parseOp1Reg (char *IR)
{
    //register number will be fourth char in IR
    //make sure to test values
    return (int) (IR[3] - 48);
}

// returns the register number of a register used as operand  2 of an instruction.
// Can be either a Pointer or General-Purpose register.
int parseOp2Reg (char *IR)
{
    //register number will be 6th char in IR
    return (int) (IR[5] - 48);
}

// returns the data stored at memory location Memory_Location
int fetchData(int Memory_Location)
{
    //data is 4 digit number

    int thousands = (int) (memory[Memory_Location][2] - 48) * 1000;
    int hundreds = (int) (memory[Memory_Location][3] - 48) * 100;
    int tens = (int) (memory[Memory_Location][4] - 48) * 10;
    int ones = (int) (memory[Memory_Location][5] - 48);
    return(thousands + hundreds + tens + ones);
}

void printMemLine(int Memory_Location)
{
    //printf("char 0: %d\n", (memory[Memory_Location][0])-48);
    int testVal = (int)(memory[Memory_Location][0] - 48);

    if(testVal < 0)
    {
        printf("ZZZZZZ\n");
        return;
    }

    else if(testVal > 10 && memory[Memory_Location][0] != 'Z')
    {
        printf("ZZZZZZ\n");
        return;
    }

    for(i = 0; i < 6; i++)
    {
        printf("%c", memory[Memory_Location][i]);
    }
    printf("\n");
}

	//Prints out the contents of the IR on the same line.
void printIR(char *IR)
{
    return;
    for(i = 0; i < 6; i++)
    {
        printf("%c", IR[i]);
    }
    printf("\n");
}

//prints out the contents of memory from row 0 to row upto.
//This should print out all instructions and data stored in memory.
void printMEM(int upTo)
{
    int i = 0;
    for(i; i <= upTo; i++)
    {
        printMemLine(i);
    }
}

 //lets me only write this switch once
 //returns the value of the relevant pointer register
int getPointerReg(int PREG)
{
    switch(PREG)
    {
        case 0:
            return P0;
	        break;

        case 1:
            return P1;
	        break;

        case 2:
            return P2;
	        break;

        case 3:
            return P3;
	        break;

	    default: printf("Invalid pointer register index %d\n", PREG);
	        return -1;
	        break;
    }


}

//retrns value of relevant data register
int getDataReg(int REG)
{
    switch(REG)
    {
        case 0:
            return R0;
	        break;

        case 1:
            return R1;
	        break;

        case 2:
            return R2;
	        break;

        case 3:
            return R3;
	        break;

	    default: printf("Invalid data register index %d\n", REG);
	        return -1;
	        break;
    }
}

//converts an interger into characters and stores in memory
void storeData(int Memory_Location, int toStore)
{
    //convert int to chars and then shove into memory
    if(Memory_Location > 99)
    {
	    printf("Invalid memory location %d\n", Memory_Location);
        return;
    }
    int thousands = 0;
    int hundreds = 0;
    int tens = 0;
    int ones = 0;

    thousands = (toStore/1000);
    hundreds = ((toStore%1000)/100);
    tens = ((toStore%100)/10);
    ones = (toStore%10);
    memory[Memory_Location][0] = 'Z';
    memory[Memory_Location][1] = 'Z';
    memory[Memory_Location][2] = (char)(thousands + 48);
    memory[Memory_Location][3] = (char)(hundreds + 48);
    memory[Memory_Location][4] = (char)(tens + 48);
    memory[Memory_Location][5] = (char)(ones + 48);

}

//stores an integer in the specified data register
void storeToReg(int regNum, int toStore)
{
    switch(regNum)
    {
        case 0:
            R0 = toStore;
	        break;

        case 1:
            R1 = toStore;
	        break;

        case 2:
            R2 = toStore;
	        break;

        case 3:
            R3 = toStore;
	        break;

	    default: printf("Invalid data register index %d\n", regNum);
	        break;
    }
}

void grabFork(int registerNum, PCB *current, int *IC)
{
    printf("ACC is %d\n", ACC);
    if(getDataReg(registerNum) == 0)
    {
        wait(Forks[ACC], current);
        if(Forks[ACC]->count < 0)
        {
            *IC = -10;
        }
    }
    else
    {
        wait(Doorman, current);
    }
}

void giveFork(int registerNum, PCB *current, int PID)
{

    if(getDataReg(registerNum) == 0)
    {
        signal(Forks[ACC], PID);
    }
    else
    {
        printf("Doorman\n");
        signal(Doorman, current->PID);
    }
}

void OP0(char *IR)
{
    int PREG, VAL;
    printf("Opcode = 00. Load Pointer Immediate\n");
    printIR(IR);
    PREG = parseOp1Reg(IR);
    VAL = parseOp2(IR);
    switch(PREG)
    {
	    case 0: P0 = VAL;
	        break;

        case 1: P1 = VAL;
	        break;

        case 2: P2 = VAL;
	        break;

        case 3: P3 = VAL;
	        break;

        default: printf("Invalid pointer register, exiting \n");
	        break;
    }
    //Will print out junk until the two helper functions are implemented!
    printf("Loaded P[%d] with immediate value %d\n", PREG, VAL );
}

/* Now its your turn! Provide the prototype and implementation for the remaining opcodes. */

//add to pointer immediate
void OP1(char *IR)
{
    int PREG, VAL;
    printf("Opcode = 01. Add to Pointer Immediate\n");
    printIR(IR);
    PREG = parseOp1Reg(IR);
    VAL = parseOp2 (IR);

    switch(PREG)
    {
	    case 0: P0 += VAL;
	        break;

	    case 1: P1 += VAL;
	        break;

	    case 2: P2 += VAL;
	        break;

	    case 3: P3 += VAL;
	        break;

	    default: printf("Invalid pointer register, exiting \n");
	        break;
    }

    printf("Added immediate value %d to P[%d]\n", VAL, PREG);
}

//subtract from pointer immediate
void OP2(char *IR)
{
    int PREG, VAL;
    printf("Opcode = 02. Sbtract from Pointer Immediate\n");
    printIR(IR);
    PREG = parseOp1Reg(IR);
    VAL = parseOp2 (IR);

    switch(PREG)
    {
	    case 0: P0 -= VAL;
	        break;

	    case 1: P1 -= VAL;
	        break;

    	case 2: P2 -= VAL;
	        break;

	    case 3: P3 -= VAL;
	        break;
    }

    printf("Subtracted immediate value %d from P[%d]\n", VAL, PREG);
}

void OP3(char *IR)
{
    printf("Opcode = 03. Load Accumulator Immediate\n");
    printIR(IR);
    ACC = parseOP1andOP2Imm(IR);
    printf("Loaded immediate value %d to the accumulator\n", ACC);
}

void OP4(char *IR)
{
    int PREG, Memory_Location;
    printf("Opcode = 04. Load Accumulator Register Addressing\n");
    printIR(IR);
    PREG = parseOp1Reg(IR);
    Memory_Location = getPointerReg(PREG);
    //printf("Testing OP4. Memory location is %d, PREG is P[%d], ACC = %d\n", Memory_Location, PREG, ACC);
    printMemLine(Memory_Location);
    /*
    switch(PREG)
    {
        case 0: Memory_Location = P0;
	    break;

        case 1: Memory_Location = P1;
	    break;

        case 2: Memory_Location = P2;
	    break;

        case 3: Memory_Location = P3;
	    break;
    }*/

    ACC = fetchData(Memory_Location);
    printf("Loaded accumulator with value pointed to by P[%d], which is %d\n", PREG, ACC);
}

void OP5(char *IR)
{
    int Memory_Location;
    printf("Opcode = 05. Load Accumulator Direct\n");
    printIR(IR);
    Memory_Location = parseOp1(IR);

    ACC = fetchData(Memory_Location);
    printf("Loaded accumulator with value at memory address %d\n", Memory_Location);
}

//store acc reg
void OP6(char *IR)
{
    int PREG;
    short int loc;
    printf("Opcode = 06. Store Accumulator Register Addressing\n");
    printIR(IR);
    PREG = parseOp1Reg(IR);

    loc = getPointerReg(PREG);
    storeData(loc, ACC);
    printf("Loaded memory address in P[%d] with value in accumulator \n", PREG);

}

void OP7(char *IR)
{
    int loc;
    printf("Opcode = 07. Store Accumulator Direct Addressing\n");
    printIR(IR);
    loc = parseOp1(IR);

    storeData(loc, ACC);
    printf("Loaded memory address %d with value in accumulator \n", loc);
}

//store reg to mem reg addressing
void OP8(char *IR)
{
    int Memory_Location, PREG, DataReg, toStore;
    printf("Opcode = 08. Store Register to Memory Register Addressing\n");
    printIR(IR);

    DataReg = parseOp1Reg(IR); //set to reg number
    PREG = parseOp2Reg(IR); //set to reg number

    Memory_Location = getPointerReg(PREG);
    toStore = getDataReg(DataReg);

    storeData(Memory_Location, toStore);

    printf("Loaded register P[%d] with data %d in R[%d] \n", PREG, toStore, DataReg);

}

void OP9(char *IR)
{
    int Memory_Location, DataReg, toStore;
    printf("Opcode = 09. Store Register to Memory Direct Addressing\n");
    printIR(IR);

    DataReg = parseOp1Reg(IR); //set to reg number
    Memory_Location = parseOp2(IR);

    toStore = getDataReg(DataReg);

    storeData(Memory_Location, toStore);

    printf("Loaded register R[%d] with data at address %d \n", DataReg, Memory_Location);
}

void OP10(char *IR)
{
    //store TO register
    int Memory_Location, PREG, DataReg, toStore;
    printf("Opcode = 10. Load Register from Memory: Register Addressing\n");
    printIR(IR);

    DataReg = parseOp1Reg(IR); //set to reg number
    PREG = parseOp2Reg(IR); //set to reg number

    Memory_Location = getPointerReg(PREG); //get memory location from pointer reg
    toStore = fetchData(Memory_Location); //we now know what data to store

    storeToReg(DataReg, toStore);

    printf("Loaded register R[%d] with data value %d from P[%d] \n", DataReg, toStore, PREG);

}

void OP11(char *IR)
{
    //11RnXX
    int Memory_Location, DataReg, toStore;
    printf("Opcode = 11. Load Register from Memory: Direct Addressing\n");
    printIR(IR);

    DataReg = parseOp1Reg(IR); //set to reg number
    Memory_Location = parseOp2(IR); //parse address from instruction

    toStore = fetchData(Memory_Location);

    storeToReg(DataReg, toStore);

    printf("Loaded register R[%d] with data value %d from address %d \n", DataReg, toStore, Memory_Location);

}

void OP12(char *IR)
{
    int toStore;

    printf("Opcode = 12. Load Register Immediate\n");
    printIR(IR);

    toStore = parseOP1andOP2Imm(IR);

    storeToReg(0, toStore);

    printf("Loaded Register R[%d] with immediate value %d\n", 0, toStore);
}

void OP13(char *IR)
{
    //13RnRp contents of rp transfered to rn
    int sourceRegNum, targetRegNum, toTransfer, toStore;

    printf("Opcode = 13. Register to Register Transfer\n");
    printIR(IR);

    sourceRegNum = parseOp2Reg(IR);
    targetRegNum = parseOp1Reg(IR);
    toStore = getDataReg(sourceRegNum);
    storeToReg(targetRegNum, toStore);

    printf("Loaded Register R[%d] with contents of register R[%d]\n", targetRegNum, sourceRegNum);
}

void OP14(char *IR)
{
    //14RnZZ
    int regNum, toStore;
    printf("Opcode = 14. Load Accumulator from Register\n");
    printIR(IR);

    regNum = parseOp1Reg(IR); //parse register number
    toStore = getDataReg(regNum);//get the value at that register number, assign to toStore
    ACC = toStore; //set ACC = toStore
    //print the results

    printf("Loaded Accumulator with value %d from R[%d]\n", toStore, regNum);
}

void OP15(char *IR)
{
    //15RnZZ
    int regNum;
    printf("Opcode = 15. Load Register from Accumulator\n");
    printIR(IR);

    regNum = parseOp1Reg(IR);//parse register number
    storeToReg(regNum, ACC);//set register to ACC using storeToReg


    printf("Loaded Register R[%d] with value of Accumulator ( = %d)\n", regNum, ACC);
}

void OP16(char *IR)
{
    //16XXXX
    int toAdd, initAcc;
    printf("Opcode = 16. Add Accumulator Immediate\n");
    printIR(IR);

    toAdd = parseOP1andOP2Imm(IR);
    initAcc = ACC;
    ACC += toAdd;

    printf("Added immediate value %d to Accumulator (initial value = %d) for final value %d\n", toAdd, initAcc, ACC);
}

void OP17(char *IR)
{
    //17XXXX
    int toSubtract, initAcc;
    printf("Opcode = 17. Subtract Accumulator Immediate\n");
    printIR(IR);

    toSubtract = parseOP1andOP2Imm(IR);
    initAcc = ACC;
    ACC -= toSubtract;

    printf("Subtracted immediate value %d from Accumulator (initial value = %d) for final value %d\n", toSubtract, initAcc, ACC);
}

void OP18(char *IR)
{
    //18RnZZ
    int regNum, toAdd, initAcc;
    printf("Opcode = 18. Add Register to Accumulator\n");
    printIR(IR);

    initAcc = ACC;
    regNum = parseOp1Reg(IR); //parse register number
    toAdd = getDataReg(regNum); //get contents of register
    ACC += toAdd; //add contents to ACC
    //print results


    printf("Added value %d from R[%d] to ACC (initial value = %d) for final value %d\n", toAdd, regNum, initAcc, ACC);
}

void OP19(char *IR)
{
    //18RnZZ
    int regNum, toSubtract, initAcc;
    printf("Opcode = 19. Subtract Register from Accumulator\n");
    printIR(IR);

    initAcc = ACC;
    regNum = parseOp1Reg(IR); //parse register number
    toSubtract = getDataReg(regNum); //get contents of register
    ACC -= toSubtract; //add contents to ACC
    //print results


    printf("Subtracted value %d from R[%d] from ACC (initial value = %d) for final value %d\n", toSubtract, regNum, initAcc, ACC);
}

void OP20(char *IR)
{
    //20PnZZ
    int PREG, Memory_Address, toAdd, initAcc;
    printf("Opcode = 20. Add Accumulator: Register Addressing\n");
    printIR(IR);

    initAcc = ACC;
    PREG = parseOp1Reg(IR);//parse preg num
    Memory_Address = getPointerReg(PREG); //get memory address
    toAdd = fetchData(Memory_Address); //get value at address
    ACC += toAdd; //add to ACC
    //print results

    printf("Added value at address contained in P[%d] (= %d) to ACC (initial value = %d) for final value %d\n", PREG, toAdd, initAcc, ACC);
}

void OP21(char *IR)
{
    //21XXZZ
    int Memory_Address, toAdd, initAcc;
    printf("Opcode = 21. Add Accumulator: Direct Addressing\n");
    printIR(IR);

    initAcc = ACC;
    Memory_Address = parseOp1(IR);//parse memory address
    toAdd = fetchData(Memory_Address); //get value at address
    ACC += toAdd; //add to ACC
    //print results

    printf("Added value at address %d (= %d) to ACC (initial value = %d) for final value %d\n", Memory_Address, toAdd, initAcc, ACC);
}

void OP22(char *IR)
{
    //22PnZZ
    int PREG, Memory_Address, toSubtract, initAcc;
    printf("Opcode = 22. Subtract Accumulator: Register Addressing\n");
    printIR(IR);

    initAcc = ACC;
    PREG = parseOp1Reg(IR);//parse preg num
    Memory_Address = getPointerReg(PREG); //get memory address
    toSubtract = fetchData(Memory_Address); //get value at address
    ACC -= toSubtract; //subtract from ACC
    //print results

    printf("Subtracted value at address contained in P[%d] from ACC (initial value = %d) for final value %d\n", PREG, initAcc, ACC);
}

void OP23(char *IR)
{
    //23XXZZ
    int Memory_Address, toSubtract, initAcc;
    printf("Opcode = 23. Subtract Accumulator: Direct Addressing\n");
    printIR(IR);

    initAcc = ACC;
    Memory_Address = parseOp1(IR); //get memory address
    toSubtract = fetchData(Memory_Address); //get value at address
    ACC -= toSubtract; //subtract from ACC
    //print results

    printf("Subtracted value at address %d from ACC (initial value = %d) for final value %d\n", Memory_Address, initAcc, ACC);
}

void OP24(char *IR, char *PSW)
{
    //24PnZZ
    //If AC	==	M(Pn) then PSW[0] =	T else PSW[0] = F
    int PREG, Memory_Address, toCompare;
    printf("Opcode = 24. Compare Equal: Register Addressing\n");
    printIR(IR);

    PREG = parseOp1Reg(IR); //parse register number
    Memory_Address = getPointerReg(PREG); //get memory address
    toCompare = fetchData(Memory_Address); //get value at that address
    PSW[0] = (ACC == toCompare) ? 'T' : 'F'; //use fancy ternary operator to set PSW[0] to T or F
    printf("Compared Accumulator(= %d) Equal to value at address pointed to by P[%d](Result: %c)\n", ACC, PREG, PSW[0]);
}

void OP25(char *IR, char *PSW)
{
    //25PnZZ
    int PREG, Memory_Address, toCompare;
    printf("Opcode = 25. Compare Less: Register Addressing\n");
    printIR(IR);

    PREG = parseOp1Reg(IR); //parse register number
    Memory_Address = getPointerReg(PREG); //get memory address
    toCompare = fetchData(Memory_Address); //get value at that address
    PSW[0] = (ACC < toCompare) ? 'T' : 'F'; //use fancy ternary operator to set PSW[0] to T or F
    printf("Compared Accumulator(= %d) Less than value at address pointed to by P[%d](Result: %c)\n", ACC, PREG, PSW[0]);
}

void OP26(char *IR, char *PSW)
{
    //26PnZZ
    int PREG, Memory_Address, toCompare;
    printf("Opcode = 26. Compare Greater: Register Addressing\n");
    printIR(IR);

    PREG = parseOp1Reg(IR); //parse register number
    Memory_Address = getPointerReg(PREG); //get memory address
    toCompare = fetchData(Memory_Address); //get value at that address
    PSW[0] = (ACC > toCompare) ? 'T' : 'F'; //use fancy ternary operator to set PSW[0] to T or F
    printf("Compared Accumulator(= %d) Greater than value at address pointed to by P[%d](Result: %c)\n", ACC, PREG, PSW[0]);
}

void OP27(char *IR, char *PSW)
{
    //27XXXX
    int toCompare;
    printf("Opcode = 27. Compare Greater: Immediate Addressing\n");
    printIR(IR);

    toCompare = parseOP1andOP2Imm(IR); //parse value
    PSW[0] = (ACC > toCompare) ? 'T' : 'F'; //use fancy ternary operator to set PSW[0] to T or F
    printf("Compared Accumulator(= %d) Greater than value %d (Result: %c)\n", ACC, toCompare, PSW[0]);
}

void OP28(char *IR, char *PSW)
{
    //28XXXX
    int toCompare;
    printf("Opcode = 28. Compare Equal: Immediate Addressing\n");
    printIR(IR);

    toCompare = parseOP1andOP2Imm(IR); //parse value
    PSW[0] = (ACC == toCompare) ? 'T' : 'F'; //use fancy ternary operator to set PSW[0] to T or F
    printf("Compared Accumulator(= %d) Equal to value %d (Result: %c)\n", ACC, toCompare, PSW[0]);
}

void OP29(char *IR, char *PSW)
{
    //29XXXX
    int toCompare;
    printf("Opcode = 29. Compare Less: Immediate Addressing\n");
    printIR(IR);

    toCompare = parseOP1andOP2Imm(IR); //parse value
    PSW[0] = (ACC < toCompare) ? 'T' : 'F'; //use fancy ternary operator to set PSW[0] to T or F
    printf("Compared Accumulator(= %d) Less than value %d (Result: %c)\n", ACC, toCompare, PSW[0]);
}

void OP30(char *IR, char *PSW)
{
    //30RnZZ
    int regNum, toCompare;
    printf("Opcode = 30. Compare Register Equal\n");
    printIR(IR);

    regNum = parseOp1Reg(IR); //parse register number
    toCompare = getDataReg(regNum); //get value in register
    PSW[0] = (ACC == toCompare) ? 'T' : 'F'; //use fancy ternary operator to set PSW[0] to T or F
    printf("Compared Accumulator (= %d) Equal to value in register R[%d](= %d) (Result: %c)\n", ACC, regNum, toCompare, PSW[0]);
}

void OP31(char *IR, char *PSW)
{
    //31RnZZ
    int regNum, toCompare;
    printf("Opcode = 31. Compare Register Less\n");
    printIR(IR);

    regNum = parseOp1Reg(IR); //parse register number
    toCompare = getDataReg(regNum); //get value in register
    PSW[0] = (ACC < toCompare) ? 'T' : 'F'; //use fancy ternary operator to set PSW[0] to T or F
    printf("Compared Accumulator (= %d) Less than value in register R[%d] (= %d)(Result: %c)\n", ACC, regNum, toCompare, PSW[0]);
}

void OP32(char *IR, char *PSW)
{
    //32RnZZ
    int regNum, toCompare;
    printf("Opcode = 32. Compare Register Greater\n");
    printIR(IR);

    regNum = parseOp1Reg(IR); //parse register number
    toCompare = getDataReg(regNum); //get value in register
    PSW[0] = (ACC > toCompare) ? 'T' : 'F'; //use fancy ternary operator to set PSW[0] to T or F
    printf("Compared Accumulator (=%d) Greater than value in register R[%d] ( = %d)(Result: %c)\n", ACC, regNum, toCompare, PSW[0]);
}

void OP33(char *IR, char *PSW, short int *PC)
{
    //33XXZZ
    //psw = t, pc == xx
    int lineNum;
    printf("Opcode = 33. Branch Conditional True\n");
    printIR(IR);

    lineNum = parseOp1(IR);//parse XX
    //set PC to lineNum if true, increment if false
    *PC = (PSW[0] == 'T') ? lineNum : (*PC + 1); //more ternary magic feat. *PC, PSW, and lineNum

    printf("Jump to %d if PSW[0] (== %c) is true\n", lineNum, PSW[0]);
}

void OP34(char *IR, char *PSW, short int *PC)
{
    //34XXZZ
    int lineNum;
    printf("Opcode = 34. Branch Conditional False\n");
    printIR(IR);

    lineNum = parseOp1(IR);//parse XX
    *PC = (PSW[0] == 'F') ? lineNum : (*PC + 1); //more ternary magic feat. *PC, PSW, and lineNum

    printf("Jump to %d if PSW[0] (== %c) is false\n", lineNum, PSW[0]);
}

void OP35(char *IR, short int *PC)
{
    //35XXZZ
    int lineNum;
    printf("Opcode = 35. Branch Unconditional\n");
    printIR(IR);

    lineNum = parseOp1(IR);//parse XX
    *PC = lineNum; //set to line number

    printf("Jump to %d\n", lineNum);
}

void OP36(char *IR, int *IC, PCB *current)
{
    //36RXRY
    //sys call num placed in one of GP regs(OP1)
        //0 for wait, 1 for signal, 2 for getPID
    //for wait and signal, OP2 contains the GP reg containing the semaphore id
        //0 for fork semaphore, 1 for doorman
        //idx of fork placed in ACC before making call to fork semaphore
    //for getPID, OP2 indicates which reg to store PID into

    //step 1: what call are we making?
    int callNo = getDataReg(parseOp1Reg(IR));
    int reg = parseOp2Reg(IR);
    //now we know what call we're making

    printf("Fork Counts:\n");
    for(i = 0; i < 5; i++)
    {
        printf("%d\n", Forks[i]->count);
    }

    switch (callNo)
    {
        case 0:
            grabFork(reg, current, IC); //wait call
            break;

        case 1:
            giveFork(reg, current, current->PID); //signal
            break;

        case 2:
            printf("Philosopher %d has made a getpid system call\n", current->PID);
            int tmp = current->PID;
            storeToReg(reg, tmp); //getPID
            break;

        default:
            printf("Illegal system call, program terminating\n");
            exit(0);
    }

    PC++;
}

void OP37(char *IR)
{
    //op1 contains dividend
    int dividend = getDataReg(parseOp1Reg(IR));
    int divisor = getDataReg(parseOp2Reg(IR));
    printf("Modulo operation: %d \% %d \n", dividend, divisor);
    ACC = dividend % divisor;
    //op2 contains divisor
    //results stored in ACC
    //37RXRY
    //return RX%RY
}

void OP99(char *IR, short int *PC)
{
    printMEM(100);
}
