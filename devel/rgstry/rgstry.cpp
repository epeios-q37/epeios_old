/*
	'rgstry' library by Claude SIMON (csimon@epeios.org)
	Requires the 'rgstry' header file ('rgstry.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
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

#include "lxmlpr.h"

using namespace rgstry;

erow__ rgstry::registry_::_SearchEntry(
	const term_ &Name,
	const erows_ &EntryRows ) const
{
	buffer Buffer;
	epeios::row__ Row = EntryRows.First();

	Buffer.Init( Terms );

	while ( ( Row != NONE ) && ( _GetName( EntryRows( Row ), Buffer ) != Name ) )
		Row = EntryRows.Next( Row );

	if ( Row != NONE )
		return EntryRows( Row );
	else
		return NONE;
}

nrow__ rgstry::registry_::_SearchNode(
	const term_ &Name,
	const nrows_ &NodeRows,
	epeios::row__ &Cursor ) const
{
	node_buffer NodeBuffer;
	buffer NameBuffer;

	epeios::row__ &Row = Cursor;
	
	if ( Row == NONE )
		Row = NodeRows.First();
	else
		Row = NodeRows.Next( Row );

	NodeBuffer.Init( Nodes );
	NameBuffer.Init( Terms );

	while ( ( Row != NONE ) && ( _GetName( NodeRows( Row ), NameBuffer, NodeBuffer ) != Name ) )
		Row = NodeRows.Next( Row );

	if ( Row != NONE )
		return NodeRows( Row );
	else
		return NONE;

}

nrow__ rgstry::registry_::_SearchNode(
	const term_ &NodeName,
	const term_ &AttributeName,
	const term_ &AttributeValue,
	nrow__ ParentNodeRow,
	epeios::row__ &Cursor ) const
{
	nrow__ NodeRow = _SearchNode( NodeName, ParentNodeRow, Cursor );

	while ( ( NodeRow != NONE ) && ( !_AttributeExists( AttributeName, AttributeValue, NodeRow ) ) )
		NodeRow = _SearchNode( NodeName, ParentNodeRow, Cursor );

	return NodeRow;
}

const term_ &rgstry::registry_::GetCompleteName(
	nrow__ NodeRow,
	term_ &Term,
	const char *Separator ) const
{
	buffer Buffer;
	node_buffer NodeBuffer;

	Buffer.Init( Terms );
	NodeBuffer.Init( Nodes );

	if ( NodeRow != NONE ) {
		Term.Append( _GetName( NodeRow, Buffer, NodeBuffer ) );
		NodeRow = GetParent( NodeRow );
	}

	while ( NodeRow != NONE ) {
		Term.Insert( Separator, 0 );
		Term.Insert( _GetName( NodeRow, Buffer, NodeBuffer ), 0 );

		NodeRow = GetParent( NodeRow );
	}

	return Term;
}

enum state__ {
	sTagName,
	sAttributeName,
	sAttributeValue,
	s_amount,
	s_Undefined
};

epeios::row__ rgstry::BuildPath(
	const term_ &Term,
	path_ &Path )
{
	epeios::row__ TermRow = NONE;
ERRProlog
	bso::bool__ Continue = true;
	state__ State = s_Undefined;
	bso::char__ C;
	path_item Item;
	bso::bool__ TagNameAsAttribute = false;
ERRBegin
	TermRow = Term.First();

	Item.Init();

	Continue = TermRow != NONE;

	State = sTagName;

	while ( Continue ) {
		C = Term( TermRow );
		switch ( State ) {
		case sTagName:
			switch ( C ) {
			case '=':
			case '"':
				Continue = false;
				break;
			case '[':
				if ( TagNameAsAttribute )
					Continue = false;
				else if ( Item.TagName.Amount() == 0 )
					Continue = false;
				State = sAttributeName;
				Item.AttributeName.Init();
				break;
			case '/':
				if ( TagNameAsAttribute )
					Continue = false;
				else if ( Item.TagName.Amount() == 0 )
					Continue = false;
				else
					Path.Append( Item );
				Item.Init();
				break;
			case '@':
				if ( Item.TagName.Amount() != 0 )
					Continue = false;
				else
					TagNameAsAttribute = true;
				break;
			default:
				Item.TagName.Append( C );
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
					if ( ( TermRow = Term.Next( TermRow ) ) == NONE )
						Continue = false;
					else if ( Term( TermRow ) != '"' )
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
					if ( ( TermRow = Term.Next( TermRow ) ) == NONE )
						Continue = false;
					else if ( Term( TermRow ) != ']' )
						Continue = false;
					else if ( ( TermRow = Term.Next( TermRow ) ) == NONE )
						Continue = false;
					else if ( Term( TermRow ) != '/' )
						Continue = false;
					Path.Append( Item );
					Item.Init();
					State = sTagName;
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
			TermRow = Term.Next( TermRow );

		if ( TermRow == NONE )
			Continue = false;
	}

	if ( Item.TagName.Amount() != 0 ) {
		if ( TagNameAsAttribute ) {
			Item.AttributeName = Item.TagName;
			Item.TagName.Init();
		}

		Path.Append( Item );
	}
ERRErr
ERREnd
ERREpilog
	return TermRow;
}

nrow__ rgstry::registry_::SearchPath(
	const path_ &Path,
	nrow__ Row ) const
{
	ctn::E_CITEM( path_item_ ) Item;
	epeios::row__ PathRow = Path.First();

	Item.Init( Path );

	while ( ( PathRow != NONE ) && ( Row != NONE ) ) {
		Row = _SearchChild( Item( PathRow ), Row );

		PathRow = Path.Next( PathRow );
	}

	return Row;
}



nrow__ rgstry::registry_::SearchPath(
	const term_ &Term,
	nrow__ Row,
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
		Row = SearchPath( Path, Row );

ERRErr
ERREnd
ERREpilog
	return Row;
}

nrow__ rgstry::registry_::CreatePath(
	const path_ &Path,
	nrow__ Row )
{
	bso::bool__ Exists = true;
	ctn::E_CITEM( path_item_ ) Item;
	epeios::row__ PathRow = Path.First();
	nrow__ Candidate = NONE;

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

nrow__ rgstry::registry_::CreatePath(
	const term_ &Term,
	nrow__ Row,
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


nrow__ rgstry::registry_::SetPathValue(
	const term_ &Term,
	const term_ &Value,
	nrow__ Row,
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

class callback__
: public lxmlpr::callback__
{
private:
	registry_ &Registry_;
	nrow__ Root_;
	nrow__ Current_;
protected:
	virtual void LXMLPRTag(
		const str::string_ &Prefix,
		const str::string_ &Name )
	{
		if ( Current_ == NONE ) {
			if ( Root_ != NONE )
				Root_ = Current_ = Registry_.AddChild( Name, Root_ );
			else
				Root_ = Current_ = Registry_.CreateNode( Name );
		} else
			Current_ = Registry_.AddChild( Name, Current_ );
	}
	virtual void LXMLPRValue( const str::string_ &Value )
	{
		Registry_.SetValue( Value, Current_ );
	}
	virtual void LXMLPRAttribute(
		const str::string_ &Prefix,
		const str::string_ &Name,
		const str::string_ &Value )
	{
		Registry_.AddAttribute( Name, Value, Current_ );
	}
	virtual void LXMLPRTagClosed( void )
	{
		Current_ = Registry_.GetParent( Current_ );
	}
public:
	callback__(
		registry_ &Registry,
		nrow__ Root )
	: Registry_( Registry )
	{
		Current_ = NONE;
		Root_ = Root;
	}
	nrow__ GetRoot( void ) const
	{
		if ( Current_ != Registry_.GetParent( Root_ ) )
			ERRf();

		return Root_;
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

void rgstry::registry_::_Delete( nrow__ Row )
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
	nrow__ ParentRow,
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

void rgstry::registry_::_Delete( const nrows_ &Rows )
{
	epeios::row__ Row = Rows.First();

	while ( Row != NONE ) {
		_Delete( Rows( Row ) );

		Row = Rows.Next( Row );
	}
}

nrow__ rgstry::Parse(
	xtf::extended_text_iflow__ &Flow,
	registry_ &Registry,
	nrow__ Root,
	xtf::location__ &ErrorLine,
	xtf::location__ &ErrorColumn )
{
ERRProlog
	callback__ Callback( Registry, Root );
ERRBegin
	if ( lxmlpr::Parse( Flow, Callback, ErrorLine, ErrorColumn ) )
		Root = Callback.GetRoot();
	else
		Root = NONE;
ERRErr
ERREnd
ERREpilog
	return Root;
}

const term_ &rgstry::overloaded_registry___::GetPathValue(
	const term_ &PathString,
	bso::bool__ &Exists,
	buffer &Buffer ) const	// Nota : ne met 'Exists' à 'false' que lorque 'Path' n'existe pas. Le laisse inchangé sinon.
{
	static term Empty;
	const term_ *Result = 0;
ERRProlog
	nrow__ Row = NONE;
	path Path;
ERRBegin
	Path.Init();
	Empty.Init();

	if ( BuildPath( PathString, Path ) != NONE ) {
		Exists = false;
		ERRReturn;
	}

	if ( ( Row = Local.Registry->SearchPath( Path, Local.Root ) ) != NONE )
		Result = &Local.Registry->GetValue( Row, Buffer );
	else if ( ( Row = Global.Registry->SearchPath( Path, Global.Root ) ) != NONE )
		Result = &Global.Registry->GetValue( Row, Buffer );
	else {
		Exists = false;
		Result = &Empty;
	}
ERRErr
ERREnd
ERREpilog
	return *Result;
}

bso::bool__ rgstry::overloaded_registry___::PathExists(
	const rgstry::term_ &PathString,
	epeios::row__ &PathRow ) const
{
	bso::bool__ Result = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	PathRow = BuildPath( PathString, Path );

	if ( PathRow != NONE )
		Result = Local.Registry->PathExists( Path, Local.Root ) || Global.Registry->PathExists( Path, Global.Root );
ERRErr
ERREnd
ERREpilog
	return Result;
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
