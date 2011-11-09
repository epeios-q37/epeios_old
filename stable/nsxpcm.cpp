/*
	'nsxpcm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'nsxpcm' header file ('nsxpcm.h').
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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
#include "nsIDOMEvent.h"
#include "nsIDOMEventTarget.h"
#include "nsIFilePicker.h"
#include "nsILocalFile.h"
#include "nsIConsoleService.h"
#include "nsIXSLTProcessor.h"
#include "nsIVariant.h"
#include "nsIDOMParser.h"
#include "nsIXMLHttpRequest.h"
#include "nsIIOService.h"
#include "nsIExternalProtocolService.h"
#include "nsIDirectoryService.h"
#include "nsIFormHistory.h"
#include "nsIDOMNodeList.h"

#include "xpp.h"
#include "txf.h"
#include "mtk.h"

#define T( f )\
	if ( ( f ) != NS_OK )\
		ERRx()

#if defined NSXPCM__ENABLE_FORMHISTORY
#	define ENABLE_FORMHISTORY
#endif

using namespace nsxpcm;

nsIDOMWindow *MasterWindow_ = NULL;
bch::E_BUNCH( nsIDOMWindow *) MasterWindows_;
mtx::mutex_handler__ MasterWindowMutex_;
bso::ulong__ MasterWindowCounter_ = 0;

void nsxpcm::AddMasterWindow( nsIDOMWindow *Window )
{
ERRProlog
	bso::bool__ Locked = false;
ERRBegin
	if ( Window == NULL )
		ERRu();

	mtx::Lock( MasterWindowMutex_ );

	Locked = true;

	if ( MasterWindow_ == NULL )
		MasterWindow_ = Window;
	else
		MasterWindows_.Append( Window );
ERRErr
ERREnd
	if ( Locked )
		mtx::Unlock( MasterWindowMutex_ );
ERREpilog
}

void nsxpcm::RemoveMasterWindow( nsIDOMWindow *Window )
{
ERRProlog
	bso::bool__ Locked = false;
ERRBegin
	if ( Window == NULL )
		ERRu();

	mtx::Lock( MasterWindowMutex_ );

	Locked = true;

	if ( MasterWindow_ == Window ) {
		while ( MasterWindowCounter_ != 0 ) {
			mtx::Unlock( MasterWindowMutex_ );
			Locked = false;

			tht::Defer();

			mtx::Lock( MasterWindowMutex_ );
			Locked = true;
		}

		if ( MasterWindows_.Amount() != 0 ) {
			MasterWindow_ = MasterWindows_( MasterWindows_.First() );

			MasterWindows_.Remove( MasterWindows_.First() );
		} else
			MasterWindow_ = NULL;
	} else {
		epeios::row__ Row = MasterWindows_.Search( Window );

		if ( Row == NONE )
			ERRu();

		MasterWindows_.Remove( Row );
	}
ERRErr
ERREnd
	if ( Locked )
		mtx::Unlock( MasterWindowMutex_ );
ERREpilog
}

nsIDOMWindowInternal *nsxpcm::RetrieveMasterWindow( void )
{
	nsIDOMWindow *Window = NULL;
ERRProlog
	bso::bool__ Locked = false;
ERRBegin
	mtx::Lock( MasterWindowMutex_ );
	Locked = true;

	if ( MasterWindow_ != NULL ) {
		if ( MasterWindowCounter_ == BSO_ULONG_MAX )
			ERRl();

		Window = MasterWindow_;
		MasterWindowCounter_++;
	}
ERRErr
ERREnd
	if ( Locked )
		mtx::Unlock( MasterWindowMutex_ );
ERREpilog
	return GetWindowInternal( Window );
}

void nsxpcm::ReleaseMasterWindow( nsIDOMWindow *Window )
{
ERRProlog
	bso::bool__ Locked = false;
ERRBegin
	mtx::Lock( MasterWindowMutex_ );
	Locked = true;

	if ( MasterWindow_ != Window )
		ERRu();

	if ( MasterWindowCounter_ == 0 )
		ERRu();

	MasterWindowCounter_--;
ERRErr
ERREnd
	if ( Locked )
		mtx::Unlock( MasterWindowMutex_ );
ERREpilog
}

static nsIDOMWindow *JSConsoleWindow_ = NULL;
class jsconsole_widget__ :
public widget_core__
{
protected:
	void NSXPCMOnEvent( event__  )
	{
//		EventData().EventPreventDefault();
//		SetAttribute( GetWindowDocument( GetWindow() ), "hidden", "true" );
	}
} JSConsoleWidget_;
static nsCOMPtr<nsIFormHistory2> FormHistory_;
#ifdef CPE__T_MT
mtx::mutex_handler__ FormHistoryMutex_;
#endif

#define CASE( m )\
	case t##m:\
	return #m;\
	break


static const char *GetLabel_( text__ Text )
{
	switch ( Text ) {
		CASE( XPRJFilterLabel );
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

bso::bool__ nsxpcm::GetTranslation(
	text__ Text,
	const lcl::rack__ &LocaleRack,
	str::string_ &Translation )
{
	return LocaleRack.GetTranslation( GetLabel_( Text ), "NSXPCM_", Translation );
}

const char *nsxpcm::GetTranslation(
	text__ Text,
	const lcl::rack__ &LocaleRack,
	STR_BUFFER___ &Buffer  )
{
	return LocaleRack.GetTranslation( GetLabel_( Text ), "NSXPCM_", Buffer );
}


void nsxpcm::GetJSConsole( nsIDOMWindow *ParentWindow )
{
	bso::bool__ InitializeWidget = ( JSConsoleWindow_ == NULL );

	nsxpcm::GetJSConsole( ParentWindow, &JSConsoleWindow_ );

	if ( InitializeWidget )
		JSConsoleWidget_.Init( JSConsoleWindow_, JSConsoleWindow_, nsxpcm::efClose );
}

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
	NS_CStringToUTF16(ECString, NS_CSTRING_ENCODING_NATIVE_FILESYSTEM, EString );
}

void nsxpcm::Transform(
	const nsEmbedString &EString,
	nsEmbedCString &ECString )
{
	NS_UTF16ToCString( EString, NS_CSTRING_ENCODING_NATIVE_FILESYSTEM, ECString );
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
	STR_BUFFER___ Buffer;
ERRBegin
	Transform( EString.Convert( Buffer ), EString.Amount() + 1, JString );
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
	STR_BUFFER___ Buffer;
ERRBegin
	Transform( String.Convert( Buffer ), EString );
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
/*
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
	Element = nsxpcm::QueryInterface<nsIDOMElement>( EventTarget, err::hSkip  );

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

	Cores( Row )->Handle( Event, S );
ERRErr
ERREnd
ERREpilog
}
*/

