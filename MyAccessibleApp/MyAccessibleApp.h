#pragma once


#include <Windows.h>


namespace UiPathTeam
{
    class Panel;


    class MyAccessibleApp
    {
    public:

        MyAccessibleApp(HINSTANCE, int);
        ~MyAccessibleApp();
        MyAccessibleApp(const MyAccessibleApp&) = delete;
        void operator =(const MyAccessibleApp&) = delete;
        HWND Create();

    private:

        static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

        void Layout();
        void EnableActiveAccessibility(HWND, bool);
        void UpdateMenu(HWND);

        HINSTANCE m_hInstance;
        int m_nCmdShow;
        HWND m_hwnd;
        Panel* m_pPanel;
    };
}
