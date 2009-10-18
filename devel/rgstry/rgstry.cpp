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

using namespace rgstry;

row__ rgstry::registry_::_FlatSearch(
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

row__ rgstry::registry_::_FlatSearch(
	nature__ Nature,
	const name_ &Name,
	const rows_ &NodeRows ) const
{
	buffer Buffer;
	epeios::row__ Cursor = NONE;
	row__ Row = NONE;

	Buffer.Init( Nodes );

	Row = _FlatSearch( Name, NodeRows, Cursor );

	while ( ( Row != NONE ) && ( Buffer( Row ).Nature() != Nature ) )
		Row = _FlatSearch( Name, NodeRows, Cursor );

	return Row;
}

row__ rgstry::registry_::_Search(
	const name_ &KeyName,
	const name_ &AttributeName,
	const value_ &AttributeValue,
	row__ ParentRow,
	epeios::row__ &Cursor ) const
{
	row__ Row = _Search( KeyName, ParentRow, Cursor );

	while ( ( Row != NONE ) && ( !_AttributeExists( AttributeName, AttributeValue, Row ) ) )
		Row = _Search( KeyName, ParentRow, Cursor );

	return Row;
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
	state__ State = s_Undefined;
	bso::char__ C;
	path_item Item;
	bso::bool__ KeyNameAsAttribute = false;
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

/*
row__ rgstry::registry_::_SearchPath(
	const path_ &Path,
	row__ &ForkRow,
	erow__ &AttributeEntryRow,
	epeios::row__ &PathRow,
	epeios::row__ &Cursor ) const
{
	ctn::E_CITEM( path_item_ ) Item;
	row__ Row = ForkRow;
	bso::bool__ ResetCursor = Cursor == NONE;
	epeios::row__ LocalPathRow = ( PathRow == NONE ? Path.First() : PathRow );

	AttributeEntryRow = NONE;

	Item.Init( Path );

	while ( ( LocalPathRow != NONE ) && ( Row != NONE ) ) {
		if ( Item( LocalPathRow ).TagName.Amount() != 0 ) {// '.../Name[@AttributeName='AttributeValue']/...'
			ForkRow = Row;
			PathRow = LocalPathRow;
			if ( ResetCursor )
				Cursor = NONE;
			Row = _SearchChild( Item( LocalPathRow ), Row, Cursor );	
		} else if ( ( AttributeEntryRow = _GetAttribute( Item( LocalPathRow ).AttributeName, Row ) ) == NONE ) // '.../@Name'
			Row = NONE;

		LocalPathRow = Path.Next( LocalPathRow );

#ifdef RGSTRY_DBG
			if ( ( AttributeEntryRow != NONE ) && ( LocalPathRow != NONE ) )
				ERRu();
#endif
	}

	return Row;
}
*/

row__ rgstry::registry_::_Search(
	const path_item_ &Item,
	row__ Row,
	epeios::row__ &Cursor ) const
{
	if ( Item.KeyName.Amount() != 0 )
		Row = _Search( Item, Row, Cursor );	// '.../KeyName[@AttributeName='AttributeValue']/...'
	else if ( Cursor == NONE ) {
		AttributeEntryRow = _GetAttribute( Item.AttributeName, Row );	// '.../@AttributeName'
		if ( AttributeEntryRow == NONE ) {
			AttributeEntryRow = *Row;	// Peu importe la valeur, pourvu qu'elle ce soit différente de 'NONE'.
			Row = NONE;	// Pour signaler que l'on n'a pas trouvé.
			// 'Row' == 'NONE 'et 'AttributeEntryRow' != 'NONE' signale que l'on a trouvé la clef, mais pas l'attribut.
		}
		Cursor = *Row;	// Peu importe la valeur, pourvu qu'elle ce soit différente de 'NONE'.
	} else
		Row = NONE;

	return Row;
}


row__ rgstry::registry_::_SearchPath(
	const path_ &Path,
	epeios::row__ PathRow,
	row__ Row,
	erow__ &AttributeEntryRow,
	rows_ &ResultKeys,
	erows_ &ResultAttributes ) const
{
	epeios::row__ CandidateRow = NONE;
	epeios::row__ Cursor = NONE;
	ctn::E_CITEM( path_item_ ) Item;
	row__ Result, ChildRow = NONE;
	bso::bool__ All = &ResultKeys != NULL;

	Item.Init( Path );

	Result = ChildRow = _SearchChild( Item( PathRow ), Row, AttributeEntryRow, Cursor );

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
		if ( AttributeEntryRow != NONE )
			ERRc();
#endif
		while ( ChildRow != NONE )  {
			AttributeEntryRow = NONE;
			Result = _SearchPath( Path, Path.Next( PathRow ), ChildRow, AttributeEntryRow, ResultKeys, ResultAttributes );

			if ( Result != NONE ) {
				if ( All ) {
					ChildRow = _SearchChild( Item( PathRow ), Row, AttributeEntryRow, Cursor );
				} else
					ChildRow = NONE;
			} else if ( AttributeEntryRow == NONE )	// Voir '_SearchChild()'.
				ChildRow = _SearchChild( Item( PathRow ), Row, AttributeEntryRow, Cursor );
			else
				ChildRow = NONE;	// pour sortir de la boucle.
		}

	} else if ( All ) {

		while ( ChildRow != NONE ) {
			ResultKeys.Append( ChildRow );
			ResultAttributes.Append( AttributeEntryRow );

			ChildRow = _SearchChild( Item( PathRow ), Row, AttributeEntryRow, Cursor );
		}

	}

	return Result;
}

row__ rgstry::registry_::_SearchPath(
	const path_ &Path,
	row__ Row,
	erow__ &AttributeEntryRow ) const
{
	epeios::row__ PathRow = NONE, Cursor = NONE;

	if ( Path.Amount() != 0 )
		return _SearchPath( Path, Path.First(), Row, AttributeEntryRow, *(rows_ *)NULL, *(erows_ *)NULL );
	else
		return NONE;
}

row__ rgstry::registry_::_SearchPath(
	const term_ &Term,
	row__ Row,
	erow__ &AttributeEntryRow,
	epeios::row__ &PathErrorRow ) const
{
ERRProlog
	epeios::row__ PathRow = NONE;
	path Path;
ERRBegin
	Path.Init();

	PathRow = BuildPath( Term, Path );

	if ( PathRow != NONE ) {
		Row = NONE;
		PathErrorRow = PathRow;
	} else
		Row = _SearchPath( Path, Row, AttributeEntryRow );

ERRErr
ERREnd
ERREpilog
	return Row;
}

row__ rgstry::registry_::CreatePath(
	const path_ &Path,
	row__ Row )
{
	bso::bool__ Exists = true;
	ctn::E_CITEM( path_item_ ) Item;
	epeios::row__ PathRow = Path.First();
	row__ Candidate = NONE;

	Item.Init( Path );

	while ( ( PathRow != NONE ) && Exists ) {
		Candidate = _SearchChild( Item( PathRow ), Row );

		if ( Candidate == NONE ) {
			Exists = false;

			Row = _CreateChild( Item( PathRow ), Row );
		} else
			Row = Candidate;

		PathRow = Path.Next( PathRow );
	}

	while ( PathRow != NONE ) {
		Row = _CreateChild( Item( PathRow ), Row );

		PathRow = Path.Next( PathRow );
	}

	return Row;
}

row__ rgstry::registry_::CreatePath(
	const term_ &Term,
	row__ Row,
	epeios::row__ &PathErrorRow )
{
ERRProlog
	epeios::row__ PathRow = NONE;
	path Path;
ERRBegin
	Path.Init();

	PathRow = BuildPath( Term, Path );

	if ( PathRow != NONE ) {
		Row = NONE;
		PathErrorRow = PathRow;
	} else
		Row = CreatePath( Path, Row );

ERRErr
ERREnd
ERREpilog
	return Row;
}


row__ rgstry::registry_::SetPathValue(
	const term_ &Term,
	const value_ &Value,
	row__ Row,
	epeios::row__ &PathErrorRow )
{
ERRProlog
	epeios::row__ PathRow = NONE;
	path Path;
ERRBegin
	Path.Init();

	PathRow = BuildPath( Term, Path );

	if ( PathRow != NONE ) {
		Row = NONE;
		PathErrorRow = PathRow;
	} else
		Row = SetPathValue( Path, Value, Row );

ERRErr
ERREnd
ERREpilog
	return Row;
}

#ifdef RGSTRY_PREPROCESSOR_NAMESPACE
#	define NAMESPACE RGSTRY_PREPROCESSOR_NAMESPACE
#else
#	define NAMESPACE "xpp"
#endif

class callback___
: public xml::callback__
{
private:
	registry_ &_Registry;
	row__ _Root, _Current;
protected:
	virtual bso::bool__ XMLProcessingInstruction( const xml::dump_ & )
	{
		return true;
	}
	virtual bso::bool__ XMLStartTag(
		const str::string_ &TagName,
		const xml::dump_ &Dump )
	{
		if ( _Current == NONE ) {
			if ( _Root == NONE )
				_Root = _Current = _Registry.CreateNode( TagName );
			else
				ERRc();
		} else
			_Current = _Registry.AddChild( TagName, _Current );

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

		_Current = _Registry.GetParent( _Current );

		if ( ( _Current == NONE ) && ( _Root == NONE ) )
			return false;

		return true;
	}
public:
	callback___( registry_ &Registry )
	: _Registry( Registry )
	{
		_Root = _Current = NONE;
	}
	void Init( row__ Root )
	{
		_Root = _Current = Root;
	}
	row__ GetRoot( void ) const
	{
		return _Root;
	}
};

void rgstry::registry_::_Delete( const erows_ &Rows )
{
	epeios::row__ Row = Rows.First();

	while ( Row != NONE ) {
		_Delete( Rows( Row ) );

		Row = Rows.Next( Row );
	}
}

void rgstry::registry_::_Delete( row__ Row )
{
	const node_ &Node = Nodes( Row );

	_Delete( Node.Attributes );

	if ( Node.NameRow() != NONE )
		Terms.Delete( Node.NameRow() );

	if ( Node.ValueRow() != NONE )
		Terms.Delete( Node.ValueRow() );

	_Delete( Node.Children );

	Nodes.Delete( Row );
}

bso::bool__ rgstry::registry_::PathExists(
	const term_ &PathString,
	row__ ParentRow,
	epeios::row__ &PathRow ) const
{
	bso::bool__ Result = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	PathRow = BuildPath( PathString, Path );

	if ( PathRow != NONE )
		Result = PathExists( Path, ParentRow );
ERRErr
ERREnd
ERREpilog
	return Result;
}

void rgstry::registry_::_DumpAttributes(
	const erows_ &Rows,
	xml::writer_ &Writer ) const
{
	epeios::row__ Row = Rows.First();
	term_buffer Name, Value;

	Name.Init( Terms );
	Value.Init( Terms );

	while ( Row != NONE ){
		Writer.PutAttribute( _GetName( Rows( Row ), Name ), _GetValue( Rows( Row ), Value ) );

		Row = Rows.Next( Row );
	}

}

epeios::size__ rgstry::registry_::_Dump(
	row__ Root,
	bso::bool__ RootToo,
	xml::writer_ &Writer,
	term_buffer &TermBuffer,
	node_buffer &NodeBuffer ) const
{
	epeios::size__ ChildAmount = 0;
ERRProlog
	rows Children;
	epeios::row__ Row = NONE;
ERRBegin
	Children.Init();
	Children = _GetNode( Root, NodeBuffer ).Children;

	if ( RootToo )
		_DumpNode( Root, Writer, TermBuffer, NodeBuffer );
	
	Row = Children.First();

	while ( Row != NONE ) {
		_Dump( Children( Row ), true, Writer, TermBuffer, NodeBuffer );

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
	bso::bool__ Indent,
	txf::text_oflow__ &Flow ) const
{
	epeios::size__ ChildAmount = 0;
ERRProlog
	xml::writer Writer;
	term_buffer TermBuffer;
	node_buffer NodeBuffer;
ERRBegin
	TermBuffer.Init( Terms );
	NodeBuffer.Init( Nodes );

	Writer.Init( Flow, Indent );

	ChildAmount = _Dump( Root, RootToo, Writer, TermBuffer, NodeBuffer );
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

const value_ &rgstry::registry_::GetPathValue_(
	const path_ &Path,
	row__ ParentRow,
	bso::bool__ *Missing,
	term_buffer &Buffer ) const	// Nota : ne met 'Exists' à 'false' que lorque 'Path' n'existe pas. Le laisse inchangé sinon.
{
	static value Empty;
	const value_ *Result = &Empty;
ERRProlog
	row__ Row = NONE;
	erow__ AttributeEntryRow = NONE;
ERRBegin
	Empty.Init();

	if ( ( Row = _SearchPath( Path, ParentRow, AttributeEntryRow ) ) != NONE )
		if ( AttributeEntryRow == NONE )
			Result = &GetValue( Row, Buffer );
		else
			Result = &GetValue( AttributeEntryRow, Buffer );
	else {
		*Missing = true;
	}
ERRErr
ERREnd
ERREpilog
	return *Result;
}

const value_ &rgstry::registry_::GetPathValue_(
	const term_ &PathString,
	row__ ParentRow,
	bso::bool__ *Missing,
	epeios::row__ &PathErrorRow,
	term_buffer &Buffer ) const	// Nota : ne met 'Exists' à 'false' que lorque 'Path' n'existe pas. Le laisse inchangé sinon.
{
	static value Empty;
	const value_ *Result = &Empty;
ERRProlog
	row__ Row = NONE;
	path Path;
	erow__ AttributeEntryRow = NONE;
ERRBegin
	Path.Init();
	Empty.Init();

	if ( ( PathErrorRow = BuildPath( PathString, Path ) ) != NONE ) {
		*Missing = true;
		ERRReturn;
	}

	Result = &GetPathValue_( Path, ParentRow, Missing, Buffer );
ERRErr
ERREnd
ERREpilog
	return *Result;
}

bso::bool__ rgstry::registry_::GetPathValues(
	const path_ &Path,
	row__ ParentRow,
	values_ &Values ) const
{
	bso::bool__ Exists = false;
ERRProlog
	epeios::row__ Row = NONE;
	erow__ AttributeEntryRow = NONE;
	term_buffer Buffer;
	rows TagRows;
	erows AttributeRows;
ERRBegin
	TagRows.Init();
	AttributeRows.Init();

	_SearchPath( Path, Path.First(), ParentRow, AttributeEntryRow, TagRows, AttributeRows );

#ifdef RGSTRY_DBG
	if ( TagRows.Amount() != AttributeRows.Amount() )
		ERRc();
#endif

	if ( TagRows.Amount() != 0 ) {
		Exists = true;
		Row = TagRows.First();

		while ( Row != NONE ) {
			if ( AttributeRows( Row ) == NONE )
				Values.Append( GetValue( TagRows( Row ), Buffer ) );
			else
				Values.Append( GetValue( AttributeRows( Row ), Buffer ) );

			Row = AttributeRows.Next( Row );
		}
	}
ERRErr
ERREnd
ERREpilog
	return Exists;
}

bso::bool__ rgstry::registry_::GetPathValues(
	const term_ &PathString,
	row__ ParentRow,
	epeios::row__ &PathErrorRow,
	values_ &Values ) const
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( ( PathErrorRow = BuildPath( PathString, Path ) ) != NONE )
		ERRReturn;

	Exists = GetPathValues( Path, ParentRow, Values );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

bso::bool__ rgstry::registry_::DeletePath(
	const term_ &PathString,
	row__ ParentRow,
	epeios::row__ &PathErrorRow )
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( ( PathErrorRow = BuildPath( PathString, Path ) ) != NONE )
		ERRReturn;

	Exists = DeletePath( Path, ParentRow );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

row__ rgstry::Parse(
	xtf::extended_text_iflow__ &Flow,
	const str::string_ &Directory,
	registry_ &Registry,
	row__ Root,
	xml::extended_status__ &Status,
	xcoord_ &ErrorCoord )
{
ERRProlog
	callback___ Callback( Registry );
	str::string ErrorFileName;
ERRBegin
	Callback.Init( Root );

	ErrorFileName.Init();

	if ( ( Status = xml::ExtendedParse( Flow, str::string( NAMESPACE ), Callback, Directory, ErrorFileName ) ) == xml::xsOK )
		Root = Callback.GetRoot();
	else {
		Root = NONE;
		ErrorCoord.File = ErrorFileName;
		ErrorCoord.SetCoord( Flow.Coord() );
	}
ERRErr
ERREnd
ERREpilog
	return Root;
}

const value_ &rgstry::overloaded_registry___::GetPathValue_(
	const term_ &PathString,
	bso::bool__ *Missing,
	epeios::row__ &PathErrorRow,
	term_buffer &Buffer ) const	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Le laisse inchangé sinon.
{
	static value Empty;
	const value_ *Result = &Empty;
ERRProlog
	row__ Row = NONE;
	path Path;
	erow__ AttributeEntryRow = NONE;
	bso::bool__ LocalMissing = false;
ERRBegin
	Path.Init();
	Empty.Init();

	if ( ( PathErrorRow = BuildPath( PathString, Path ) ) != NONE ) {
		*Missing = true;
		ERRReturn;
	}

	if ( Local.Registry != NULL )
		Result = &Local.Registry->GetPathValue_( Path, Local.Root, &LocalMissing, Buffer );
	else
		LocalMissing = true;

	if ( LocalMissing )
		if ( Global.Registry != NULL )
			Result = &Global.Registry->GetPathValue_( Path, Global.Root, Missing, Buffer );
ERRErr
ERREnd
ERREpilog
	return *Result;
}

bso::bool__ rgstry::overloaded_registry___::GetPathValues(
	const term_ &PathString,
	epeios::row__ &PathErrorRow,
	values_ &Values ) const
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
	erow__ AttributeEntryRow = NONE;
ERRBegin
	Path.Init();

	if ( ( PathErrorRow = BuildPath( PathString, Path ) ) != NONE )
		ERRReturn;

	if ( Local.Registry != NULL )
		Exists = Local.Registry->GetPathValues( Path, Local.Root, Values );

	Exists = Global.Registry->GetPathValues( Path, Global.Root, Values ) || Exists;
ERRErr
ERREnd
ERREpilog
	return Exists;
}

bso::bool__ rgstry::overloaded_registry___::PathExists(
	const rgstry::term_ &PathString,
	epeios::row__ &PathRow ) const
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	PathRow = BuildPath( PathString, Path );

	if ( PathRow != NONE )
		if ( Local.Registry != NULL )
			Exists = Local.Registry->PathExists( Path, Local.Root );
	
	Exists = Exists || Global.Registry->PathExists( Path, Global.Root );
ERRErr
ERREnd
ERREpilog
	return Exists;
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
