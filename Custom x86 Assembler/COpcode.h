/*
    Custom 8086 ASM Compiler
    Author : Amr Zaki ( aka Crspy )
    Do not delete this comment block. Respect others' work!
*/

#pragma once
#include "stdafx.h"
#include "CROMBlock.h"
#include "tMemAddress.h"
#include "ErrorHandler.h"


enum eRegID : uint8_t
{
    AX,
    BX,
    CX,
    DX
};

enum eOpcode : uint8_t
{
    BREAK_POINT = 0,
    MOVE_IN = 1,
    MOVE_OUT = MOVE_IN,
    IMM_MOVE = 2,
    LOAD = 3,
    INDIRECT_IN = 4,
    INDIRECT_OUT = INDIRECT_IN,

    ALU = 6,
    CALL = 7,
    COND_JUMP = 8,
    RETURN = 9,
    UNCOND_JUMP = 10,
    DATA_OUT = 11,
    NO_OPERATION = 13,
    DATA_IN = 14
};

enum eOpcodeDir : uint8_t
{
    DIR_IN,
    DIR_OUT,
    DIR_IMM
};

enum eALUOpcode
{
    

    ADD,
    NOT,
    TRANSFER,
    INC,
    XOR,
    SUB,
    DEC,
    OR,
    AND,
    RSUB, // reversed sub
    DIV,
    MODULUS,
    MUL,
    SHIFT_RIGHT,
    SHIFT_LEFT,
    COMPARE,

/*
    iADD,
    iNOT,
    iTRANSFER,
    iINC,
    iXOR,
    iSUB,
    iDEC,
    iOR,
    iAND,
    iRSUB, // reversed sub
    iDIV,
    iMODULUS,
    iMUL,
    iSHIFT_RIGHT,
    iSHIFT_LEFT,
    iCOMPARE,
    */


};



class COpcode
{
public:
    static void EliminateComments(char* line);
    static void EliminateTabs(char* line);

    static int8_t GetRegID(const char* lineReg);

    static eOpcodeDir GetOpcodeDir(std::string& line, eErrorType* errortype);


    static eErrorType COpcode::ProcessImmMove(tMemAddress* memadd, tInstBlock* currentInst, std::string& line,
        std::map<uint32_t, std::string>& constDataMovLabelsMap, uint32_t PC);

    static eErrorType ProcessMoveOUT(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer,
        bool* bMovingData, CROMBlock* myrom,std::map<uint32_t,std::string>& constDataMovLabelsMap,uint32_t PC);
    
    static eErrorType ProcessMoveIN(tMemAddress* memadd, tInstBlock* currentInst, std::string& line,
        std::map< uint32_t, std::string>& constDataMovLabelsMap,uint32_t PC);

    static eErrorType ProcessIndirectMoveOUT(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer);

    static eErrorType ProcessIndirectMoveIN(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer);

    static eErrorType ProcessCompare(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessShiftRight(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessShiftLeft(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessNot(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessTransfer(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessInc(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessDec(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessXor(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessOR(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessAND(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessModulus(tInstBlock* currentInst, char* linebuffer);

    static eErrorType ProcessAdd(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessSub(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessReversedSub(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessMul(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessDiv(tInstBlock* currentInst, char* linebuffer);    


/*
    static eErrorType ProcessiCompare(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiShiftRight(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiShiftLeft(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiNot(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiTransfer(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiInc(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiDec(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiXor(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiOR(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiAND(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiModulus(tInstBlock* currentInst, char* linebuffer);

    static eErrorType ProcessiAdd(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiSub(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiReversedSub(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiMul(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiDiv(tInstBlock* currentInst, char* linebuffer);
*/

    static bool ProcessALUOpcodes(char* opToken, tInstBlock* currentInst, char* linebuff, eErrorType* errortype);

    static eErrorType ProcessConstData(tMemAddress* memadd, tInstBlock* currentInst, std::string& line, uint32_t PC,
        std::map<std::string, uint32_t>& datalabelsmap,bool* bMovingData, CROMBlock* myrom);


    static eErrorType ProcessInstOut(tInstBlock* currentInst, char* linebuffer);

    static eErrorType ProcessDataOut(tInstBlock* currentInst, char* linebuffer);

    static eErrorType ProcessNoOperation(tInstBlock * currentInst);

    static eErrorType ProcessBreakPoint(tInstBlock * currentInst);

	static eErrorType ProcessInput(tInstBlock* currentInst, char* linebuffer);
};
