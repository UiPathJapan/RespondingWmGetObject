#pragma once


#include <Windows.h>
#include <oleacc.h>


namespace UiPathTeam
{
    class Element
    {
    public:

        Element(PCWSTR, DWORD);
        Element(const Element&) = delete;
        virtual ~Element();
        void operator =(const Element&) = delete;
        virtual ULONG AddRef();
        virtual ULONG Release();
        virtual PCWSTR GetName() const { return m_pszName; }
        virtual void SetName(PCWSTR);
        virtual DWORD GetRole() const { return m_role; }
        virtual Element* GetParent() const { return m_pParent; }
        virtual void SetParent(Element* value) { m_pParent = value; }
        virtual UINT GetChildCount() const = 0;
        virtual Element* GetChild(VARIANT&) const = 0;
        virtual void AddChild(Element*) {}
        virtual HWND GetWindow() const { return m_hwnd; }
        virtual POINT GetLocation() const;
        virtual void SetLocation(POINT&);
        virtual void SetLocation(LONG, LONG);
        virtual SIZE GetSize() const;
        virtual void SetSize(SIZE&);
        virtual void SetSize(LONG, LONG);
        virtual bool HitTest(LONG, LONG) const;
        virtual bool IsMouseOver() const { return m_bMouseOver; }
        virtual void SetMouseOver(bool value) { m_bMouseOver = value; }
        virtual DWORD GetButtonState() const { return m_dwBtnState; }
        virtual void SetButtonState(DWORD value) { m_dwBtnState = value; }
        virtual HWND Create(HWND = nullptr, HINSTANCE = nullptr);
        virtual void Layout() {}
        virtual void Draw(HDC) {}
        virtual PCWSTR GetValue() const { return nullptr; }
        virtual void SetValue(PCWSTR) {}
        virtual void OnMouseEnter() {}
        virtual void OnMouseLeave() {}
        virtual void OnMouseClicked(DWORD) {}

    protected:

        ULONG m_references;
        PWSTR m_pszName;
        DWORD m_role;
        Element* m_pParent;
        HWND m_hwnd;
        LONG m_x;
        LONG m_y;
        LONG m_cx;
        LONG m_cy;
        bool m_bMouseOver;
        DWORD m_dwBtnState;
    };
}
