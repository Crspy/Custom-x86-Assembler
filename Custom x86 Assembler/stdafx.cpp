/*
    Custom 8086 ASM Compiler
    Author : Amr Zaki ( aka Crspy )
    Do not delete this comment block. Respect others' work!
*/


#include "stdafx.h"


bool is_alpha_only(const char* s)
{
    while (*s) 
    {
        if ((isalpha(*s++) == 0))   return false;
    }

    return true;
}

// either binary,decimal or hex
bool is_numbers_only(const char *s)
{
    while (*s) {
        if ((isxdigit(*s) == 0)
            && (isdigit(*s) == 0)
            && (*s != '\n')
            && (*s != '\t')
            && (*s != ' ')
            && (*s != '-')
            && (*s != 'x')
            && (*s != 'b')) return false;

        s++;
    }
    return true;
}

bool DoesStringStartWithNumber(const char* line)
{
    printf("%c\n",*line);
    if (*line) 
    {
        if (isdigit(*line) == 0) return false;
    }
    return true;
}

bool DoesStringContainArithmeticOperation(std::string& line)
{
    const char* pch = line.c_str();
    while (*pch) {
        if ( *pch == '+'
            ||*pch == '-'
            ||*pch == '*'
            ||*pch == '/'
            ||*pch == '%') return true;
        pch++;
    }
    return false;    
}