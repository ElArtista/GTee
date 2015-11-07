#include "LoggerWindow.hpp"
#include <iostream>
#include <thread>
#include <windows.h>

const TCHAR* LoggerWindow::wndClassName = _T("LoggerWndClass");
const long LoggerWindow::editCtrlId = 1000;

LoggerWindow::LoggerWindow() : mHwnd(nullptr)
{
    // Check if window class is registered and register it if it isn't
    WNDCLASS wc;
    if (!GetClassInfo(GetModuleHandle(nullptr), wndClassName, &wc))
        Register();

    // Spawn the window instance
    Create();
}

LoggerWindow::~LoggerWindow()
{
    SendMessage(mHwnd, WM_DESTROY, 0, 0);
}

void LoggerWindow::Register()
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_DROPSHADOW;
    wc.lpfnWndProc = UIElement::ProxyMsgHandler;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hIcon = nullptr;                 // TODO: Add window icon
    wc.lpszClassName = wndClassName;
    RegisterClassEx(&wc);
}

void LoggerWindow::Create()
{
    mHwnd = CreateWindowEx(
        WS_EX_LAYERED,
        wndClassName,
        "Logger",
        WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800,
        600,
        0,
        0,
        GetModuleHandle(nullptr),
        this
    );

    //LONG style = GetWindowLongPtr(hwnd, GWL_STYLE);
    //style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
    //SetWindowLongPtr(hwnd, GWL_STYLE, style);
    //LONG exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    //exStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
    //SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);

    UpdateWindow(mHwnd);

    // Force redraw
    //SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
}

void LoggerWindow::Log(const std::string& line)
{
    // Take the edit control handle
    HWND hEdit = GetDlgItem(mHwnd, editCtrlId);
    // Find the end index position of the data in the edit control
    long endPos = GetWindowTextLength(hEdit);
    // Place the cursor in the end
    SendMessage(hEdit, EM_SETSEL, static_cast<WPARAM>(endPos), static_cast<LPARAM>(endPos));
    // Append given text
    SendMessage(hEdit, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(line.c_str()));
}

LRESULT CALLBACK LoggerWindow::MessageHandler(HWND hh, UINT mm, WPARAM ww, LPARAM ll)
{
    switch (mm)
    {
        case WM_CREATE:
        {
            // Store the window handle
            mHwnd = hh;

            // Set opacity
            COLORREF col = {0};
            unsigned int opacity = 90; // Percent
            SetLayeredWindowAttributes(hh, col, opacity * 255 / 100, LWA_ALPHA);

            // Create child edit area
            CreateWindowEx(
                0, "EDIT", 0,
                WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_READONLY,
                0, 0, 0, 0, hh, (HMENU) editCtrlId,
                reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hh, GWL_HINSTANCE)), 0
            );
            
            break;
        }
        case WM_SIZE:
        {
            // Resize child edit with the parent
            HWND edit = GetDlgItem(hh, editCtrlId);
            RECT rc = {};
            GetClientRect(hh, &rc);
            SetWindowPos(
                edit, 0, 0, 0, rc.right - rc.left,
                rc.bottom - rc.top,
                SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER
            );
            break;
        }
        case WM_ERASEBKGND:
            return 1;
        case WM_CLOSE:
        {
            DestroyWindow(hh);
            SetWindowLongPtr(mHwnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(nullptr));
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hh, mm, ww, ll);
    }
    return 0;
}

