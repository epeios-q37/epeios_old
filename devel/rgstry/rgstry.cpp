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
	const name_ &Name,
	const erows_ &EntryRows ) const
{
	buffer Buffer;
	epeios::row__ Row = EntryRows.First();

	Buffer.Init( Contents );

	while ( ( Row != NONE ) && ( _GetName( EntryRows( Row ), Buffer ) != Name ) )
		Row = EntryRows.Next( Row );

	if ( Row != NONE )
		return EntryRows( Row );
	else
		return NONE;
}

nrow__ rgstry::registry_::_SearchNode(
	const name_ &Name,
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
	NameBuffer.Init( Contents );

	while ( ( Row != NONE ) && ( _GetName( NodeRows( Row ), NameBuffer, NodeBuffer ) != Name ) )
		Row = NodeRows.Next( Row );

	if ( Row != NONE )
		return NodeRows( Row );
	else
		return NONE;

}

nrow__ rgstry::registry_::_SearchNode(
	const name_ &NodeName,
	const name_ &AttributeName,
	const value_ &AttributeValue,
	nrow__ ParentNodeRow,
	epeios::row__ &Cursor ) const
{
	nrow__ NodeRow = _SearchNode( NodeName, ParentNodeRow, Cursor );

	while ( ( NodeRow != NONE ) && ( !_AttributeExists( AttributeName, AttributeValue, NodeRow ) ) )
		NodeRow = _SearchNode( NodeName, ParentNodeRow, Cursor );

	return NodeRow;
}

const content_ &rgstry::registry_::GetCompleteName(
	nrow__ NodeRow,
	content_ &Content ) const
{
	buffer Buffer;
	node_buffer NodeBuffer;

	Buffer.Init( Contents );
	NodeBuffer.Init( Nodes );

	if ( NodeRow != NONE ) {
		Content.Append( _GetName( NodeRow, Buffer, NodeBuffer ) );
		NodeRow = GetParent( NodeRow );
	}

	while ( NodeRow != NONE ) {
		Content.Insert( "::", 0 );
		Content.Insert( _GetName( NodeRow, Buffer, NodeBuffer ), 0 );

		NodeRow = GetParent( NodeRow );
	}

	return Content;
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
				ERRf();

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
	virtual void LXMLPRError(
		lxmlpr::location__ Line,
		lxmlpr::location__ Column )
	{
		ERRf();
	}
public:
	callback__( registry_ &Registry )
	: Registry_( Registry )
	{
		Current_ = NONE;
		Root_ = NONE;
	}
	nrow__ GetRoot( void ) const
	{
		if ( ( Current_ != NONE ) )
			ERRf();

		return Root_;
	}
};

nrow__ rgstry::Parse(
	xtf::extended_text_iflow__ &Flow,
	registry_ &Registry )
{
	nrow__ Root = NONE;
ERRProlog
	callback__ Callback( Registry );
	lxmlpr::parser Parser;	
ERRBegin
	Parser.Init();

	Parser.Parse( Flow, Callback );

	Root = Callback.GetRoot();
ERRErr
ERREnd
ERREpilog
	return Root;
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
