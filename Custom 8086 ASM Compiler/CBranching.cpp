/*
    Custom 8086 ASM Compiler
    Author : Crspy
    Do not delete this comment block. Respect others' work!
*/


#include "CBranching.h"



eErrorType CBranching::ProcessUnCondJumpAndCall(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
    std::map<uint32_t , std::string>& jmplabelsmap, bool bIsCall)
{
    if (bIsCall)
        currentInst[0].opcode = eOpcode::CALL;
    else
        currentInst[0].opcode = eOpcode::UNCOND_JUMP;


    char * token = strtok(nullptr, " [], \t");
    COpcode::EliminateComments(token); COpcode::EliminateTabs(token);
    printf("UnCondJmpOperand:%s\n", token);
    if (is_numbers_only(token))
    {
        memadd->m_Address = strtol(token, nullptr, 0);
        if (!memadd->InsureJmpAddress())
        {
            return eErrorType::MEM_ADDRESS_EXCEEDS;
        }
        if (memadd->m_bNeedLoading)
        {
            currentInst[1].address = memadd->byte0; // lowbyte for mov
            currentInst[1].opcode = currentInst[0].opcode;
           // currentInst[1].dir_flag = currentInst[0].dir_flag;
            currentInst[0].opcode = eOpcode::LOAD;
            //currentInst[0].dir_flag = 0;  
            currentInst[0].address = memadd->byte1; // highbyte for load
        }
        else
        {
            
            currentInst[0].address = memadd->byte0;           
        }
    }
    else if (!DoesStringStartWithNumber(token)) 
    {
        memadd->m_bNeedLoading = true; // needs loading by default 
        currentInst[1].opcode = currentInst[0].opcode;
        currentInst[0].opcode = eOpcode::LOAD;
        jmplabelsmap.insert(std::pair<uint32_t , std::string>(PC,token));
    }
    else
    {
        return eErrorType::LABEL_NAMES_CANT_START_WITH_A_NUMBER;

    }
    return eErrorType::NO_ERROR_DETECTED;
}

eErrorType CBranching::ProcessJumpIfZero(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer,
    uint32_t PC, std::map<uint32_t , std::string>& jmplabelsmap, eCheckFlag checkflag)
{
    return ProcessJumpIfFlag(memadd,currentInst,linebuffer,PC,jmplabelsmap,checkflag,eJumpFlag::FLAG_ZF);
}

eErrorType CBranching::ProcessJumpIfEqual(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer,
    uint32_t PC, std::map<uint32_t , std::string>& jmplabelsmap, eCheckFlag checkflag)
{
    return ProcessJumpIfFlag(memadd,currentInst,linebuffer,PC,jmplabelsmap,checkflag,eJumpFlag::FLAG_ZF);
}

eErrorType CBranching::ProcessJumpIfFlag(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer,
    uint32_t PC, std::map<uint32_t , std::string>& jmplabelsmap,eCheckFlag checkflag,eJumpFlag jumpflag)
{
    currentInst[0].const_condJump_opcode = eOpcode::COND_JUMP;
    currentInst[0].IfCheck_Flag = checkflag;// IF_TRUE  or  IF_NOT_TRUE
    currentInst[0].jmp_Flag = jumpflag;


    char * token = strtok(nullptr, " [], \t");
    COpcode::EliminateComments(token); COpcode::EliminateTabs(token);
    printf("CondJmpOperand:%s\n", token);
    if (is_numbers_only(token))
    {
        memadd->m_Address = strtol(token, nullptr, 0);
        if (!memadd->InsureJmpAddress())
        {
            return eErrorType::MEM_ADDRESS_EXCEEDS;
        }
        if (memadd->m_bNeedLoading)
        {
            currentInst[1].JMPaddress = memadd->byte0; // lowbyte for mov
            currentInst[1].const_condJump_opcode = currentInst[0].const_condJump_opcode;
            currentInst[1].IfCheck_Flag = currentInst[0].IfCheck_Flag;
            currentInst[1].jmp_Flag = currentInst[0].jmp_Flag;

            // for loading
            currentInst[0].reg_id = 0;
            currentInst[0].dir_flag = 0; 
            currentInst[0].opcode = eOpcode::LOAD;
            currentInst[0].address = memadd->byte1; // highbyte for load
        }
        else
        {           
            currentInst[0].JMPaddress = memadd->byte0;           
        }
    }
    else if (!DoesStringStartWithNumber(token)) 
    {
        memadd->m_bNeedLoading = true; // needs loading by default 

        currentInst[1].const_condJump_opcode = currentInst[0].const_condJump_opcode;
        currentInst[1].IfCheck_Flag = currentInst[0].IfCheck_Flag;
        currentInst[1].jmp_Flag = currentInst[0].jmp_Flag;

        currentInst[0].opcode = eOpcode::LOAD;
        currentInst[0].dir_flag = 0; 
        currentInst[0].reg_id = 0;

        jmplabelsmap.insert(std::pair<uint32_t , std::string>(PC ,token));
    }
    else
    {
        return eErrorType::LABEL_NAMES_CANT_START_WITH_A_NUMBER;

    }
    return eErrorType::NO_ERROR_DETECTED;
}

