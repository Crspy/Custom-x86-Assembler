/*
    Custom 8086 ASM Compiler
    Author : Crspy
    Do not delete this comment block. Respect others' work!
*/

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
    static eErrorType ProcessUnCondJumpAndCall(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<uint32_t , std::string>& jmplabelsmap, bool bIsCall);

    static eErrorType ProcessJumpIfZero(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<uint32_t , std::string>& jmplabelsmap,eCheckFlag checkflag);

    static eErrorType ProcessJumpIfEqual(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer, uint32_t PC,
        std::map<uint32_t , std::string>& jmplabelsmap, eCheckFlag checkflag);

    static eErrorType ProcessJumpIfFlag(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<uint32_t , std::string>& jmplabelsmap,eCheckFlag checkflag,eJumpFlag jumpflag);

    // for jmp if greater ,  jmp if greater or equal  ( for signed numbers )
    static eErrorType ProcessJumpIfGreater(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<uint32_t , std::string>& jmplabelsmap,eCheckFlag checkflag);

    // for jmp if above ,  jmp if above or equal  ( for unsigned numbers )
    static eErrorType ProcessJumpIfAbove(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<uint32_t, std::string>& jmplabelsmap, eCheckFlag checkflag);

    // for jmp if lesser ,  jmp if lesser or equal  ( for signed numbers )
    static eErrorType ProcessJumpIfLess(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<uint32_t , std::string>& jmplabelsmap, eCheckFlag checkflag);

    // for jmp if below ,  jmp if below or equal  ( for unsigned numbers )
    static eErrorType ProcessJumpIfBelow(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<uint32_t, std::string>& jmplabelsmap, eCheckFlag checkflag);

    static eErrorType ProcessJumpIfOverFlow(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<uint32_t , std::string>& jmplabelsmap,eCheckFlag checkflag);

    static eErrorType ProcessJumpIfSigned(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<uint32_t , std::string>& jmplabelsmap,eCheckFlag checkflag);

    static eErrorType ProcessJumpIfCarry(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
        std::map<uint32_t, std::string>& jmplabelsmap, eCheckFlag checkflag);

    static eErrorType ProcessReturn(tInstBlock * currentInst);

    static bool ProcessBranchingOpcodes(char* opToken,tMemAddress* memadd, tInstBlock* currentInst, char* linebuff,
        eErrorType* errortype,uint32_t PC,std::map<uint32_t , std::string>& jmplabelsmap,bool* bDoubleJmp);

};