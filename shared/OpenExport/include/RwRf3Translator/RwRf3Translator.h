/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Feb 12 13:12:31 2004
 */
/* Compiler settings for C:\daily\export\RwRf3Translator\RwRf3Translator.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


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

#ifndef __RwRf3Translator_h__
#define __RwRf3Translator_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IRf3Translator_FWD_DEFINED__
#define __IRf3Translator_FWD_DEFINED__
typedef interface IRf3Translator IRf3Translator;
#endif 	/* __IRf3Translator_FWD_DEFINED__ */


#ifndef __IRf3AssetInfo_FWD_DEFINED__
#define __IRf3AssetInfo_FWD_DEFINED__
typedef interface IRf3AssetInfo IRf3AssetInfo;
#endif 	/* __IRf3AssetInfo_FWD_DEFINED__ */


#ifndef __IRf3ExportOptions_FWD_DEFINED__
#define __IRf3ExportOptions_FWD_DEFINED__
typedef interface IRf3ExportOptions IRf3ExportOptions;
#endif 	/* __IRf3ExportOptions_FWD_DEFINED__ */


#ifndef __IRf3AssetInfoCollection_FWD_DEFINED__
#define __IRf3AssetInfoCollection_FWD_DEFINED__
typedef interface IRf3AssetInfoCollection IRf3AssetInfoCollection;
#endif 	/* __IRf3AssetInfoCollection_FWD_DEFINED__ */


#ifndef __IRf3ProgressEvents_FWD_DEFINED__
#define __IRf3ProgressEvents_FWD_DEFINED__
typedef interface IRf3ProgressEvents IRf3ProgressEvents;
#endif 	/* __IRf3ProgressEvents_FWD_DEFINED__ */


#ifndef __IRf3MessageEvents_FWD_DEFINED__
#define __IRf3MessageEvents_FWD_DEFINED__
typedef interface IRf3MessageEvents IRf3MessageEvents;
#endif 	/* __IRf3MessageEvents_FWD_DEFINED__ */


#ifndef ___IRf3MessageEvents_FWD_DEFINED__
#define ___IRf3MessageEvents_FWD_DEFINED__
typedef interface _IRf3MessageEvents _IRf3MessageEvents;
#endif 	/* ___IRf3MessageEvents_FWD_DEFINED__ */


#ifndef __Rf3Translator_FWD_DEFINED__
#define __Rf3Translator_FWD_DEFINED__

#ifdef __cplusplus
typedef class Rf3Translator Rf3Translator;
#else
typedef struct Rf3Translator Rf3Translator;
#endif /* __cplusplus */

#endif 	/* __Rf3Translator_FWD_DEFINED__ */


#ifndef __Rf3AssetInfo_FWD_DEFINED__
#define __Rf3AssetInfo_FWD_DEFINED__

#ifdef __cplusplus
typedef class Rf3AssetInfo Rf3AssetInfo;
#else
typedef struct Rf3AssetInfo Rf3AssetInfo;
#endif /* __cplusplus */

#endif 	/* __Rf3AssetInfo_FWD_DEFINED__ */


#ifndef __Rf3ExportOptions_FWD_DEFINED__
#define __Rf3ExportOptions_FWD_DEFINED__

#ifdef __cplusplus
typedef class Rf3ExportOptions Rf3ExportOptions;
#else
typedef struct Rf3ExportOptions Rf3ExportOptions;
#endif /* __cplusplus */

#endif 	/* __Rf3ExportOptions_FWD_DEFINED__ */


#ifndef __Rf3AssetInfoCollection_FWD_DEFINED__
#define __Rf3AssetInfoCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class Rf3AssetInfoCollection Rf3AssetInfoCollection;
#else
typedef struct Rf3AssetInfoCollection Rf3AssetInfoCollection;
#endif /* __cplusplus */

#endif 	/* __Rf3AssetInfoCollection_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_RwRf3Translator_0000 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_RwRf3Translator_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_RwRf3Translator_0000_v0_0_s_ifspec;

#ifndef __IRf3Translator_INTERFACE_DEFINED__
#define __IRf3Translator_INTERFACE_DEFINED__

