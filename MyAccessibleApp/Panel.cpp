#include "framework.h"
#include "Panel.h"
#include "AccessibleObject.h"
#include "AccessibleObjectStore.h"


using namespace UiPathTeam;


static const WCHAR s_szClassName[] = { L"MyAccessibleAppPanel" };
static ULONG s_WindowClassCount;
static ATOM s_WindowClassAtom;
static HINSTANCE s_hInstance;


Panel::Panel(PCWSTR pszName)
    : Element(pszName, ROLE_SYSTEM_CLIENT)
    , m_pszValue(_wcsdup(L""))
    , m_pElements()
    , m_marginLeft(40)
    , m_marginTop(20)
    , m_marginRight(40)
    , m_marginBottom(20)
    , m_paddingHeight(10)
    , m_bAccessible(false)
{
    if (!m_pszValue) throw std::bad_alloc();
}


Panel::~Panel()
{
    std::vector<Element*>::iterator iter = m_pElements.begin();
    while (iter != m_pElements.end())
    {
        auto pElement = *iter;
        iter = m_pElements.erase(iter);
        AccessibleObjectStore::Release(pElement);
        pElement->Release();
    }
    DestroyWindow(m_hwnd);
    m_hwnd = nullptr;
    if (!--s_WindowClassCount)
    {
        UnregisterClassW(s_szClassName, s_hInstance);
    }
    free(m_pszValue);
}


Element* Panel::GetChild(VARIANT& varChild) const
{
    if (varChild.vt == VT_I4)
    {
        if (varChild.lVal == CHILDID_SELF)
        {
            return const_cast<Panel*>(this);
        }
        else if (1L <= varChild.lVal && varChild.lVal <= static_cast<LONG>(m_pElements.size()))
        {
            return m_pElements[varChild.lVal - 1];
        }
    }
    return nullptr;
}


void Panel::AddChild(Element* pElement)
{
    m_pElements.push_back(pElement);
    pElement->SetParent(this);
}


HWND Panel::Create(HWND hwndParent, HINSTANCE hInstance)
{
    if (!s_WindowClassCount++)
    {
        WNDCLASSEXW wcex = { 0 };
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.hInstance = hInstance;
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszClassName = s_szClassName;
        s_WindowClassAtom = RegisterClassExW(&wcex);
        s_hInstance = hInstance;
    }
    m_hwnd = CreateWindowExW(0, s_szClassName, m_pszName, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwndParent, nullptr, hInstance, this);
    ShowWindow(m_hwnd, SW_SHOW);
    return m_hwnd;
}


LRESULT CALLBACK Panel::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Panel* pThis = reinterpret_cast<Panel*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    if (pThis)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
            AccessibleObjectStore::Release(pThis);
            pThis->Release();
            return 0;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            pThis->Draw(hdc);
            EndPaint(hWnd, &ps);
            return 0;
        }
        case WM_GETOBJECT:
        {
            if (pThis->m_bAccessible)
            {
                if (lParam == OBJID_CLIENT)
                {
                    return LresultFromObject(IID_IAccessible, wParam, static_cast<IAccessible*>(AccessibleObjectStore::Get(pThis)));
                }
            }
            break;
        }
        case WM_GETTEXT:
            _snwprintf_s(reinterpret_cast<PWCHAR>(lParam), wParam, _TRUNCATE, L"%s", pThis->GetValue());
            return static_cast<LRESULT>(wcslen(reinterpret_cast<PWCHAR>(lParam)));
        case WM_GETTEXTLENGTH:
            return static_cast<LRESULT>(wcslen(pThis->GetValue()));
        case WM_SETTEXT:
            pThis->SetValue(reinterpret_cast<BSTR>(lParam));
            return TRUE;
        case WM_MOUSEMOVE:
            pThis->OnMouseMove(static_cast<LONG>(LOWORD(lParam)), static_cast<LONG>(HIWORD(lParam)));
            return 0;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            pThis->OnButtonDown(static_cast<LONG>(LOWORD(lParam)), static_cast<LONG>(HIWORD(lParam)), static_cast<DWORD>(wParam));
            return 0;
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
            pThis->OnButtonUp(static_cast<LONG>(LOWORD(lParam)), static_cast<LONG>(HIWORD(lParam)), static_cast<DWORD>(wParam));
            return 0;
        default:
            break;
        }
    }
    else if (uMsg == WM_CREATE)
    {
        LPCREATESTRUCTW pCS = reinterpret_cast<LPCREATESTRUCTW>(lParam);
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCS->lpCreateParams));
        return 0;
    }
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}