void nsxpcm::file_picker_::AddFilter(
	const str::string_ &Title,
	const str::string_ &Mask )
{
ERRProlog
	file_picker_filter FilePickerFilter;
ERRBegin
	FilePickerFilter.Init();

	FilePickerFilter.Title = Title;
	FilePickerFilter.Mask = Mask;

	Filters.Append( FilePickerFilter );
ERRErr
ERREnd
ERREpilog
}

static inline PRInt16 ConvertType_( file_picker_type__ Type )
{
	switch( Type ) {
	case fptOpen :
		return nsIFilePicker::modeOpen;
		break;
	case fptSave :
		return nsIFilePicker::modeSave;
		break;
	case fptFolder :
		return nsIFilePicker::modeGetFolder;
		break;
	default:
		ERRu();
		break;
	}

	return 0;	// Pour éviter un 'warning'.
}

static inline void AddFilter_(
	const char *Title,
	const char *Mask,
	nsIFilePicker *FilePicker )
{
	nsEmbedString ETitle, EMask;

	Transform( Title, ETitle );
	Transform( Mask, EMask );

	FilePicker->AppendFilter( ETitle, EMask );
}


static inline void AddFilter_(
	const file_picker_filter_ &Filter,
	nsIFilePicker *FilePicker )
{
ERRProlog
	STR_BUFFER___ TitleBuffer, MaskBuffer;
ERRBegin
	AddFilter_( Filter.Title.Convert( TitleBuffer ), Filter.Mask.Convert( MaskBuffer ), FilePicker );
ERRErr
ERREnd
ERREpilog
}


static void AddFilters_(
	const file_picker_filters_ &Filters,
	nsIFilePicker *FilePicker )
{
	ctn::E_CITEM( file_picker_filter_ ) Filter;
	epeios::row__ Row = Filters.First();

	Filter.Init( Filters );

	while( Row != NONE ) {
		AddFilter_( Filter( Row ), FilePicker );

		Row = Filters.Next( Row );
	}
}

#define H( mask, value )	ConvertedPredefinedFilters |= ( Filters & fpmf##mask ? nsIFilePicker::filter##value : 0 )
#define SH( mask )	H( mask, mask )


static inline PRInt32 ConvertPredefinedFilters_( int Filters )
{
	PRInt32 ConvertedPredefinedFilters = 0;

	SH( All );
	SH( HTML );
	SH( Text );
	SH( Images );
	SH( XML );
	SH( XUL );
	SH( Apps );

	return ConvertedPredefinedFilters;
}

void AddExtraPredefinedFilters_(
	int Filters,
	const lcl::rack__ &LocaleRack,
	nsIFilePicker *FilePicker )
{
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	if ( Filters & fpmfXPRJ )
		AddFilter_( GetTranslation( nsxpcm::tXPRJFilterLabel, LocaleRack, Buffer ), "*.xprj", FilePicker );
ERRErr
ERREnd
ERREpilog
}

#undef H
#undef SH

