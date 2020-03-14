#pragma once


#include "Element.h"
#include <vector>


namespace UiPathTeam
{
    class Panel
        : public Element
    {
    public:

        Panel(PCWSTR);
        Panel(const Panel&) = delete;
        virtual ~Panel();
        void operator =(const Panel&) = delete;
        virtual UINT GetChildCount() const { return static_cast<UINT>(m_pElements.size()); }
        virtual Element* GetChild(VARIANT&) const;
        virtual void AddChild(Element*);
        virtual HWND Create(HWND, HINSTANCE = nullptr);
        virtual void Layout();
        virtual void Draw(HDC);
        virtual PCWSTR GetValue() const;
        virtual void SetValue(PCWSTR);
        virtual bool IsAccessible() const { return m_bAccessible; }
        virtual void SetAccessible(bool value) { m_bAccessible = value; }

    protected:

        static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

        void OnMouseMove(LONG, LONG);
        void OnButtonDown(LONG, LONG, DWORD);
        void OnButtonUp(LONG, LONG, DWORD);

        PWSTR m_pszValue;
        std::vector<Element*> m_pElements;
        LONG m_marginLeft = 40;
        LONG m_marginTop = 20;
        LONG m_marginRight = 40;
        LONG m_marginBottom = 20;
        LONG m_paddingHeight = 10;
        bool m_bAccessible;
    };
}
