#include "framework.h"
#include "ResourceString.h"


using namespace UiPathTeam;


HINSTANCE ResourceString::InstanceHandle = NULL;


ResourceString::ResourceString(UINT uId, HINSTANCE hInstance)
{
    PWSTR psz = nullptr;
    int n = LoadStringW(hInstance ? hInstance : InstanceHandle, uId, reinterpret_cast<LPWSTR>(&psz), 0);
    if (n < 0) n = 0;
    m_psz = new WCHAR[n + 1];
    wmemcpy_s(m_psz, n + 1, psz, n);
    m_psz[n] = L'\0';
}


ResourceString::~ResourceString()
{
    delete m_psz;
}