bso::bool__ nsxpcm::file_picker_::Show(
	nsIDOMWindow *ParentWindow,
	file_picker_type__ Type,
	const char *DefaultExtension,
	const lcl::rack__ &LocaleRack,
	str::string_ &FileName )
{
	bso::bool__ FileSelected = false;
ERRProlog
	nsCOMPtr<nsIFilePicker> FilePicker = NULL;
	nsresult Error = NS_OK;
	nsEmbedString EString;
	nsIDOMWindow *MasterWindow = NULL;
ERRBegin
	CreateInstance( "@mozilla.org/filepicker;1", FilePicker );

	Transform( Title, EString );

	if ( ParentWindow == NULL )
		ERRu();
	/* 'ParentWindow' doit être fourni et ne peut êtr edéduit de 'MAsterWindow', car,
	cette fonction bloquant toute action sur 'ParentWindow', si plusieurs sessions,
	c'est une mauvaise fenêtre (c'est-à-dire une qui n'a pas initié l'ouverture de
	sélecteur) qui rique d'être bloquée). */

	if ( ( Error = FilePicker->Init( ParentWindow, EString, ConvertType_( Type ) ) ) != NS_OK )
		ERRx();

	if ( ( Error = FilePicker->AppendFilters( ConvertPredefinedFilters_( S_.PredefinedFilters ) ) ) != NS_OK )
		ERRx();

	AddFilters_( Filters, FilePicker );

	AddExtraPredefinedFilters_( S_.PredefinedFilters, LocaleRack, FilePicker );

	if ( ( DefaultExtension != NULL ) && ( *DefaultExtension != 0 ) ) {
		Transform( DefaultExtension, EString );
		T( FilePicker->SetDefaultExtension( EString ) );
	}

	PRInt16 _retval = 0;

	if ( ( Error = FilePicker->Show( &_retval ) ) != NS_OK )
		ERRx();

	if ( _retval == nsIFilePicker::returnCancel ) {
		FileSelected = false;
		ERRReturn;
	} else if ( ( _retval != nsIFilePicker::returnOK ) && ( _retval != nsIFilePicker::returnReplace ) )
		ERRx();

	nsILocalFile *File;

	if ( ( Error = FilePicker->GetFile( &File ) ) != NS_OK )
		ERRx();

	if ( ( Error = File->GetPath( EString ) ) != NS_OK )
		ERRx();

	nsxpcm::Transform( EString, FileName );

	FileSelected = true;
ERRErr
ERREnd
	if ( MasterWindow != NULL )
		ReleaseMasterWindow( MasterWindow );
ERREpilog
	return FileSelected;
}

#if 0
static bso::bool__ FileDialogBox_(
	nsIDOMWindow *Parent,
	const str::string_ &Title,
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

	if ( Parent == NULL )
		Parent = ::MasterWindow;

	if ( Parent == NULL )
		ERRu();

	if ( ( Error = FilePicker->Init( Parent, EString, Mode ) ) != NS_OK )
		ERRx();

	if ( ( Error = FilePicker->AppendFilters( FilterMask ) ) != NS_OK )
		ERRx();

	if ( ( DefaultExtension != NULL ) && ( *DefaultExtension != 0 ) ) {
		Transform( DefaultExtension, EString );
		T( FilePicker->SetDefaultExtension( EString ) );
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
#else
static bso::bool__ FileDialogBox_(
	nsIDOMWindow *ParentWindow,
	const str::string_ &Title,
	file_picker_type__ Type,
	const char *DefaultExtension,
	int PredefineFilters,
	const lcl::rack__ &LocaleRack,
	str::string_ &FileName )
{
	bso::bool__ FileSelected = false;
ERRProlog
	file_picker FilePicker;
ERRBegin
	FilePicker.Init( Title );

	FilePicker.SetPredefinedFilter( PredefineFilters );

	FileSelected = FilePicker.Show( ParentWindow, Type, DefaultExtension, LocaleRack, FileName );
ERRErr
ERREnd
ERREpilog
	return FileSelected;
}
#endif

bso::bool__ nsxpcm::FileOpenDialogBox(
	nsIDOMWindow *ParentWindow,
	const str::string_ &Title,
	const char *DefaultExtension,
	int PredefinedFilters,
	const lcl::rack__ &LocaleRack,
	str::string_ &FileName )
{
	return FileDialogBox_( ParentWindow, Title, fptOpen, DefaultExtension, PredefinedFilters, LocaleRack, FileName );
}

bso::bool__ nsxpcm::FileSaveDialogBox(
	nsIDOMWindow *ParentWindow,
	const str::string_ &Title,
	const char *DefaultExtension,
	int PredefinedFilters,
	const lcl::rack__ &LocaleRack,
	str::string_ &FileName )
{
	return FileDialogBox_( ParentWindow, Title, fptSave, "", PredefinedFilters, LocaleRack, FileName );
}

bso::bool__ nsxpcm::DirectorySelectDialogBox(
	nsIDOMWindow *ParentWindow,
	const str::string_ &Title,
	const lcl::rack__ &LocaleRack,
	str::string_ &FileName )
{
	return FileDialogBox_( ParentWindow, Title, fptFolder, "", fpmf_None, LocaleRack, FileName );
}

void nsxpcm::Delete( widget_cores_ &Widgets )
{
	epeios::row__ Row = Widgets.First();

	while ( Row != NONE ) {
		delete Widgets( Row );

		Widgets.Set( NULL, Row );

		Row = Widgets.Next( Row );
	}

	Widgets.Init();
}


#ifdef NSXPCM__FBL

void nsxpcm::Convert(
	const strings_ &Items,
	fblfrd::ids32_ &Ids )
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
	fblfrd::ids16_ &Ids )
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
	fblfrd::ids8_ &Ids )
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
	fblfrd::ids32_ &Ids )
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
	fblfrd::ids16_ &Ids )
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
	fblfrd::ids8_ &Ids )
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
	const fblfrd::ids8_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<fblfrd::id8__, fblfrd::id8_t__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const fblfrd::ids8_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<fblfrd::id8__, fblfrd::id8_t__>( Ids, Separator, JString );
}