/* interface IRf3Translator */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IRf3Translator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("68ED4EB9-A2DE-4284-BBE2-8433C43BF51A")
    IRf3Translator : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Translate( 
            VARIANT vRf3FileNames,
            BSTR strProjTempl,
            BSTR strProjectName,
            BSTR strExportPath,
            /* [in] */ IRf3ExportOptions __RPC_FAR *pOptionOverrides) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAssetInfo( 
            /* [in] */ BSTR strRf3FileName,
            /* [in] */ BSTR strProjectTemplate,
            /* [in] */ BSTR strExportPath,
            /* [in] */ BSTR strProjectName,
            /* [in] */ IRf3ExportOptions __RPC_FAR *pOptionOverrides,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pAssetInfoCollection) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ProjectTemplateFolder( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ProjectTemplateFolder( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AssetTemplateFolder( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AssetTemplateFolder( 
            /* [in] */ BSTR newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRf3TranslatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRf3Translator __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRf3Translator __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRf3Translator __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IRf3Translator __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IRf3Translator __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IRf3Translator __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IRf3Translator __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Translate )( 
            IRf3Translator __RPC_FAR * This,
            VARIANT vRf3FileNames,
            BSTR strProjTempl,
            BSTR strProjectName,
            BSTR strExportPath,
            /* [in] */ IRf3ExportOptions __RPC_FAR *pOptionOverrides);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAssetInfo )( 
            IRf3Translator __RPC_FAR * This,
            /* [in] */ BSTR strRf3FileName,
            /* [in] */ BSTR strProjectTemplate,
            /* [in] */ BSTR strExportPath,
            /* [in] */ BSTR strProjectName,
            /* [in] */ IRf3ExportOptions __RPC_FAR *pOptionOverrides,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pAssetInfoCollection);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ProjectTemplateFolder )( 
            IRf3Translator __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ProjectTemplateFolder )( 
            IRf3Translator __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AssetTemplateFolder )( 
            IRf3Translator __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AssetTemplateFolder )( 
            IRf3Translator __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        END_INTERFACE
    } IRf3TranslatorVtbl;

    interface IRf3Translator
    {
        CONST_VTBL struct IRf3TranslatorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRf3Translator_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRf3Translator_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRf3Translator_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRf3Translator_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRf3Translator_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRf3Translator_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRf3Translator_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRf3Translator_Translate(This,vRf3FileNames,strProjTempl,strProjectName,strExportPath,pOptionOverrides)	\
    (This)->lpVtbl -> Translate(This,vRf3FileNames,strProjTempl,strProjectName,strExportPath,pOptionOverrides)

#define IRf3Translator_GetAssetInfo(This,strRf3FileName,strProjectTemplate,strExportPath,strProjectName,pOptionOverrides,pAssetInfoCollection)	\
    (This)->lpVtbl -> GetAssetInfo(This,strRf3FileName,strProjectTemplate,strExportPath,strProjectName,pOptionOverrides,pAssetInfoCollection)

#define IRf3Translator_get_ProjectTemplateFolder(This,pVal)	\
    (This)->lpVtbl -> get_ProjectTemplateFolder(This,pVal)

#define IRf3Translator_put_ProjectTemplateFolder(This,newVal)	\
    (This)->lpVtbl -> put_ProjectTemplateFolder(This,newVal)

#define IRf3Translator_get_AssetTemplateFolder(This,pVal)	\
    (This)->lpVtbl -> get_AssetTemplateFolder(This,pVal)

#define IRf3Translator_put_AssetTemplateFolder(This,newVal)	\
    (This)->lpVtbl -> put_AssetTemplateFolder(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRf3Translator_Translate_Proxy( 
    IRf3Translator __RPC_FAR * This,
    VARIANT vRf3FileNames,
    BSTR strProjTempl,
    BSTR strProjectName,
    BSTR strExportPath,
    /* [in] */ IRf3ExportOptions __RPC_FAR *pOptionOverrides);


void __RPC_STUB IRf3Translator_Translate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRf3Translator_GetAssetInfo_Proxy( 
    IRf3Translator __RPC_FAR * This,
    /* [in] */ BSTR strRf3FileName,
    /* [in] */ BSTR strProjectTemplate,
    /* [in] */ BSTR strExportPath,
    /* [in] */ BSTR strProjectName,
    /* [in] */ IRf3ExportOptions __RPC_FAR *pOptionOverrides,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pAssetInfoCollection);


void __RPC_STUB IRf3Translator_GetAssetInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRf3Translator_get_ProjectTemplateFolder_Proxy( 
    IRf3Translator __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IRf3Translator_get_ProjectTemplateFolder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRf3Translator_put_ProjectTemplateFolder_Proxy( 
    IRf3Translator __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IRf3Translator_put_ProjectTemplateFolder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRf3Translator_get_AssetTemplateFolder_Proxy( 
    IRf3Translator __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IRf3Translator_get_AssetTemplateFolder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRf3Translator_put_AssetTemplateFolder_Proxy( 
    IRf3Translator __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IRf3Translator_put_AssetTemplateFolder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRf3Translator_INTERFACE_DEFINED__ */


#ifndef __IRf3AssetInfo_INTERFACE_DEFINED__
#define __IRf3AssetInfo_INTERFACE_DEFINED__

/* interface IRf3AssetInfo */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IRf3AssetInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F7DD30AD-1FC4-4EED-A5F7-AC418DD5D4BD")
    IRf3AssetInfo : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TemplateName( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TemplateName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Guid( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Guid( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Translator( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Translator( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            /* [in] */ BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CustomExportFile( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CustomExportFile( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ExportFileNames( 
            /* [retval][out] */ VARIANT __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ExportFileNames( 
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ExportPlatforms( 
            /* [retval][out] */ VARIANT __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ExportPlatforms( 
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Children( 
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pAssetInfoCollection) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRf3AssetInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRf3AssetInfo __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRf3AssetInfo __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TemplateName )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TemplateName )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Guid )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Guid )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Translator )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Translator )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Enabled )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Enabled )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [in] */ BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CustomExportFile )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_CustomExportFile )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ExportFileNames )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ExportFileNames )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ExportPlatforms )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ExportPlatforms )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Children )( 
            IRf3AssetInfo __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pAssetInfoCollection);
        
        END_INTERFACE
    } IRf3AssetInfoVtbl;

    interface IRf3AssetInfo
    {
        CONST_VTBL struct IRf3AssetInfoVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRf3AssetInfo_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRf3AssetInfo_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRf3AssetInfo_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRf3AssetInfo_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRf3AssetInfo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRf3AssetInfo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRf3AssetInfo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRf3AssetInfo_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define IRf3AssetInfo_put_Name(This,newVal)	\
    (This)->lpVtbl -> put_Name(This,newVal)

#define IRf3AssetInfo_get_TemplateName(This,pVal)	\
    (This)->lpVtbl -> get_TemplateName(This,pVal)

#define IRf3AssetInfo_put_TemplateName(This,newVal)	\
    (This)->lpVtbl -> put_TemplateName(This,newVal)

#define IRf3AssetInfo_get_Guid(This,pVal)	\
    (This)->lpVtbl -> get_Guid(This,pVal)

#define IRf3AssetInfo_put_Guid(This,newVal)	\
    (This)->lpVtbl -> put_Guid(This,newVal)

#define IRf3AssetInfo_get_Translator(This,pVal)	\
    (This)->lpVtbl -> get_Translator(This,pVal)

#define IRf3AssetInfo_put_Translator(This,newVal)	\
    (This)->lpVtbl -> put_Translator(This,newVal)

#define IRf3AssetInfo_get_Enabled(This,pVal)	\
    (This)->lpVtbl -> get_Enabled(This,pVal)

#define IRf3AssetInfo_put_Enabled(This,newVal)	\
    (This)->lpVtbl -> put_Enabled(This,newVal)

#define IRf3AssetInfo_get_CustomExportFile(This,pVal)	\
    (This)->lpVtbl -> get_CustomExportFile(This,pVal)

#define IRf3AssetInfo_put_CustomExportFile(This,newVal)	\
    (This)->lpVtbl -> put_CustomExportFile(This,newVal)

#define IRf3AssetInfo_get_ExportFileNames(This,pVal)	\
    (This)->lpVtbl -> get_ExportFileNames(This,pVal)

#define IRf3AssetInfo_put_ExportFileNames(This,newVal)	\
    (This)->lpVtbl -> put_ExportFileNames(This,newVal)

#define IRf3AssetInfo_get_ExportPlatforms(This,pVal)	\
    (This)->lpVtbl -> get_ExportPlatforms(This,pVal)

#define IRf3AssetInfo_put_ExportPlatforms(This,newVal)	\
    (This)->lpVtbl -> put_ExportPlatforms(This,newVal)

#define IRf3AssetInfo_get_Children(This,pAssetInfoCollection)	\
    (This)->lpVtbl -> get_Children(This,pAssetInfoCollection)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_get_Name_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IRf3AssetInfo_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_put_Name_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IRf3AssetInfo_put_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_get_TemplateName_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IRf3AssetInfo_get_TemplateName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_put_TemplateName_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IRf3AssetInfo_put_TemplateName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_get_Guid_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IRf3AssetInfo_get_Guid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_put_Guid_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IRf3AssetInfo_put_Guid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_get_Translator_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IRf3AssetInfo_get_Translator_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_put_Translator_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IRf3AssetInfo_put_Translator_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_get_Enabled_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [retval][out] */ BOOL __RPC_FAR *pVal);


void __RPC_STUB IRf3AssetInfo_get_Enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_put_Enabled_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [in] */ BOOL newVal);


void __RPC_STUB IRf3AssetInfo_put_Enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_get_CustomExportFile_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IRf3AssetInfo_get_CustomExportFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_put_CustomExportFile_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IRf3AssetInfo_put_CustomExportFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_get_ExportFileNames_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pVal);


