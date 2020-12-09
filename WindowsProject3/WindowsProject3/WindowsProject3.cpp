// WindowsProject3.cpp : Определяет точку входа для приложения.
//
#pragma comment (lib, "Msimg32.lib")
#include "framework.h"
#include "WindowsProject3.h"
#include <vector>
#include <commdlg.h>
#include <commctrl.h>
#include <wingdi.h>

#define MAX_LOADSTRING 100
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
int size = 1;
bool LMouseIsClicked = false;
POINT LastP;
POINT NewP;
std::vector<POINT> points;
int CurDrawMode = 1;
std::vector<HBITMAP> hbmMemory;
std::vector<HDC> hdcMemory;
std::vector<POINT> figMem;
int index = -1;
HPEN pen = CreatePen(PS_SOLID, 2*size, RGB(0, 255, 255));
COLORREF lastColor = RGB(0, 255, 255);
HBRUSH brush = (HBRUSH)GetStockObject(NULL_BRUSH);
HPEN erasePen = CreatePen(PS_SOLID, 2 * size, RGB(0, 0, 0));
bool saveFlag = false;
bool loadFlag = true;
bool missFlag = false;
HDC hdcMem;
HBITMAP hbmMem;
POINT WindSize;
HBITMAP hbmScreen;
char num[2];
HWND hPenc, hLine, hBack, hForward, hCircle, hRect, hPen, hEraser, hWidth, hFill, hClear;


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT3, szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT3));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = 0;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT3));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = 0;
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT3);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
bool MouseInClientZone(HWND hWnd)
{
    RECT rt;
    POINT cur;
    GetCursorPos(&cur);
    GetClientRect(hWnd, &rt);
    POINT zeroP = { 0,0 };
    ClientToScreen(hWnd, &zeroP);
    rt.left += zeroP.x;
    rt.right += zeroP.x;
    rt.top += zeroP.y;
    rt.bottom += zeroP.y;
    return (cur.x >= rt.left) && (cur.x <= rt.right) && (cur.y >= rt.top) && (cur.y <= rt.bottom);
}
void CheckDrawing(HWND hWnd)
{
    if (!MouseInClientZone(hWnd) || IsIconic(hWnd))
    {
        LMouseIsClicked = false;
        figMem.clear();
        loadFlag = true;
        InvalidateRect(hWnd, NULL, TRUE);
    }
}

CHOOSECOLOR GetColorD(HWND hWndc)
{
    CHOOSECOLOR cc;
    static COLORREF acrCustClr[16];
    static DWORD rgbCurrent;

    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hWndc;
    cc.lpCustColors = (LPDWORD)acrCustClr;
    cc.rgbResult = rgbCurrent;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    return cc;
}

void DrawLine(HDC hdc, POINT temp1, POINT temp2)
{
    MoveToEx(hdc, temp2.x, temp2.y, NULL);
    LineTo(hdc, temp1.x, temp1.y);
}
void DrawCircle(HDC hdc, POINT temp1, POINT temp2)
{
    Ellipse(hdc, temp1.x, temp1.y, temp2.x, temp2.y);
}
void DrawRect(HDC hdc, POINT temp1, POINT temp2)
{
    Rectangle(hdc, temp1.x, temp1.y, temp2.x, temp2.y);
}
void Paint(HDC hdc, POINT temp1)
{
    HBRUSH tempbrush = (HBRUSH)CreateSolidBrush(lastColor);
    SelectObject(hdc, tempbrush);
    ExtFloodFill(hdc, temp1.x, temp1.y, GetPixel(hdc, temp1.x, temp1.y), FLOODFILLSURFACE);
    DeleteObject(tempbrush);
}

