/* $Id$ */

#include "egeckocom.h"
#include "nsxpcm.h"
#include "nsIDOMEventListener.h"
#include "nsIDOMEventTarget.h"

global<kernel___, krow__> Global;
static nsIDOMWindowInternal *_JSConsole = NULL;

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
	str::string Version;
tol::buffer__ Buffer;
RBB
	Version.Init( VERSION );

	Version.Append( " (" );
	Version.Append( tol::DateAndTime( Buffer ) );
	Version.Append( ')' );

	nsxpcm::Log( Version );


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

	nsxpcm::Register( Core, UI.Window, Id, nsxpcm::ef_All & ~nsxpcm::efFocus & ~nsxpcm::efBlur );
}

static void _RegisterSpecific( ui__::main &UI )
{
	_Register( UI.Input, UI, "input" );
	_Register( UI.Shared, UI, "shared" );
	_Register( UI.Output, UI, "output" );
	_Register( UI.JSConsole, UI, "jsconsole" );
	_Register( UI.XSLT, UI, "xslt" );
	_Register( UI.Endianess, UI, "endianess" );
	_Register( UI.LinkCommand, UI, "cmdXPCOM" );
}

static void _RegisterSpecific( ui__::page &UI )
{
	_Register( UI.Error, UI, str::string( "error" ) );	// Version 'const str::string_', pour test.
	_Register( UI.Link, UI, "link" );
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
		Global.GetCurrentObject().DisplayEndianess();
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

void ui_input_textbox__::NSXPCMOnInput( void )
{
	if ( UI().Main.Shared.IsChecked() )
		Kernel().InputToAllOutputs();
	else
		Kernel().InputToOutput();
}

void ui_link__::NSXPCMOnCommand( void )
{
	nsxpcm::GetWindowInternal( UI().Page.Window )->Alert( NS_LITERAL_STRING ( "Command" ) );
}
void ui_link__::NSXPCMOnClick( void )
{
	nsxpcm::GetWindowInternal( UI().Page.Window )->Alert( NS_LITERAL_STRING ("The BIG click" ) );
}


void kernel___::InputToAllOutputs( void )
{
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();
	UI.Main.Input.GetValue( Value );

	krow__ Row = Global.First();

	while ( Row != NONE ) {

		Global.Get( Row )->UI.Main.Output.SetValue( Value );

		Row = Global.Next( Row );
	}

ERRErr
ERREnd
ERREpilog
}

static void _ConvertNibble(
	bso::ubyte__ Byte,
	str::string_ &String )
{
	if ( Byte < 10 )
		String.Append( '0' + Byte );
	else if ( Byte < 16 )
		String.Append( 'A' - 10 + Byte );
	else
		ERRu();
}

static void _ConvertByte(
	bso::ubyte__ Byte,
	str::string_ &String )
{
	_ConvertNibble( Byte >> 4, String );
	_ConvertNibble( Byte & 0xF, String );
}

static bso::ubyte__ _Extract(
	bso::ullong__ Long,
	bso::ubyte__ Position )
{
	return ( Long >> ( Position * 8 ) ) & 0xFF;
}

static void _Convert(
	bso::ullong__ Long ,
	str::string_ &String )
{
	int i = 8;

	while ( i-- )
		_ConvertByte( _Extract( Long, i ), String );
}

nsIDOMWindowInternal *_GetWindowInternal( nsIDOMWindow *Window )
{
	return (nsIDOMWindowInternal *)Window;
}

void kernel___::OpenJSConsole( void )
{
	nsxpcm::GetJSConsole( UI.Main.Window, &_JSConsole );
}

static inline void _DisplayEndianess(
	 bso::ullong__ Value,
	 str::string_ &Result )
{
ERRProlog
	const bso::ubyte__ *Byte = (bso::ubyte__ *)&Value;
	int i = 8;
ERRBegin
	_Convert( Value, Result );

	Result.Append( " " );

	while ( i-- ) {
		Result.Append( " " );
		_ConvertByte( Byte[i], Result );
	}
ERRErr
ERREnd
ERREpilog
}

void kernel___::DisplayEndianess( void )
{
ERRProlog
	str::string Result;
ERRBegin
	Result.Init();

	_DisplayEndianess( 0x0123456789abcdefULL, Result );
	Result.Append( " - " );
	_DisplayEndianess( 0xfedcba9876543210ULL, Result );

	UI.Main.Endianess.SetValue( Result );
ERRErr
ERREnd
ERREpilog
}

void ui_error_button__::NSXPCMOnCommand( void )
{
ERRProlog
ERRBegin
	ERRu();
ERRErr
ERREnd
ERREpilog
}

void ui_xslt_button__::NSXPCMOnCommand( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	nsIDOMDocumentFragment *Fragment = NULL;
	str::string Value;
ERRBegin
	Value.Init();
	UI().Main.Input.GetValue( Value );

	Parameters.Init();
	Parameters.Append( "Value", Value );

	Fragment = nsxpcm::XSLTransformByFileName( str::string( "<Root/>" ), str::string( "chrome://epeios/content/epeios.xsl" ), UI().Main.Document, Parameters );
//	Fragment = nsxpcm::XSLTTransform( str::string( "<Root/>" ), str::string( "file://H:/cvs/epeios/examples/Gecko/XUL/chrome/epeios/epeios.xsl" ), UI().Main.Document, Parameters );

	nsxpcm::RemoveChildren( nsxpcm::GetElementById( UI().Main.Document, "xsltTarget" ) );

	nsxpcm::AppendChild( nsxpcm::GetElementById( UI().Main.Document, "xsltTarget" ), Fragment );

//	ERRu();
ERRErr
ERREnd
ERREpilog
}

void ui_command__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
	nsxpcm::Alert( this->Kernel().UI.Main.Window, "cmdXPCOM !!!" );

//	GetObject();
}