eErrorType CBranching::ProcessJumpIfGreater(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer,
    uint32_t PC, std::map<uint32_t , std::string>& jmplabelsmap, eCheckFlag checkflag)
{

    currentInst[0].const_condJump_opcode = eOpcode::COND_JUMP;
    currentInst[0].jmp_Flag = eJumpFlag::FLAG_ZF;
    currentInst[1].const_condJump_opcode = eOpcode::COND_JUMP;
    currentInst[1].jmp_Flag = eJumpFlag::FLAG_SF;
    

    if (checkflag == eCheckFlag::FLAG_IF_TRUE)
    {
        currentInst[0].IfCheck_Flag = 1;     // !ZF && !SF , but we invert ZF to skip jumping if ZF is true
        currentInst[1].IfCheck_Flag = 0;
    }
    else // GREATER OR EQUAL
    {
        currentInst[0].IfCheck_Flag = 1;    // ZF || !SF , we don't invert ZF here
        currentInst[1].IfCheck_Flag = 0;
    }


    tMemAddress nextJmpAddress(PC + 4);
    nextJmpAddress.InsureJmpAddress();

    char * token = strtok(nullptr, " [], \t");
    COpcode::EliminateComments(token); COpcode::EliminateTabs(token);
    printf("jg_Operand:%s\n", token);
    if (is_numbers_only(token))
    {    
        memadd->m_Address = strtol(token, nullptr, 0);
        if (!memadd->InsureJmpAddress())
        {
            return eErrorType::MEM_ADDRESS_EXCEEDS;
        }
        memadd->m_bNeedLoading = true; // force load
        if (memadd->m_bNeedLoading)
        {
            
            currentInst[3].const_condJump_opcode = currentInst[1].const_condJump_opcode;
            currentInst[3].IfCheck_Flag = currentInst[1].IfCheck_Flag;
            currentInst[3].jmp_Flag = currentInst[1].jmp_Flag;
            currentInst[3].JMPaddress = memadd->byte0; // lowbyte for mov

            currentInst[1].const_condJump_opcode = currentInst[0].const_condJump_opcode;
            currentInst[1].IfCheck_Flag = currentInst[0].IfCheck_Flag;
            currentInst[1].jmp_Flag = currentInst[0].jmp_Flag;
           

            // for loading
            currentInst[0].reg_id = 0;
            currentInst[0].dir_flag = 0; 
            currentInst[0].opcode = eOpcode::LOAD;
            if (checkflag == eCheckFlag::FLAG_IF_TRUE)
            {
                currentInst[0].address = nextJmpAddress.byte1; // highbyte for load
                currentInst[1].JMPaddress = nextJmpAddress.byte0; // lowbyte for mov
            }
            else // ZF || !SF
            {
                currentInst[0].address = memadd->byte1;
                currentInst[1].JMPaddress = memadd->byte0;
            }

            currentInst[2].reg_id = 0;
            currentInst[2].dir_flag = 0; 
            currentInst[2].opcode = eOpcode::LOAD;
            currentInst[2].address = memadd->byte1; // highbyte for load
        }
    }
    else if (!DoesStringStartWithNumber(token)) 
    {

       memadd->m_bNeedLoading = true; // needs loading by default 

       currentInst[3].const_condJump_opcode = currentInst[1].const_condJump_opcode;
       currentInst[3].IfCheck_Flag = currentInst[1].IfCheck_Flag;
       currentInst[3].jmp_Flag = currentInst[1].jmp_Flag;

       currentInst[1].const_condJump_opcode = currentInst[0].const_condJump_opcode;
       currentInst[1].IfCheck_Flag = currentInst[0].IfCheck_Flag;
       currentInst[1].jmp_Flag = currentInst[0].jmp_Flag;


       // for loading
       currentInst[0].reg_id = 0;
       currentInst[0].dir_flag = 0; 
       currentInst[0].opcode = eOpcode::LOAD;
       if (checkflag == eCheckFlag::FLAG_IF_TRUE)
       {
           currentInst[0].JMPaddress = nextJmpAddress.byte1; // highbyte for load
           currentInst[1].JMPaddress = nextJmpAddress.byte0; // lowbyte for mov
       }
       else // ZF || !SF
       {
           currentInst[0].JMPaddress = memadd->byte1;
           currentInst[1].JMPaddress = memadd->byte0;
       }

       currentInst[2].reg_id = 0;
       currentInst[2].dir_flag = 0; 
       currentInst[2].opcode = eOpcode::LOAD;
        

        jmplabelsmap.insert(std::pair<uint32_t , std::string>(PC + 2,token));
        if (checkflag != eCheckFlag::FLAG_IF_TRUE)
            jmplabelsmap.insert(std::pair<uint32_t, std::string>(PC , token));
    }
    else
    {
        return eErrorType::LABEL_NAMES_CANT_START_WITH_A_NUMBER;

    }
    return eErrorType::NO_ERROR_DETECTED;
}

