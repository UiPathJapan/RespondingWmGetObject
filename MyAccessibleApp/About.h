#pragma once


#include <Windows.h>


namespace UiPathTeam
{
    class About
    {
    public:

        static void Show(HINSTANCE, HWND);

    private:

        static INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
    };
}
