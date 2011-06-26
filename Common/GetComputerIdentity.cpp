#include "stdafx.h"
#include "GetComputerIdentity.h"

#include <nb30.h>
#include <wincon.h>   
#include <snmp.h>

static CComBSTR IdentifySelf();
static CComBSTR IdentifySelf2();
static CComBSTR IdentifySelf3();
static CComBSTR GlobalID();
static CComBSTR GetMACViaNetbios(BOOL legacy_search = FALSE);
static CComBSTR GetMACViaSNMP();
static CComBSTR Format(const TCHAR* szFormat, ...);

// ----------------------------------------------------------------------------

CComBSTR GetComputerIdentity()
{
	return IdentifySelf3();
}

// ----------------------------------------------------------------------------

static CComBSTR IdentifySelf()
{
	CComBSTR  self ;

	TCHAR  volume_name[1024] ;
	DWORD  volume_serial_number ;
	DWORD  max_filename_length ;
	DWORD  file_system_flags ;
	TCHAR  file_system_name[1024] ;

	BOOL rval ;
	rval = GetVolumeInformation(	TEXT("C:\\"),
									volume_name,
									sizeof(volume_name) / sizeof(TCHAR),
									&volume_serial_number,
									&max_filename_length,
									&file_system_flags,
									file_system_name,
									sizeof(file_system_name) / sizeof(TCHAR) ) ;
	if ( rval != 0 )
	{
		//  successfully retrieved volume info
		DWORD SectorsPerCluster = 0 ;
		DWORD BytesPerSector = 0 ;
		DWORD NumberOfFreeClusters = 0 ;
		DWORD TotalNumberOfClusters = 0 ;
		//  Get the size of the disk
		rval = GetDiskFreeSpace(	TEXT("C:\\"),
									&SectorsPerCluster,
									&BytesPerSector,
									&NumberOfFreeClusters,
									&TotalNumberOfClusters ) ;

		if ( file_system_flags & FS_VOL_IS_COMPRESSED )
		{
			// If this is a compressed volume, then we need to 
			// eliminate some numbers off the end of the clusters.
			// Compresed volumes compute clusters dynamically
			// based on available space.

			// We'll assume that the number of clusters will never
			// change more than 4096 in size for compressed drives.
			// So we just zero out the bottom 3 bytes.
			TotalNumberOfClusters = TotalNumberOfClusters & 0xFFFFF000;
		}

		//  Combine the pieces into a single string
		self = Format( TEXT("%08X%s%s%08X"), 
						volume_serial_number, 
						volume_name, 
						file_system_name, 
						TotalNumberOfClusters ) ;
	}

	// LICTRACE( "IdentifySelf : Returning '%s'\n", (const char*)self ) ;

	return self ;
}

/*
	Total possible length is:
	12   MAC Address
	15   MAX_COMPUTERNAME_LENGTH (31 on Mac)
	8+   volume serial number
	?    volume name
	5+   file system name
	8    number of clusters
	---
	48 + volume name length + extras
*/
static CComBSTR IdentifySelf2()
{
	CComBSTR  self ;

	self = GetMACViaNetbios();
	// LICTRACE( "IdentifySelf2 : GetMACViaNetbios returned '%s'\n", (const char*)self) ;
	if(self.Length() == 0)
	{
		self = GetMACViaSNMP();
		// LICTRACE( "IdentifySelf2 : GetMACViaSNMP returned '%s'\n", (const char*)self) ;
	}

	TCHAR szMachineName[MAX_COMPUTERNAME_LENGTH + 1];
	szMachineName[0] = 0;
	DWORD cchMachineName = sizeof(szMachineName) / sizeof(TCHAR);
	if(GetComputerName(szMachineName, &cchMachineName))
	{
		// LICTRACE( "IdentifySelf2 : GetComputerName return '%s'\n", szMachineName ) ;
		self += szMachineName;
	}

	self += IdentifySelf();

	return self ;
}

