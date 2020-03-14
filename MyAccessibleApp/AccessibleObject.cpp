#include "framework.h"
#include "AccessibleObject.h"
#include "Element.h"
#include "AccessibleObjectStore.h"


using namespace UiPathTeam;


AccessibleObject::AccessibleObject(Element* pSelf)
    : m_references(1)
    , m_pSelf(pSelf)
    , m_index(0)
{
    m_pSelf->AddRef();
}


AccessibleObject::~AccessibleObject()
{
    m_pSelf->Release();
}


HRESULT STDMETHODCALLTYPE AccessibleObject::QueryInterface(REFIID riid, void** ppvObject)
{
    if (!ppvObject)
    {
        return E_POINTER;
    }
    if (IsEqualIID(riid, IID_IAccessible))
    {
        AddRef();
        *ppvObject = static_cast<IAccessible*>(this);
        return S_OK;
    }
    else if (IsEqualIID(riid, IID_IDispatch))
    {
        AddRef();
        *ppvObject = static_cast<IAccessible*>(this);
        return S_OK;
    }
    else if (IsEqualIID(riid, IID_IUnknown))
    {
        AddRef();
        *ppvObject = static_cast<IAccessible*>(this);
        return S_OK;
    }
    else if (IsEqualIID(riid, IID_IEnumVARIANT))
    {
        AddRef();
        *ppvObject = static_cast<IEnumVARIANT*>(this);
        return S_OK;
    }
    return E_NOINTERFACE;
}


ULONG STDMETHODCALLTYPE AccessibleObject::AddRef(void)
{
    return InterlockedIncrement(&m_references);
}


