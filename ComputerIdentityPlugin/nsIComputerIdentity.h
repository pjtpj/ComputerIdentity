/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM nsIComputerIdentity.idl
 */

#ifndef __gen_nsIComputerIdentity_h__
#define __gen_nsIComputerIdentity_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIComputerIdentity */
#define NS_ICOMPUTERIDENTITY_IID_STR "417eb9ab-562b-426f-bd66-a0226bddd3cc"

#define NS_ICOMPUTERIDENTITY_IID \
  {0x417eb9ab, 0x562b, 0x426f, \
    { 0xbd, 0x66, 0xa0, 0x22, 0x6b, 0xdd, 0xd3, 0xcc }}

class NS_NO_VTABLE nsIComputerIdentity : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ICOMPUTERIDENTITY_IID)

  /* readonly attribute string Identity; */
  NS_IMETHOD GetIdentity(char * *aIdentity) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICOMPUTERIDENTITY \
  NS_IMETHOD GetIdentity(char * *aIdentity); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICOMPUTERIDENTITY(_to) \
  NS_IMETHOD GetIdentity(char * *aIdentity) { return _to GetIdentity(aIdentity); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICOMPUTERIDENTITY(_to) \
  NS_IMETHOD GetIdentity(char * *aIdentity) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIdentity(aIdentity); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsComputerIdentity : public nsIComputerIdentity
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSICOMPUTERIDENTITY

  nsComputerIdentity();

private:
  ~nsComputerIdentity();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsComputerIdentity, nsIComputerIdentity)

nsComputerIdentity::nsComputerIdentity()
{
  /* member initializers and constructor code */
}

nsComputerIdentity::~nsComputerIdentity()
{
  /* destructor code */
}

/* readonly attribute string Identity; */
NS_IMETHODIMP nsComputerIdentity::GetIdentity(char * *aIdentity)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIComputerIdentity_h__ */