void nsxpcm::ConvertAndJoin(
	const fblfrd::ids16_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<fblfrd::id16__, fblfrd::id16_t__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const fblfrd::ids16_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<fblfrd::id16__, fblfrd::id16_t__>( Ids, Separator, JString );
}

void nsxpcm::ConvertAndJoin(
	const fblfrd::ids32_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<fblfrd::id32__, fblfrd::id32_t__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const fblfrd::ids32_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<fblfrd::id32__, fblfrd::id32_t__>( Ids, Separator, JString );
}

void nsxpcm::ConvertAndJoin(
	const fblfrd::slongs_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<fblfrd::slong__, fblfrd::slong__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const fblfrd::slongs_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<fblfrd::slong__, fblfrd::slong__>( Ids, Separator, JString );
}

void nsxpcm::ConvertAndJoin(
	const fblfrd::ulongs_ &Ids,
	bso::char__ Separator,
	string_ &Joined )
{
	ConvertAndJoin_<fblfrd::ulong__, fblfrd::ulong__>( Ids, Separator, Joined );
}

void nsxpcm::ConvertJoinAndTransform(
	const fblfrd::ulongs_ &Ids,
	bso::char__ Separator,
	char **JString )
{
	ConvertJoinAndTransform_<fblfrd::ulong__, fblfrd::ulong__>( Ids, Separator, JString );
}


void nsxpcm::ConvertAndJoin(
	const fblfrd::booleans_ &Booleans,
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
	const fblfrd::booleans_ &Booleans,
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
	fblfrd::xstrings_ &DataCluster )
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

