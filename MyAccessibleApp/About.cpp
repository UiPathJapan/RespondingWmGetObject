#include "About.h"
#include "Resource.h"


using namespace UiPathTeam;


void About::Show(HINSTANCE hInstance, HWND hWnd)
{
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, DlgProc);
}


INT_PTR CALLBACK About::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
