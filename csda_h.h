

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Apr 25 00:41:16 2016
 */
/* Compiler settings for csda.odl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __csda_h_h__
#define __csda_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICsda_FWD_DEFINED__
#define __ICsda_FWD_DEFINED__
typedef interface ICsda ICsda;

#endif 	/* __ICsda_FWD_DEFINED__ */


#ifndef __Document_FWD_DEFINED__
#define __Document_FWD_DEFINED__

#ifdef __cplusplus
typedef class Document Document;
#else
typedef struct Document Document;
#endif /* __cplusplus */

#endif 	/* __Document_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Csda_LIBRARY_DEFINED__
#define __Csda_LIBRARY_DEFINED__

/* library Csda */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_Csda,0xF60F1186,0xA60D,0x11D0,0x97,0x5C,0x00,0x00,0xC0,0xC2,0x48,0x4C);

#ifndef __ICsda_DISPINTERFACE_DEFINED__
#define __ICsda_DISPINTERFACE_DEFINED__

/* dispinterface ICsda */
/* [uuid] */ 


DEFINE_GUID(DIID_ICsda,0xF60F1187,0xA60D,0x11D0,0x97,0x5C,0x00,0x00,0xC0,0xC2,0x48,0x4C);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("F60F1187-A60D-11D0-975C-0000C0C2484C")
    ICsda : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ICsdaVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICsda * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICsda * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICsda * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICsda * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICsda * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICsda * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICsda * This,
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
        
        END_INTERFACE
    } ICsdaVtbl;

    interface ICsda
    {
        CONST_VTBL struct ICsdaVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICsda_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICsda_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICsda_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICsda_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICsda_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICsda_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICsda_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ICsda_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Document,0xF60F1185,0xA60D,0x11D0,0x97,0x5C,0x00,0x00,0xC0,0xC2,0x48,0x4C);

#ifdef __cplusplus

class DECLSPEC_UUID("F60F1185-A60D-11D0-975C-0000C0C2484C")
Document;
#endif
#endif /* __Csda_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