template <typename t> static void Alert_(
	nsIDOMWindow *Window,
	t Text )
{
ERRProlog
	nsEmbedString NSText;
ERRBegin
	nsxpcm::Transform( Text, NSText );

	T( GetWindowInternal( Window )->Alert( NSText ) );
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::Alert(
	nsIDOMWindow *Window,
	const char *Text )
{
	Alert_<const char *>( Window, Text );
}

void nsxpcm::Alert(
	nsIDOMWindow *Window,
	const str::string_ &Text )
{
	Alert_<const str::string_ &>( Window, Text );
}

template <typename t> bso::bool__ Confirm_(
	nsIDOMWindow *Window,
	t Text )
{
	PRBool Answer = 0;
ERRProlog
	nsEmbedString NSText;
ERRBegin
	nsxpcm::Transform( Text, NSText );

	T( GetWindowInternal( Window )->Confirm( NSText, &Answer ) );
ERRErr
ERREnd
ERREpilog
	return Answer != 0;
}

bso::bool__ nsxpcm::Confirm(
	nsIDOMWindow *Window,
	const char *Text )
{
	return Confirm_<const char *>( Window, Text );
}

bso::bool__ nsxpcm::Confirm(
	nsIDOMWindow *Window,
	const str::string_ &Text )
{
	return Confirm_<const str::string_ &>( Window, Text );
}


void nsxpcm::Log( const str::string_ &Text )
{
ERRProlog
	nsEmbedString String;
	nsCOMPtr<nsIConsoleService> ConsoleService = NULL;
	str::string StampedText;
	tol::buffer__ Buffer;
ERRBegin
	StampedText.Init( "[" );
	StampedText.Append( tol::DateAndTime( Buffer ) );
	StampedText.Append( "]\n" );
	StampedText.Append( Text );

	nsxpcm::GetService<nsIConsoleService>( NS_CONSOLESERVICE_CONTRACTID, ConsoleService );

	nsxpcm::Transform( StampedText, String );

	T( ConsoleService->LogStringMessage( String.get() ) );
ERRErr
ERREnd
ERREpilog
}



static event__ Convert_( const char *RawEvent )
{
	event__ Event = e_Undefined;

	if ( !strcmp( RawEvent, "command" ) )
		Event = eCommand;
	else if ( !strcmp( RawEvent, "input" ) )
		Event = eInput;
	else if ( !strcmp( RawEvent, "click" ) )
		Event = eClick;
	else if ( !strcmp( RawEvent, "dblclick" ) )
		Event = eDblClick;
	else if ( !strcmp( RawEvent, "focus" ) )
		Event = eFocus;
	else if ( !strcmp( RawEvent, "blur" ) )
		Event = eBlur;
	else if ( !strcmp( RawEvent, "select" ) )
		Event = eSelect;
	else if ( !strcmp( RawEvent, "DOMAttrModified" ) )
		Event = eAttributeChange;
	else if ( !strcmp( RawEvent, "keypress" ) )
		Event = eKeyPress;
	else if ( !strcmp( RawEvent, "load" ) )
		Event = eLoad;
	else if ( !strcmp( RawEvent, "close" ) )
		Event = eClose;
	else
		ERRl();

	return Event;
}

bso::bool__ nsxpcm::widget_core__::Handle( nsIDOMEvent *RawEvent )
{
	bso::bool__ Success = false;
ERRProlog
	nsEmbedString String;
	str::string EventString;
	event__ Event = e_Undefined;
	STR_BUFFER___ StrBuffer;
ERRBegin
	// Sauvegarde pour la gestion d'évènements imbriqués.
	nsIDOMEvent *RawEventBuffer = _EventData._RawEvent;
	nsIDOMMutationEvent *MutationEventBuffer = _EventData._MutationEvent;
	nsIDOMKeyEvent *KeyEventBuffer = _EventData._KeyEvent;

	_EventData._RawEvent = RawEvent;

	T( RawEvent->GetType( String ) );

	EventString.Init();

	nsxpcm::Transform( String, EventString );

	Event = Convert_( EventString.Convert( StrBuffer ) );

	if ( EventString == "DOMAttrModified" )
		_EventData._MutationEvent = QueryInterface<nsIDOMMutationEvent>( RawEvent );

	if ( EventString == "keypress" )
		_EventData._KeyEvent = QueryInterface<nsIDOMKeyEvent> ( RawEvent );

	if ( _EventData._EventImbricationLevel++ == NSXPCM__EVENT_IMBRICATION_LEVEL_MAX )
		ERRl();

	if ( !( _EventData._EventsToIgnore & ( 1 << Event ) ) )
		if ( _EventHandler != NULL )
			_EventHandler->OnEvent( _EventData, Event );
		else
			NSXPCMOnEvent( Event );

	if ( _EventData._EventImbricationLevel-- < -1 )
		ERRc();

	_EventData._RawEvent = RawEventBuffer;
	_EventData._MutationEvent = MutationEventBuffer;
	_EventData._KeyEvent = KeyEventBuffer;

	Success = true;
ERRErr
	if ( ( ERRMajor == err::itn ) && ( ERRMinor == err::iAbort ) )
		Success = true;	// L'erreur a été déjà traite (ou va l'être ci-dessous) ; plus la peine de la remonter.
	NSXPCM_ERR( _Window );
ERREnd
ERREpilog
	return Success;
}


void nsxpcm::widget_core__::Init(
	nsISupports *Supports,
	nsIDOMWindow *Window,
	int Events )
{
#ifdef NSXPCM_DBG
	if ( _Supports != NULL )
		ERRu();
#endif
	reset();

	_Window = GetWindowInternal( Window );
	_Supports = Supports;

	nsIDOMEventTarget *EventTarget = nsxpcm::QueryInterface<nsIDOMEventTarget>( Supports );

	nsxpcm::CreateInstance( NSXPCM_EVENT_LISTENER_CONTRACTID, _EventData._EventListener );

	if ( Events & efCommand )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "command" ), _EventData._EventListener, false ) != NS_OK )
			ERRc();

	if ( Events & efInput )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "input" ), _EventData._EventListener, false ) != NS_OK )
			ERRc();

	if ( Events & efClick )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "click" ), _EventData._EventListener, false ) != NS_OK )
			ERRc();

	if ( Events & efDblClick )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "dblclick" ), _EventData._EventListener, false ) != NS_OK )
			ERRc();

	if ( Events & efFocus )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "focus" ), _EventData._EventListener, false ) != NS_OK )
			ERRc();

	if ( Events & efBlur )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "blur" ), _EventData._EventListener, false ) != NS_OK )
			ERRc();

	if ( Events & efSelect )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "select" ), _EventData._EventListener, false ) != NS_OK )
			ERRc();

	if ( Events & efAttributeChange )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "DOMAttrModified" ), _EventData._EventListener, false ) != NS_OK )
			ERRc();

	if ( Events & efKeyPress )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "keypress" ), _EventData._EventListener, false ) != NS_OK )
			ERRc();

	if ( Events & efLoad )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "load" ), _EventData._EventListener, false ) != NS_OK )
			ERRc();

	if ( Events & efClose )
		if ( EventTarget->AddEventListener( NS_LITERAL_STRING( "close" ), _EventData._EventListener, false ) != NS_OK )
			ERRc();

	_EventData._EventListener->Init( *this );
}

