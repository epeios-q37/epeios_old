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

	nsxpcm::Register( Core, UI.Document, Id );
}

static void _RegisterSpecific( ui__::main &UI )
{
	_Register( UI.Input, UI, "input" );
	_Register( UI.Shared, UI, "shared" );
	_Register( UI.Output, UI, "output" );
	_Register( UI.JSConsole, UI, "jsconsole" );
	_Register( UI.Endianess, UI, "endianess" );
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
	// Ne sait pas r�cup�rer une 'window' � partir de son document.
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
	Result.Append( "\n" );
	_DisplayEndianess( 0xfedcba9876543210ULL, Result );

	UI.Main.Endianess.SetValue( Result );
ERRErr
ERREnd
ERREpilog
}


#include "nsIDOMNodeList.h"
#include "nsIDOMWindow.h"
#include "toolkitcomps/nsICommandLineRunner.h"

void ui_error_button__::NSXPCMOnClick( void )
{
/*
	nsCOMPtr<nsICommandLine> CommandLine = NULL;
	nsresult Result = NS_OK;
	PRInt32 Length = 32;

	nsCID CID = NS_ICOMMANDLINE_IID;

//	CommandLine = do_GetService( CID, &Result );

//	nsxpcm::GetService<nsICommandLine>( "@mozilla.org/toolkit/command-line;1", CommandLine );

//	nsxpcm::CreateInstance<nsICommandLineRunner>( "@mozilla.org/toolkit/command-line;1", CommandLine );

	CommandLine = nsxpcm::QueryInterface<nsICommandLine>( this->Kernel().UI.Page.Window );


	CommandLine->GetLength( &Length );

//	nsxpcm::QueryInterface<nsIDOMWindowInternal>( UI().Window )->Alert( NS_LITERAL_STRING( "Yesss !" ) );
*/
	ERRu();
}

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