void Panel::Draw(HDC hdc)
{
    for (std::vector<Element*>::iterator iter = m_pElements.begin(); iter != m_pElements.end(); iter++)
    {
        auto pElement = *iter;
        pElement->Draw(hdc);
    }
}


PCWSTR Panel::GetValue() const
{
    return m_pszValue;
}


void Panel::SetValue(PCWSTR value)
{
    free(m_pszValue);
    m_pszValue = _wcsdup(value);
    if (!m_pszValue) throw std::bad_alloc();
}


void Panel::Layout()
{
    POINT curPos;
    curPos.x = m_marginLeft;
    curPos.y = m_marginTop;

    for (std::vector<Element*>::iterator iter = m_pElements.begin(); iter != m_pElements.end(); iter++)
    {
        auto pElement = *iter;
        auto size = pElement->GetSize();
        pElement->SetLocation(curPos);
        LONG cx = curPos.x + size.cx + m_marginRight;
        if (m_cx < cx) m_cx = cx;
        curPos.y += size.cy + m_paddingHeight;
    }

    m_cy = curPos.y - m_paddingHeight + m_marginBottom;

    SetSize(m_cx, m_cy);
}


void Panel::OnMouseMove(LONG x, LONG y)
{
    if (HitTest(x, y))
    {
        for (std::vector<Element*>::iterator iter = m_pElements.begin(); iter != m_pElements.end(); iter++)
        {
            auto pElement = *iter;
            bool bHit = pElement->HitTest(x, y);
            if (bHit != pElement->IsMouseOver())
            {
                pElement->SetMouseOver(bHit);
                if (bHit)
                {
                    pElement->OnMouseEnter();
                }
                else
                {
                    pElement->OnMouseLeave();
                }
            }
        }
        if (!m_bMouseOver)
        {
            m_bMouseOver = true;
            SetCapture(m_hwnd);
            OnMouseEnter();
        }
    }
    else if (m_bMouseOver)
    {
        for (std::vector<Element*>::iterator iter = m_pElements.begin(); iter != m_pElements.end(); iter++)
        {
            auto pElement = *iter;
            if (pElement->IsMouseOver())
            {
                pElement->SetMouseOver(false);
                pElement->OnMouseLeave();
            }
        }
        m_bMouseOver = false;
        ReleaseCapture();
        OnMouseLeave();
    }
}


void Panel::OnButtonDown(LONG x, LONG y, DWORD dwBtnState)
{
    for (std::vector<Element*>::iterator iter = m_pElements.begin(); iter != m_pElements.end(); iter++)
    {
        auto pElement = *iter;
        bool bHit = pElement->HitTest(x, y);
        if (bHit)
        {
            pElement->SetButtonState(dwBtnState);
        }
        else
        {
            pElement->SetButtonState(0);
        }
    }
}


void Panel::OnButtonUp(LONG x, LONG y, DWORD dwBtnState)
{
    for (std::vector<Element*>::iterator iter = m_pElements.begin(); iter != m_pElements.end(); iter++)
    {
        auto pElement = *iter;
        bool bHit = pElement->HitTest(x, y);
        if (bHit)
        {
            DWORD dwClicked = pElement->GetButtonState() ^ dwBtnState;
            pElement->SetButtonState(dwBtnState);
            if (dwClicked)
            {
                pElement->OnMouseClicked(dwClicked);
            }
        }
        else
        {
            pElement->SetButtonState(0);
        }
    }
}
