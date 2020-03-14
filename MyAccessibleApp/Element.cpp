#include "framework.h"
#include "Element.h"


using namespace UiPathTeam;


Element::Element(PCWSTR pszName, DWORD dwRole)
    : m_references(1)
    , m_pszName(_wcsdup(pszName))
    , m_role(dwRole)
    , m_pParent(nullptr)
    , m_hwnd(nullptr)
    , m_x(0)
    , m_y(0)
    , m_cx(0)
    , m_cy(0)
    , m_bMouseOver(false)
    , m_dwBtnState(0)
{
    if (!m_pszName)
    {
        throw std::bad_alloc();
    }
}


Element::~Element()
{
    free(m_pszName);
}


ULONG Element::AddRef()
{
    return InterlockedIncrement(&m_references);
}


ULONG Element::Release()
{
    ULONG ret = InterlockedDecrement(&m_references);
    if (!ret)
    {
        delete this;
    }
    return ret;
}


void Element::SetName(PCWSTR value)
{
    auto pszNew = _wcsdup(value);
    if (!pszNew) throw std::bad_alloc();
    free(InterlockedExchangePointer(reinterpret_cast<PVOID*>(&m_pszName), pszNew));
}


POINT Element::GetLocation() const
{
    POINT value;
    value.x = m_x;
    value.y = m_y;
    return value;
}


void Element::SetLocation(POINT& value)
{
    SetLocation(value.x, value.y);
}


void Element::SetLocation(LONG x, LONG y)
{
    m_x = x;
    m_y = y;
    if (m_hwnd && (!m_pParent || m_pParent->m_hwnd != m_hwnd))
    {
        SetWindowPos(m_hwnd, nullptr, m_x, m_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
}


SIZE Element::GetSize() const
{
    SIZE value;
    value.cx = m_cx;
    value.cy = m_cy;
    return value;
}


void Element::SetSize(SIZE& value)
{
    SetSize(value.cx, value.cy);
}


void Element::SetSize(LONG cx, LONG cy)
{
    m_cx = cx;
    m_cy = cy;
    if (m_hwnd && (!m_pParent || m_pParent->m_hwnd != m_hwnd))
    {
        SetWindowPos(m_hwnd, nullptr, 0, 0, m_cx, m_cy, SWP_NOMOVE | SWP_NOZORDER);
    }
}


bool Element::HitTest(LONG x, LONG y) const
{
    return m_x <= x && x < m_x + m_cx && m_y <= y && y < m_y + m_cy;
}


HWND Element::Create(HWND hwndParent, HINSTANCE unused)
{
    m_hwnd = hwndParent;
    return m_hwnd;
}
