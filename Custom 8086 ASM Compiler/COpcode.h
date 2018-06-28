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
    MOVE_IN = 1,
    MOVE_OUT = MOVE_IN,
    LOAD = 3,
    INDIRECT_IN = 4,
    INDIRECT_OUT = INDIRECT_IN,
    //place holders
    ALU = 6,
    CALL = 7,
    COND_JUMP = 8,
    RETURN = 9,
    UNCOND_JUMP = 10,
    INSTRUCTION_OUT = 11,
    DATA_OUT = 12
    
};

enum eOpcodeDir : uint8_t
{
    DIR_IN,
    DIR_OUT
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
    iSUB,
    DIV,
    MODULUS,
    MUL,
    SHIFT_RIGHT,
    SHIFT_LEFT,
    COMPARE,
    iDIV // place holder
};



class COpcode
{
public:
    static void EliminateComments(char* line);
    static void EliminateTabs(char* line);

    static int8_t GetRegID(const char* lineReg);

    static eOpcodeDir GetOpcodeDir(std::string& line);

    static eErrorType ProcessMoveOUT(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer,
        bool* bMovingData, CROMBlock* myrom,std::map<std::string, uint32_t>& constDataMovLabelsMap,uint32_t PC);
    
    static eErrorType ProcessMoveIN(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer,
        std::map<std::string, uint32_t>& constDataMovLabelsMap,uint32_t PC);

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
    static eErrorType ProcessMul(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessDiv(tInstBlock* currentInst, char* linebuffer);
    //static eErrorType ProcessiAdd(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiSub(tInstBlock* currentInst, char* linebuffer);
    //static eErrorType ProcessiMul(tInstBlock* currentInst, char* linebuffer);
    static eErrorType ProcessiDiv(tInstBlock* currentInst, char* linebuffer);

    static bool ProcessALUOpcodes(char* opToken, tInstBlock* currentInst, char* linebuff, eErrorType* errortype);

    static eErrorType ProcessConstData(tMemAddress* memadd, tInstBlock* currentInst, std::string& line, uint32_t PC,
        std::map<std::string, uint32_t>& datalabelsmap,bool* bMovingData, CROMBlock* myrom);


    static eErrorType ProcessInstOut(tInstBlock* currentInst, char* linebuffer);

    static eErrorType ProcessDataOut(tInstBlock* currentInst, char* linebuffer);
    



};