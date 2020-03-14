#pragma once


#include <Windows.h>


namespace UiPathTeam
{
    class ResourceString
    {
    public:

        ResourceString(HINSTANCE, UINT);
        ResourceString(const ResourceString&) = delete;
        ~ResourceString();
        void operator =(const ResourceString&) = delete;
        PCWSTR Value() const { return m_psz; }
        operator PCWSTR() const { return Value(); }
        PCWSTR operator &() const { return Value(); }

    protected:

        PWSTR m_psz;
    };
}