/*
	Total possible length is:
	12   MAC Address
	15   MAX_COMPUTERNAME_LENGTH (31 on Mac)
	8+   volume serial number
	?    volume name
	5+   file system name
	8    number of clusters
	---
	48 + volume name length + extras
*/
static CComBSTR IdentifySelf3()
{
	CComBSTR  self ;

	self = GetMACViaSNMP();
	// LICTRACE( "IdentifySelf3 : GetMACViaSNMP returned '%s'\n", (const char*)self) ;
	if(self.Length() == 0)
	{
		self = GetMACViaNetbios();
		// LICTRACE( "IdentifySelf2 : GetMACViaNetbios returned '%s'\n", (const char*)self) ;
	}

	TCHAR szMachineName[MAX_COMPUTERNAME_LENGTH + 1];
	szMachineName[0] = 0;
	DWORD cchMachineName = sizeof(szMachineName) / sizeof(TCHAR);
	if(GetComputerName(szMachineName, &cchMachineName))
	{
		// LICTRACE( "IdentifySelf3 : GetComputerName return '%s'\n", szMachineName ) ;
		self += szMachineName;
	}

	self += IdentifySelf();

	return self ;
}


static CComBSTR GlobalID()
{
	CComBSTR  self ;

#ifdef _WIN32

	HRESULT result ;
	GUID    guid ;

	result = CoCreateGuid( &guid ) ;
	if ( result == S_OK )
	{
		self = Format( TEXT("%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X"),
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7] ) ;
	}

#else

	self = CComBSTR( time(NULL) ) ;  //  not really global

#endif

	return self ;
}

static bool IsRASMAC(UCHAR rgchMAC[])
{
	UCHAR rgchRASMAC[] = { 0x44, 0x45, 0x53, 0x54, 0, 0 };  // "DEST"
	if(memcmp(rgchMAC, rgchRASMAC, sizeof(rgchRASMAC)) == 0)
		return true;

	UCHAR rgchAOLMAC[] = { 0x00, 0x03, 0x8a };
	if(memcmp(rgchMAC, rgchAOLMAC, sizeof(rgchAOLMAC)) == 0)
		return true;

	return false;
}

static CComBSTR GetMACViaNetbios(BOOL legacy_search)
{
	CComBSTR mac;

	HINSTANCE hinstNetbios = LoadLibrary(TEXT("netapi32.dll"));
	if(!hinstNetbios)
	{
		// printf("Cannot load netapi32.dll\n");
		return mac;
	}
	typedef UCHAR (WINAPI* PFNNETBIOS)(PNCB pncb);
	PFNNETBIOS pfnNetbios = (PFNNETBIOS)GetProcAddress(hinstNetbios ,"Netbios");
	if(!pfnNetbios)
	{
		// printf("Cannot get address for Netbios function\n");
		FreeLibrary(hinstNetbios);
		return mac;
	}

	NCB ncb;
	LANA_ENUM lane;
	memset(&ncb, 0 ,sizeof(ncb));
	memset(&lane, 0, sizeof(lane));
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (unsigned char*)&lane;
	ncb.ncb_length = sizeof(lane);
	if(pfnNetbios(&ncb) != NRC_GOODRET)
	{
		// printf("Cannot execute NCBENUM\n");
		FreeLibrary(hinstNetbios);
		return mac;
	}

	for(int i = 0 ; i < lane.length ; i++)
	{
		// reset netbios interface
		memset(&ncb, 0, sizeof(ncb));
		ncb.ncb_command = NCBRESET;
		ncb.ncb_lana_num = lane.lana[i];
		if(pfnNetbios(&ncb) != NRC_GOODRET)
		{
			// printf("Cannot execute NCBRESET for adapter %d\n", ncb.ncb_lana_num);
			continue;
		}

		memset(&ncb, 0, sizeof(ncb));;
		ncb.ncb_command = NCBASTAT;
		ncb.ncb_lana_num = lane.lana[i];
		strcpy((char*)ncb.ncb_callname , "*               ");
		struct ADAPTER
		{
			ADAPTER_STATUS adas;
			NAME_BUFFER rgnb[30];
		};
		ADAPTER ada;
		memset(&ada, 0, sizeof(ada));
		ncb.ncb_buffer = (unsigned char*)&ada;
		ncb.ncb_length = sizeof(ada);
		if(pfnNetbios(&ncb) != NRC_GOODRET)
		{
			// printf("Cannot execute NCBASTAT for adapter %d\n", ncb.ncb_lana_num);
			continue;
		}

		bool fIsEth = false;
		if(ada.adas.adapter_type == 0xfe && !IsRASMAC(ada.adas.adapter_address))
			fIsEth = true;

		if(legacy_search || fIsEth)
		{
			mac = Format( TEXT("%02X%02X%02X%02X%02X%02X"),
				ada.adas.adapter_address[0],
				ada.adas.adapter_address[1],
				ada.adas.adapter_address[2],
				ada.adas.adapter_address[3],
				ada.adas.adapter_address[4],
				ada.adas.adapter_address[5] ) ;
			break;
		}
	}

	FreeLibrary(hinstNetbios);
	return mac;
} 

