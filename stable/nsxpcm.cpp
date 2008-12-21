/*
	'nsxpcm' library by Claude SIMON (csimon@epeios.org)
	Requires the 'nsxpcm' header file ('nsxpcm.h').
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

	This file is part of the Epeios (http://epeios.org/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id$

#define NSXPCM__COMPILATION

#include "nsxpcm.h"

class nsxpcmtutor
: public ttr_tutor
{
public:
	nsxpcmtutor( void )
	: ttr_tutor( NSXPCM_NAME )
	{
#ifdef NSXPCM_DBG
		Version = NSXPCM_VERSION "\b\bD $";
#else
		Version = NSXPCM_VERSION;
#endif
		Owner = NSXPCM_OWNER;
		Date = "$Date$";
	}
	virtual ~nsxpcmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "nsMemory.h"
#include "dom/nsIDOMEvent.h"
#include "dom/nsIDOMEventTarget.h"
#include "widget/nsIFilePicker.h"
#include "nsILocalFile.h"
#include "nsIConsoleService.h"
#include "content/nsIXSLTProcessor.h"
#include "nsIVariant.h"
#include "nsIDOMParser.h"
#include "nsIXMLHttpRequest.h"

using namespace nsxpcm;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */


void nsxpcm::Transform(
	const char *CString,
	epeios::size__ Size,
	char **JString )
{
    if ( !JString )
        ERRu();
	else
	    *JString = (char*) nsMemory::Clone( CString, sizeof(char) * Size );
    
	if ( !JString )
		ERRa();
}

void nsxpcm::Transform(
	const char *CString,
	char **JString )
{
	Transform( CString, strlen( CString ) + 1, JString );
}

void nsxpcm::Transform(
	const nsEmbedCString &ECString,
	nsEmbedString &EString )
{
	NS_CStringToUTF16(ECString, NS_CSTRING_ENCODING_ASCII, EString );
}

void nsxpcm::Transform(
	const nsEmbedString &EString,
	nsEmbedCString &ECString )
{
	NS_UTF16ToCString( EString, NS_CSTRING_ENCODING_ASCII, ECString );
}

