#pragma once


#include <Windows.h>


namespace UiPathTeam
{
    class SimpleLock
    {
    public:

        typedef ULONG Type; // Initialize values of this type by zero

        inline SimpleLock(Type&);
        inline ~SimpleLock();

        SimpleLock(const SimpleLock&) = delete;
        void operator =(const SimpleLock&) = delete;

    private:

        volatile Type& m_value;
    };

    inline SimpleLock::SimpleLock(SimpleLock::Type& value)
        : m_value(value)
    {
        Type threadId = GetCurrentThreadId();
        while (InterlockedCompareExchange(&m_value, threadId, 0))
        {
            Sleep(0);
        }
    }

    inline SimpleLock::~SimpleLock()
    {
        InterlockedExchange(&m_value, 0);
    }
}