eErrorType CBranching::ProcessJumpIfLess(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer, uint32_t PC,
    std::map<uint32_t , std::string>& jmplabelsmap, eCheckFlag checkflag)
{
    currentInst[0].const_condJump_opcode = eOpcode::COND_JUMP;
    currentInst[0].jmp_Flag = eJumpFlag::FLAG_ZF;
    currentInst[1].const_condJump_opcode = eOpcode::COND_JUMP;
    currentInst[1].jmp_Flag = eJumpFlag::FLAG_SF;
    

    if (checkflag == eCheckFlag::FLAG_IF_TRUE)
    {
        currentInst[0].IfCheck_Flag = 1; // !ZF && SF ,  but we invert ZF to skip jumping if ZF is true
        currentInst[1].IfCheck_Flag = 1;
    }
    else // LESS OR EQUAL
    {
        currentInst[0].IfCheck_Flag = 1; // ZF || SF , we don't invert ZF here
        currentInst[1].IfCheck_Flag = 1;
    }

    tMemAddress nextJmpAddress(PC + 4);
    nextJmpAddress.InsureJmpAddress();

    char * token = strtok(nullptr, " [], \t");
    COpcode::EliminateComments(token); COpcode::EliminateTabs(token);
    printf("jl_Operand:%s\n", token);
    if (is_numbers_only(token))
    {
        
        memadd->m_Address = strtol(token, nullptr, 0);
        if (!memadd->InsureJmpAddress())
        {
            return eErrorType::MEM_ADDRESS_EXCEEDS;
        }
        memadd->m_bNeedLoading = true; // force load
        if (memadd->m_bNeedLoading)
        {
            
            currentInst[3].const_condJump_opcode = currentInst[1].const_condJump_opcode;
            currentInst[3].IfCheck_Flag = currentInst[1].IfCheck_Flag;
            currentInst[3].jmp_Flag = currentInst[1].jmp_Flag;
            currentInst[3].JMPaddress = memadd->byte0; // lowbyte for mov

            currentInst[1].const_condJump_opcode = currentInst[0].const_condJump_opcode;
            currentInst[1].IfCheck_Flag = currentInst[0].IfCheck_Flag;
            currentInst[1].jmp_Flag = currentInst[0].jmp_Flag;


            // for loading
            currentInst[0].reg_id = 0;
            currentInst[0].dir_flag = 0; 
            currentInst[0].opcode = eOpcode::LOAD;
            if (checkflag == eCheckFlag::FLAG_IF_TRUE)
            {
                currentInst[0].JMPaddress = nextJmpAddress.byte1; // highbyte for load
                currentInst[1].JMPaddress = nextJmpAddress.byte0; // lowbyte for mov
            }
            else // ZF || SF 
            {
                currentInst[0].JMPaddress = memadd->byte1;
                currentInst[1].JMPaddress = memadd->byte0;
            }


            currentInst[2].reg_id = 0;
            currentInst[2].dir_flag = 0; 
            currentInst[2].opcode = eOpcode::LOAD;
            currentInst[2].JMPaddress = memadd->byte1; // highbyte for load
        }
    }
    else if (!DoesStringStartWithNumber(token)) 
    {

        memadd->m_bNeedLoading = true; // needs loading by default 

        currentInst[3].const_condJump_opcode = currentInst[1].const_condJump_opcode;
        currentInst[3].IfCheck_Flag = currentInst[1].IfCheck_Flag;
        currentInst[3].jmp_Flag = currentInst[1].jmp_Flag;
        currentInst[3].JMPaddress = memadd->byte0; // lowbyte for mov

        currentInst[1].const_condJump_opcode = currentInst[0].const_condJump_opcode;
        currentInst[1].IfCheck_Flag = currentInst[0].IfCheck_Flag;
        currentInst[1].jmp_Flag = currentInst[0].jmp_Flag;

        // for loading
        currentInst[0].reg_id = 0;
        currentInst[0].dir_flag = 0; 
        currentInst[0].opcode = eOpcode::LOAD;
        if (checkflag == eCheckFlag::FLAG_IF_TRUE)
        {
            currentInst[0].JMPaddress = nextJmpAddress.byte1; // highbyte for load
            currentInst[1].JMPaddress = nextJmpAddress.byte0; // lowbyte for mov
        }

        currentInst[2].reg_id = 0;
        currentInst[2].dir_flag = 0; 
        currentInst[2].opcode = eOpcode::LOAD;
        currentInst[2].address = memadd->byte1; // highbyte for load
        

        jmplabelsmap.insert(std::pair<uint32_t , std::string>(PC + 2, token));
        if (checkflag != eCheckFlag::FLAG_IF_TRUE)
            jmplabelsmap.insert(std::pair<uint32_t, std::string>(PC, token));
    }
    else
    {
        return eErrorType::LABEL_NAMES_CANT_START_WITH_A_NUMBER;

    }
    return eErrorType::NO_ERROR_DETECTED;
}