ULONG STDMETHODCALLTYPE AccessibleObject::Release(void)
{
    ULONG ret = InterlockedDecrement(&m_references);
    if (!ret)
    {
        delete this;
    }
    return ret;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::GetTypeInfoCount(UINT* pctinfo)
{
    return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
{
    return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
{
    return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr)
{
    return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::get_accParent(IDispatch **ppdispParent)
{
    if (!ppdispParent)
    {
        return E_POINTER;
    }
    auto pElement = m_pSelf->GetParent();
    if (!pElement)
    {
        *ppdispParent = nullptr;
        return S_FALSE;
    }
    *ppdispParent = AccessibleObjectStore::Get(pElement);
    return S_OK;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::get_accChildCount(long* pcountChildren)
{
    if (!pcountChildren)
    {
        return E_POINTER;
    }
    *pcountChildren = static_cast<long>(m_pSelf->GetChildCount());
    return S_OK;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::get_accChild(VARIANT varChild, IDispatch** ppdispChild)
{
    auto pElement = m_pSelf->GetChild(varChild);
    if (!pElement)
    {
        return E_INVALIDARG;
    }
    if (!ppdispChild)
    {
        return E_POINTER;
    }
    *ppdispChild = AccessibleObjectStore::Get(pElement);
    return S_OK;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::get_accName(VARIANT varChild, BSTR* pszName)
{
    auto pElement = m_pSelf->GetChild(varChild);
    if (!pElement)
    {
        return E_INVALIDARG;
    }
    if (!pszName)
    {
        return E_POINTER;
    }
    *pszName = SysAllocString(pElement->GetName());
    return S_OK;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::get_accValue(VARIANT varChild, BSTR* pszValue)
{
    auto pElement = m_pSelf->GetChild(varChild);
    if (!pElement)
    {
        return E_INVALIDARG;
    }
    if (!pszValue)
    {
        return E_POINTER;
    }
    *pszValue = SysAllocString(pElement->GetValue());
    return S_OK;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::get_accDescription(VARIANT varChild, BSTR* pszDescription)
{
    return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::get_accRole(VARIANT varChild, VARIANT* pvarRole)
{
    auto pElement = m_pSelf->GetChild(varChild);
    if (!pElement)
    {
        return E_INVALIDARG;
    }
    if (!pvarRole)
    {
        return E_POINTER;
    }
    VariantInit(pvarRole);
    pvarRole->vt = VT_I4;
    pvarRole->lVal = pElement->GetRole();
    return S_OK;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::get_accState(VARIANT varChild, VARIANT* pvarState)
{
    auto pElement = m_pSelf->GetChild(varChild);
    if (!pElement)
    {
        return E_INVALIDARG;
    }
    if (!pvarState)
    {
        return E_POINTER;
    }
    VariantInit(pvarState);
    pvarState->vt = VT_I4;
    pvarState->lVal = STATE_SYSTEM_FOCUSABLE;
    if (pElement->IsMouseOver() && pElement->GetWindow() == GetFocus())
    {
        pvarState->lVal |= STATE_SYSTEM_FOCUSED;
    }
    return S_OK;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::get_accHelp(VARIANT varChild, BSTR* pszHelp)
{
    return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::get_accHelpTopic(BSTR* pszHelpFile, VARIANT varChild, long* pidTopic)
{
    return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::get_accKeyboardShortcut(VARIANT varChild, BSTR* pszKeyboardShortcut)
{
    return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::get_accFocus(VARIANT* pvarChild)
{
    if (!pvarChild)
    {
        return E_POINTER;
    }
    VariantInit(pvarChild);
    pvarChild->vt = VT_EMPTY;
    return S_FALSE;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::get_accSelection(VARIANT* pvarChildren)
{
    if (!pvarChildren)
    {
        return E_POINTER;
    }
    VariantInit(pvarChildren);
    pvarChildren->vt = VT_EMPTY;
    return S_FALSE;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::get_accDefaultAction(VARIANT varChild, BSTR* pszDefaultAction)
{
    return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::accSelect(long flagsSelect, VARIANT varChild)
{
    return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::accLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
{
    auto pElement = m_pSelf->GetChild(varChild);
    if (!pElement)
    {
        return E_INVALIDARG;
    }
    POINT pt = pElement->GetLocation();
    SIZE sz = pElement->GetSize();
    ClientToScreen(pElement->GetWindow(), &pt);
    if (pxLeft) *pxLeft = pt.x;
    if (pyTop) *pyTop = pt.y;
    if (pcxWidth) *pcxWidth = sz.cx;
    if (pcyHeight) *pcyHeight = sz.cy;
    return S_OK;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::accNavigate(long navDir, VARIANT varStart, VARIANT* pvarEndUpAt)
{
    return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::accHitTest(long xLeft, long yTop, VARIANT* pvarChild)
{
    if (!pvarChild)
    {
        return E_POINTER;
    }
    POINT pt;
    pt.x = xLeft;
    pt.y = yTop;
    ScreenToClient(m_pSelf->GetWindow(), &pt);
    if (m_pSelf->HitTest(pt.x, pt.y))
    {
        VariantInit(pvarChild);
        pvarChild->vt = VT_I4;
        pvarChild->lVal = CHILDID_SELF;
        return S_OK;
    }
    UINT n = m_pSelf->GetChildCount();
    for (UINT idx = 0; idx < n; idx++)
    {
        VARIANT varChild;
        VariantInit(&varChild);
        varChild.vt = VT_I4;
        varChild.lVal = idx + 1;
        auto pElement = m_pSelf->GetChild(varChild);
        if (!pElement)
        {
            continue;
        }
        POINT pt;
        pt.x = xLeft;
        pt.y = yTop;
        ScreenToClient(pElement->GetWindow(), &pt);
        if (pElement->HitTest(pt.x, pt.y))
        {
            *pvarChild = varChild;
            return S_OK;
        }
    }
    VariantInit(pvarChild);
    pvarChild->vt = VT_EMPTY;
    return S_FALSE;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::accDoDefaultAction(VARIANT varChild)
{
    auto pElement = m_pSelf->GetChild(varChild);
    if (!pElement)
    {
        return E_INVALIDARG;
    }
    POINT pt = pElement->GetLocation();
    SIZE size = pElement->GetSize();
    HWND hwnd = pElement->GetWindow();
    LPARAM lParam = MAKELONG(pt.x + size.cx / 2, pt.y + size.cy / 2);
    PostMessageW(hwnd, WM_MOUSEMOVE, 0, lParam);
    PostMessageW(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
    PostMessageW(hwnd, WM_LBUTTONUP, 0, lParam);
    return S_OK;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::put_accName(VARIANT varChild, BSTR szName)
{
    auto pElement = m_pSelf->GetChild(varChild);
    if (!pElement)
    {
        return E_INVALIDARG;
    }
    pElement->SetName(szName);
    return S_OK;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::put_accValue(VARIANT varChild, BSTR szValue)
{
    auto pElement = m_pSelf->GetChild(varChild);
    if (!pElement)
    {
        return E_INVALIDARG;
    }
    pElement->SetValue(szValue);
    return S_OK;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::Next(ULONG celt, VARIANT *rgVar, ULONG *pCeltFetched)
{
    *pCeltFetched = m_pSelf->GetChildCount() - m_index;
    *pCeltFetched = min(celt, *pCeltFetched);
    for (ULONG idx = 0; idx < *pCeltFetched; idx++, m_index++)
    {
        VariantInit(&rgVar[idx]);
        rgVar[idx].vt = VT_I4;
        rgVar[idx].lVal = m_index + 1;
    }
    return S_OK;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::Skip(ULONG celt)
{
    m_index += celt;
    return S_OK;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::Reset(void)
{
    m_index = 0;
    return S_OK;
}


HRESULT STDMETHODCALLTYPE AccessibleObject::Clone(IEnumVARIANT **ppEnum)
{
    if (!ppEnum)
    {
        return E_POINTER;
    }
    *ppEnum = new AccessibleObject(m_pSelf);
    return S_OK;
}
