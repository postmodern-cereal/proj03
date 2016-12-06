//Noah Ransom
#ifndef FUNCS_H_INCLUDED
#define FUNCS_H_INCLUDED

#include "Vars.h"

void PrintQ(PCB *Head);
PCB *GetNextProcess(PCB **Head);
void MvToTail(PCB *Current, PCB **RQT);
int ExecuteProc(PCB *CurrentProc);
void RestoreState(PCB *NextProc);
void SaveState(PCB **PrevProc);

void LoadProgram(int PID, char *fileName, PCB **tmp);

int parseOp1 (char *IR);
int parseOp2 (char *IR);
int parseOP1andOP2Imm(char *IR);
int parseOp1Reg (char *IR);
int parseOp2Reg (char *IR);
int fetchData(int Memory_Location);
void printMemLine(int Memory_Location);
void printMEM();
void printIR(char *IR);
int getPointerReg(int PREG);
int getDataReg(int REG);
void storeData(int Memory_Location, int toStore);
void storeToReg(int regNum, int toStore);
int getEffectiveAddress(short int pc);

void OP0(char *IR);
void OP1(char *IR);
void OP2(char *IR);
void OP3(char *IR);
void OP4(char *IR);
void OP5(char *IR);
void OP6(char *IR);
void OP7(char *IR);
void OP8(char *IR);
void OP9(char *IR);
void OP10(char *IR);
void OP11(char *IR);
void OP12(char *IR);
void OP13(char *IR);
void OP14(char *IR);
void OP15(char *IR);
void OP16(char *IR);
void OP17(char *IR);
void OP18(char *IR);
void OP19(char *IR);
void OP20(char *IR);
void OP21(char *IR);
void OP22(char *IR);
void OP23(char *IR);
void OP24(char *IR, char *PSW);
void OP25(char *IR, char *PSW);
void OP26(char *IR, char *PSW);
void OP27(char *IR, char *PSW);
void OP28(char *IR, char *PSW);
void OP29(char *IR, char *PSW);
void OP30(char *IR, char *PSW);
void OP31(char *IR, char *PSW);
void OP32(char *IR, char *PSW);
void OP33(char *IR, char *PSW, short int *PC);
void OP34(char *IR, char *PSW, short int *PC);
void OP35(char *IR, short int *PC);
void OP36(char *IR, int *IC, PCB *Current);
void OP37(char *IR);
void OP99(char *IR, short int *PC);

#endif
