/*
    Custom 8086 ASM Compiler
    Author : Amr Zaki ( aka Crspy )
    Do not delete this comment block. Respect others' work!
*/


#pragma once

#include "targetver.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <tchar.h>
#include <iostream>
#include <ios>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <Windows.h>
#include <map>
#include <vector>

bool is_alpha_only(const char* s);
bool is_numbers_only(const char *s);
bool DoesStringStartWithNumber(const char* line);
bool DoesStringContainArithmeticOperation(std::string& line);
template<class T>
bool is_blank_line(const T* s)
{
    while (*s)
    {
        if ((isblank(*s++) == 0))   return false;
    }

    return true;
}

