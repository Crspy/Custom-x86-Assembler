/*
    Custom 8086 ASM Compiler
    Author : Amr Zaki ( aka Crspy )
    Do not delete this comment block. Respect others' work!
*/


#pragma once
#include "stdafx.h"

struct tMemAddress
{
    union
    {
        uint32_t m_Address = 0;
        struct
        {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        };
    };
    bool m_bNeedLoading;

    tMemAddress() : m_Address(0), m_bNeedLoading(false) {}

    tMemAddress(uint32_t memoryAddress) : m_Address(memoryAddress), m_bNeedLoading(false) {}



    bool InsureMovAddress()
    {
        if (this->m_Address > (32768))   
            return false; 

        this->m_Address += 0x8000; // to access the data segment 

        this->m_bNeedLoading = (this->m_Address > 0xFF) ? true : false; // will always be true anyway

        return true;
    }

    bool InsureJmpAddress()
    {
        if (this->m_Address > (0xFFFF))   return false; // we can't jmp to the data segment

        this->m_bNeedLoading = (this->m_Address > 0xFF) ? true : false;

        return true;
    }

    bool InsureDataAddress() const
    {
        return (this->m_Address > 32768) ? false : true;
    }

};
static_assert(sizeof(tMemAddress) == 8, "tMemAddress size is incorrect");
