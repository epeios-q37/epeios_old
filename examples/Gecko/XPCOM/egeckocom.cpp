/* $Id$ */

#include "egeckocom.h"
#include "nsxpcm.h"
#include "nsIConsoleservice.h"

NS_IMPL_ISUPPORTS1(eshared, ieshared)

char * StrAllocThrowA(size_t cchSize)
{
	return new char[cchSize];
}
wchar_t * StrAllocThrowW(size_t cchSize)
{
	return new wchar_t[cchSize];
}
void StrFreeA(char * s)
{
	delete[] s;
}
void StrFreeW(wchar_t * s)
{
	delete[] s;
}
 
//Surcharge C++, plus simple.
inline void StrFree( char   * s) { return StrFreeA(s); }
inline void StrFree(wchar_t * s) { return StrFreeW(s); }
 


wchar_t * ambstowcs(char const *sczA)
{
	size_t const cchLenA = strlen(sczA);
	size_t const cchLenW = mbstowcs(NULL, sczA, cchLenA+1);
	wchar_t * szW = StrAllocThrowW(cchLenW+1);
	mbstowcs(szW, sczA, cchLenA+1);
	return szW;
}


#define RBB	ERRBegin

#define RP	\
		nsresult NSResult = NS_OK;\
	ERRProlog
#define RB	RBB { if ( !Connected ) ERRu(); kernel::SpecialDebug( "--> " __FILE__ " ", __LINE__ ); }
#define RR	\
	ERRErr	\
/*	err::buffer__ Buffer;\
	if ( ERRMajor != err::itn ) {\
		Backend.SetMessage( "Frontend", err::Message( Buffer ) );\
		NSResult = NS_ERROR_FAILURE;\
	} else if ( ERRMinor != err::iReturn ) {\
		NSResult = NS_ERROR_FAILURE;\
		if ( ERRMinor != err::iBeam )\
			Backend.SetMessage( "Frontend", err::Message( Buffer ) );\
	}\
*/	ERRRst();
#define RN	ERREnd
#define RE	\
	ERREpilog\
	return NSResult;

NS_IMETHODIMP eshared::Test( void )
{
RP
	nsCOMPtr<nsIConsoleService> ConsoleService = NULL;
	char const * orig = "Hello, World!";
RBB
	wchar_t *wcstring = ambstowcs(orig);
 
	nsxpcm::GetService<nsIConsoleService>( "@mozilla.org/consoleservice;1", ConsoleService );

	ConsoleService->LogStringMessage( wcstring );
	
	StrFree(wcstring), wcstring = NULL;
RR
RN
RE
}


eshared::eshared()
{
  /* member initializers and constructor code */
}

eshared::~eshared()
{
  /* destructor code */
}


NS_IMPL_ISUPPORTS1(eprivate, ieprivate)

eprivate::eprivate()
{
  /* member initializers and constructor code */
}

eprivate::~eprivate()
{
  /* destructor code */
}

/* Gecko required part. */

#include "nsIGenericFactory.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(eshared)
NS_GENERIC_FACTORY_CONSTRUCTOR(eprivate)

static nsModuleComponentInfo components[] =
{
    {
       ESHARED_CLASSNAME, 
       ESHARED_CID,
       ESHARED_CONTRACTID,
       esharedConstructor,
    },
    {
       EPRIVATE_CLASSNAME, 
       EPRIVATE_CID,
       EPRIVATE_CONTRACTID,
       eprivateConstructor,
    },
};

NS_IMPL_NSGETMODULE("EpeiosModule", components) 
