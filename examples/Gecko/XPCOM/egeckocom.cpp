/* $Id$ */

#include "egeckocom.h"
#include "nsxpcm.h"
#include "nsIConsoleService.h"

NS_IMPL_ISUPPORTS1(eshared, ieshared)

#define RBB	ERRBegin

#define RP	\
		nsresult NSResult = NS_OK;\
	ERRProlog
#define RB	RBB { if ( !Connected ) ERRu(); kernel::SpecialDebug( "--> " __FILE__ " ", __LINE__ ); }
#define RR	\
	ERRErr	\
	if ( ERRMajor != err::itn ) {\
		NSResult = NS_ERROR_FAILURE;\
	} else if ( ERRMinor != err::iReturn ) {\
		NSResult = NS_ERROR_FAILURE;\
	}\
	ERRRst();
#define RN	ERREnd
#define RE	\
	ERREpilog\
	return NSResult;

NS_IMETHODIMP eshared::Test( void )
{
RP
	nsCOMPtr<nsIConsoleService> ConsoleService = NULL;
	NS_NAMED_LITERAL_STRING(wideString, "some string" );
RBB

	nsxpcm::GetService<nsIConsoleService>( "@mozilla.org/consoleservice;1", ConsoleService );

	ConsoleService->LogStringMessage( wideString.get() );
RR
RN
RE
}

NS_IMETHODIMP eshared::Set(const char *Value)
{
RP
RBB
	this->Value.Init( Value );
RR
RN
RE
}

/* string Get (); */
NS_IMETHODIMP eshared::Get(char **_retval)
{
RP
RBB
	nsxpcm::Transform( Value, _retval );
RR
RN
RE
}



eshared::eshared()
{
	Value.Init();
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