void nsxpcm::Transform(
	const nsEmbedString &EString,
	char **JString )
{
ERRProlog
	str::string String;
ERRBegin
	String.Init();

	Transform( EString, String );
	Transform( String, JString );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Transform(
	const str::string_ &EString,
	char **JString )
{
ERRProlog
	tol::E_FPOINTER___( char ) Buffer;
ERRBegin
	Buffer = EString.Convert();

	Transform( Buffer, EString.Amount() + 1, JString );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Transform(
	const char *String,
	nsEmbedString &EString )
{
	Transform( nsEmbedCString( String ), EString );
}

void nsxpcm::Transform(
	const str::string_ &String,
	nsEmbedString &EString )
{
ERRProlog
	tol::E_FPOINTER___( char ) Buffer;
ERRBegin
	Buffer = String.Convert();

	Transform( Buffer, EString );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Transform(
	const nsEmbedString &EString,
	str::string_ &String )
{
	nsEmbedCString ECString;

	Transform( EString, ECString );

	String.Append( ECString.get() );
}

void nsxpcm::Transform(
	const nsEmbedCString &ECString,
	str::string_ &String )
{
	nsEmbedString EString;

	Transform( ECString, EString );

	Transform( EString, String );
}

void nsxpcm::Transform(
	const str::string_ &String,
	nsEmbedCString &ECString )
{
	nsEmbedString EString;

	Transform( String, EString );

	Transform( EString, ECString );
}



void nsxpcm::Split( 
	const string_ &Joined,
	bso::char__ Separator,
	strings_ &Splitted )
{
ERRProlog
	epeios::row__ Row = NONE;
	string Item;
ERRBegin
	if ( Joined.Amount() != 0 ) {	

		Item.Init();

		Row = Joined.First();

		while ( Row != NONE ) {
			if ( Joined( Row ) != Separator )
				Item.Append( Joined( Row ) );
			else {
				Splitted.Append( Item );
				Item.Init();
			}

			Row = Joined.Next( Row );
		}

		Splitted.Append( Item );
	}
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Split(
	const char *Joined,
	bso::char__ Separator,
	strings_ &Splitted )
{
ERRProlog
	string SJoined;
ERRBegin
	SJoined.Init( Joined );

	Split( SJoined, Separator, Splitted );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Join(
	const strings_ &Splitted,
	bso::char__ Separator,
	string_ &Joined )
{
	epeios::row__ Row = Splitted.First();
	ctn::E_CMITEM( string_ ) Item;

	Item.Init( Splitted );

	if ( Row != NONE ) {
		Joined.Append( Item( Row ) );
		Row = Splitted.Next( Row );
	}

	while ( Row != NONE ) {
		Joined.Append( Separator );
		Joined.Append( Item( Row ) );
		Row = Splitted.Next( Row );
	}
}

void nsxpcm::JoinAndTransform(
	const strings_ &Splitted,
	bso::char__ Separator,
	char **JString )
{
ERRProlog
	string Joined;
ERRBegin
	Joined.Init();
	
	Join( Splitted, Separator, Joined );

	Transform( Joined, JString );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::RemoveListboxContent( listbox_ *Listbox )
{
ERRProlog
	str::string Name;
	nsIDOMNode *Node = NULL, *Next = NULL;
ERRBegin
	Node = GetFirstChild( Listbox );

	while ( Node != NULL ) {
		Next = GetNextSibling( Node );

		Name.Init();

		if ( GetNodeName( Node, Name ) == "listitem" )
			RemoveChild( Listbox, Node );

		Node = Next;
	}

ERRErr
ERREnd
ERREpilog
}

nsIDOMNode *nsxpcm::_FindParent(
	nsIDOMNode *Node,
	const str::string_ &NodeName )
{
ERRProlog
	str::string Name;
ERRBegin
	while ( Node != NULL ) {
		Name.Init();

		GetNodeName( Node, Name );

		if ( NodeName == Name )
			break;

		Node = GetParentNode( Node );
	}

ERRErr
ERREnd
ERREpilog
	return Node;
}

void nsxpcm::Handle( 
	nsIDOMEvent *Event,
	const element_cores_ &Cores )
{
ERRProlog
	nsIDOMEventTarget *EventTarget = NULL;
	nsIDOMElement *Element = NULL;
	nsEmbedString String;
	str::string S;
	epeios::row__ Row = NONE;
ERRBegin
	Event->GetTarget( &EventTarget );
	Element = nsxpcm::QueryInterface< nsIDOMElement>( EventTarget, err::hSkip  );

	if ( Element == NULL )
		ERRReturn;

	Row = Cores.First();

	while ( ( Row != NONE ) && ( Cores( Row )->GetElement() != Element ) )
		Row = Cores.Next( Row );

	if ( Row == NONE )
		ERRu();

	Event->GetType( String );

	S.Init();

	nsxpcm::Transform( String, S );

	Cores( Row )->Handle( S );
ERRErr
ERREnd
ERREpilog
}

static bso::bool__ FileDialogBox_(
	nsIDOMWindow *Parent,
	const char *Title,
	const char *DefaultExtension,
	PRInt16 Mode,
	PRInt32 FilterMask,
	str::string_ &Name )
{
	bso::bool__ Success = false;
ERRProlog
	nsCOMPtr<nsIFilePicker> FilePicker = NULL;
	nsresult Error = NS_OK;
	nsEmbedString EString;
ERRBegin
	CreateInstance( "@mozilla.org/filepicker;1", FilePicker );

	Transform( Title, EString );

	if ( ( Error = FilePicker->Init( Parent, EString, Mode ) ) != NS_OK )
		ERRx();

	if ( ( Error = FilePicker->AppendFilters( FilterMask ) ) != NS_OK )
		ERRx();

	if ( *DefaultExtension != 0 ) {
		Transform( DefaultExtension, EString );
		FilePicker->SetDefaultExtension( EString );
	}

	PRInt16 _retval = 0;

	if ( ( Error = FilePicker->Show( &_retval ) ) != NS_OK )
		ERRx();

	if ( _retval == nsIFilePicker::returnCancel ) {
		Success = false;
		ERRReturn;
	} else if ( ( _retval != nsIFilePicker::returnOK ) && ( _retval != nsIFilePicker::returnReplace ) )
		ERRx();

	nsILocalFile *File;

	if ( ( Error = FilePicker->GetFile( &File ) ) != NS_OK )
		ERRx();

	if ( ( Error = File->GetPath( EString ) ) != NS_OK )
		ERRx();

	nsxpcm::Transform( EString, Name );

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

bso::bool__ nsxpcm::FileOpenDialogBox(
	nsIDOMWindow *Parent,
	const char *Title,
	str::string_ &FileName )
{
	return FileDialogBox_( Parent, Title, "", nsIFilePicker::modeOpen, nsIFilePicker::filterAll, FileName );
}

bso::bool__ nsxpcm::HTMLFileOpenDialogBox(
	nsIDOMWindow *Parent,
	const char *Title,
	str::string_ &FileName )
{
	return FileDialogBox_( Parent, Title, "html", nsIFilePicker::modeOpen, nsIFilePicker::filterHTML, FileName );
}

bso::bool__ nsxpcm::FileSaveDialogBox(
	nsIDOMWindow *Parent,
	const char *Title,
	str::string_ &FileName )
{
	return FileDialogBox_( Parent, Title, "", nsIFilePicker::modeSave, nsIFilePicker::filterAll, FileName );
}

bso::bool__ nsxpcm::HTMLFileSaveDialogBox(
	nsIDOMWindow *Parent,
	const char *Title,
	str::string_ &FileName )
{
	return FileDialogBox_( Parent, Title, "html", nsIFilePicker::modeSave, nsIFilePicker::filterHTML, FileName );
}

bso::bool__ nsxpcm::DirectorySelectDialogBox(
	nsIDOMWindow *Parent,
	const char *Title,
	str::string_ &FileName )
{
	return FileDialogBox_( Parent, Title, "", nsIFilePicker::modeGetFolder, nsIFilePicker::filterAll, FileName );
}

void nsxpcm::Delete( element_cores_ &Cores )
{
	epeios::row__ Row = Cores.First();

	while ( Row != NONE ) {
		delete Cores( Row );

		Cores.Set( NULL, Row );

		Row = Cores.Next( Row );
	}

	Cores.Init();
}


#ifdef NSXPCM__BKD

void nsxpcm::Convert(
	const strings_ &Items,
	bkdacc::ids32_ &Ids )
{
	epeios::row__ Row = Items.First();
	epeios::row__ Error = NONE;
	ctn::E_CMITEM( string_ ) Item;

	Item.Init( Items );

	while( Row != NONE ) {
		Ids.Append( Item( Row ).ToUL( &Error ) );

		Row = Items.Next( Row );
	}

	if ( Error != NONE )
		ERRu();
}

void nsxpcm::Convert(
	const strings_ &Items,
	bkdacc::ids16_ &Ids )
{
	epeios::row__ Row = Items.First();
	epeios::row__ Error = NONE;
	ctn::E_CMITEM( string_ ) Item;

	Item.Init( Items );

	while( Row != NONE ) {
		Ids.Append( Item( Row ).ToUS( &Error ) );

		Row = Items.Next( Row );
	}

	if ( Error != NONE )
		ERRu();
}

void nsxpcm::Convert(
	const strings_ &Items,
	bkdacc::ids8_ &Ids )
{
	epeios::row__ Row = Items.First();
	epeios::row__ Error = NONE;
	ctn::E_CMITEM( string_ ) Item;

	Item.Init( Items );

	while( Row != NONE ) {
		Ids.Append( Item( Row ).ToUB( &Error ) );

		Row = Items.Next( Row );
	}

	if ( Error != NONE )
		ERRu();
}

void nsxpcm::SplitAndConvert(
	const char *Joined,
	bso::char__ Separator,
	bkdacc::ids32_ &Ids )
{
ERRProlog
	strings Splitted;
ERRBegin
	Splitted.Init();

	Split( Joined, Separator, Splitted );

	Convert( Splitted, Ids );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::SplitAndConvert(
	const char *Joined,
	bso::char__ Separator,
	bkdacc::ids16_ &Ids )
{
ERRProlog
	strings Splitted;
ERRBegin
	Splitted.Init();

	Split( Joined, Separator, Splitted );

	Convert( Splitted, Ids );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::SplitAndConvert(
	const char *Joined,
	bso::char__ Separator,
	bkdacc::ids8_ &Ids )
{
ERRProlog
	strings Splitted;
ERRBegin
	Splitted.Init();

	Split( Joined, Separator, Splitted );

	Convert( Splitted, Ids );
ERRErr
ERREnd
ERREpilog
}

template< typename t> inline static const char *Convert_(
	t Value,
	bso::integer_buffer__ &Buffer )
{
	if ( Value == NONE )
		return "";
	else
		return bso::Convert( Value, Buffer );
}

template<typename id__, typename id_t__> static inline void ConvertAndJoin_(
	const bch::E_BUNCH_( id__ ) &RawIds,
	bso::char__ Separator,
	string_ &Joined )
{
	const bch::E_BUNCH_( id_t__ ) &Ids = *(const bch::E_BUNCH_( id_t__ ) *)&RawIds;
	epeios::row__ Row = Ids.First();
	bso::integer_buffer__ Buffer;

	if ( Row != NONE ) {
		Joined.Append( Convert_( Ids( Row ), Buffer ) );
		Row = Ids.Next( Row );
	}

	while ( Row != NONE ) {
		Joined.Append( Separator );
		Joined.Append( Convert_( Ids( Row ), Buffer ) );
		Row = Ids.Next( Row );
	}
}

template<typename id__, typename id_t__> static inline void ConvertJoinAndTransform_(
	const bch::E_BUNCH_( id__ ) &Ids,
	bso::char__ Separator,
	char **JString )
{
ERRProlog
	string Joined;
ERRBegin
	Joined.Init();

	ConvertAndJoin_<id__, id_t__>( Ids, Separator, Joined );

	Transform( Joined, JString );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::ConvertAndJoin(
	const bkdacc::ids8_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<bkdacc::id8__, bkdacc::id8_t__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const bkdacc::ids8_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<bkdacc::id8__, bkdacc::id8_t__>( Ids, Separator, JString );
}

void nsxpcm::ConvertAndJoin(
	const bkdacc::ids16_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<bkdacc::id16__, bkdacc::id16_t__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const bkdacc::ids16_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<bkdacc::id16__, bkdacc::id16_t__>( Ids, Separator, JString );
}

void nsxpcm::ConvertAndJoin(
	const bkdacc::ids32_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<bkdacc::id32__, bkdacc::id32_t__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const bkdacc::ids32_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<bkdacc::id32__, bkdacc::id32_t__>( Ids, Separator, JString );
}

void nsxpcm::ConvertAndJoin(
	const bkdacc::slongs_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<bkdacc::slong__, bkdacc::slong__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const bkdacc::slongs_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<bkdacc::slong__, bkdacc::slong__>( Ids, Separator, JString );
}

void nsxpcm::ConvertAndJoin(
	const bkdacc::ulongs_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<bkdacc::ulong__, bkdacc::ulong__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const bkdacc::ulongs_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<bkdacc::ulong__, bkdacc::ulong__>( Ids, Separator, JString );
}


void nsxpcm::ConvertAndJoin(
	const bkdacc::booleans_ &Booleans,
	bso::char__ Separator,
	string_ &Joined )
{
	epeios::row__ Row = Booleans.First();

	if ( Row != NONE ) {
		Joined.Append( Booleans( Row ) ? "1" : "0" );
		Row = Booleans.Next( Row );
	}

	while ( Row != NONE ) {
		Joined.Append( Separator );
		Joined.Append( Booleans( Row ) ? "1" : "0" );
		Row = Booleans.Next( Row );
	}
}

void nsxpcm::ConvertJoinAndTransform(
	const bkdacc::booleans_ &Booleans,
	bso::char__ Separator,
	char **JString )
{
ERRProlog
	string Joined;
ERRBegin
	Joined.Init();

	ConvertAndJoin( Booleans, Separator, Joined );

	Transform( Joined, JString );
ERRErr
ERREnd
ERREpilog
}

void GetData_(
	bso::ulong__ Amount,
	const strings_ &Splitted,
	strings_ &Data,
	epeios::row__ &Row )
{
	ctn::E_CMITEM( str::string_ ) Item;

	Item.Init( Splitted );

	while ( Amount-- ) {
#ifdef NSXPCM_DBG
		if ( Row == NONE )
			ERRu();
#endif
		Data.Append( Item( Row ) );

		Row = Splitted.Next( Row );
	}
}

void nsxpcm::Split(
	const char *Joined,
	const char *JAmounts,
	bso::char__ Separator,
	bkdacc::xstrings_ &DataCluster )
{
ERRProlog
	nsxpcm::strings Amounts;
	nsxpcm::strings Splitted;
	epeios::row__ Error = NONE;
	epeios::row__ ARow = NONE, SRow = NONE;
	strings Data;
ERRBegin
	Splitted.Init();
	Split( Joined, Separator, Splitted );

	Amounts.Init();
	Split( JAmounts, Separator, Amounts );

	ARow = Amounts.First();

	SRow = Splitted.First();

	while ( ARow != NONE )
	{
		Data.Init();

		GetData_( Amounts( ARow ).ToUL( &Error ), Splitted, Data, SRow );

		ARow = Amounts.Next( ARow );
	}
ERRErr
ERREnd
ERREpilog
}


#endif

void nsxpcm::Alert(
	nsIDOMWindow *Window,
	const char *Text )
{
ERRProlog
	nsEmbedString NSText;
ERRBegin
	nsxpcm::Transform( Text, NSText );

	GetWindowInternal( Window )->Alert( NSText );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Alert(
	nsIDOMWindow *Window,
	const str::string_ &Text )
{
ERRProlog
	nsEmbedString NSText;
ERRBegin
	nsxpcm::Transform( Text, NSText );

	GetWindowInternal( Window )->Alert( NSText );
ERRErr
ERREnd
ERREpilog
}


void nsxpcm::Log( const char *Text )
{
	nsEmbedString String;
	nsCOMPtr<nsIConsoleService> ConsoleService = NULL;

	nsxpcm::GetService<nsIConsoleService>( NS_CONSOLESERVICE_CONTRACTID, ConsoleService );

	nsxpcm::Transform( Text, String );

	ConsoleService->LogStringMessage( String.get() );
}

void nsxpcm::Log( const str::string_ &Text )
{
ERRProlog
	tol::E_FPOINTER___( char ) Buffer;
ERRBegin
	Buffer = Text.Convert();

	Log( Buffer );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::element_core__::Init( nsIDOMElement *Element )
{
#ifdef NSXPCM_DBG
	if ( _Element != NULL )
		ERRu();
#endif
	reset();

	_Element = Element;

	nsIDOMEventTarget *EventTarget = NULL;

	EventTarget = nsxpcm::QueryInterface<nsIDOMEventTarget>( Element );

	nsxpcm::CreateInstance( NSXPCM_EVENT_LISTENER_CONTRACTID, _EventListener );

	if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "command" ), _EventListener, true ) != NS_OK )
		ERRc();

	if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "input" ), _EventListener, true ) != NS_OK )
		ERRc();

	if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "click" ), _EventListener, true ) != NS_OK )
		ERRc();

	if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "focus" ), _EventListener, true ) != NS_OK )
		ERRc();

	if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "blur" ), _EventListener, true ) != NS_OK )
		ERRc();

	_EventListener->Init( *this );

}