eErrorType CBranching::ProcessJumpIfOverFlow(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer, 
    uint32_t PC, std::map<uint32_t , std::string>& jmplabelsmap, eCheckFlag checkflag)
{
    return ProcessJumpIfFlag(memadd,currentInst,linebuffer,PC,jmplabelsmap, checkflag,eJumpFlag::FLAG_OF);
}

eErrorType CBranching::ProcessJumpIfSigned(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer,
    uint32_t PC, std::map<uint32_t , std::string>& jmplabelsmap, eCheckFlag checkflag)
{
    return ProcessJumpIfFlag(memadd,currentInst,linebuffer,PC,jmplabelsmap, checkflag,eJumpFlag::FLAG_SF);
}

eErrorType CBranching::ProcessReturn(tInstBlock * currentInst)
{
    currentInst[0].opcode = eOpcode::RETURN;    
    
    return eErrorType::NO_ERROR_DETECTED;
}

bool CBranching::ProcessBranchingOpcodes(char * opToken, tMemAddress * memadd, tInstBlock * currentInst,char * linebuff, 
    eErrorType * errortype, uint32_t PC, std::map<uint32_t , std::string>& jmplabelsmap,bool* bDoubleJmp)
{
    
    if (_strcmpi(opToken, "call") == 0)
    {
        *errortype = CBranching::ProcessUnCondJumpAndCall(memadd, currentInst, linebuff, PC, jmplabelsmap , true);
    }
    else if (_strcmpi(opToken, "jmp") == 0)
    {
        *errortype = CBranching::ProcessUnCondJumpAndCall(memadd, currentInst, linebuff, PC, jmplabelsmap , false);
    }
    else if (_strcmpi(opToken, "je") == 0)
    {
        *errortype = CBranching::ProcessJumpIfEqual(memadd, currentInst, linebuff, PC, jmplabelsmap, 
            eCheckFlag::FLAG_IF_TRUE);
    }
    else if (_strcmpi(opToken, "jne") == 0)
    {
        *errortype = CBranching::ProcessJumpIfEqual(memadd, currentInst, linebuff, PC, jmplabelsmap,eCheckFlag::FLAG_IF_NOT);
    }
    else if (_strcmpi(opToken, "jz") == 0)
    {
        *errortype = CBranching::ProcessJumpIfZero(memadd, currentInst, linebuff, PC, jmplabelsmap,eCheckFlag::FLAG_IF_TRUE);
    }
    else if (_strcmpi(opToken, "jnz") == 0)
    {
        *errortype = CBranching::ProcessJumpIfZero(memadd, currentInst, linebuff, PC, jmplabelsmap,eCheckFlag::FLAG_IF_NOT);
    }
    else if (_strcmpi(opToken, "jo") == 0)
    {
        *errortype = CBranching::ProcessJumpIfOverFlow(memadd, currentInst, linebuff, PC, jmplabelsmap,eCheckFlag::FLAG_IF_TRUE);
    }
    else if (_strcmpi(opToken, "jno") == 0)
    {
        *errortype = CBranching::ProcessJumpIfOverFlow(memadd, currentInst, linebuff, PC, jmplabelsmap, eCheckFlag::FLAG_IF_NOT);
    }
    else if (_strcmpi(opToken, "js") == 0)
    {
        *errortype = CBranching::ProcessJumpIfSigned(memadd, currentInst, linebuff, PC, jmplabelsmap, eCheckFlag::FLAG_IF_TRUE);
    }
    else if (_strcmpi(opToken, "jns") == 0)
    {
        *errortype = CBranching::ProcessJumpIfSigned(memadd, currentInst, linebuff, PC, jmplabelsmap, eCheckFlag::FLAG_IF_NOT);
    }
    else if (_strcmpi(opToken, "jg") == 0)
    {
        *bDoubleJmp = true;
        *errortype = CBranching::ProcessJumpIfGreater(memadd, currentInst, linebuff, PC, jmplabelsmap, eCheckFlag::FLAG_IF_TRUE);
    }
    else if (_strcmpi(opToken, "jge") == 0)
    {
        *bDoubleJmp = true;
        *errortype = CBranching::ProcessJumpIfGreater(memadd, currentInst, linebuff, PC, jmplabelsmap, eCheckFlag::FLAG_IF_TRUE_OR_EQUAL);
    }
    else if (_strcmpi(opToken, "jl") == 0)
    {
        *bDoubleJmp = true;
        *errortype = CBranching::ProcessJumpIfLess(memadd, currentInst, linebuff, PC, jmplabelsmap, eCheckFlag::FLAG_IF_TRUE);
    }
    else if (_strcmpi(opToken, "jle") == 0)
    {
        *bDoubleJmp = true;
        *errortype = CBranching::ProcessJumpIfLess(memadd, currentInst, linebuff, PC, jmplabelsmap, eCheckFlag::FLAG_IF_TRUE_OR_EQUAL);

    }
    else if (_strcmpi(opToken, "ret") == 0)
    {
        *errortype = CBranching::ProcessReturn(currentInst);
    }
    else
        return false;

    return true;
}

