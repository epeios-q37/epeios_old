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
	_KernelRow =  NONE;
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
	nsIDOMWindow *Window,
	const id_type &Id )
{
	Core.Init( Global.GetCurrentKernelRow() );

	nsxpcm::Register( Core, Window, Id );
}


NS_IMETHODIMP egeckocom::Register( nsIDOMWindow *Window )
{
	// Ne sait pas récupèrer une 'window' à partir de son document.
RP
RBB
	ui__ &UI = Global.GetCurrentKernel().UI;

	UI.Window = Window;
	

#ifdef XXX_DBG
	if ( _KernelRow != NONE )
		ERRu();
#endif
	_KernelRow = Global.GetCurrentKernelRow();

	Window->GetDocument( &UI.Document );

	_Register( UI.Error, Window, str::string( "error" ) );	// Version 'const str::string_', pour test.
	_Register( UI.Input, Window, "input" );
	_Register( UI.Shared, Window, "shared" );
	_Register( UI.Output, Window, "output" );
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

NS_IMETHODIMP egeckocom::Unregister( void )
{
RP
RBB
	Global.Delete( _KernelRow );
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