static CComBSTR GetMACViaSNMP()
{
	CComBSTR mac;

	HINSTANCE hinstExt = LoadLibrary(TEXT("inetmib1.dll"));
	if(!hinstExt)
	{
		// printf("Cannot load inetmib1.dll\n");
		return mac;
	}
	PFNSNMPEXTENSIONINIT pfnInit = (PFNSNMPEXTENSIONINIT)GetProcAddress(hinstExt, "SnmpExtensionInit");
	PFNSNMPEXTENSIONQUERY pfnQuery = (PFNSNMPEXTENSIONQUERY)GetProcAddress(hinstExt, "SnmpExtensionQuery");
	PFNSNMPEXTENSIONCLOSE pfnClose = (PFNSNMPEXTENSIONCLOSE)GetProcAddress(hinstExt, "SnmpExtensionClose");
	if(!pfnInit || !pfnQuery)
	{
		// printf("Cannot get function addresses from inetmib1.dll\n");
		FreeLibrary(hinstExt);
		return mac;
	}

	HANDLE hTrapEvent;
	AsnObjectIdentifier aoiRegion;

	if(!pfnInit(GetTickCount(), &hTrapEvent, &aoiRegion))
	{
		// printf("Cannot initialize Windows SNMP Extension DLL\n");
		FreeLibrary(hinstExt);
		return mac;
	}

// SNMP interface for # of NIC Entries.
	UINT				OID_ifNumEntries[] = {1, 3, 6, 1, 2, 1, 2, 1};
	AsnObjectIdentifier MIB_ifNumEntries = {sizeof(OID_ifNumEntries) / sizeof(UINT), OID_ifNumEntries};

// SNMP interface for Entry Type.
	UINT				OID_ifEntryType[] = {1, 3, 6, 1, 2, 1, 2, 2, 1, 3};
	AsnObjectIdentifier MIB_ifEntryType = {sizeof(OID_ifEntryType) / sizeof(UINT), OID_ifEntryType};

// SNMP interface for MAC Address.
	UINT				OID_ifMAC[] = {1, 3, 6, 1, 2, 1, 2, 2, 1, 6};
	AsnObjectIdentifier MIB_ifMAC = {sizeof(OID_ifMAC) / sizeof(UINT), OID_ifMAC};

// SNMP interface for IP Address.
	UINT				OID_ifIPAddr[] = {1, 3, 6, 1, 2, 1, 4, 20, 1, 1};
	AsnObjectIdentifier MIB_ifIPAddr = {sizeof(OID_ifIPAddr) / sizeof(UINT), OID_ifIPAddr};

// SNMP interface for Subnet Mask.
	UINT				OID_ifSubnetMask[] = {1, 3, 6, 1, 2, 1, 4, 20, 1, 3};
	AsnObjectIdentifier MIB_ifSubnetMask = {sizeof(OID_ifSubnetMask) / sizeof(UINT), OID_ifSubnetMask};
	
// SNMP interface for Description.
	UINT				OID_ifDesc[] = {1, 3, 6, 1, 2, 1, 2, 2, 1, 2};
	AsnObjectIdentifier MIB_ifDesc = {sizeof(OID_ifDesc) / sizeof(UINT), OID_ifDesc};
	
// SNMP interface for Interface Index
	UINT				OID_ifIndex[] = {1, 3, 6, 1, 2, 1, 4, 20, 1, 2};
	AsnObjectIdentifier MIB_ifIndex = {sizeof(OID_ifIndex) / sizeof(UINT), OID_ifIndex};

// SNMP interface for IP Routing Table
	/* UINT				OID_ifIPRouteTable[] = {1, 3, 6, 1, 2, 1, 4, 21, 1};
	AsnObjectIdentifier MIB_ifIPRouteTable = {sizeof(OID_ifIPRouteTable) / sizeof(UINT), OID_ifIPRouteTable}; */

// SNMP interface for Interface Entry Number
	UINT				OID_ifEntryNum[] = {1, 3, 6, 1, 2, 1, 2, 2, 1, 1};
	AsnObjectIdentifier MIB_ifEntryNum = {sizeof(OID_ifEntryNum) / sizeof(UINT), OID_ifEntryNum};

	const UINT NUM_VARBIND_LIST = 7;
	RFC1157VarBindList	varBindList;
	RFC1157VarBind		varBind[NUM_VARBIND_LIST];
	AsnInteger			errorStatus;
	AsnInteger			errorIndex;
	/* AsnObjectIdentifier MIB_NULL = {0, 0}; */

	// Initialize the variable list to be retrieved by Query
	varBindList.list = varBind;

	// Get number of entries in the Interface table
	varBindList.len = 1;
	SNMP_oidcpy(&varBind[0].name, &MIB_ifNumEntries);
	if(!pfnQuery(ASN_RFC1157_GETNEXTREQUEST, &varBindList, &errorStatus, &errorIndex))
	{
		// printf("Cannot get number of adapters\n");
		pfnClose();
		FreeLibrary(hinstExt);
		return mac;
	}
	int cAdapters = varBind[0].value.asnValue.number;
	SNMP_FreeVarBind(&varBind[0]);
	if(cAdapters == 0)
	{
		pfnClose();
		FreeLibrary(hinstExt);
		return mac;
	}

	// Setup query - you could get a subset of these, if you wished

	varBindList.len = NUM_VARBIND_LIST;
	SNMP_oidcpy(&varBind[0].name, &MIB_ifEntryType);
	SNMP_oidcpy(&varBind[1].name, &MIB_ifMAC);
	SNMP_oidcpy(&varBind[2].name, &MIB_ifIPAddr);
	SNMP_oidcpy(&varBind[3].name, &MIB_ifSubnetMask);
	SNMP_oidcpy(&varBind[4].name, &MIB_ifDesc);
	SNMP_oidcpy(&varBind[5].name, &MIB_ifIndex);
	SNMP_oidcpy(&varBind[6].name, &MIB_ifEntryNum);

	for(;;)
	{
		if(!pfnQuery(ASN_RFC1157_GETNEXTREQUEST, &varBindList, &errorStatus, &errorIndex))
			break;

		// Confirm OIDs returned for EntryType and MAC Address
		if(SNMP_oidncmp(&varBind[0].name, &MIB_ifEntryType, MIB_ifEntryType.idLength) != 0)
			break;
		if(SNMP_oidncmp(&varBind[1].name, &MIB_ifMAC, MIB_ifMAC.idLength) != 0)
			break;

		int fType = varBind[0].value.asnValue.number;

		UCHAR rgchMAC[6] = { 0, 0, 0, 0, 0, 0 };
		memcpy(rgchMAC, varBind[1].value.asnValue.address.stream, 
			varBind[1].value.asnValue.address.length);

		// See http://www.alvestrand.no/objectid/1.3.6.1.2.1.2.2.1.3.html for a 
		// list of adapter types

		bool fIsEth = false;
		if(varBind[1].value.asnValue.address.length > 0 && 
			(fType == 6 || fType == 7 || fType == 26) &&
			!IsRASMAC(rgchMAC))
		{
			fIsEth = true;
		}

		if(fIsEth)
		{
			mac = Format( TEXT("%02X%02X%02X%02X%02X%02X"),
				rgchMAC[0],
				rgchMAC[1],
				rgchMAC[2],
				rgchMAC[3],
				rgchMAC[4],
				rgchMAC[5] ) ;

			// Do not break - the last Ethernet adapter is typically the best choice
		}


#if 0   /* Other interesting data... */
		UCHAR rgchIP[4] = { 0, 0, 0, 0 };
		if(!SNMP_oidncmp(&varBind[2].name, &MIB_ifIPAddr, MIB_ifIPAddr.idLength))
		{
			memcpy(rgchIP, varBind[2].value.asnValue.address.stream, 
				varBind[2].value.asnValue.address.length);
		}

		UCHAR rgchSubnetMask[4] = { 0, 0, 0, 0 };
		if(!SNMP_oidncmp(&varBind[3].name, &MIB_ifSubnetMask, MIB_ifSubnetMask.idLength))
		{
			memcpy(rgchSubnetMask, varBind[3].value.asnValue.address.stream, 
				varBind[3].value.asnValue.address.length);
		}

		// varBind[4].value.asnValue.address.stream contains the description
#endif
	}

	// Free the bindings
	for(UINT i = 0; i < varBindList.len; i++)
		SNMP_FreeVarBind(&varBind[i]);

	if(pfnClose)
		pfnClose();
	FreeLibrary(hinstExt);
	return mac ;
}


