

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Sun Jun 26 15:49:08 2011
 */
/* Compiler settings for .\ComputerIdentityCtrl.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ComputerIdentityCtrl_h__
#define __ComputerIdentityCtrl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IComputerIdentity_FWD_DEFINED__
#define __IComputerIdentity_FWD_DEFINED__
typedef interface IComputerIdentity IComputerIdentity;
#endif 	/* __IComputerIdentity_FWD_DEFINED__ */


#ifndef __ComputerIdentity_FWD_DEFINED__
#define __ComputerIdentity_FWD_DEFINED__

#ifdef __cplusplus
typedef class ComputerIdentity ComputerIdentity;
#else
typedef struct ComputerIdentity ComputerIdentity;
#endif /* __cplusplus */

#endif 	/* __ComputerIdentity_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IComputerIdentity_INTERFACE_DEFINED__
#define __IComputerIdentity_INTERFACE_DEFINED__

/* interface IComputerIdentity */
/* [unique][helpstring][nonextensible][oleautomation][dual][uuid][object] */ 


EXTERN_C const IID IID_IComputerIdentity;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("417EB9AB-562B-426F-BD66-A0226BDDD3CC")
    IComputerIdentity : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Identity( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IComputerIdentityVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IComputerIdentity * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IComputerIdentity * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IComputerIdentity * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IComputerIdentity * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IComputerIdentity * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IComputerIdentity * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IComputerIdentity * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Identity )( 
            IComputerIdentity * This,
            /* [retval][out] */ BSTR *pVal);
        
        END_INTERFACE
    } IComputerIdentityVtbl;

    interface IComputerIdentity
    {
        CONST_VTBL struct IComputerIdentityVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IComputerIdentity_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IComputerIdentity_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IComputerIdentity_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IComputerIdentity_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IComputerIdentity_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IComputerIdentity_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IComputerIdentity_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IComputerIdentity_get_Identity(This,pVal)	\
    (This)->lpVtbl -> get_Identity(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IComputerIdentity_get_Identity_Proxy( 
    IComputerIdentity * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IComputerIdentity_get_Identity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IComputerIdentity_INTERFACE_DEFINED__ */



#ifndef __ComputerIdentityCtrlLib_LIBRARY_DEFINED__
#define __ComputerIdentityCtrlLib_LIBRARY_DEFINED__

/* library ComputerIdentityCtrlLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ComputerIdentityCtrlLib;

EXTERN_C const CLSID CLSID_ComputerIdentity;

#ifdef __cplusplus

class DECLSPEC_UUID("4B631C42-F4D9-42B8-9E4E-5FEA45DF488D")
ComputerIdentity;
#endif
#endif /* __ComputerIdentityCtrlLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


