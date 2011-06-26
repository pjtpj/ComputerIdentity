// ComputerIdentity.cpp : Implementation of CComputerIdentity
#include "stdafx.h"
#include "ComputerIdentity.h"
#include "../Common/GetComputerIdentity.h"


// CComputerIdentity

STDMETHODIMP CComputerIdentity::get_Identity(BSTR* pVal)
{
	CComBSTR computerIdentity = GetComputerIdentity();

	*pVal = computerIdentity.Copy();

	return S_OK;
}
