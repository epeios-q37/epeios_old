/* $Id$ */

#include "egeckocom.h"
#include "nsxpcm.h"
#include "nsIDOMEventListener.h"
#include "nsIDOMEventTarget.h"

global Global;

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


NS_IMPL_ISUPPORTS1(egeckocom, iegeckocom)

egeckocom::egeckocom()
{
  /* member initializers and constructor code */
}

egeckocom::~egeckocom()
{
  /* destructor code */
}

NS_IMETHODIMP egeckocom::RegisteringStart( void )
{
RP
RBB
	Global.CreateNewKernel();
RR
RN
RE
}

template <typename element, typename id_type > static void _Register(
	element &Core,
	nsIDOMDocument *Document,
	const id_type &Id )
{
	Core.Init( Global.GetCurrentKernelRow() );

	nsxpcm::Register( Core, Document, Id );
}


NS_IMETHODIMP egeckocom::Register(nsIDOMDocument *Document)
{
RP
RBB
	kernel___ &Kernel = Global.GetCurrentKernel();

	Kernel.Document = Document;

	_Register( Kernel.Set, Document, "XPCOMSet" );	// Version 'const char *'.
	_Register( Kernel.Get, Document, str::string( "XPCOMGet" ) );	// Version 'const str::string_'.
	_Register( Kernel.Text, Document, "XPCOMText" );
	_Register( Kernel.Shared, Document, "Shared" );
	_Register( Kernel.Label, Document, "XPCOMLabel" );
RR
RN
RE
}

NS_IMETHODIMP egeckocom::RegisteringEnd( void )
{
RP
RBB
	Global.DismissCurrentKernel();
RR
RN
RE
}




/* Gecko required part. */

#include "nsIGenericFactory.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(egeckocom)

static nsModuleComponentInfo components[] =
{
    {
       EGECKOCOM_CLASSNAME, 
       EGECKOCOM_CID,
       EGECKOCOM_CONTRACTID,
       egeckocomConstructor,
    },
	NSXPCM_COMPONENTS,
};

NS_IMPL_NSGETMODULE("EpeiosModule", components) 
