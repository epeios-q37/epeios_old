/*
	'rgstry' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'rgstry' header file ('rgstry.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (http://zeusw.org/intl/contact.html).
$_RAW_$
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

#define RGSTRY__COMPILATION

#include "rgstry.h"

class rgstrytutor
: public ttr_tutor
{
public:
	rgstrytutor( void )
	: ttr_tutor( RGSTRY_NAME )
	{
#ifdef RGSTRY_DBG
		Version = RGSTRY_VERSION "\b\bD $";
#else
		Version = RGSTRY_VERSION;
#endif
		Owner = RGSTRY_OWNER;
		Date = "$Date$";
	}
	virtual ~rgstrytutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "flx.h"
#include "flf.h"
#include "fnm.h"
#include "lcl.h"
#include "crptgr.h"

using namespace rgstry;

#define MESSAGE_PREFIX	"RGSTRY_"

#define CASE( name )			LCL_CASE( name, s )

const char *rgstry::Label( status__ Status )
{
	switch ( Status ) {
	CASE( UnableToOpenFile )
	CASE( ParseError )
	CASE( UnableToFindRootPath )
	CASE( RootPathError )
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour éviter un '<arbibg'.

}

const str::string_ &rgstry::GetTranslation(
	const context___ &Context,
	const lcl::rack__ &LocaleRack,
	str::string_ &Translation )
{
ERRProlog
	lcl::strings TagValues;
	str::string TagValue;
	str::string Message;
ERRBegin
	Message.Init();

	switch ( Context.Status ) {
	case sOK:
		ERRu();
		break;
	case sUnableToOpenFile:
		Message.Init();

		LocaleRack.GetTranslation( Label( Context.Status ), MESSAGE_PREFIX, Message );

		TagValues.Init();
		TagValues.Append( Context.FileName );

		lcl::ReplaceTags( Message, TagValues );
		break;
	case sParseError:
		xpp::GetTranslation( Context, LocaleRack, Message );
		break;
	case sRootPathError:
		if ( Context.PathErrorRow != NONE )
			ERRu();
	case sUnableToFindRootPath:
		LocaleRack.GetTranslation( Label( Context.Status ), MESSAGE_PREFIX, Message );
		break;
	default:
		ERRu();
		break;
	}

	Translation.Append( Message );

ERRErr
ERREnd
ERREpilog
	return Translation;
}

row__ rgstry::registry_::_Search(
	const name_ &Name,
	const rows_ &NodeRows,
	epeios::row__ &Cursor ) const
{
	buffer Buffer;

	epeios::row__ &Row = Cursor;
	
	if ( Row == NONE )
		Row = NodeRows.First();
	else
		Row = NodeRows.Next( Row );

	Buffer.Init( Nodes );

	while ( ( Row != NONE ) && ( Buffer( NodeRows( Row ) ).Name != Name ) )
		Row = NodeRows.Next( Row );

	if ( Row != NONE )
		return NodeRows( Row );
	else
		return NONE;

}

row__ rgstry::registry_::_Search(
	nature__ Nature,
	const name_ &Name,
	const rows_ &NodeRows,
	cursor__ &Cursor ) const
{
	buffer Buffer;

	row__ Row = NONE;
	
	Buffer.Init( Nodes );

	Row = _Search( Name, NodeRows, Cursor );

	while ( ( Row != NONE ) && ( Buffer( Row ).Nature() != Nature ) )
		Row = _Search( Name, NodeRows, Cursor );

	return Row;
}

row__ rgstry::registry_::_Search(
	const name_ &KeyName,
	const name_ &AttributeName,
	const value_ &AttributeValue,
	row__ Row,
	cursor__ &Cursor ) const
{
	row__ ResultRow = NONE;

	if ( KeyName.Amount() != 0 ) {
		// .../KeyName...

		ResultRow = _SearchKey( KeyName, Row, Cursor );

		if ( ( ResultRow != NONE ) && ( AttributeName.Amount() != 0 ) ) {
			// .../KeyName[@AttributeName='AttributeValue']/...

			while ( ( ResultRow != NONE ) && ( !_AttributeExists( AttributeName, AttributeValue, ResultRow ) ) ) {
				ResultRow = _SearchKey( KeyName, Row, Cursor );
			}
		}
	} else {
		if ( AttributeName.Amount() != 0 ) {
			// .../@AttributeName...
#ifdef RGSTRY_DBG
			if ( AttributeValue.Amount() != 0 )
				ERRc();
#endif
				ResultRow = _SearchAttribute( AttributeName, Row, Cursor );
		}
	}

	return ResultRow;
}

const str::string_ &rgstry::registry_::GetCompleteName(
	row__ Row,
	str::string_ &Name,
	const char *Separator ) const
{
	buffer Buffer;

	Buffer.Init( Nodes );

	if ( Row != NONE ) {
		Name.Append( Buffer( Row ).Name );
		Row = Buffer( Row ).ParentRow();
	}

	while ( Row != NONE ) {
		Name.Insert( Separator, 0 );
		Name.Insert( Buffer( Row ).Name, 0 );

		Row = Buffer( Row ).ParentRow();
	}

	return Name;
}

enum state__ {
	sKeyName,
	sAttributeName,
	sAttributeValue,
	s_amount,
	s_Undefined
};

epeios::row__ rgstry::BuildPath(
	const str::string_ &PathString,
	path_ &Path )
{
	epeios::row__ Row = NONE;
ERRProlog
	bso::bool__ Continue = true;
	state__ State = ::s_Undefined;
	bso::char__ C;
	path_item Item;
	bso::bool__ KeyNameAsAttribute = false;
	bso::bool__ AttributeMarkerFound = false;
ERRBegin
	Row = PathString.First();

	Item.Init();

	Continue = Row != NONE;

	State = sKeyName;

	while ( Continue ) {
		C = PathString( Row );
		switch ( State ) {
		case sKeyName:
			switch ( C ) {
			case '=':
			case '"':
				Continue = false;
				break;
			case '[':
				if ( KeyNameAsAttribute )
					Continue = false;
				else if ( Item.KeyName.Amount() == 0 )
					Continue = false;
				State = sAttributeName;
				Item.AttributeName.Init();
				AttributeMarkerFound = false;
				break;
			case '/':
				if ( KeyNameAsAttribute )
					Continue = false;
				else if ( Item.KeyName.Amount() == 0 )
					Continue = false;
				else
					Path.Append( Item );
				Item.Init();
				break;
			case '@':
				if ( Item.KeyName.Amount() != 0 )
					Continue = false;
				else
					KeyNameAsAttribute = true;
				break;
			default:
				Item.KeyName.Append( C );
				break;
			}
			break;
			case sAttributeName:
				switch ( C ) {
				case ']':
					Continue = false;
					break;
				case '"':
					Continue = false;
					break;
				case '@':
					if ( Item.AttributeName.Amount() != 0 )
						Continue = false;
					else if ( AttributeMarkerFound )
						Continue = false;
					else
						AttributeMarkerFound = true;
					break;
				case '=':
					if ( Item.AttributeName.Amount() == 0 )
						Continue = false;
					State = sAttributeValue;
					Item.AttributeValue.Init();
					if ( ( Row = PathString.Next( Row ) ) == NONE )
						Continue = false;
					else if ( PathString( Row ) != '"' )
						Continue = false;
					break;
				default:
					Item.AttributeName.Append( C );
					break;
				}
				break;
			case sAttributeValue:
				switch ( C ) {
				case '[':
				case ']':
				case '/':
					Continue = false;
					break;
				case '"':
					if ( ( Row = PathString.Next( Row ) ) == NONE )
						Continue = false;
					else if ( PathString( Row ) != ']' )
						Continue = false;
					else if ( ( Row = PathString.Next( Row ) ) == NONE )
						Continue = false;
					else if ( PathString( Row ) != '/' )
						Continue = false;
					Path.Append( Item );
					Item.Init();
					State = sKeyName;
					break;
				default:
					Item.AttributeValue.Append( C );
					break;
				}
				break;
			default:
				ERRc();
				break;
		}

		if ( Continue )
			Row = PathString.Next( Row );

		if ( Row == NONE )
			Continue = false;
	}

	if ( Item.KeyName.Amount() != 0 ) {
		if ( KeyNameAsAttribute ) {
			Item.AttributeName = Item.KeyName;
			Item.KeyName.Init();
		}

		Path.Append( Item );
	}
ERRErr
ERREnd
ERREpilog
	return Row;
}

// Pour simplifier la mise en oeuvre de 'PathErrorRow' en tant que paramètre optionnel (alors == 'NULL').
static inline bso::bool__ BuildPath_(
	const str::string_ &PathString,
	path_ &Path,
	epeios::row__ *PathErrorRow )
{
	epeios::row__ PathErrorRowBuffer = BuildPath( PathString, Path );

	if ( PathErrorRowBuffer != NONE )
		if ( PathErrorRow == NULL )
			ERRu();
		else {
			*PathErrorRow = PathErrorRowBuffer;
			return false;
		}

	return true;

}

row__ rgstry::registry_::_Search(
	const path_ &Path,
	epeios::row__ PathRow,
	row__ Row,
	rows_ &ResultRows ) const
{
	cursor__ CandidateRow = NONE;
	cursor__ Cursor = NONE;
	ctn::E_CITEM( path_item_ ) Item;
	row__ ResultRow, ChildRow = NONE;
	bso::bool__ All = &ResultRows != NULL;
	buffer Buffer;

	Item.Init( Path );
	Buffer.Init( Nodes );

	ResultRow = ChildRow = _Search( Item( PathRow ), Row, Cursor );

#ifdef RGSTRY_DBG
	tol::E_FPOINTER___( bso::char__ ) Key;
	tol::E_FPOINTER___( bso::char__ ) AttributeName;
	tol::E_FPOINTER___( bso::char__ ) AttributeValue;

	Item( PathRow ).KeyName.Convert( Key );
	Item( PathRow ).AttributeName.Convert( AttributeName );
	Item( PathRow ).AttributeValue.Convert( AttributeValue );
#endif

	if ( PathRow != Path.Last() ) {
#ifdef RGSTRY_DBG
		if ( ( ResultRow != NONE ) && ( Buffer( ResultRow ).Nature() == nAttribute ) )
			ERRc();
#endif
		while ( ChildRow != NONE )  {
			ResultRow = _Search( Path, Path.Next( PathRow ), ChildRow, ResultRows );

			if ( ResultRow != NONE ) {
				if ( All ) {
					ChildRow = _Search( Item( PathRow ), Row, Cursor );
				} else
					ChildRow = NONE;
			} else
				ChildRow = _Search( Item( PathRow ), Row, Cursor );
		}

	} else if ( All ) {

		while ( ChildRow != NONE ) {
			ResultRows.Append( ChildRow );

			ChildRow = _Search( Item( PathRow ), Row, Cursor );
		}

	}

	return ResultRow;
}

row__ rgstry::registry_::_Search(
	const path_ &Path,
	row__ Row ) const
{
	epeios::row__ PathRow = NONE, Cursor = NONE;

	if ( Path.Amount() != 0 )
		return _Search( Path, Path.First(), Row, *(rows_ *)NULL );
	else
		return NONE;
}

row__ rgstry::registry_::_Search(
	const str::string_ &PathString,
	row__ Row,
	epeios::row__ *PathErrorRow ) const
{
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		Row = NONE;
	else
		Row = _Search( Path, Row );

ERRErr
ERREnd
ERREpilog
	return Row;
}

row__ rgstry::registry_::Create(
	const path_ &Path,
	row__ Row )
{
	bso::bool__ Exists = true;
	ctn::E_CITEM( path_item_ ) Item;
	epeios::row__ PathRow = Path.First();
	row__ CandidateRow = NONE;

	Item.Init( Path );

	while ( ( PathRow != NONE ) && Exists ) {
		CandidateRow = _Search( Item( PathRow ), Row );

		if ( CandidateRow == NONE ) {
			Exists = false;

			Row = _Create( Item( PathRow ), Row );
		} else
			Row = CandidateRow;

		PathRow = Path.Next( PathRow );
	}

	while ( PathRow != NONE ) {
		Row = _Create( Item( PathRow ), Row );

		PathRow = Path.Next( PathRow );
	}

	return Row;
}

row__ rgstry::registry_::Create(
	const str::string_ &PathString,
	row__ Row,
	epeios::row__ *PathErrorRow )
{
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		Row = NONE;
	else
		Row = Create( Path, Row );

ERRErr
ERREnd
ERREpilog
	return Row;
}


row__ rgstry::registry_::SetValue(
	const str::string_ &PathString,
	const value_ &Value,
	row__ Row,
	epeios::row__ *PathErrorRow )
{
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		Row = SetValue( Path, Value, Row );
	else
		Row = NONE;

ERRErr
ERREnd
ERREpilog
	return Row;
}

#ifdef RGSTRY_PREPROCESSOR_NAMESPACE
#	define DEFAULT_NAMESPACE RGSTRY_PREPROCESSOR_NAMESPACE
#else
#	define DEFAULT_NAMESPACE XPP__PREPROCESSOR_DEFAULT_NAMESPACE
#endif

class callback___
: public xml::callback__
{
private:
	registry_ &_Registry;
	row__ _Target, _Current;
protected:
	virtual bso::bool__ XMLProcessingInstruction( const xml::dump_ & )
	{
		return true;
	}
	virtual bso::bool__ XMLStartTag(
		const str::string_ &TagName,
		const xml::dump_ &Dump )
	{

		if ( _Current == NONE )
			_Current = _Registry.CreateRegistry( str::string( "_root" ) );
		
		_Current = _Registry.AddKey( TagName, _Current );

		if ( _Target == NONE )
			_Target = _Current;

		return true;
	}
	virtual bso::bool__ XMLAttribute(
		const str::string_ &TagName,
		const str::string_ &Name,
		const str::string_ &Value,
		const xml::dump_ &Dump )
	{
		_Registry.AddAttribute( Name, Value, _Current );

		return true;
	}
	virtual bso::bool__ XMLStartTagClosed(
		const str::string_ &TagName,
		const xml::dump_ &Dump )
	{
		return true;
	}
	virtual bso::bool__ XMLValue(
		const str::string_ &TagName,
		const str::string_ &Value,
		const xml::dump_ &Dump )
	{
		_Registry.SetValue( Value, _Current, true );

		return true;
	}
	virtual bso::bool__ XMLEndTag(
		const str::string_ &TagName,
		const xml::dump_ &Dump )
	{

		_Current = _Registry.GetParentRow( _Current );

		if ( _Current == NONE )
			return false;

		return true;
	}
	virtual bso::bool__ XMLComment(
		const str::string_ &Value,
		const xml::dump_ &Dump )
	{
		return true;
	}
public:
	callback___( registry_ &Registry )
	: _Registry( Registry )
	{
		_Target = _Current = NONE;
	}
	void Init( row__ Root )
	{
		_Target = NONE;
		_Current = Root;
	}
	row__ GetTarget( void ) const
	{
		return _Target;
	}
	row__ GetRoot( void ) const
	{
		return _Current;
	}
};

void rgstry::registry_::_Delete( row__ Row )
{
	const node_ &Node = Nodes( Row );

	_Delete( Node.Children );

	Nodes.Delete( Row );
}

bso::bool__ rgstry::registry_::Exists(
	const str::string_ &PathString,
	row__ ParentRow,
	epeios::row__ *PathErrorRow ) const
{
	bso::bool__ Result = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		Result = Exists( Path, ParentRow );
ERRErr
ERREnd
ERREpilog
	return Result;
}

void rgstry::registry_::_DumpAttributes(
	row__ Row,
	xml::writer_ &Writer ) const
{
ERRProlog
	buffer Buffer;
	rows Rows;
	cursor__ Cursor = NONE;
ERRBegin
	Buffer.Init( Nodes );

	Rows.Init();

	Rows = Buffer( Row ).Children;

	Cursor = Rows.First();

	while ( Cursor != NONE ){
		if ( Buffer( Rows( Cursor ) ).Nature() == nAttribute )
			Writer.PutAttribute( Buffer().Name, Buffer().Value );

		Cursor = Rows.Next( Cursor );
	}
ERRErr
ERREnd
ERREpilog
}

epeios::size__ rgstry::registry_::_Dump(
	row__ Root,
	bso::bool__ RootToo,
	xml::writer_ &Writer,
	buffer &Buffer ) const
{
	epeios::size__ ChildAmount = 0;
ERRProlog
	rows Children;
	epeios::row__ Row = NONE;
ERRBegin
	Children.Init();
	Children = Buffer( Root ).Children;

	if ( RootToo )
		_DumpNode( Root, Writer, Buffer );
	
	Row = Children.First();

	while ( Row != NONE ) {
		if ( Buffer( Children( Row ) ).GetNature() != nAttribute )
			_Dump( Children( Row ), true, Writer, Buffer );

		Row = Children.Next( Row );
	}

	if ( RootToo )
		Writer.PopTag();

	ChildAmount = Children.Amount();
ERRErr
ERREnd
ERREpilog
	return ChildAmount;
}


epeios::size__ rgstry::registry_::Dump(
	row__ Root,
	bso::bool__ RootToo,
	xml::writer_ &Writer ) const
{
	epeios::size__ ChildAmount = 0;
ERRProlog
	buffer Buffer;
ERRBegin
	Buffer.Init( Nodes );

	ChildAmount = _Dump( Root, RootToo, Writer, Buffer );
ERRErr
ERREnd
ERREpilog
	return ChildAmount;
}

epeios::size__ rgstry::registry_::Dump(
	row__ Root,
	bso::bool__ RootToo,
	xml::outfit__ Outfit,
	xml::encoding__ Encoding,
	txf::text_oflow__ &Flow ) const
{
	epeios::size__ ChildAmount = 0;
ERRProlog
	xml::writer Writer;
	buffer Buffer;
ERRBegin
	Buffer.Init( Nodes );

	Writer.Init( Flow, Outfit, Encoding );

	ChildAmount = Dump( Root, RootToo, Writer );
ERRErr
ERREnd
ERREpilog
	return ChildAmount;
}


void rgstry::registry_::_Delete( const rows_ &Rows )
{
	epeios::row__ Row = Rows.First();

	while ( Row != NONE ) {
		_Delete( Rows( Row ) );

		Row = Rows.Next( Row );
	}
}

const value_ &rgstry::registry_::GetValue(
	const path_ &Path,
	row__ Row,
	bso::bool__ *Missing,
	buffer &Buffer ) const	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
{
	static value Empty;
	const value_ *Result = &Empty;
	row__ ResultRow = NONE;

	if ( *Missing )
		return *Result;

	Empty.Init();
	Buffer.Init( Nodes );

	if ( ( ResultRow = _Search( Path, Row ) ) != NONE )
		Result = &Buffer( ResultRow ).Value;
	else
		*Missing = true;

	return *Result;
}

const value_ &rgstry::registry_::GetValue(
	const str::string_ &PathString,
	row__ Row,
	bso::bool__ *Missing,
	buffer &Buffer,
	epeios::row__ *PathErrorRow ) const	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
{
	static value Empty;
	const value_ *Result = &Empty;
ERRProlog
	path Path;
ERRBegin
	Path.Init();
	Empty.Init();

	if ( *Missing )
		ERRReturn;

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		Result = &GetValue( Path, Row, Missing, Buffer );
	else
		*Missing = true;
ERRErr
ERREnd
ERREpilog
	return *Result;
}

bso::bool__ rgstry::registry_::GetValues(
	const path_ &Path,
	row__ Row,
	values_ &Values ) const
{
	bso::bool__ Exists = false;
ERRProlog
	cursor__ Cursor = NONE;
	buffer Buffer;
	rows ResultRows;
ERRBegin
	Buffer.Init( Nodes );
	ResultRows.Init();

	_Search( Path, Path.First(), Row, ResultRows );

	if ( ResultRows.Amount() != 0 ) {
		Exists = true;
		Cursor = ResultRows.First();

		while ( Cursor != NONE ) {
			Values.Append( Buffer( ResultRows( Cursor ) ).Value );

			Cursor = ResultRows.Next( Cursor );
		}
	}
ERRErr
ERREnd
ERREpilog
	return Exists;
}

bso::bool__ rgstry::registry_::GetValues(
	const str::string_ &PathString,
	row__ ParentRow,
	values_ &Values,
	epeios::row__ *PathErrorRow ) const
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		Exists = GetValues( Path, ParentRow, Values );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

bso::bool__ rgstry::registry_::Delete(
	const str::string_ &PathString,
	row__ Row,
	epeios::row__ *PathErrorRow )
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		Exists = Delete( Path, Row );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

row__ rgstry::Parse(
	xtf::extended_text_iflow__ &XFlow,
	const xpp::criterions___ &Criterions,
	registry_ &Registry,
	row__ &Root,
	xpp::context___ &Context )
{
	row__ Target = NONE;
ERRProlog
	callback___ Callback( Registry );
	xpp::preprocessing_iflow___ PFlow;
	xtf::extended_text_iflow__ PXFlow;
ERRBegin
	Callback.Init( Root );

	XFlow.UndelyingFlow().EOFD( XTF_EOXT );

	PFlow.Init( XFlow, xpp::criterions___( Criterions.Directory, Criterions.CypherKey, Criterions.IsNamespaceDefined() ? Criterions.Namespace : str::string( DEFAULT_NAMESPACE ) ) );
	PXFlow.Init( PFlow );

	switch ( xml::Parse( PXFlow, xml::ehReplace, Callback ) ) {
	case xml::sOK:
		Target = Callback.GetTarget();
		Root = Callback.GetRoot();
		break;
	case xml::sUnexpectedEOF:
		Root = NONE;
		PFlow.GetContext( Context );
		break;
	default:
		// Puisque l'on passe par le préprocesseur, si une erreur est rencontrée, xml::Parse(...)' ne peut normalement retourner que 'xml::sUndexpectedEOF'.
		ERRc();
		break;
	}
ERRErr
ERREnd
ERREpilog
	return Target;
}
#if 1
const value_ &rgstry::overloaded_registry___::GetValue(
	const str::string_ &PathString,
	bso::bool__ *Missing,
	buffer &Buffer,
	epeios::row__ *PathErrorRow )const // Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
{
	static value Empty;
	const value_ *Result = &Empty;
ERRProlog
	row__ Row = NONE;
	path Path;
ERRBegin
	if ( *Missing )
		ERRReturn;

	Path.Init();
	Empty.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) ) {
		*Missing = true;
		ERRReturn;
	}

	if ( Local.Registry != NULL )
		Result = &Local.Registry->GetValue( Path, Local.Root, Missing, Buffer );
	else
		*Missing = true;

	if ( *Missing ) {
		*Missing = false;

		if ( Global.Registry != NULL )
			Result = &Global.Registry->GetValue( Path, Global.Root, Missing, Buffer );
	}
ERRErr
ERREnd
ERREpilog
	return *Result;
}

bso::bool__ rgstry::overloaded_registry___::GetValues(
	const str::string_ &PathString,
	values_ &Values,
	epeios::row__ *PathErrorRow ) const
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		ERRReturn;

	if ( Local.Registry != NULL )
		Exists = Local.Registry->GetValues( Path, Local.Root, Values );

	Exists = Global.Registry->GetValues( Path, Global.Root, Values ) || Exists;
ERRErr
ERREnd
ERREpilog
	return Exists;
}

bso::bool__ rgstry::overloaded_registry___::Exists(
	const str::string_ &PathString,
	epeios::row__ *PathErrorRow ) const
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( BuildPath_( PathString, Path, PathErrorRow ) )
		if ( Local.Registry != NULL )
			Exists = Local.Registry->Exists( Path, Local.Root );
	
	Exists = Exists || Global.Registry->Exists( Path, Global.Root );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

void rgstry::overloaded_registry___::Search(
	const str::string_ &RawPath,
	row__ &GlobalRow,
	row__ &LocalRow,
	epeios::row__ *PathErrorRow ) const
{
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( BuildPath_( RawPath, Path, PathErrorRow ) )
		Search( Path, GlobalRow, LocalRow );
ERRErr
ERREnd
ERREpilog
}
#endif
const value_ &rgstry::multi_level_registry_::GetValue(
	const str::string_ &PathString,
	bso::bool__ *Missing,
	buffer &Buffer,
	epeios::row__ *PathErrorRow ) const	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
{
	static value Empty;
	const value_ *Result = &Empty;
ERRProlog
	level__ Level = NONE;
	path Path;
ERRBegin
	if ( *Missing )
		ERRReturn;

	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		ERRReturn;

	Level = Entries.Last();

	while ( Level != NONE ) {
		*Missing = false;

		Result = &GetValue( Level, Path, Missing, Buffer );

		if ( *Missing  )
			Level = Entries.Previous( Level );
		else
			Level = NONE;
	}
ERRErr
ERREnd
ERREpilog
	return *Result;
}

bso::bool__ rgstry::multi_level_registry_::GetValue(
	const str::string_ &PathString,
	value_ &Value,
	epeios::row__ *PathErrorRow ) const	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
{
	bso::bool__ Found = false;
ERRProlog
	level__ Level = NONE;
	path Path;
ERRBegin
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		ERRReturn;

	Level = Entries.Last();

	while ( ( Level != NONE ) && ( !Found ) ) {
		Found = GetValue( Level, Path, Value );

		Level = Entries.Previous( Level );
	}
ERRErr
ERREnd
ERREpilog
	return Found;
}


bso::bool__ rgstry::multi_level_registry_::GetValues(
	const str::string_ &PathString,
	values_ &Values,
	epeios::row__ *PathErrorRow ) const
{
	bso::bool__ Found = false;
ERRProlog
	level__ Level = NONE;
	path Path;
ERRBegin
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		ERRReturn;

	Level = Entries.Last();

	while ( Level != NONE ) {
		Found |= GetValues( Level, Path, Values );

		Level = Entries.Previous( Level );
	}
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ rgstry::multi_level_registry_::SetValue(
	const str::string_ &PathString,
	const value_ &Value,
	epeios::row__ *PathErrorRow )
{
	bso::bool__ Set = false;
ERRProlog
	value CurrentValue;
	epeios::row__ LocalPathErrorRow = NONE;
ERRBegin
	CurrentValue.Init();

	if ( !GetValue( PathString, CurrentValue, &LocalPathErrorRow ) ) {
		if ( LocalPathErrorRow != NONE ) {
			*PathErrorRow = LocalPathErrorRow;
			ERRReturn;
		}
	} else 
		if ( CurrentValue == Value )
			ERRReturn;

	SetValue( Entries.Last(), PathString, Value, PathErrorRow );

	Set = true;
ERRErr
ERREnd
ERREpilog
	return Set;
}

bso::bool__ rgstry::multi_level_registry_::Delete(
	const str::string_ &PathString,
	epeios::row__ *PathErrorRow )
{
	bso::bool__ Deleted = false;
ERRProlog
	level__ Level = NONE;
	path Path;
ERRBegin
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		ERRReturn;

	Level = Entries.Last();

	while ( Level != NONE ) {
		Deleted |= Delete( Path, Level );

		Level = Entries.Previous( Level );
	}
ERRErr
ERREnd
ERREpilog
	return Deleted;
}


bso::bool__ rgstry::multi_level_registry_::MoveTo(
	const str::string_ &Path,
	level__ Level )
{
	bso::bool__ Moved = false;
ERRProlog
	str::string Value;
	str::string LevelValue;
ERRBegin
	Value.Init();

	if ( GetValue( Path, Value ) ) {
		LevelValue.Init();

		if ( GetValue( Level, Path, LevelValue ) || ( Value != LevelValue ) ) {
			Delete( Path );
			SetValue( Level, Path, Value );

			Moved = true;
		}
	}
ERRErr
ERREnd
ERREpilog
	return Moved;
}

row__ rgstry::multi_level_registry_::Search(
	const str::string_ &PathString,
	epeios::row__ *PathErrorRow ) const
{
	row__ Row = NONE;
ERRProlog
	level__ Level = NONE;
	path Path;
ERRBegin
	Path.Init();

	if ( !BuildPath_( PathString, Path, PathErrorRow ) )
		ERRReturn;

	Level = Entries.Last();

	while ( ( Level != NONE ) && ( Row == NONE ) ) {
		Row = Search( Level, Path );

		Level = Entries.Previous( Level );
	}
ERRErr
ERREnd
ERREpilog
	return Row;
}

status__ rgstry::FillRegistry(
	xtf::extended_text_iflow__ &XFlow,
	const xpp::criterions___ &Criterions,
	const char *RootPath,
	rgstry::registry_ &Registry,
	rgstry::row__ &RegistryRoot,
	context___ &Context )
{
	epeios::row__ PathErrorRow = NONE;

	if ( rgstry::Parse( XFlow, Criterions, Registry, RegistryRoot, Context ) == NONE )
		return Context.Status = sParseError;

	if ( ( RootPath != NULL ) && ( *RootPath ) )
		if ( ( RegistryRoot = Registry.Search( str::string( RootPath ), RegistryRoot, &PathErrorRow ) ) == NONE )
			if ( PathErrorRow != NONE ) {
				Context.PathErrorRow = PathErrorRow;
				return Context.Status = sRootPathError;
			} else
				return Context.Status = sUnableToFindRootPath;
		else  if ( Registry.GetNature( RegistryRoot ) == nAttribute )
			return Context.Status = sRootPathError;


	return sOK;
}

status__ rgstry::FillRegistry(
	const char *FileName,
	const xpp::criterions___ &Criterions,
	const char *RootPath,
	rgstry::registry_ &Registry,
	rgstry::row__ &RegistryRoot,
	context___ &Context )
{
	status__ Status = s_Undefined;
ERRProlog
	flf::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
	FNM_BUFFER___ DirectoryBuffer;
ERRBegin
	if ( FFlow.Init( FileName, err::hUserDefined ) != fil::sSuccess ) {
		Status = Context.Status = sUnableToOpenFile;
		Context.FileName = FileName;
		ERRReturn;
	}

	XFlow.Init( FFlow );

	if ( Criterions.Directory.Amount() != 0 )
		ERRu();

	Status = FillRegistry( XFlow, xpp::criterions___( str::string( fnm::GetLocation( FileName, DirectoryBuffer ) ), Criterions.CypherKey, Criterions.Namespace ), RootPath, Registry, RegistryRoot, Context );

	if ( Status == sParseError )
		if ( Context.FileName.Amount() == 0 )
			Context.FileName = FileName;
ERRErr
ERREnd
ERREpilog
	return Status;
}

#define LIMIT 9

static bso::ubyte__ GetTagAmount_( const str::string_ &String )
{
	bso::ubyte__ Amount = 0;

	epeios::row__ Row = String.First();

	while ( ( Row != NONE ) && ( ( Row = String.Search( RGSTRY__TAG_MARKER, Row ) ) != NONE ) )
	{
		Row = String.Next( Row );

		if ( Row == NONE )
			ERRc();

		if ( String( Row ) != RGSTRY__TAG_MARKER )
			if ( Amount == LIMIT )
				ERRl();
			else
				Amount++;

		Row = String.Next( Row );
	}

	return Amount;
}

static void HandleTag_(
	bso::ubyte__ Indice,
	const char *Source,
	str::string_ &Target )
{
	while ( *Source ) {
		if ( *Source == RGSTRY__TAG_MARKER ) {
			Target.Append( '%' );

			Source++;

			if ( *Source != RGSTRY__TAG_MARKER ) {
				if ( Indice > LIMIT )
					ERRl();

				Target.Append( '0' + Indice++ );
			} else if ( *Source == 0 )
				ERRc();

		} 

		Target.Append( *Source++ );
	}
}

void rgstry::entry_::Init(
	const char *Path,
	const entry_ &Parent )
{
ERRProlog
	str::string Buffer;
ERRBegin
	S_.Parent = &Parent;
	this->Path.Init();

	if ( Path != NULL ) {
		Buffer.Init();
		GetParentPath_( parameters(), false, Buffer );
		HandleTag_( GetTagAmount_( Buffer ) + 1, Path, this->Path );
	}
ERRErr
ERREnd
ERREpilog
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class rgstrypersonnalization
: public rgstrytutor
{
public:
	rgstrypersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~rgstrypersonnalization( void )
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

static rgstrypersonnalization Tutor;

ttr_tutor &RGSTRYTutor = Tutor;
