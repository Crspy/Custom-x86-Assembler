#include "CBranching.h"



eErrorType CBranching::ProcessUnCondJump(tMemAddress* memadd, tInstBlock* currentInst, char* linebuffer, uint32_t PC,
    std::map<std::string, uint32_t>& jmplabelsmap)
{
    currentInst[0].opcode = eOpcode::UNCOND_JUMP;


    char * token = strtok(nullptr, " [], \t");
    logger(token);
    COpcode::EliminateComments(token); COpcode::EliminateTabs(token);
    logger(token);
    if (is_numbers_only(token))
    {
        //logger("Numbers ONLY");
        memadd->m_Address = strtol(token, nullptr, 0);
        logger(token);
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
    else if (is_alpha_only(token)) 
    {
        //logger("ALPHA ONLY");
        //std::cin.get();
        memadd->m_bNeedLoading = true; // needs loading by default 
        currentInst[1].opcode = currentInst[0].opcode;
        currentInst[0].opcode = eOpcode::LOAD;
        std::string jmplabel(token);
        jmplabelsmap.insert(std::pair<std::string, uint32_t>(jmplabel, PC));
    }
    else
    {
        return eErrorType::ONLY_ADDRESSES_OR_LABELS_ALLOWED;

    }
    return eErrorType::NO_ERROR_DETECTED;
}

eErrorType CBranching::ProcessJumpIfZero(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer, uint32_t PC, std::map<std::string, uint32_t>& jmplabelsmap, eCheckFlag checkflag)
{
    return ProcessJumpIfFlag(memadd,currentInst,linebuffer,PC,jmplabelsmap,checkflag,eJumpFlag::FLAG_ZF);
}

eErrorType CBranching::ProcessJumpIfEqual(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer, uint32_t PC, std::map<std::string, uint32_t>& jmplabelsmap, eCheckFlag checkflag)
{
    return ProcessJumpIfFlag(memadd,currentInst,linebuffer,PC,jmplabelsmap,checkflag,eJumpFlag::FLAG_ZF);
}

eErrorType CBranching::ProcessJumpIfFlag(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer,
    uint32_t PC, std::map<std::string, uint32_t>& jmplabelsmap,eCheckFlag checkflag,eJumpFlag jumpflag)
{
    currentInst[0].const_condJump_opcode = eOpcode::COND_JUMP;
    currentInst[0].IfCheck_Flag = checkflag;// IF_TRUE  or  IF_NOT_TRUE
    currentInst[0].jmp_Flag = jumpflag;


    char * token = strtok(nullptr, " [], \t");
    logger(token);
    COpcode::EliminateComments(token); COpcode::EliminateTabs(token);
    logger(token);
    if (is_numbers_only(token))
    {
        //logger("Numbers ONLY");
        memadd->m_Address = strtol(token, nullptr, 0);
        logger(token);
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
    else if (is_alpha_only(token)) 
    {
        //logger("ALPHA ONLY");
        //std::cin.get();
        memadd->m_bNeedLoading = true; // needs loading by default 

        currentInst[1].const_condJump_opcode = currentInst[0].const_condJump_opcode;
        currentInst[1].IfCheck_Flag = currentInst[0].IfCheck_Flag;
        currentInst[1].jmp_Flag = currentInst[0].jmp_Flag;

        currentInst[0].opcode = eOpcode::LOAD;
        currentInst[0].dir_flag = 0; 
        currentInst[0].opcode = eOpcode::LOAD;

        std::string jmplabel(token);
        jmplabelsmap.insert(std::pair<std::string, uint32_t>(jmplabel, PC));
    }
    else
    {
        return eErrorType::ONLY_ADDRESSES_OR_LABELS_ALLOWED;

    }
    return eErrorType::NO_ERROR_DETECTED;
}

eErrorType CBranching::ProcessJumpIfGreater(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer,
    uint32_t PC, std::map<std::string, uint32_t>& jmplabelsmap, eCheckFlag checkflag)
{

    currentInst[0].const_condJump_opcode = eOpcode::COND_JUMP;
    currentInst[0].jmp_Flag = eJumpFlag::FLAG_ZF;
    currentInst[1].const_condJump_opcode = eOpcode::COND_JUMP;
    currentInst[1].jmp_Flag = eJumpFlag::FLAG_SF;
    currentInst[1].IfCheck_Flag = 0;

    if (checkflag == eCheckFlag::FLAG_IF_TRUE)
        currentInst[0].IfCheck_Flag = 0;
    else
        currentInst[0].IfCheck_Flag = 1;


    tMemAddress nextJmpAddress(PC + 2);
    nextJmpAddress.InsureJmpAddress();

    char * token = strtok(nullptr, " [], \t");
    logger(token);
    COpcode::EliminateComments(token); COpcode::EliminateTabs(token);
    logger(token);
    if (is_numbers_only(token))
    {
        
        //logger("Numbers ONLY");
        memadd->m_Address = strtol(token, nullptr, 0);
        logger(token);
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

            if (checkflag == eCheckFlag::FLAG_IF_TRUE)
                currentInst[1].JMPaddress = nextJmpAddress.byte0; // lowbyte for mov
            else
                currentInst[1].JMPaddress = memadd->byte0; // lowbyte for mov

            // for loading
            currentInst[0].reg_id = 0;
            currentInst[0].dir_flag = 0; 
            currentInst[0].opcode = eOpcode::LOAD;
            if (checkflag == eCheckFlag::FLAG_IF_TRUE)
                currentInst[0].address = nextJmpAddress.byte1; // highbyte for load
            else
                currentInst[0].address = memadd->byte1; // highbyte for load

            currentInst[2].reg_id = 0;
            currentInst[2].dir_flag = 0; 
            currentInst[2].opcode = eOpcode::LOAD;
            currentInst[2].address = memadd->byte1; // highbyte for load
        }
    }
    else if (is_alpha_only(token)) 
    {

        memadd->m_bNeedLoading = true; // needs loading by default 

        currentInst[3].const_condJump_opcode = currentInst[1].const_condJump_opcode;
        currentInst[3].IfCheck_Flag = currentInst[1].IfCheck_Flag;
        currentInst[3].jmp_Flag = currentInst[1].jmp_Flag;

        currentInst[1].const_condJump_opcode = currentInst[0].const_condJump_opcode;
        currentInst[1].IfCheck_Flag = currentInst[0].IfCheck_Flag;
        currentInst[1].jmp_Flag = currentInst[0].jmp_Flag;

        if (checkflag == eCheckFlag::FLAG_IF_TRUE)
            currentInst[1].JMPaddress = nextJmpAddress.byte0; // lowbyte for mov


        // for loading
        currentInst[0].reg_id = 0;
        currentInst[0].dir_flag = 0; 
        currentInst[0].opcode = eOpcode::LOAD;
        if (checkflag == eCheckFlag::FLAG_IF_TRUE)
            currentInst[0].address = nextJmpAddress.byte1; // highbyte for load

        currentInst[2].reg_id = 0;
        currentInst[2].dir_flag = 0; 
        currentInst[2].opcode = eOpcode::LOAD;
        

        std::string jmplabel(token);

        jmplabelsmap.insert(std::pair<std::string, uint32_t>(jmplabel, PC + 2));
        if (checkflag != eCheckFlag::FLAG_IF_TRUE)
        {
            jmplabelsmap.insert(std::pair<std::string, uint32_t>(jmplabel, PC));
        }
    }
    else
    {
        return eErrorType::ONLY_ADDRESSES_OR_LABELS_ALLOWED;

    }
    return eErrorType::NO_ERROR_DETECTED;
}

eErrorType CBranching::ProcessJumpIfLess(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer, uint32_t PC,
    std::map<std::string, uint32_t>& jmplabelsmap, eCheckFlag checkflag)
{
    currentInst[0].const_condJump_opcode = eOpcode::COND_JUMP;
    currentInst[0].jmp_Flag = eJumpFlag::FLAG_ZF;
    currentInst[1].const_condJump_opcode = eOpcode::COND_JUMP;
    currentInst[1].jmp_Flag = eJumpFlag::FLAG_SF;
    currentInst[1].IfCheck_Flag = 1;

    if (checkflag == eCheckFlag::FLAG_IF_TRUE)
        currentInst[0].IfCheck_Flag = 0;
    else
        currentInst[0].IfCheck_Flag = 1;


    tMemAddress nextJmpAddress(PC + 2);
    nextJmpAddress.InsureJmpAddress();

    char * token = strtok(nullptr, " [], \t");
    logger(token);
    COpcode::EliminateComments(token); COpcode::EliminateTabs(token);
    logger(token);
    if (is_numbers_only(token))
    {
        
        //logger("Numbers ONLY");
        memadd->m_Address = strtol(token, nullptr, 0);
        logger(token);
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

            if (checkflag == eCheckFlag::FLAG_IF_TRUE)
                currentInst[1].JMPaddress = nextJmpAddress.byte0; // lowbyte for mov
            else
                currentInst[1].JMPaddress = memadd->byte0; // lowbyte for mov

            // for loading
            currentInst[0].reg_id = 0;
            currentInst[0].dir_flag = 0; 
            currentInst[0].opcode = eOpcode::LOAD;
            if (checkflag == eCheckFlag::FLAG_IF_TRUE)
                currentInst[0].address = nextJmpAddress.byte1; // highbyte for load
            else
                currentInst[0].address = memadd->byte1; // highbyte for load

            currentInst[2].reg_id = 0;
            currentInst[2].dir_flag = 0; 
            currentInst[2].opcode = eOpcode::LOAD;
            currentInst[2].address = memadd->byte1; // highbyte for load
        }
    }
    else if (is_alpha_only(token)) 
    {

        memadd->m_bNeedLoading = true; // needs loading by default 

        currentInst[3].const_condJump_opcode = currentInst[1].const_condJump_opcode;
        currentInst[3].IfCheck_Flag = currentInst[1].IfCheck_Flag;
        currentInst[3].jmp_Flag = currentInst[1].jmp_Flag;

        currentInst[1].const_condJump_opcode = currentInst[0].const_condJump_opcode;
        currentInst[1].IfCheck_Flag = currentInst[0].IfCheck_Flag;
        currentInst[1].jmp_Flag = currentInst[0].jmp_Flag;

        if (checkflag == eCheckFlag::FLAG_IF_TRUE)
            currentInst[1].JMPaddress = nextJmpAddress.byte0; // lowbyte for mov


        // for loading
        currentInst[0].reg_id = 0;
        currentInst[0].dir_flag = 0; 
        currentInst[0].opcode = eOpcode::LOAD;
        if (checkflag == eCheckFlag::FLAG_IF_TRUE)
            currentInst[0].address = nextJmpAddress.byte1; // highbyte for load

        currentInst[2].reg_id = 0;
        currentInst[2].dir_flag = 0; 
        currentInst[2].opcode = eOpcode::LOAD;
        

        std::string jmplabel(token);

        jmplabelsmap.insert(std::pair<std::string, uint32_t>(jmplabel, PC + 2));
        if (checkflag != eCheckFlag::FLAG_IF_TRUE)
        {
            jmplabelsmap.insert(std::pair<std::string, uint32_t>(jmplabel, PC));
        }
    }
    else
    {
        return eErrorType::ONLY_ADDRESSES_OR_LABELS_ALLOWED;

    }
    return eErrorType::NO_ERROR_DETECTED;
}

