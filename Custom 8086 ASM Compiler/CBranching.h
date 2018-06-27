#pragma once
#include "COpcode.h"


enum eJumpFlag
{
    FLAG_ZF,
    FLAG_SF, // negative flag
    FLAG_CF,
    FLAG_OF
};

enum eCheckFlag
{
    FLAG_IF_NOT,
    FLAG_IF_TRUE,
    FLAG_IF_TRUE_OR_EQUAL // used only with jumps that contain Or Equal 
};


class CBranching
{
public:
    static eErrorType ProcessUnCondJump(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<std::string, uint32_t>& jmplabelsmap);

    static eErrorType ProcessJumpIfZero(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<std::string, uint32_t>& jmplabelsmap,eCheckFlag checkflag);

    static eErrorType ProcessJumpIfEqual(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer, uint32_t PC,
        std::map<std::string, uint32_t>& jmplabelsmap, eCheckFlag checkflag);

    static eErrorType ProcessJumpIfFlag(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<std::string, uint32_t>& jmplabelsmap,eCheckFlag checkflag,eJumpFlag jumpflag);

    // for jmp if greater ,  jmp if greater or equal
    static eErrorType ProcessJumpIfGreater(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<std::string, uint32_t>& jmplabelsmap,eCheckFlag checkflag);

    // for jmp if lesser ,  jmp if lesser or equal
    static eErrorType ProcessJumpIfLess(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<std::string, uint32_t>& jmplabelsmap, eCheckFlag checkflag);

    static eErrorType ProcessJumpIfOverFlow(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<std::string, uint32_t>& jmplabelsmap,eCheckFlag checkflag);

    static eErrorType ProcessJumpIfSigned(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<std::string, uint32_t>& jmplabelsmap,eCheckFlag checkflag);

    static eErrorType ProcessReturn(tInstBlock * currentInst);

    static bool ProcessBranchingOpcodes(char* opToken,tMemAddress* memadd, tInstBlock* currentInst, char* linebuff,
        eErrorType* errortype,uint32_t PC,std::map<std::string, uint32_t>& jmplabelsmap,bool* bDoubleJmp);

};