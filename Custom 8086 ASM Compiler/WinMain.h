/*
    Custom 8086 ASM Compiler
    Author : Crspy
    Do not delete this comment block. Respect others' work!
*/


#pragma once
#include "stdafx.h"
#include "resource.h"
#include "8086_Assembler.h"

void ProcessOpenFileButton(HWND hWnd, WPARAM wParam, LPARAM lParam, HWND hwndOpenFileBtn, HWND hwnd_ed_u, wchar_t(*szFile)[WCHAR_MAX], wchar_t** pSelectedFileName);

void ProcessCompileButton(HWND hWnd, WPARAM wParam, LPARAM lParam, HWND hwndCompileBtn, wchar_t** pSelectedFileName, BOOL bSortInFolders);

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void ProcessCmdLineCompilation(const wchar_t** pFirstLoadedFileName);

// Global variables

// The main window class name.
extern TCHAR szWindowClass[];

// The string that appears in the application's title bar.
extern TCHAR szTitle[];

extern HINSTANCE hInst;

extern const wchar_t* FirstLoadedFileName;
