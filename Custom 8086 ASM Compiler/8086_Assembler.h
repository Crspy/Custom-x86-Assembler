#pragma once
#include "stdafx.h"
#include "COpcode.h"
#include "CROMBlock.h"

#define MAX_PC 32767

void ProcessCompile(const wchar_t* filename, BOOL bSortInFolders);
bool ProcessAllJmpInst(CROMBlock& myrom, std::map<std::string, uint32_t>& jmplabelsmap, std::map<std::string, uint32_t>& labelsmap);
bool IsCommentLine(std::string& line);
bool DoesFileExist(const wchar_t* filename);
void CloseConsole();