// ==============================
// ! Scriptability related code !
// ==============================

/////////////////////////////////////////////////////
//
// This file implements the nsScriptablePeer object
// The native methods of this class are supposed to
// be callable from JavaScript
//

#include "plugin.h"
/* #include "nsStringAPI.h" */
#include "stdafx.h"
#include "../Common/GetComputerIdentity.h"


static NS_DEFINE_IID(kIScriptableIID, NS_ICOMPUTERIDENTITY_IID);
static NS_DEFINE_IID(kIClassInfoIID, NS_ICLASSINFO_IID);
static NS_DEFINE_IID(kISupportsIID, NS_ISUPPORTS_IID);

nsScriptablePeer::nsScriptablePeer(nsPluginInstance* aPlugin)
{
  mPlugin = aPlugin;
  mRefCnt = 0;
}

nsScriptablePeer::~nsScriptablePeer()
{
}

// AddRef, Release and QueryInterface are common methods and must 
// be implemented for any interface
NS_IMETHODIMP_(nsrefcnt) nsScriptablePeer::AddRef() 
{ 
  ++mRefCnt; 
  return mRefCnt; 
} 

NS_IMETHODIMP_(nsrefcnt) nsScriptablePeer::Release() 
{ 
  --mRefCnt; 
  if (mRefCnt == 0) { 
    delete this;
    return 0; 
  } 
  return mRefCnt; 
} 

// here nsScriptablePeer should return three interfaces it can be asked for by their iid's
// static casts are necessary to ensure that correct pointer is returned
NS_IMETHODIMP nsScriptablePeer::QueryInterface(const nsIID& aIID, void** aInstancePtr) 
{ 
  if(!aInstancePtr) 
    return NS_ERROR_NULL_POINTER; 

  if(aIID.Equals(kIScriptableIID)) {
    *aInstancePtr = static_cast<nsIComputerIdentity*>(this); 
    AddRef();
    return NS_OK;
  }

  if(aIID.Equals(kIClassInfoIID)) {
    *aInstancePtr = static_cast<nsIClassInfo*>(this); 
    AddRef();
    return NS_OK;
  }

  if(aIID.Equals(kISupportsIID)) {
    *aInstancePtr = static_cast<nsISupports*>(static_cast<nsIComputerIdentity*>(this)); 
    AddRef();
    return NS_OK;
  }

  return NS_NOINTERFACE; 
}

void nsScriptablePeer::SetInstance(nsPluginInstance* plugin)
{
  mPlugin = plugin;
}

//
// the following methods will be callable from JavaScript
NS_IMETHODIMP nsScriptablePeer::GetIdentity(char** aIdentity)
{
	USES_CONVERSION;

	CComBSTR computerIdentity = GetComputerIdentity();

	*aIdentity = (char*)NPN_MemAlloc(computerIdentity.Length() + 1);
	strcpy(*aIdentity, OLE2A(computerIdentity.m_str));

	// NS_CStringSetData(aIdentity, OLE2A(computerIdentity.m_str));

	// NS_CStringSetData(aIdentity, "THIS IS A TEST");

	return NS_OK;
}
