// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include "stdafx.h"
#include "resource.h"
#include "8086_Assembler.h"


void ProcessOpenFileButton(HWND hWnd, WPARAM wParam, LPARAM lParam, HWND hwndOpenFileBtn, HWND hwnd_ed_u, wchar_t (*szFile)[WCHAR_MAX], wchar_t** pSelectedFileName);

void ProcessCompileButton(HWND hWnd, WPARAM wParam, LPARAM lParam, HWND hwndCompileBtn, wchar_t** pSelectedFileName,BOOL* bSortInFolders);

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = L"Custom 8086 ASM Compiler";

// The string that appears in the application's title bar.
static TCHAR szTitle[] = L"Custom 8086 ASM Compiler";

HINSTANCE hInst;


// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;


    int argc = 0;
    LPWSTR* argv = NULL;
    LPCWSTR cmdline = GetCommandLine();
    argv = CommandLineToArgvW(cmdline, &argc);
    if (argc > 1)
    {
        for (int i = 1; i != argc; i++)
        {
            if (AllocConsole())
            {
                freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
                freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
            }
            BOOL bSortInFolders = TRUE;
            ProcessCompile(argv[i],&bSortInFolders);
        }

    }



    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCWSTR)IDI_ICON1);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCWSTR)IDI_ICON1);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            L"Call to RegisterClassEx failed!",
            L"Error",
            MB_OK);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 200,
        NULL,
        NULL,
        hInstance,
        NULL
    );



    if (!hWnd)
    {
        MessageBox(NULL,
            L"Call to CreateWindow failed!",
            L"Error",
            MB_OK);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static BOOL bSortInFolders = true;
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = L"Hello, Welcome to 8086 ASM Compiler";
    static HWND hwndOpenFileBtn = NULL;
    static HWND hwndCompileBtn = NULL;
    static HWND hwnd_st_u = NULL, hwnd_ed_u = NULL;
    static wchar_t* SelectedFileName = NULL;
    static HWND hWndCheckBox = NULL;
    wchar_t szFile[WCHAR_MAX];       // buffer for file name


    GetWindowText(hwnd_ed_u, szFile, WCHAR_MAX);
    SelectedFileName = szFile;

    switch (message)
    {
    case WM_CREATE:
    {
        hWndCheckBox = CreateWindow(L"button",L"Sort In Folders",
            WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
            20, 20, 185, 35,
            hWnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        CheckDlgButton(hWnd, 1, BST_CHECKED);

        hwndOpenFileBtn = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"OpenFile",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            380,         // x position 
            5,         // y position 
            100,        // Button width
            50,        // Button height
            hWnd,     // Parent window
            NULL,       // No menu.
            (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
            NULL);

        hwndCompileBtn = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"Compile",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            380,         // x position 
            85,         // y position 
            100,        // Button width
            50,        // Button height
            hWnd,     // Parent window
            NULL,       // No menu.
            (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
            NULL);

        int x, w, y, h;
        y = 60; h = 20;
        x = 1; w = 60;
        hwnd_st_u = CreateWindow(L"static", L"ST_U",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            x, y, w, h,
            hWnd, (HMENU)(501),
            (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
        SetWindowText(hwnd_st_u, L"FilePath:");

        x += w; w = 300;
        hwnd_ed_u = CreateWindow(L"edit", L"",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP
            | ES_AUTOHSCROLL | WS_BORDER,
            x, y, w, h,
            hWnd, (HMENU)(502),
            (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
        SetWindowText(hwnd_ed_u, L"");
        //GetWindowText()

        break;
    }
    case WM_COMMAND:
    {


        if (((HWND)lParam == hwndOpenFileBtn) && (LOWORD(wParam) == BN_CLICKED))
            ProcessOpenFileButton(hWnd, wParam, lParam, hwndOpenFileBtn, hwnd_ed_u, &szFile, &SelectedFileName);
        else if (((HWND)lParam == hwndCompileBtn) && (LOWORD(wParam) == BN_CLICKED))
            ProcessCompileButton(hWnd, wParam, lParam, hwndCompileBtn, &SelectedFileName, &bSortInFolders);
        else if (IsDlgButtonChecked(hWnd, 1)) {
            CheckDlgButton(hWnd, 1, BST_UNCHECKED);
            bSortInFolders = false;
        }
        else {
            CheckDlgButton(hWnd, 1, BST_CHECKED);
            bSortInFolders = true;
        }
        break;
    }
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Here your application is laid out.
        // For this introduction, we just print out "Hello, Windows desktop!"
        // in the top left corner.
        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));
        // End application-specific layout section.

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}


void ProcessOpenFileButton(HWND hWnd, WPARAM wParam, LPARAM lParam, HWND hwndOpenFileBtn,HWND hwnd_ed_u,wchar_t (*szFile)[WCHAR_MAX], wchar_t** pSelectedFileName)
{
    OPENFILENAME ofn;       // common dialog box structure
    //HANDLE hf;              // file handle

                            // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = *szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = WCHAR_MAX;
    ofn.lpstrFilter = L"All\0*.*\0ASM\0*.asm\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box. 

    if (GetOpenFileName(&ofn) == TRUE)
    {
        *pSelectedFileName = *szFile;
        SetWindowText(hwnd_ed_u, *szFile);
    }

    //lstrcpyW(szFile,L"Sub opcode Trial.asm");
    //DoesFileExist(szFile);
    

    //getchar();
    //CloseConsole();

}

void ProcessCompileButton(HWND hWnd, WPARAM wParam, LPARAM lParam, HWND hwndCompileBtn, wchar_t** pSelectedFileName,BOOL* bSortInFolders)
{
    if (*pSelectedFileName == NULL)
        MessageBox(hWnd, L"Please Browse the file path first !", L"Error", MB_OK);
    else
    {
        if (AllocConsole())
        {
            _wfreopen_s((FILE**)stdout, L"CONOUT$", L"w", stdout);
            _wfreopen_s((FILE**)stdin, L"CONIN$", L"r", stdin);
        }
        ProcessCompile(*pSelectedFileName,bSortInFolders);
    }
}
