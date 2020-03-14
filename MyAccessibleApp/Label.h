#pragma once


#include "Element.h"


namespace UiPathTeam
{
    class Label
        : public Element
    {
    public:

        Label(PCWSTR, PCWSTR);
        Label(const Label&) = delete;
        virtual ~Label();
        void operator =(const Label&) = delete;
        virtual UINT GetChildCount() const { return 0; }
        virtual Element* GetChild(VARIANT&) const;
        virtual void Draw(HDC);
        virtual PCWSTR GetValue() const { return GetText(); }
        virtual void SetValue(PCWSTR value) { SetText(value); }
        virtual void OnMouseEnter();
        virtual void OnMouseLeave();
        virtual void OnMouseClicked(DWORD);

        PCWSTR GetText() const { return m_pszText; }
        Label& SetText(PCWSTR value);
        UINT GetTextFormat() const { return m_uTextFormat; }
        Label& SetTextFormat(UINT value) { m_uTextFormat = value; return *this; }

    protected:

        void Invalidate();

        PWSTR m_pszText;
        UINT m_uTextFormat;
    };
}
