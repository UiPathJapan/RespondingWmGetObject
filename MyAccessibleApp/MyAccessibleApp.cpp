#include "framework.h"
#include "MyAccessibleApp.h"
#include "Panel.h"
#include "Label.h"
#include "About.h"
#include "AccessibleObject.h"
#include "AccessibleObjectStore.h"
#include "ResourceString.h"
#include "Resource.h"


#pragma comment(lib,"oleacc.lib")


using namespace UiPathTeam;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MyAccessibleApp App(hInstance, nCmdShow);

    App.Create();

    HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDC_MYACCESSIBLEAPP));

    MSG msg;

    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    App.Destroy();

    return (int) msg.wParam;
}


MyAccessibleApp::MyAccessibleApp(HINSTANCE hInstance, int nCmdShow)
    : m_hInstance(hInstance)
    , m_nCmdShow(nCmdShow)
    , m_pPanel(nullptr)
{
}


MyAccessibleApp::~MyAccessibleApp()
{
}


HWND MyAccessibleApp::Create()
{
    ResourceString szWindowClass(m_hInstance, IDC_MYACCESSIBLEAPP);
    ResourceString szTitle(m_hInstance, IDS_APP_TITLE);

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_MYACCESSIBLEAPP));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MYACCESSIBLEAPP);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    m_hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInstance, this);

    m_pPanel = new Panel(L"パネル");
    m_pPanel->SetAccessible(true);
    auto hwndPanel = m_pPanel->Create(m_hwnd, m_hInstance);

    auto pLabel1 = new Label(L"ラベル1", ResourceString(m_hInstance, IDS_LABEL1));
    pLabel1->Create(hwndPanel, m_hInstance);
    pLabel1->SetSize(200, 24);
    m_pPanel->AddChild(pLabel1);

    auto pLabel2 = new Label(L"ラベル2", ResourceString(m_hInstance, IDS_LABEL2));
    pLabel2->Create(hwndPanel, m_hInstance);
    pLabel2->SetSize(200, 24);
    m_pPanel->AddChild(pLabel2);

    auto pLabel3 = new Label(L"ラベル3", ResourceString(m_hInstance, IDS_LABEL3));
    pLabel3->Create(hwndPanel, m_hInstance);
    pLabel3->SetSize(200, 24);
    m_pPanel->AddChild(pLabel3);

    auto pLabel4 = new Label(L"ラベル4", ResourceString(m_hInstance, IDS_LABEL4));
    pLabel4->Create(hwndPanel, m_hInstance);
    pLabel4->SetSize(200, 24);
    m_pPanel->AddChild(pLabel4);

    auto pLabel5 = new Label(L"ラベル5", ResourceString(m_hInstance, IDS_LABEL5));
    pLabel5->Create(hwndPanel, m_hInstance);
    pLabel5->SetSize(200, 24);
    m_pPanel->AddChild(pLabel5);

    Layout();

    UpdateMenu(m_hwnd);

    ShowWindow(m_hwnd, m_nCmdShow);
    UpdateWindow(m_hwnd);

    return m_hwnd;
}


void MyAccessibleApp::Destroy()
{
    m_pPanel->Release();
    m_pPanel = nullptr;

    UnregisterClassW(ResourceString(m_hInstance, IDC_MYACCESSIBLEAPP), m_hInstance);
}


LRESULT CALLBACK MyAccessibleApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MyAccessibleApp* pThis = reinterpret_cast<MyAccessibleApp*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    if (pThis)
    {
        switch (message)
        {
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                About::Show(pThis->m_hInstance, hWnd);
                return 0;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                return 0;
            case IDM_ENABLE_AA:
                pThis->EnableActiveAccessibility(hWnd, true);
                return 0;
            case IDM_DISABLE_AA:
                pThis->EnableActiveAccessibility(hWnd, false);
                return 0;
            default:
                break;
            }
            break;
        }
        case WM_DESTROY:
            pThis->m_hwnd = nullptr;
            PostQuitMessage(0);
            return 0;
        default:
            break;
        }
    }
    else if (message == WM_CREATE)
    {
        LPCREATESTRUCTW pCS = reinterpret_cast<LPCREATESTRUCTW>(lParam);
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCS->lpCreateParams));
        return 0;
    }
    return DefWindowProcW(hWnd, message, wParam, lParam);
}


void MyAccessibleApp::Layout()
{
    m_pPanel->Layout();
    RECT rc = { 0 };
    GetWindowRect(m_hwnd, &rc);
    RECT rcClient = { 0 };
    GetClientRect(m_hwnd, &rcClient);
    LONG cxFrame = (rc.right - rc.left) - (rcClient.right - rcClient.left);
    LONG cyFrame = (rc.bottom - rc.top) - (rcClient.bottom - rcClient.top);
    auto size = m_pPanel->GetSize();
    SetWindowPos(m_hwnd, nullptr, 0, 0, size.cx + cxFrame, size.cy + cyFrame, SWP_NOMOVE | SWP_NOZORDER);
}


void MyAccessibleApp::EnableActiveAccessibility(HWND hWnd, bool value)
{
    m_pPanel->SetAccessible(value);
    UpdateMenu(hWnd);
}


void MyAccessibleApp::UpdateMenu(HWND hWnd)
{
    HMENU hMenu = GetMenu(hWnd);
    if (m_pPanel->IsAccessible())
    {
        DefWindowProcW(hWnd, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(&ResourceString(m_hInstance, IDS_AA_ENABLED)));
        EnableMenuItem(hMenu, IDM_ENABLE_AA, MF_DISABLED);
        EnableMenuItem(hMenu, IDM_DISABLE_AA, MF_ENABLED);
    }
    else
    {
        DefWindowProcW(hWnd, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(&ResourceString(m_hInstance, IDS_AA_DISABLED)));
        EnableMenuItem(hMenu, IDM_ENABLE_AA, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_DISABLE_AA, MF_DISABLED);
    }
}