NS_IMPL_ISUPPORTS1(nsxpcm::event_listener, nsxpcm::ievent_listener)

NS_IMETHODIMP nsxpcm::event_listener::HandleEvent(nsIDOMEvent *Event)
{
	nsresult NSResult = NS_OK;
ERRProlog
	nsEmbedString String;
	str::string S;
	err::buffer__ Buffer;
ERRBegin
	Event->GetType( String );

	S.Init();

	nsxpcm::Transform( String, S );

	_Core->Handle( S );
ERRErr
	NSResult = NS_ERROR_FAILURE;

	if ( ( ERRMajor != err::itn ) || ( ERRMinor != err::iBeam ) )
		Log( err::Message( Buffer ) );	

	ERRRst();
ERREnd
ERREpilog
    return NSResult;
}

void nsxpcm::xslt_parameters_::Append(
	const str::string_ &Name,
	const str::string_ &Value )
{
ERRProlog
	xslt_parameter Parameter;
ERRBegin
	Parameter.Init();

	Parameter.Name = Name;
	Parameter.Value = Value;

	_xslt_parameters_::Append( Parameter );
ERRErr
ERREnd
ERREpilog
}


nsIDOMDocumentFragment *nsxpcm::XSLTTransform(
	nsIDOMDocument *XMLDocument,
	nsIDOMDocument *XSLStylesheet,
	nsIDOMDocument *Owner,
	const xslt_parameters_ &Parameters )
{
	nsIDOMDocumentFragment *Fragment = NULL;
ERRProlog
	nsCOMPtr<nsIXSLTProcessor> Processor;
	nsEmbedString Name;
	nsCOMPtr<nsIWritableVariant> Value;
	tol::E_FPOINTER___( char ) NameBuffer, ValueBuffer;
	ctn::E_CITEM( xslt_parameter_ ) Parameter;
	nsresult Result = NS_OK;
	epeios::row__ Row = NONE;
ERRBegin
	nsxpcm::CreateInstance( "@mozilla.org/document-transformer;1?type=xslt", Processor );

	Result = Processor->ImportStylesheet( XSLStylesheet );

	if ( Result != NS_OK )
		ERRu();

	Parameter.Init( Parameters );

	Row = Parameters.First();

	while ( Row != NONE ) {
		Transform( Parameter( Row ).Name, Name );

		nsxpcm::CreateInstance( NS_VARIANT_CONTRACTID, Value );
		Value->SetAsString( NameBuffer = Parameter( Row ).Value.Convert() );

		Result = Processor->SetParameter( NS_LITERAL_STRING( "" ), Name, Value );

		if ( Result != NS_OK )
			ERRu();

		Row = Parameters.Next( Row );
	}


	Result = Processor->TransformToFragment( XMLDocument, Owner, &Fragment );

	if ( Result != NS_OK )
		ERRu();
ERRErr
	Fragment = NULL;
ERREnd
ERREpilog
	return Fragment;
}

