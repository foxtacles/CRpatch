#include "windows.h"
#include "resource.h"
#include "stdio.h"
#include "ufmod.h"

HDC hdc, hdcMem;
HWND hwndmain, hwndtext;
HFONT hFont, hFont2;
HBITMAP hBitmap;
BITMAP bitmap;
PAINTSTRUCT ps;
HINSTANCE instance;

void CRpatch()
{
    FILE* engine;
    engine = fopen("Engine.dll", "r+b");

    unsigned char patch[11] = {0xB0, 0x01, 0x5E, 0x5B, 0x81, 0xC4, 0x9C, 0x00, 0x00, 0x00, 0xC3};

    if (engine != NULL)
    {
        // offset 0x19C26F
        fseek(engine, 1688175, SEEK_SET);
        fwrite(patch, 1, sizeof(patch), engine);
        fclose(engine);

        MessageBox(hwndmain, (char*) "successfully patched Engine.dll\n\nyou have to manually set PhysXPrimarySceneHardware\nin RealityEngine.ini to false!", (char*) "Cellfactor Revolution PhysX Patch", MB_ICONINFORMATION);
    }
    else
        MessageBox(hwndmain, (char*) "could not find Engine.dll", (char*) "Cellfactor Revolution PhysX Patch", MB_ICONSTOP);
}

HWND CreateMainWindow()
{
    HWND wnd;
    wnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, WND_CLASS_NAME, "Cellfactor Revolution PhysX Patch", WS_OVERLAPPED, (GetSystemMetrics(SM_CXSCREEN) / 2) - 200, (GetSystemMetrics(SM_CYSCREEN) / 2) - 150, 400, 300, HWND_DESKTOP, NULL, instance, NULL);
    ShowWindow(wnd, SW_SHOWNORMAL);
    UpdateWindow(wnd);
    return wnd;
}

int MessageLoop()
{
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
        {
            hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(CRBG));
            GetObject(hBitmap, sizeof(BITMAP), &bitmap);
            uFMOD_PlaySong(MAKEINTRESOURCE(CHIPTUNE), GetModuleHandle(NULL), XM_RESOURCE);
            break;
        }

        case WM_PAINT:
        {
            hdc = BeginPaint(hwnd, &ps);
            hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, hBitmap);
            BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
            DeleteDC(hdcMem);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_LBUTTONDOWN:
        {
            ReleaseCapture();
            SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
            break;
        }

        case WM_COMMAND:
        {
            switch (wParam)
            {
                case IDC_BT0:
                {
                    CRpatch();
                    break;
                }

                case IDC_BT1:
                {
                    MessageBox(hwnd, (char*) "Cellfactor Revolution PhysX patch:\nIsPhysXHardwareInstalled_CHECK\n\ncode: Recycler\nmusic: cerror - notice me\n\ngreets fly out to:\nmqidx, benG, ArminSeiko\n\nwww.brickster.net", (char*) "Cellfactor Revolution PhysX Patch", MB_ICONINFORMATION);
                    break;
                }

                case IDC_BT2:
                {
                    SendMessage(hwnd, WM_DESTROY, 0, 0);
                    break;
                }
            }
            break;
        }

        case WM_DESTROY:
            DeleteObject(hBitmap) ;
            uFMOD_StopSong();
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

void CreateWindowContent(HWND parent)
{
    HWND wnd;

    wnd = CreateWindowEx(0x00000000, "Button", "Patch", WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 35, 235, 100, 25, parent, (HMENU) IDC_BT0, instance, NULL);
    SendMessage(wnd, WM_SETFONT, (WPARAM) hFont2, TRUE);

    wnd = CreateWindowEx(0x00000000, "Button", "About", WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 145, 235, 100, 25, parent, (HMENU) IDC_BT1, instance, NULL);
    SendMessage(wnd, WM_SETFONT, (WPARAM) hFont2, TRUE);

    wnd = CreateWindowEx(0x00000000, "Button", "Exit", WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 255, 235, 100, 25, parent, (HMENU) IDC_BT2, instance, NULL);
    SendMessage(wnd, WM_SETFONT, (WPARAM) hFont2, TRUE);
}

int RegisterClasses()
{
    WNDCLASSEX wc;

    wc.hInstance = instance;
    wc.lpszClassName = WND_CLASS_NAME;
    wc.lpfnWndProc = WindowProcedure;
    wc.style = CS_DBLCLKS;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hIcon = LoadIcon(instance, MAKEINTRESOURCE(ICON_MAIN));
    wc.hIconSm = LoadIcon(instance, MAKEINTRESOURCE(ICON_MAIN));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszMenuName = NULL;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;

    wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);

    return RegisterClassEx(&wc);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdline, int show)
{
    instance = hInstance;
    hFont = CreateFont(-17, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana");
    hFont2 = CreateFont(-11, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana");
    RegisterClasses();
    hwndmain = CreateMainWindow();
    CreateWindowContent(hwndmain);
    return MessageLoop();
}
