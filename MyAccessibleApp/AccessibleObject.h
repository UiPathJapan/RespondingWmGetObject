#pragma once


#include <oleacc.h>


namespace UiPathTeam
{
    class Element;


    class AccessibleObject
        : public IAccessible
        , public IEnumVARIANT
    {
    public:

        AccessibleObject(Element*);
        virtual ~AccessibleObject();
        AccessibleObject(const AccessibleObject&) = delete;
        void operator =(const AccessibleObject&) = delete;

        //IUnknown-specific
        virtual HRESULT STDMETHODCALLTYPE QueryInterface(
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject);
        virtual ULONG STDMETHODCALLTYPE AddRef(void);
        virtual ULONG STDMETHODCALLTYPE Release(void);

        //IDispatch-specific
        virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(
            /* [out] */ __RPC__out UINT *pctinfo);

        virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);

        virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ __RPC__in_range(0, 16384) UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);

        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke(
            /* [annotation][in] */
            _In_  DISPID dispIdMember,
            /* [annotation][in] */
            _In_  REFIID riid,
            /* [annotation][in] */
            _In_  LCID lcid,
            /* [annotation][in] */
            _In_  WORD wFlags,
            /* [annotation][out][in] */
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */
            _Out_opt_  UINT *puArgErr);

        //IAccessible-specific
        virtual /* [id][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_accParent(
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppdispParent);

        virtual /* [id][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_accChildCount(
            /* [retval][out] */ __RPC__out long *pcountChildren);

        virtual /* [id][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_accChild(
            /* [in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppdispChild);

        virtual /* [id][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_accName(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pszName);

        virtual /* [id][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_accValue(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pszValue);

        virtual /* [id][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_accDescription(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pszDescription);

        virtual /* [id][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_accRole(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__out VARIANT *pvarRole);

        virtual /* [id][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_accState(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__out VARIANT *pvarState);

        virtual /* [id][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_accHelp(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pszHelp);

        virtual /* [id][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_accHelpTopic(
            /* [out] */ __RPC__deref_out_opt BSTR *pszHelpFile,
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__out long *pidTopic);

        virtual /* [id][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_accKeyboardShortcut(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pszKeyboardShortcut);

        virtual /* [id][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_accFocus(
            /* [retval][out] */ __RPC__out VARIANT *pvarChild);

        virtual /* [id][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_accSelection(
            /* [retval][out] */ __RPC__out VARIANT *pvarChildren);

        virtual /* [id][propget][hidden] */ HRESULT STDMETHODCALLTYPE get_accDefaultAction(
            /* [optional][in] */ VARIANT varChild,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pszDefaultAction);

        virtual /* [id][hidden] */ HRESULT STDMETHODCALLTYPE accSelect(
            /* [in] */ long flagsSelect,
            /* [optional][in] */ VARIANT varChild);

        virtual /* [id][hidden] */ HRESULT STDMETHODCALLTYPE accLocation(
            /* [out] */ __RPC__out long *pxLeft,
            /* [out] */ __RPC__out long *pyTop,
            /* [out] */ __RPC__out long *pcxWidth,
            /* [out] */ __RPC__out long *pcyHeight,
            /* [optional][in] */ VARIANT varChild);

        virtual /* [id][hidden] */ HRESULT STDMETHODCALLTYPE accNavigate(
            /* [in] */ long navDir,
            /* [optional][in] */ VARIANT varStart,
            /* [retval][out] */ __RPC__out VARIANT *pvarEndUpAt);

        virtual /* [id][hidden] */ HRESULT STDMETHODCALLTYPE accHitTest(
            /* [in] */ long xLeft,
            /* [in] */ long yTop,
            /* [retval][out] */ __RPC__out VARIANT *pvarChild);

        virtual /* [id][hidden] */ HRESULT STDMETHODCALLTYPE accDoDefaultAction(
            /* [optional][in] */ VARIANT varChild);

        virtual /* [id][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_accName(
            /* [optional][in] */ VARIANT varChild,
            /* [in] */ __RPC__in BSTR szName);

        virtual /* [id][propput][hidden] */ HRESULT STDMETHODCALLTYPE put_accValue(
            /* [optional][in] */ VARIANT varChild,
            /* [in] */ __RPC__in BSTR szValue);

        //IEnumVARIANT-specific
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next(
            /* [in] */ ULONG celt,
            /* [length_is][size_is][out] */ VARIANT *rgVar,
            /* [out] */ ULONG *pCeltFetched);

        virtual HRESULT STDMETHODCALLTYPE Skip(
            /* [in] */ ULONG celt);

        virtual HRESULT STDMETHODCALLTYPE Reset(void);

        virtual HRESULT STDMETHODCALLTYPE Clone(
            /* [out] */ __RPC__deref_out_opt IEnumVARIANT **ppEnum);

    private:

        ULONG m_references;
        Element* m_pSelf;
        ULONG m_index;
    };
}