BOOL SaveHBITMAPToFile(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
    HDC hDC;
    int iBits;
    WORD wBitCount;
    DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
    BITMAP Bitmap0;
    BITMAPFILEHEADER bmfHdr;
    BITMAPINFOHEADER bi;
    LPBITMAPINFOHEADER lpbi;
    HANDLE fh, hDib, hPal, hOldPal2 = NULL;
    hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
    iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
    DeleteDC(hDC);
    if (iBits <= 1)
        wBitCount = 1;
    else if (iBits <= 4)
        wBitCount = 4;
    else if (iBits <= 8)
        wBitCount = 8;
    else
        wBitCount = 24;
    GetObject(hBitmap, sizeof(Bitmap0), (LPSTR)&Bitmap0);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = Bitmap0.bmWidth;
    bi.biHeight = -Bitmap0.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = wBitCount;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrImportant = 0;
    bi.biClrUsed = 256;
    dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount + 31) & ~31) / 8
        * Bitmap0.bmHeight;
    hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;

    hPal = GetStockObject(DEFAULT_PALETTE);
    if (hPal)
    {
        hDC = GetDC(NULL);
        hOldPal2 = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
        RealizePalette(hDC);
    }


    GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
        + dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

    if (hOldPal2)
    {
        SelectPalette(hDC, (HPALETTE)hOldPal2, TRUE);
        RealizePalette(hDC);
        ReleaseDC(NULL, hDC);
    }

    fh = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (fh == INVALID_HANDLE_VALUE)
        return FALSE;

    bmfHdr.bfType = 0x4D42; // "BM"
    dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
    bmfHdr.bfSize = dwDIBSize;
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

    WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
    GlobalUnlock(hDib);
    GlobalFree(hDib);
    CloseHandle(fh);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case(WM_CREATE):
        RECT rt;
        SendMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
        GetClientRect(hWnd, &rt);
        WindSize.x = rt.right;
        WindSize.y = rt.bottom;
        SetTimer(hWnd, 1, 100, NULL);
        hPenc = CreateWindow(L"button", L"Карандаш",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            0, 0, 90, 30, hWnd, 0, hInst, NULL);
        hLine = CreateWindow(L"button", L"Прямая",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            90, 0, 90, 30, hWnd, 0, hInst, NULL);
        hBack = CreateWindow(L"button", L"назад",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            0, 30, 90, 30, hWnd, 0, hInst, NULL);
        hForward = CreateWindow(L"button", L"Вперед",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            90, 30, 90, 30, hWnd, 0, hInst, NULL);
        hCircle = CreateWindow(L"button", L"Окружность",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            180, 0, 90, 30, hWnd, 0, hInst, NULL);
        hRect = CreateWindow(L"button", L"Прямоугольник",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            270, 0, 90, 30, hWnd, 0, hInst, NULL);
        hPen = CreateWindow(L"button", L"Цвет кисти",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            360, 0, 90, 30, hWnd, 0, hInst, NULL);
        hEraser = CreateWindow(L"button", L"Стёрка",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            450, 0, 90, 30, hWnd, 0, hInst, NULL);
        hWidth = CreateWindow(_T("ComboBox"), _T("aa"), WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
            540, 0, 90, 400,
            hWnd, 0, hInst, NULL);
        hFill = CreateWindow(L"button", L"Закрасить",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            630, 0, 90, 30, hWnd, 0, hInst, NULL);
        hClear = CreateWindow(L"button", L"Очистить экран",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            810, 0, 120, 30, hWnd, 0, hInst, NULL);
        for (int i = 1; i < 10; i++)
        {
            _itoa_s(i, num, 10);
            SendMessage(hWidth, CB_ADDSTRING, 1, (LPARAM)num);
        }
        SendMessage(hWidth, CB_SETCURSEL, 0, 0L);
        break;
    case(WM_TIMER):
        CheckDrawing(hWnd);
        break;