NS_IMPL_ISUPPORTS1(nsxpcm::event_listener, nsxpcm::ievent_listener)

// ATTENTION, IMPORTANT : en cas de comportement étrange, voir remarque concernant cet objet dans le '.h' !
NS_IMETHODIMP nsxpcm::event_listener::HandleEvent(nsIDOMEvent *Event)
{
	nsresult NSResult = NS_OK;

	if ( !_Widget->Handle( Event ) )
		NSResult = NS_ERROR_FAILURE;

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


nsIDOMDocumentFragment *nsxpcm::XSLTransform(
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
	STR_BUFFER___ NameBuffer;
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
		T( Value->SetAsString( Parameter( Row ).Value.Convert( NameBuffer ) ) );

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

static void Preprocess_(
	flw::iflow__ &IFlow,
	const str::string_ &BaseDirectory,
	str::string_ &ProcessedXMLString )
{
ERRProlog
	flx::E_STRING_OFLOW___ SFlow;
	txf::text_oflow__ TFlow;
ERRBegin
	SFlow.Init( ProcessedXMLString );
	TFlow.Init( SFlow );

	if ( xpp::Process( IFlow, BaseDirectory, xml::oCompact, TFlow ) != xpp::sOK )
		ERRu();
ERRErr
ERREnd
ERREpilog
}

static bso::bool__ _GetXMLDocument(
	const str::string_ &XMLString,
	nsIDOMDocument *&XMLDocument )
{
	nsresult Result = NS_OK;

	nsEmbedString XMLEmbedString;
	nsCOMPtr<nsIDOMParser> Parser;

	nsxpcm::Transform( XMLString, XMLEmbedString );
	
	CreateInstance( NS_DOMPARSER_CONTRACTID, Parser );
	Result = Parser->ParseFromString( XMLEmbedString.get(), "text/xml", &XMLDocument );

	return Result == NS_OK;
}

static bso::bool__ _GetXSLStylesheet(
	const str::string_ &XSLStylesheetFileName,
	nsIDOMDocument *&XSLStylesheet )
{
	nsresult Result = NS_OK;
	nsEmbedCString Method, URL;
	nsEmbedString Empty;
	nsCOMPtr<nsIXMLHttpRequest> HTTPRequest;

	Transform( "GET", Method );
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

	return true;
}

nsIDOMDocumentFragment *nsxpcm::XSLTransformByContent(
	const str::string_ &XMLString,
	const str::string_ &XSLString,
	nsIDOMDocument *Owner,
	const xslt_parameters_ &Parameters )
{
	nsIDOMDocumentFragment *Fragment = NULL;
ERRProlog
	nsIDOMDocument *XMLDocument = NULL, *XSLStylesheet = NULL;
ERRBegin
	if ( !_GetXMLDocument( XMLString, XMLDocument ) )
		ERRu();

	if ( !_GetXMLDocument( XSLString, XSLStylesheet ) )
		ERRu();

	Fragment = XSLTransform( XMLDocument, XSLStylesheet, Owner, Parameters );
ERRErr
	Fragment = NULL;
ERREnd
ERREpilog
	return Fragment;
}

nsIDOMDocumentFragment *nsxpcm::XSLTransformByFileName(
	const str::string_ &XMLString,
	const str::string_ &XSLFileName,
	nsIDOMDocument *Owner,
	const xslt_parameters_ &Parameters )
{
	nsIDOMDocumentFragment *Fragment = NULL;
ERRProlog
	nsIDOMDocument *XMLDocument = NULL, *XSLStylesheet = NULL;
ERRBegin
	if ( !_GetXMLDocument( XMLString, XMLDocument ) )
		ERRu();

	if ( !_GetXSLStylesheet( XSLFileName, XSLStylesheet ) )
		ERRu();

	Fragment = XSLTransform( XMLDocument, XSLStylesheet, Owner, Parameters );
ERRErr
	Fragment = NULL;
ERREnd
ERREpilog
	return Fragment;
}

nsIDOMNode *browser__::GetNext( void )
{
	if ( _Current != NULL ) {
		if ( _Current == _Root )
			_Current = GetFirstChild( _Root );
		else if ( GetFirstChild( _Current ) != NULL )
			_Current = GetFirstChild( _Current );
		else if ( GetNextSibling( _Current ) != NULL )
			_Current = GetNextSibling( _Current );
		else {
			do
				_Current = GetParentNode( _Current );
			while ( ( _Current != _Root ) && ( GetNextSibling( _Current ) == NULL ) );

			if ( _Current == _Root )
				_Current = NULL;
			else
				_Current = GetNextSibling( _Current );
		}
	}

	return _Current;
}

void nsxpcm::LaunchURI( const str::string_ &RawURI )
{
	nsCOMPtr<nsIIOService> IOService = NULL;
	nsCOMPtr<nsIExternalProtocolService> ExternalProtocolService = NULL;
	nsEmbedCString TransformedURI;
	nsIURI *URI = NULL;

	nsresult Result = NS_OK;	// Uniquement à des fins de débogage.

	CreateInstance( "@mozilla.org/network/io-service;1", IOService );
	CreateInstance( "@mozilla.org/uriloader/external-protocol-service;1", ExternalProtocolService );

	Transform( RawURI, TransformedURI );

	if ( ( Result = IOService->NewURI( TransformedURI, NULL, NULL, &URI ) ) != NS_OK )
		ERRu();

	if ( ( Result = ExternalProtocolService->LoadURI( URI, NULL ) ) != NS_OK )
		ERRu();
}

void nsxpcm::LaunchEmbedFile( const char *RawFile )
{
ERRProlog
	nsCOMPtr<nsILocalFile> LocalFile = NULL;
	nsCOMPtr<nsIDirectoryServiceProvider> DirectoryServiceProvider;
	nsIFile *File = NULL;
	nsEmbedString Path;
	nsEmbedString Transformed;
	str::string Joined;
	strings Splitted;
	epeios::row__ Row = NONE;
ERRBegin
	nsresult Result = NS_OK;	// Uniquement à des fins de débogage.

	CreateInstance( "@mozilla.org/file/local;1", LocalFile );
	GetService<nsIDirectoryServiceProvider>( "@mozilla.org/file/directory_service;1", DirectoryServiceProvider );

	PRBool Persistent = false;
	T( DirectoryServiceProvider->GetFile( "CurWorkD", &Persistent, &File ) );

	if ( ( Result = File->GetPath( Path ) ) != NS_OK )
		ERRu();

	if ( ( Result = LocalFile->InitWithPath( Path ) ) != NS_OK )
		ERRu();

	Joined.Init( RawFile );
	Splitted.Init();

	Split( Joined, '/', Splitted );

	if ( Splitted.Amount() == 0 )
		Splitted.Append( Joined );

	Row = Splitted.First();

	while ( Row != NONE ) {
		Transform( Splitted( Row ), Transformed );

		if ( ( Result = LocalFile->AppendRelativePath( Transformed ) ) != NS_OK )
			ERRu();

		Row = Splitted.Next( Row );
	}

	if ( ( Result = LocalFile->Launch() ) != NS_OK )
		ERRu();
ERRErr
ERREnd
ERREpilog
}

void nsxpcm::AddFormHistoryEntry(
	const str::string_ &RawName,
	const str::string_ &RawEntry )
{
#ifdef ENABLE_FORMHISTORY
	nsEmbedString Name, Entry;

	nsxpcm::Transform( RawName, Name );
	nsxpcm::Transform( RawEntry, Entry );

#	ifdef CPE__T_MT
	mtx::Lock( FormHistoryMutex_ );
#	endif
	FormHistory_->AddEntry( Name, Entry );
#	ifdef CPE__T_MT
	mtx::Unlock( FormHistoryMutex_ );
#	endif
#else
	ERRl();
#endif
}

void nsxpcm::RemoveFormHistoryEntry(
	const str::string_ &RawName,
	const str::string_ &RawEntry )
{
#ifdef ENABLE_FORMHISTORY
	nsEmbedString Name, Entry;

	nsxpcm::Transform( RawName, Name );
	nsxpcm::Transform( RawEntry, Entry );

#	ifdef CPE__T_MT
	mtx::Lock( FormHistoryMutex_ );
#	endif
	FormHistory_->RemoveEntry( Name, Entry );
#	ifdef CPE__T_MT
	mtx::Unlock( FormHistoryMutex_ );
#	endif
#else
	ERRl();
#endif
}

void nsxpcm::RemoveEntriesForName( const str::string_ &RawName )
{
#ifdef ENABLE_FORMHISTORY
	nsEmbedString Name;

	nsxpcm::Transform( RawName, Name );

#	ifdef CPE__T_MT
	mtx::Lock( FormHistoryMutex_ );
#	endif
	FormHistory_->RemoveEntriesForName( Name );
#	ifdef CPE__T_MT
	mtx::Unlock( FormHistoryMutex_ );
#	endif
#else
	ERRl();
#endif
}

bso::ulong__ nsxpcm::GetArguments(
	nsICommandLine *CommandLine,
	arguments_ &Arguments )
{
	PRInt32 Amount = 0;
ERRProlog
	PRInt32 Counter = 0;
	str::string Argument;
	nsEmbedString RawArgument;
ERRBegin
	T( CommandLine->GetLength( &Amount ) );

	if ( ( Amount < 0 ) || ( Amount > BSO_ULONG_MAX ) )
		ERRs();

	while ( Counter < Amount ) {
		T( CommandLine->GetArgument( Counter++, RawArgument ) );

		Argument.Init();
		Transform( RawArgument, Argument );
		Arguments.Append( Argument );
	}
		
ERRErr
ERREnd
ERREpilog
	return (bso::ulong__ )Amount;
}

static void GetBroadcastAttributeValue_(
	nsIDOMDocument *Document,
	const str::string_ &Id,
	const char *AttributeName,
	str::string_ &Value )
{
	nsIDOMNode *Node = nsxpcm::GetElementById( Document, Id );

	if ( Node != NULL )
		nsxpcm::GetAttribute( Node, AttributeName, Value );
}

static void CopyBroadcastCommand_(
	nsIDOMDocument *Document,
	nsIDOMNode *Node )
{
ERRProlog
	str::string ObservesAttributeValue;
	str::string CommandAttributeValue;
ERRBegin
	ObservesAttributeValue.Init();

	nsxpcm::GetAttribute( Node, "observes", ObservesAttributeValue );

	if ( ObservesAttributeValue.Amount() != 0 ) {
		CommandAttributeValue.Init();
		GetBroadcastAttributeValue_( Document, ObservesAttributeValue, "command", CommandAttributeValue );

		if ( CommandAttributeValue.Amount() != 0 )
			if ( !HasAttribute( Node, "oncommand" ) )
				nsxpcm::SetAttribute( Node, "command", CommandAttributeValue );
	}
ERRErr
ERREnd
ERREpilog
}

static void CopyBroadcastCommand_(
	nsIDOMDocument *Document,
	nsIDOMNodeList *List )
{
	PRUint32 Length = 0;
	nsIDOMNode *Node = NULL;

	T( List->GetLength( &Length ) );

	while ( Length-- ) {
		T( List->Item( Length, &Node ) );

		CopyBroadcastCommand_( Document, Node );
	}
}

void nsxpcm::PatchBadCommandBehaviorForListeningMenuItems( nsIDOMDocument *Document )
{
	nsIDOMNodeList *List = NULL;
	nsEmbedString EId;

	nsxpcm::Transform( "menuitem", EId );

	T( Document->GetElementsByTagName( EId, &List ) );

	CopyBroadcastCommand_( Document, List );
}


static void AddSemiColonCommand_(nsIDOMNodeList *List )
{
	PRUint32 Length = 0;
	nsIDOMNode *Node = NULL;

	T( List->GetLength( &Length ) );

	while ( Length-- ) {
		T( List->Item( Length, &Node ) );

		if ( !HasAttribute( Node, "oncommand" ) )
			nsxpcm::SetAttribute( Node, "oncommand", ";" );
	}
}

void nsxpcm::PatchCommandBadCommandBehaviorforKeysetListener( nsIDOMDocument *Document )
{
	nsIDOMNodeList *List = NULL;
	nsEmbedString EId;

	nsxpcm::Transform( "command", EId );

	if ( Document->GetElementsByTagName( EId, &List ) != NS_OK )
		ERRu();

	AddSemiColonCommand_( List );
}

nsIDOMWindowInternal *nsxpcm::GetDocumentWindow( nsIDOMDocument *Document )
{
	nsEmbedString EId;
	nsIDOMNodeList *List = NULL;
	PRUint32 Length = 0;
	nsIDOMNode *Node = NULL;
	nsIDOMWindow *Window = NULL;

	nsxpcm::Transform( "window", EId );

	if ( Document->GetElementsByTagName( EId, &List ) != NS_OK )
		ERRu();

	T( List->GetLength( &Length ) );

	if ( Length != 1 )
		ERRx();

	T( List->Item( Length, &Node ) );

#if 0
	return QueryInterface<nsIDOMWindow>( Node );
#else
	return GetWindowInternal( Node );
#endif

}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class nsxpcmpersonnalization
: public nsxpcmtutor
{
public:
	nsxpcmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
#ifdef ENABLE_FORMHISTORY
		nsxpcm::GetService( "@mozilla.org/satchel/form-history;1", FormHistory_ );
#	ifdef CPE__T_MT
		FormHistoryMutex_ = mtx::Create( mtx::mOwned );
#	endif
#endif
		MasterWindowMutex_ = mtx::Create( mtx::mOwned );
		MasterWindows_.Init();
	}
	~nsxpcmpersonnalization( void )
	{
#ifdef ENABLE_FORMHISTORY
#	ifdef CPE__T_MT
		mtx::Delete( FormHistoryMutex_ );
#	endif
#endif
		mtx::Delete( MasterWindowMutex_ );

		if ( ::JSConsoleWindow_ != NULL )
			Close( JSConsoleWindow_ );
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