#if 0
template <typename element> inline element *_QueryInterface(
	nsISupports *GenericElement,
	err::handle ErrHandle = err::hUsual )
{
	element *Element = NULL;
	nsresult Result = NS_OK;
	bso::integer_buffer__ Buffer;

#ifdef NSXPCM_DBG

	if ( GenericElement == NULL )
		ERRu();
#endif
	Result = GenericElement->QueryInterface( element::GetIID(), (void **)&Element );

	nsxpcm::Log( bso::Convert( (bso::ulong__)Result, Buffer ) );

	if ( Element == NULL )
		nsxpcm::Log( "Failure" );
	else
		nsxpcm::Log( "Success" );
/*
	if ( (Result != NS_OK ) && ( ErrHandle != err::hSkip ) )
		ERRu();
*/
	return Element;
}
#endif

void ui_jsconsole_button__::NSXPCMOnCommand( void )
{
/*
	nsxpcm::Log( "Window2" );

	nsIDOMWindow2 *Window2 = NULL;

	if ( ( Window2 = _QueryInterface<nsIDOMWindow2>( UI().Main.Window ) ) == NULL )
		ERRu();

	nsxpcm::Log( "WindowInternal" );

	if ( _QueryInterface<nsIDOMWindowInternal>( Window2 ) == NULL )
		ERRu();

	nsxpcm::Log( "OK" );
*/

	Kernel().OpenJSConsole();
}


/* Gecko required part. */
#if 0

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
#else
NS_GENERIC_FACTORY_CONSTRUCTOR(egeckocom)

// The following line defines a kNS_SAMPLE_CID CID variable.
NS_DEFINE_NAMED_CID(EGECKOCOM_CID);

// Build a table of ClassIDs (CIDs) which are implemented by this module. CIDs
// should be completely unique UUIDs.
// each entry has the form { CID, service, factoryproc, constructorproc }
// where factoryproc is usually NULL.
static const mozilla::Module::CIDEntry kEGeckoComCIDs[] = {
    { &kEGECKOCOM_CID, false, NULL, egeckocomConstructor },
    { NULL }
};

// Build a table which maps contract IDs to CIDs.
// A contract is a string which identifies a particular set of functionality. In some
// cases an extension component may override the contract ID of a builtin gecko component
// to modify or extend functionality.
static const mozilla::Module::ContractIDEntry kEGeckoComContracts[] = {
    { EGECKOCOM_CONTRACTID, &kEGECKOCOM_CID },
    { NULL }
};

// Category entries are category/key/value triples which can be used
// to register contract ID as content handlers or to observe certain
// notifications. Most modules do not need to register any category
// entries: this is just a sample of how you'd do it.
// @see nsICategoryManager for information on retrieving category data.
static const mozilla::Module::CategoryEntry kGesbibCategories[] = {
    { NULL }
};

static const mozilla::Module EGeckoComModule = {
    mozilla::Module::kVersion,
    kEGeckoComCIDs,
    kEGeckoComContracts,
    kGesbibCategories
};

// The following line implements the one-and-only "NSModule" symbol exported from this
// shared library.
NSMODULE_DEFN(EGeckoComModule) = &EGeckoComModule;
//NSMODULE_DEFN(EventListenerModule) = &EventListenerModule;

#endif