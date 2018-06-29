/*
    Custom 8086 ASM Compiler
    Author : Crspy
    Do not delete this comment block. Respect others' work!
*/

#include "ErrorHandler.h"


void CErrorHandler::PrintErrorMessage(eErrorType errortype, unsigned long linecount,const char* jmpOrDataLabel)
{
    const char* message = nullptr;
    switch (errortype)
    {
    case USING_REGNAME_INSTEAD_OF_ADDRESS:
        message = "Compiler Error, using Reg name instead of a memory address...in line : %ld\n";
        break;
    case MEM_ADDRESS_EXCEEDS:
        message = "Compiler Error, Memory Address out of bounds, exceeds 0xFFFE...in line : %ld\n";
        break;
    case UNKNOWN_REG_NAME:
        message = "Compiler Error, unknown reg name....in line : %ld\n";
        break;
    case DATA_VALUE_OUTOFBOUNDS:
        message = "Compiler Error, data value is Out Of Bounds signed short integer...in line : %ld\n";
        break;
    case UNKNOWN_REG_NAME_OR_NOT_USING_BX_IN_LEFT_OPERAND:
        message = "Compiler Error, unknown reg name or not using bx in left operand...in line : %ld\n";
        break;
    case UNKNOWN_REG_NAME_OR_USING_BX_IN_RIGHT_OPERAND:
        message = "Compiler Error, unknown reg name or using bx in right operand...in line : %ld\n";
        break;
    case UNKNOWN_REG_NAME_OR_USING_BX_IN_LEFT_OPERAND:
        message = "Compiler Error, unknown reg name or using bx in left operand...in line : %ld\n";
        break;
    case UNKNOWN_REG_NAME_OR_NOT_USING_BX_IN_RIGHT_OPERAND:
        message = "Compiler Error, unknown reg name or not using bx in right operand...in line : %ld\n";
        break;
    case UNKNOWN_OPCODE:
        message = "Compiler Error, Unknown opcode... in line : %ld\n";
        break;
    case ROM_INSTRUCTION_SEGMENT_OVERFLOW:
        message = "Compiler Error, Rom Instruction Segment overflow \nPossible Solution: Reduce your code lines\n";
        break;
    case ONLY_ADDRESSES_OR_LABELS_ALLOWED:
        message = "Compiler Error, only labels or memory addresses allowed in jmp operation... in line : %ld\n";
        break;
    case LABEL_NAMES_CANT_START_WITH_A_NUMBER:
        message = "Compiler Error, label names can't start with a number... in line : %ld\n";
        break;
    case JUMPING_TO_NON_EXISTANT_LABEL:
        message = "Compiler Error, jumping to non existant Label : \"%s\"\n";
        break;
    case USING_SAME_NAME_FOR_TWO_LABELS:
        message = "Compiler Error, Using same name for two labels : \"%s\"\n";
        break;
    case DATA_LABEL_NOT_FOUND:
        message = "Compiler Error , Not defined data Label : \"%s\"\n";
    default:
        break;
    }
    if (message)
    {
        if (jmpOrDataLabel)
            printf(message, jmpOrDataLabel);
        else
            printf(message, linecount);
        printf("Press Enter to continue...\n");
        getchar();
    }
}