//######################################
    case(WM_LBUTTONDOWN):
        LMouseIsClicked = true;
        missFlag = true;
        switch (CurDrawMode)
        {
        case 1:
        case 5:
        {
            LastP = { LOWORD(lParam), HIWORD(lParam) };
            NewP = LastP;
        }
        break;
        case 2:
        case 3:
        case 4:
        {
            LastP = { LOWORD(lParam), HIWORD(lParam) };
        }
        break;
        case 6:
            LastP = { LOWORD(lParam), HIWORD(lParam) };
            saveFlag = true;
            InvalidateRect(hWnd, NULL, TRUE);
        break;
        default:
            break;
        }

        break;
    case(WM_LBUTTONUP):
        LMouseIsClicked = false;
        switch (CurDrawMode)
        {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            if (!missFlag)
            {
                saveFlag = true;
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            }
        }
        break;
    case WM_MOUSEMOVE:
    {
        if (LMouseIsClicked)
        {
            switch (CurDrawMode)
            {
            case 1:
            case 5:
                missFlag = false;
                LastP = NewP;
                NewP = { LOWORD(lParam), HIWORD(lParam) };
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case 2:
            case 3:
            case 4:
                missFlag = false;
                NewP = { LOWORD(lParam), HIWORD(lParam) };
                InvalidateRect(hWnd, NULL, TRUE);
                break;

            default:
                break;
            }
        }

    }
    break;
    case WM_COMMAND:
        {
            if (lParam == (LPARAM)hPenc)    // если нажали на кнопку
            {
                CurDrawMode = 1;
            }
            else if (lParam == (LPARAM)hLine)    // если нажали на кнопку
            {
                CurDrawMode = 2;
            }
            else if (lParam == (LPARAM)hBack)    // если нажали на кнопку
            {
                if (index > -1)
                {
                    index--;
                    loadFlag = true;
                    InvalidateRect(hWnd, NULL, TRUE);
                }
            }
            else if (lParam == (LPARAM)hForward)    // если нажали на кнопку
            {
                if (index < (int)hbmMemory.size() - 1)
                {
                    index++;
                    loadFlag = true;
                    InvalidateRect(hWnd, NULL, TRUE);
                }
            }
            if (lParam == (LPARAM)hCircle)    // если нажали на кнопку
            {
                CurDrawMode = 3;
            }
            else if (lParam == (LPARAM)hRect)    // если нажали на кнопку
            {
                CurDrawMode = 4;
            }
            else if (lParam == (LPARAM)hPen)    // если нажали на кнопку
            {
                CHOOSECOLOR temp = GetColorD(hWnd);
                if (ChooseColor(&temp) == TRUE)
                {
                    DeleteObject(pen);
                    lastColor = temp.rgbResult;
                    pen = CreatePen(PS_SOLID, 2 * size, temp.rgbResult);
                }
            }
            else if (lParam == (LPARAM)hEraser)    // если нажали на кнопку
            {
                CurDrawMode = 5;
            }
            else if (HIWORD(wParam) == CBN_SELCHANGE)
            {
                int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
                    (WPARAM)0, (LPARAM)0);
                TCHAR  ListItem[256];
                (TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
                    (WPARAM)ItemIndex, (LPARAM)ListItem);
                int newSize = ListItem[0] - '0';
                pen = CreatePen(PS_SOLID, 2 * newSize, lastColor);
                erasePen = CreatePen(PS_SOLID, 2 * newSize, RGB(0, 0, 0));
            }
            else if (lParam == (LPARAM)hFill)
            {
                CurDrawMode = 6;
            }
            else if (lParam == (LPARAM)hClear)
            {
                hbmMemory.clear();
                hdcMemory.clear();
                index = -1;
                loadFlag = true;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_SAVE:
                if(index != -1)
                    SaveHBITMAPToFile(hbmMemory.back(), L"save.png");
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hWnd, &ps);
        if (loadFlag)
        {
            if (index == -1)
            {
                hdcMem = CreateCompatibleDC(hDC);
                hbmMem = CreateCompatibleBitmap(hDC, WindSize.x, WindSize.y);   
            }
            else
            {
                hdcMem = hdcMemory[index];
                hbmMem = hbmMemory[index];
            }
            SelectObject(hdcMem, hbmMem);

            BitBlt(hDC, 0, 0, WindSize.x, WindSize.y, hdcMem, 0, 0, SRCCOPY);
            loadFlag = false;
            NewP.x = -1;
            NewP.y = -1;
            LastP.x = -1;
            LastP.y = -1;
        }
        else
        {
            if (saveFlag)
            {
                SelectObject(hdcMem, pen);

                HBITMAP temp = (HBITMAP)CopyImage(hbmMem, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
                switch (CurDrawMode)
                {
                case 5:
                    SelectObject(hdcMem, erasePen);
                case 1:
                    while (figMem.size() != 0)
                    {
                        DrawLine(hdcMem, figMem.back(), figMem[figMem.size() - 2]);
                        figMem.pop_back();
                        figMem.pop_back();
                    }
                    break;
                case 2:
                    DrawLine(hdcMem, LastP, NewP);
                    break;
                case 3:
                    SelectObject(hdcMem, brush);
                    DrawCircle(hdcMem, LastP, NewP);
                    break;
                case 4:
                    SelectObject(hdcMem, brush);
                    DrawRect(hdcMem, LastP, NewP);
                    break;
                case 6:
                    Paint(hDC, LastP);
                    Paint(hdcMem, LastP);
                    break;
                default:
                    return 0;
                }
                int n = hbmMemory.size() - index - 1;
                for (int i = 0; i < n; i++)
                {
                    hbmMemory.pop_back();
                    hdcMemory.pop_back();
                }
                hbmMemory.push_back((HBITMAP)CopyImage(hbmMem, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE));
                hdcMemory.push_back(hdcMem);
                if(index > -1)
                hbmMemory[index] = temp;
                index++;
                saveFlag = false;
                NewP.x = -1;
                NewP.y = -1;
                LastP.x = -1;
                LastP.y = -1;
            }
            SelectObject(hDC, pen);
            switch (CurDrawMode)
            {
            case 5:
                SelectObject(hDC, erasePen);
                
            case 1:
                DrawLine(hDC, LastP, NewP);
                
                figMem.push_back(LastP);
                figMem.push_back(NewP);
                break;
            case 2:

                BitBlt(hDC, 0, 0, WindSize.x, WindSize.y, hdcMem, 0, 0, SRCCOPY);

                DrawLine(hDC, LastP, NewP);
                break;
            case 3:

                BitBlt(hDC, 0, 0, WindSize.x, WindSize.y, hdcMem, 0, 0, SRCCOPY);
                SelectObject(hDC, brush);

                DrawCircle(hDC, LastP, NewP);
                break;
            case 4:

                BitBlt(hDC, 0, 0, WindSize.x, WindSize.y, hdcMem, 0, 0, SRCCOPY);
                SelectObject(hDC, brush);

                DrawRect(hDC, LastP, NewP);
                break;
            }
        }
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_KEYDOWN:
    {
        switch (wParam) {
        case VK_LEFT:
            if (index > -1)
            {
                index--;
                loadFlag = true;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        case VK_RIGHT:
            if (index < (int)hbmMemory.size() - 1)
            {
                index++;
                loadFlag = true;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        default:
            break;
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