/*
	Windows has a driver arch. for SNMP. In that arch., a "manager" loads
	"drivers" called SNMP Extension DLLs. The Extension DLLs support queries 
	to and traps from various parts of the SNMP MIB hierarchy. Windows ships 
	with an Extension DLL for its own data. This Extension DLL knows the 
	secrets to getting various interesting NIC and IP address information.
	However, Windows does not install the manager by default. ITOT you can 
	bypass the manager and load the Extension DLL directly. When doing this, 
	you miss out on a few Windows SNMP functions that are presumably 
	implemented in the manager DLL. A subset of these are included to make 
	this work without having to have the manager installed.
*/

SNMPAPI
SNMP_FUNC_TYPE
SnmpUtilOidCpy(
    OUT AsnObjectIdentifier *DstObjId,
    IN  AsnObjectIdentifier *SrcObjId
    )
{
  DstObjId->ids = (UINT *)GlobalAlloc(GMEM_ZEROINIT,SrcObjId->idLength * 
          sizeof(UINT));
  if(!DstObjId->ids){
    SetLastError(1);
    return 0;
  }

  memcpy(DstObjId->ids,SrcObjId->ids,SrcObjId->idLength*sizeof(UINT));
  DstObjId->idLength = SrcObjId->idLength;

  return 1;
}


