#include "framework.h"
#include "Label.h"


using namespace UiPathTeam;


Label::Label(PCWSTR pszName, PCWSTR pszText)
    : Element(pszName, ROLE_SYSTEM_STATICTEXT)
    , m_pszText(_wcsdup(pszText))
    , m_uTextFormat(DT_CENTER | DT_VCENTER)
{
    if (!m_pszText)
    {
        throw std::bad_alloc();
    }
}


Label::~Label()
{
    free(m_pszText);
}


Element* Label::GetChild(VARIANT& varChild) const
{
    if (varChild.vt == VT_I4 && varChild.lVal == CHILDID_SELF)
    {
        return const_cast<Label*>(this);
    }
    else
    {
        return nullptr;
    }
}


void Label::Draw(HDC hdc)
{
    auto brush0 = SelectObject(hdc, GetStockObject(m_bMouseOver ? LTGRAY_BRUSH : NULL_BRUSH));
    Rectangle(hdc, m_x, m_y, m_x + m_cx, m_y + m_cy);
    SelectObject(hdc, brush0);

    LONG thickness = 1;

    auto bkMode0 = SetBkMode(hdc, TRANSPARENT);
    RECT rcText;
    rcText.left = m_x + thickness;
    rcText.top = m_y + thickness;
    rcText.right = m_x + m_cx - thickness * 2;
    rcText.bottom = m_y + m_cy - thickness * 2;
    DrawTextW(hdc, m_pszText, static_cast<int>(wcslen(m_pszText)), &rcText, m_uTextFormat);
    SetBkMode(hdc, bkMode0);
}


Label& Label::SetText(PCWSTR value)
{

    PWSTR pszNew;
    if (value)
    {
        pszNew = _wcsdup(value);
        if (!pszNew) throw std::bad_alloc();
    }
    else
    {
        pszNew = _wcsdup(L"");
    }
    free(InterlockedExchangePointer(reinterpret_cast<PVOID*>(&m_pszText), pszNew));
    Invalidate();
    return *this;
}


void Label::OnMouseEnter()
{
    m_dwBtnState = 0;
    Invalidate();
}


void Label::OnMouseLeave()
{
    m_dwBtnState = 0;
    Invalidate();
}


void Label::OnMouseClicked(DWORD dwBtnClicked)
{
    MessageBeep(MB_OK);
    MessageBoxW(m_hwnd, m_pszText, dwBtnClicked == MK_LBUTTON ? L"左クリック" : dwBtnClicked == MK_RBUTTON ? L"右クリック" : L"?", MB_OK);
}


void Label::Invalidate()
{
    RECT rc;
    rc.left = m_x;
    rc.top = m_y;
    rc.right = m_x + m_cx;
    rc.bottom = m_y + m_cy;
    InvalidateRect(m_hwnd, &rc, TRUE);
}
