/* $Id$ */

#include "egeckocom.h"
#include "nsxpcm.h"
#include "nsIDOMEventListener.h"
#include "nsIDOMEventTarget.h"

global<kernel___, krow__> Global;

struct autostart {
	autostart( void )
	{
		Global.Init();
	}
} AutoStart;

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
	Global.CreateNewObject();
RR
RN
RE
}

template <typename element, typename ui, typename id > static void _Register(
	element &Core,
	ui &UI,
	const id &Id )
{
	Core.Init( Global.GetCurrentRow() );

	nsxpcm::Register( Core, UI.Document, Id );
}

static void _RegisterSpecific( ui__::main &UI )
{
	_Register( UI.Input, UI, "input" );
	_Register( UI.Shared, UI, "shared" );
	_Register( UI.Output, UI, "output" );
}

static void _RegisterSpecific( ui__::page &UI )
{
	_Register( UI.Error, UI, str::string( "error" ) );	// Version 'const str::string_', pour test.
}

template <typename ui> static void _RegisterCommon(
	nsIDOMWindow *Window,
	ui &UI )
{
	UI.Window = Window;

	Window->GetDocument( &UI.Document );
}


template <typename ui> static void _Register(
	nsIDOMWindow *Window,
	ui &UI )
{
	_RegisterCommon( Window, UI );
	_RegisterSpecific( UI );
}

NS_IMETHODIMP egeckocom::Register(
	nsIDOMWindow *Window,
	const char *UIDesignation )
{
	// Ne sait pas récupèrer une 'window' à partir de son document.
RP
RBB
	ui__ &UI = Global.GetCurrentObject().UI;

#ifdef XXX_DBG
	if ( _KernelRow != NONE )
		ERRu();
#endif
	_KernelRow = Global.GetCurrentRow();

	if ( !strcmp( UIDesignation, "main" ) ) {
		_Register( Window, UI.Main );
	} else if ( !strcmp( UIDesignation, "page" ) ) {
		_Register( Window, UI.Page );
	} else
		ERRu();

RR
RN
RE
}

NS_IMETHODIMP egeckocom::RegisteringEnd( void )
{
RP
RBB
	Global.DismissCurrentObject();
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