static void _GetXMLDocument(
	const str::string_ &XMLString,
	nsIDOMDocument *&XMLDocument )
{
	nsEmbedString XMLEmbedString;
	nsCOMPtr<nsIDOMParser> Parser;

	nsxpcm::Transform( XMLString, XMLEmbedString );
	
	CreateInstance( NS_DOMPARSER_CONTRACTID, Parser );
	Parser->ParseFromString( XMLEmbedString.get(), "text/xml", &XMLDocument );
}

static void _GetXSLStylesheet(
	const str::string_ &XSLStylesheetFileName,
	nsIDOMDocument *&XSLStylesheet )
{
	nsresult Result = NS_OK;
	nsEmbedCString Method, URL;
	nsEmbedString Empty;
	nsCOMPtr<nsIXMLHttpRequest> HTTPRequest;

	Transform( str::string( "GET" ), Method );
	Transform( XSLStylesheetFileName, URL );

	CreateInstance( NS_XMLHTTPREQUEST_CONTRACTID, HTTPRequest );

	Result = HTTPRequest->OpenRequest( Method, URL, false, Empty, Empty );

	if ( Result != NS_OK )
		ERRu();

	Result = HTTPRequest->Send( NULL );

	if ( Result != NS_OK )
		ERRu();

	Result = HTTPRequest->GetResponseXML( &XSLStylesheet );

	if ( Result != NS_OK )
		ERRu();
}

nsIDOMDocumentFragment *nsxpcm::XSLTTransform(
	const str::string_ &XMLString,
	const str::string_ &XSLStylesheetFileName,
	nsIDOMDocument *Owner,
	const xslt_parameters_ &Parameters )
{
	nsIDOMDocumentFragment *Fragment = NULL;
ERRProlog
	nsIDOMDocument *XMLDocument = NULL, *XSLStylesheet = NULL;
ERRBegin
	_GetXMLDocument( XMLString, XMLDocument );

	_GetXSLStylesheet( XSLStylesheetFileName, XSLStylesheet );

	Fragment = XSLTTransform( XMLDocument, XSLStylesheet, Owner, Parameters );
ERRErr
	Fragment = NULL;
ERREnd
ERREpilog
	return Fragment;
}

class nsxpcmpersonnalization
: public nsxpcmtutor
{
public:
	nsxpcmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~nsxpcmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static nsxpcmpersonnalization Tutor;

ttr_tutor &NSXPCMTutor = Tutor;
