#pragma once
#include "stdafx.h"
#include "COpcode.h"
#include "CROMBlock.h"
#include "CBranching.h"

#define MAX_PC 32767

void ProcessCompile(const wchar_t* filename, BOOL bSortInFolders);
bool ProcessAllJmpInst(CROMBlock& myrom, std::map<std::string, uint32_t>& jmplabelsmap, std::map<std::string, 
    uint32_t>& labelsmap);
bool ProcessAllConstDataMovInst(CROMBlock& myrom, std::map<std::string, uint32_t>& constDataMovLabelsMap, 
    std::map<std::string, uint32_t>& constDataLabelsMap);
bool IsCommentLine(std::string& line);
bool DoesFileExist(const wchar_t* filename);
void CloseConsole();