void __RPC_STUB IRf3AssetInfo_get_ExportFileNames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_put_ExportFileNames_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [in] */ VARIANT newVal);


void __RPC_STUB IRf3AssetInfo_put_ExportFileNames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_get_ExportPlatforms_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pVal);


void __RPC_STUB IRf3AssetInfo_get_ExportPlatforms_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_put_ExportPlatforms_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [in] */ VARIANT newVal);


void __RPC_STUB IRf3AssetInfo_put_ExportPlatforms_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfo_get_Children_Proxy( 
    IRf3AssetInfo __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pAssetInfoCollection);


void __RPC_STUB IRf3AssetInfo_get_Children_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRf3AssetInfo_INTERFACE_DEFINED__ */


#ifndef __IRf3ExportOptions_INTERFACE_DEFINED__
#define __IRf3ExportOptions_INTERFACE_DEFINED__

/* interface IRf3ExportOptions */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IRf3ExportOptions;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5431A951-7F95-42C7-B6A5-6803AD1AEF80")
    IRf3ExportOptions : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOption( 
            BSTR strOptionName,
            VARIANT varOptionValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetOption( 
            BSTR strOptionName,
            VARIANT __RPC_FAR *varOptionValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRf3ExportOptionsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRf3ExportOptions __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRf3ExportOptions __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRf3ExportOptions __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IRf3ExportOptions __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IRf3ExportOptions __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IRf3ExportOptions __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IRf3ExportOptions __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOption )( 
            IRf3ExportOptions __RPC_FAR * This,
            BSTR strOptionName,
            VARIANT varOptionValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOption )( 
            IRf3ExportOptions __RPC_FAR * This,
            BSTR strOptionName,
            VARIANT __RPC_FAR *varOptionValue);
        
        END_INTERFACE
    } IRf3ExportOptionsVtbl;

    interface IRf3ExportOptions
    {
        CONST_VTBL struct IRf3ExportOptionsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRf3ExportOptions_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRf3ExportOptions_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRf3ExportOptions_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRf3ExportOptions_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRf3ExportOptions_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRf3ExportOptions_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRf3ExportOptions_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRf3ExportOptions_SetOption(This,strOptionName,varOptionValue)	\
    (This)->lpVtbl -> SetOption(This,strOptionName,varOptionValue)

#define IRf3ExportOptions_GetOption(This,strOptionName,varOptionValue)	\
    (This)->lpVtbl -> GetOption(This,strOptionName,varOptionValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRf3ExportOptions_SetOption_Proxy( 
    IRf3ExportOptions __RPC_FAR * This,
    BSTR strOptionName,
    VARIANT varOptionValue);


void __RPC_STUB IRf3ExportOptions_SetOption_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRf3ExportOptions_GetOption_Proxy( 
    IRf3ExportOptions __RPC_FAR * This,
    BSTR strOptionName,
    VARIANT __RPC_FAR *varOptionValue);


void __RPC_STUB IRf3ExportOptions_GetOption_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRf3ExportOptions_INTERFACE_DEFINED__ */


#ifndef __IRf3AssetInfoCollection_INTERFACE_DEFINED__
#define __IRf3AssetInfoCollection_INTERFACE_DEFINED__

/* interface IRf3AssetInfoCollection */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IRf3AssetInfoCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A20DE028-9968-43A2-A968-5808FE9FED27")
    IRf3AssetInfoCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT nIndex,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *pEnumerator) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRf3AssetInfoCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRf3AssetInfoCollection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRf3AssetInfoCollection __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRf3AssetInfoCollection __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IRf3AssetInfoCollection __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IRf3AssetInfoCollection __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IRf3AssetInfoCollection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IRf3AssetInfoCollection __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IRf3AssetInfoCollection __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Item )( 
            IRf3AssetInfoCollection __RPC_FAR * This,
            /* [in] */ VARIANT nIndex,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get__NewEnum )( 
            IRf3AssetInfoCollection __RPC_FAR * This,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *pEnumerator);
        
        END_INTERFACE
    } IRf3AssetInfoCollectionVtbl;

    interface IRf3AssetInfoCollection
    {
        CONST_VTBL struct IRf3AssetInfoCollectionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRf3AssetInfoCollection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRf3AssetInfoCollection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRf3AssetInfoCollection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRf3AssetInfoCollection_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRf3AssetInfoCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRf3AssetInfoCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRf3AssetInfoCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRf3AssetInfoCollection_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define IRf3AssetInfoCollection_get_Item(This,nIndex,pVal)	\
    (This)->lpVtbl -> get_Item(This,nIndex,pVal)

#define IRf3AssetInfoCollection_get__NewEnum(This,pEnumerator)	\
    (This)->lpVtbl -> get__NewEnum(This,pEnumerator)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfoCollection_get_Count_Proxy( 
    IRf3AssetInfoCollection __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IRf3AssetInfoCollection_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfoCollection_get_Item_Proxy( 
    IRf3AssetInfoCollection __RPC_FAR * This,
    /* [in] */ VARIANT nIndex,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IRf3AssetInfoCollection_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRf3AssetInfoCollection_get__NewEnum_Proxy( 
    IRf3AssetInfoCollection __RPC_FAR * This,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *pEnumerator);


void __RPC_STUB IRf3AssetInfoCollection_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRf3AssetInfoCollection_INTERFACE_DEFINED__ */



#ifndef __RWRF3TRANSLATORLib_LIBRARY_DEFINED__
#define __RWRF3TRANSLATORLib_LIBRARY_DEFINED__

/* library RWRF3TRANSLATORLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_RWRF3TRANSLATORLib;

#ifndef __IRf3ProgressEvents_INTERFACE_DEFINED__
#define __IRf3ProgressEvents_INTERFACE_DEFINED__

/* interface IRf3ProgressEvents */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID_IRf3ProgressEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D37814AD-4F28-47bc-A16C-13940B7D3F10")
    IRf3ProgressEvents : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetProgress( 
            float fProgress) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsCancelled( 
            BOOL bCancelled) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRf3ProgressEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRf3ProgressEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRf3ProgressEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRf3ProgressEvents __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetProgress )( 
            IRf3ProgressEvents __RPC_FAR * This,
            float fProgress);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsCancelled )( 
            IRf3ProgressEvents __RPC_FAR * This,
            BOOL bCancelled);
        
        END_INTERFACE
    } IRf3ProgressEventsVtbl;

    interface IRf3ProgressEvents
    {
        CONST_VTBL struct IRf3ProgressEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRf3ProgressEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRf3ProgressEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRf3ProgressEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRf3ProgressEvents_SetProgress(This,fProgress)	\
    (This)->lpVtbl -> SetProgress(This,fProgress)

#define IRf3ProgressEvents_IsCancelled(This,bCancelled)	\
    (This)->lpVtbl -> IsCancelled(This,bCancelled)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRf3ProgressEvents_SetProgress_Proxy( 
    IRf3ProgressEvents __RPC_FAR * This,
    float fProgress);


void __RPC_STUB IRf3ProgressEvents_SetProgress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRf3ProgressEvents_IsCancelled_Proxy( 
    IRf3ProgressEvents __RPC_FAR * This,
    BOOL bCancelled);


void __RPC_STUB IRf3ProgressEvents_IsCancelled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRf3ProgressEvents_INTERFACE_DEFINED__ */


#ifndef __IRf3MessageEvents_INTERFACE_DEFINED__
#define __IRf3MessageEvents_INTERFACE_DEFINED__

/* interface IRf3MessageEvents */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID_IRf3MessageEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3F1A036D-8841-4937-AA8E-FAC3701364EE")
    IRf3MessageEvents : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddMessage( 
            BSTR strObject,
            BSTR strMessage,
            int nSeverity,
            int nVerbosity) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRf3MessageEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRf3MessageEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRf3MessageEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRf3MessageEvents __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddMessage )( 
            IRf3MessageEvents __RPC_FAR * This,
            BSTR strObject,
            BSTR strMessage,
            int nSeverity,
            int nVerbosity);
        
        END_INTERFACE
    } IRf3MessageEventsVtbl;

    interface IRf3MessageEvents
    {
        CONST_VTBL struct IRf3MessageEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRf3MessageEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRf3MessageEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRf3MessageEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRf3MessageEvents_AddMessage(This,strObject,strMessage,nSeverity,nVerbosity)	\
    (This)->lpVtbl -> AddMessage(This,strObject,strMessage,nSeverity,nVerbosity)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRf3MessageEvents_AddMessage_Proxy( 
    IRf3MessageEvents __RPC_FAR * This,
    BSTR strObject,
    BSTR strMessage,
    int nSeverity,
    int nVerbosity);


void __RPC_STUB IRf3MessageEvents_AddMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRf3MessageEvents_INTERFACE_DEFINED__ */


#ifndef ___IRf3MessageEvents_DISPINTERFACE_DEFINED__
#define ___IRf3MessageEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IRf3MessageEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IRf3MessageEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("66B45D2E-5EFF-4a22-8988-92DF309E4A00")
    _IRf3MessageEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IRf3MessageEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _IRf3MessageEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _IRf3MessageEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _IRf3MessageEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _IRf3MessageEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _IRf3MessageEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _IRf3MessageEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _IRf3MessageEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _IRf3MessageEventsVtbl;

    interface _IRf3MessageEvents
    {
        CONST_VTBL struct _IRf3MessageEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IRf3MessageEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IRf3MessageEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IRf3MessageEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IRf3MessageEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IRf3MessageEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IRf3MessageEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IRf3MessageEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IRf3MessageEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Rf3Translator;

#ifdef __cplusplus

class DECLSPEC_UUID("2E89F367-2D1C-47A7-A3E3-5959087489D6")
Rf3Translator;
#endif

EXTERN_C const CLSID CLSID_Rf3AssetInfo;

#ifdef __cplusplus

class DECLSPEC_UUID("3C174C3F-C9C2-4306-97BC-EE032034C62F")
Rf3AssetInfo;
#endif

EXTERN_C const CLSID CLSID_Rf3ExportOptions;

#ifdef __cplusplus

class DECLSPEC_UUID("34228538-CA51-4318-ABA1-9318256CD478")
Rf3ExportOptions;
#endif

EXTERN_C const CLSID CLSID_Rf3AssetInfoCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("624F7D47-05B9-4A9C-A103-5FC29E9D9A8B")
Rf3AssetInfoCollection;
#endif
#endif /* __RWRF3TRANSLATORLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