VOID
SNMP_FUNC_TYPE
SnmpUtilOidFree(
    IN OUT AsnObjectIdentifier *ObjId
    )
{
  GlobalFree(ObjId->ids);
  ObjId->ids = 0;
  ObjId->idLength = 0;
}

SNMPAPI
SNMP_FUNC_TYPE
SnmpUtilOidNCmp(
    IN AsnObjectIdentifier *ObjIdA,
    IN AsnObjectIdentifier *ObjIdB,
    IN UINT                 Len
    )
{
  UINT CmpLen;
  UINT i;
  int  res;

  CmpLen = Len;
  if(ObjIdA->idLength < CmpLen)
    CmpLen = ObjIdA->idLength;
  if(ObjIdB->idLength < CmpLen)
    CmpLen = ObjIdB->idLength;

  for(i=0;i<CmpLen;i++){
    res = ObjIdA->ids[i] - ObjIdB->ids[i];
    if(res!=0)
      return res;
  }
  return 0;
}

VOID
SNMP_FUNC_TYPE
SnmpUtilVarBindFree(
    IN OUT RFC1157VarBind *VarBind
    )
{
  BYTE asnType;
  // free object name
  SnmpUtilOidFree(&VarBind->name);

  asnType = VarBind->value.asnType;

  if(asnType==ASN_OBJECTIDENTIFIER){
    SnmpUtilOidFree(&VarBind->value.asnValue.object);
  }
  else if(
        (asnType==ASN_OCTETSTRING) ||
        (asnType==ASN_RFC1155_IPADDRESS) ||
        (asnType==ASN_RFC1155_OPAQUE) ||
        (asnType==ASN_SEQUENCE)){
    if(VarBind->value.asnValue.string.dynamic){
      GlobalFree(VarBind->value.asnValue.string.stream);
    }
  }

  VarBind->value.asnType = ASN_NULL;
}

static CComBSTR Format(const TCHAR* szFormat, ...)
{
	va_list vaArgs;
	va_start(vaArgs, szFormat);

	TCHAR szBuffer[1025];

	wvsprintf(szBuffer, szFormat, vaArgs);
	szBuffer[1024] = 0;

	va_end(vaArgs);

	CComBSTR strBuffer(szBuffer);

	return strBuffer;
}	