eErrorType CBranching::ProcessJumpIfOverFlow(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer, uint32_t PC, std::map<std::string, uint32_t>& jmplabelsmap, eCheckFlag checkflag)
{
    return ProcessJumpIfFlag(memadd,currentInst,linebuffer,PC,jmplabelsmap, checkflag,eJumpFlag::FLAG_OF);
}

eErrorType CBranching::ProcessJumpIfSigned(tMemAddress * memadd, tInstBlock * currentInst, char * linebuffer, uint32_t PC, std::map<std::string, uint32_t>& jmplabelsmap, eCheckFlag checkflag)
{
    return ProcessJumpIfFlag(memadd,currentInst,linebuffer,PC,jmplabelsmap, checkflag,eJumpFlag::FLAG_SF);
}

eErrorType CBranching::ProcessReturn(tInstBlock * currentInst)
{
    memset(currentInst,0,sizeof(tInstBlock));

    currentInst[0].opcode = eOpcode::RETURN;    
    
    return eErrorType::NO_ERROR_DETECTED;
}

bool CBranching::ProcessBranchingOpcodes(char * opToken, tMemAddress * memadd, tInstBlock * currentInst,char * linebuff, 
    eErrorType * errortype, uint32_t PC, std::map<std::string, uint32_t>& jmplabelsmap,bool* bDoubleJmp)
{
    
    if (strcmp(opToken, "jmp") == 0)
    {
        logger("FOUND A JUMP");
        *errortype = CBranching::ProcessUnCondJump(memadd, currentInst, linebuff, PC, jmplabelsmap);
    }
    else if (strcmp(opToken, "je") == 0)
    {
        logger("FOUND A JUMP");
        *errortype = CBranching::ProcessJumpIfEqual(memadd, currentInst, linebuff, PC, jmplabelsmap, 
            eCheckFlag::FLAG_IF_TRUE);
    }
    else if (strcmp(opToken, "jne") == 0)
    {
        logger("FOUND A JUMP");
        *errortype = CBranching::ProcessJumpIfEqual(memadd, currentInst, linebuff, PC, jmplabelsmap,eCheckFlag::FLAG_IF_NOT);
    }
    else if (strcmp(opToken, "jz") == 0)
    {
        logger("FOUND A JUMP");
        *errortype = CBranching::ProcessJumpIfZero(memadd, currentInst, linebuff, PC, jmplabelsmap,eCheckFlag::FLAG_IF_TRUE);
    }
    else if (strcmp(opToken, "jnz") == 0)
    {
        logger("FOUND A JUMP");
        *errortype = CBranching::ProcessJumpIfZero(memadd, currentInst, linebuff, PC, jmplabelsmap,eCheckFlag::FLAG_IF_NOT);
    }
    else if (strcmp(opToken, "jo") == 0)
    {
        logger("FOUND A JUMP");
        *errortype = CBranching::ProcessJumpIfOverFlow(memadd, currentInst, linebuff, PC, jmplabelsmap,eCheckFlag::FLAG_IF_TRUE);
    }
    else if (strcmp(opToken, "jno") == 0)
    {
        logger("FOUND A JUMP");
        *errortype = CBranching::ProcessJumpIfOverFlow(memadd, currentInst, linebuff, PC, jmplabelsmap, eCheckFlag::FLAG_IF_NOT);
    }
    else if (strcmp(opToken, "js") == 0)
    {
        logger("FOUND A JUMP");
        *errortype = CBranching::ProcessJumpIfSigned(memadd, currentInst, linebuff, PC, jmplabelsmap, eCheckFlag::FLAG_IF_TRUE);
    }
    else if (strcmp(opToken, "jns") == 0)
    {
        logger("FOUND A JUMP");
        *errortype = CBranching::ProcessJumpIfSigned(memadd, currentInst, linebuff, PC, jmplabelsmap, eCheckFlag::FLAG_IF_NOT);
    }
    else if (strcmp(opToken, "jg") == 0)
    {
        logger("FOUND A JUMP");
        *bDoubleJmp = true;
        *errortype = CBranching::ProcessJumpIfGreater(memadd, currentInst, linebuff, PC, jmplabelsmap, eCheckFlag::FLAG_IF_TRUE);
    }
    else if (strcmp(opToken, "jge") == 0)
    {
        logger("FOUND A JUMP");
        *bDoubleJmp = true;
        *errortype = CBranching::ProcessJumpIfGreater(memadd, currentInst, linebuff, PC, jmplabelsmap, eCheckFlag::FLAG_IF_TRUE_OR_EQUAL);
    }
    else if (strcmp(opToken, "jl") == 0)
    {
        logger("FOUND A JUMP");
        *bDoubleJmp = true;
        *errortype = CBranching::ProcessJumpIfLess(memadd, currentInst, linebuff, PC, jmplabelsmap, eCheckFlag::FLAG_IF_TRUE);
    }
    else if (strcmp(opToken, "jle") == 0)
    {
        logger("FOUND A JUMP");
        *bDoubleJmp = true;
        *errortype = CBranching::ProcessJumpIfLess(memadd, currentInst, linebuff, PC, jmplabelsmap, eCheckFlag::FLAG_IF_TRUE_OR_EQUAL);

    }
    else if (strcmp(opToken, "ret") == 0)
    {
        *errortype = CBranching::ProcessReturn(currentInst);
    }
    else
        return false;

    return true;
}

