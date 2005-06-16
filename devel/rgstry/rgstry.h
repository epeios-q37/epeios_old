/*
	Header for the 'rgstry' library by Claude SIMON (csimon@epeios.org)
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

#ifndef RGSTRY__INC
#define RGSTRY__INC

#define RGSTRY_NAME		"RGSTRY"

#define	RGSTRY_VERSION	"$Revision$"

#define RGSTRY_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &RGSTRYTutor;

#if defined( XXX_DBG ) && !defined( RGSTRY_NODBG )
#define RGSTRY_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D ReGISTRY 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "str.h"
#include "lstbch.h"
#include "lstctn.h"
#include "xtf.h"

namespace rgstry {

	typedef str::string_	content_;
	typedef str::string		content;

	typedef content_		name_;
	typedef content			name;

	typedef content_		value_;
	typedef content_		value;

	E_ROW( crow__ );		// content row.

	typedef lstctn::E_LXMCONTAINERt_( content_, crow__ )	contents_;
	E_AUTO( contents )

	typedef ctn::E_CMITEMt( content_, crow__ )	buffer;

	struct entry__ {
		crow__ NameRow;
		crow__ ValueRow;
		void operator()( void )
		{
			NameRow = NONE;
			ValueRow = NONE;
		}
		entry__( void )
		{
			operator()();
		}
	};

	E_ROW( erow__ );	// Entry row.
	typedef bch::E_BUNCH_( erow__ ) erows_;
	E_AUTO( erows );

	typedef lstbch::E_LBUNCHt_( entry__, erow__ ) entries_;
	E_AUTO( entries )

	E_ROW( nrow__ );	// Node row.
	typedef bch::E_BUNCH_( nrow__ ) nrows_;
	E_AUTO( nrows );

	class node_ {
	public:
		struct s {
			nrow__ Parent;
			crow__ NameRow;
			crow__ ValueRow;
			erows_::s Attributes;
			nrows_::s Children;
		} &S_;
		erows_ Attributes;
		nrows_ Children;
		node_( s &S )
		: S_( S ),
		  Attributes( S.Attributes ),
		  Children( S.Children )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Parent = NONE;
			S_.NameRow = NONE;
			S_.ValueRow = NONE;

			Attributes.reset( P );
			Children.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Attributes.plug( MM );
			Children.plug( MM );
		}
		node_ &operator =( const node_ &N )
		{
			S_.Parent = N.S_.Parent;
			S_.NameRow = N.S_.NameRow;
			S_.ValueRow = N.S_.ValueRow;

			Attributes = N.Attributes;
			Children = N.Children;
		}
		void Init(
			crow__ NameRow = NONE,
			nrow__ Parent = NONE )
		{
			S_.Parent = Parent;
			S_.NameRow = NameRow;

			Attributes.Init();
			Children.Init();
		}
		E_RODISCLOSE_( crow__, NameRow )
		E_RWDISCLOSE_( crow__, ValueRow )
		E_RWDISCLOSE_( nrow__, Parent )
	};

	typedef lstctn::E_LXCONTAINERt_( node_, nrow__ ) nodes_;
	E_AUTO( nodes )

	typedef ctn::E_CITEMt( node_, nrow__ )	node_buffer;

	class registry_ {
	private:
		void _AddEntry(
			const name_ &Name,
			const value_ &Value,
			erows_ &EntryRows )
		{
			entry__ Entry;

			Entry.NameRow = Contents.Add( Name );
			Entry.ValueRow = Contents.Add( Value );

			EntryRows.Append( Entries.Add( Entry ) );
		}
		const content_ &_GetContent(
			crow__ ContentRow,
			buffer &Buffer ) const
		{
			return Buffer( ContentRow );
		}
		const name_ &_GetName(
			const entry__ &Entry,
			buffer &Buffer ) const
		{
			return _GetContent( Entry.NameRow, Buffer );
		}
		const name_ &_GetValue(
			const entry__ &Entry,
			buffer &Buffer ) const
		{
			return _GetContent( Entry.ValueRow, Buffer );
		}
		const name_ &_GetName(
			erow__ EntryRow,
			buffer &Buffer ) const
		{
			return _GetName( Entries( EntryRow ), Buffer );
		}
		const name_ &_GetValue(
			erow__ EntryRow,
			buffer &Buffer ) const
		{
			return _GetValue( Entries( EntryRow ), Buffer );
		}
		erow__ _SearchEntry(
			const name_ &Name,
			const erows_ &EntryRows ) const;
		erow__ _SearchEntry(
			const name_ &Name,
			const value_ &Value,
			const erows_ &EntryRows ) const
		{
			erow__ EntryRow = _SearchEntry( Name, EntryRows );

			if ( EntryRow != NONE ) {
				buffer Buffer;

				if ( _GetValue( EntryRow, Buffer ) != Value )
					EntryRow = NONE;
			}

			return EntryRow;
		}
		bso::bool__ _AttributeExists(
			const name_ &Name,
			const value_ &Value,
			nrow__ NodeRow ) const
		{
			node_buffer Buffer;

			Buffer.Init( Nodes );

			return _SearchEntry( Name, Value, _GetNode( NodeRow, Buffer ).Attributes ) != NONE;
		}
		const node_ &_GetNode(
			nrow__ NodeRow,
			node_buffer &Buffer ) const
		{
			return Buffer( NodeRow );
		}
		const name_ &_GetName(
			nrow__ NodeRow,
			buffer &NameBuffer,
			node_buffer &NodeBuffer ) const
		{	
			return NameBuffer( _GetNode( NodeRow, NodeBuffer ).GetNameRow() );
		}
		const name_ &_GetValue(
			nrow__ NodeRow,
			buffer &ValueBuffer,
			node_buffer &NodeBuffer ) const
		{	
			return ValueBuffer( _GetNode( NodeRow, NodeBuffer ).GetValueRow() );
		}
		nrow__ _SearchNode(
			const name_ &Name,
			const nrows_ &NodeRows,
			epeios::row__ &Cursor ) const;
		nrow__ _SearchNode(
			const name_ &Name,
			nrow__ ParentNodeRow,
			epeios::row__ &Cursor ) const
		{
			node_buffer Buffer;

			Buffer.Init( Nodes );

			return _SearchNode( Name, Buffer( ParentNodeRow ).Children, Cursor );
		}
		nrow__ _SearchNode(
			const name_ &NodeName,
			const name_ &AttributeName,
			const value_ &AttributeValue,
			nrow__ ParentNodeRow,
			epeios::row__ &Cursor ) const;
	public:
		struct s {
			contents_::s Contents;
			entries_::s Entries;
			nodes_::s Nodes;
		};
		contents_ Contents;
		entries_ Entries;
		nodes_ Nodes;
		registry_( s &S )
		: Contents( S.Contents ),
		  Entries( S.Entries ),
		  Nodes( S.Nodes )
		{}
		void reset( bso::bool__ P = true )
		{
			Contents.Init();
			Entries.reset( P );
			Nodes.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Contents.plug( MM );
			Entries.plug( MM );
			Nodes.plug( MM );
		}
		registry_ &operator =( const registry_ &R )
		{
			Contents = R.Contents;
			Entries = R.Entries;
			Nodes = R.Nodes;

			return *this;
		}
		void Init( void )
		{
			Contents.Init();
			Entries.Init();
			Nodes.Init();
		}
		nrow__ CreateNode( const content_ &Content )
		{
			nrow__ Row = Nodes.New();

			Nodes( Row ).Init( Contents.Add( Content ) );

			Nodes.Flush();

			return Row;
		}
		void AddChild(
			nrow__ ChildRow,
			nrow__ ParentRow )
		{
			Nodes( ParentRow ).Children.Append( ChildRow );
#ifdef RGISTRY_DBG
			if ( Nodes( ChildRow ).Parent != NONE )
				ERRu();
#endif
			Nodes( ChildRow ).Parent() = ParentRow;

			Nodes.Flush();
		}
		nrow__ AddChild(
			const content_ &Name,
			nrow__ ParentRow )
		{
			nrow__ Row = Nodes.New();

			Nodes( Row ).Init( Contents.Add( Name ), ParentRow );

			Nodes( ParentRow ).Children.Append( Row );

			Nodes.Flush();

			return Row;
		}
		void AddAttribute(
			const name_ &Name,
			const value_ &Value,
			nrow__ NodeRow )
		{
			_AddEntry( Name, Value, Nodes( NodeRow ).Attributes );

			Nodes.Flush();
		}
		void SetValue(
			const value_ &Value,
			nrow__ NodeRow )
		{
			crow__ ValueRow = Nodes( NodeRow ).ValueRow();

			if ( ValueRow == NONE )
				ValueRow = Contents.New();

			Contents( ValueRow ) = Value;

			Nodes( NodeRow ).ValueRow() = ValueRow;

			Contents.Flush();
			Nodes.Flush();

		}
		nrow__ SearchChild(
			const name_ &Name,
			nrow__ ParentNodeRow,
			epeios::row__ &Cursor ) const
		{
			return _SearchNode( Name, ParentNodeRow, Cursor );
		}
		nrow__ SearchChild(
			const name_ &Name,
			nrow__ ParentNodeRow ) const
		{
			epeios::row__ Cursor = NONE;

			return _SearchNode( Name, ParentNodeRow, Cursor );
		}
		nrow__ GetParent( nrow__ NodeRow ) const
		{
			node_buffer NodeBuffer;

			NodeBuffer.Init( Nodes );

			return _GetNode( NodeRow, NodeBuffer ).Parent();
		}
		const content_ &GetName(
			nrow__ NodeRow,
			buffer &Buffer ) const
		{
			node_buffer NodeBuffer;

			Buffer.Init( Contents );
			NodeBuffer.Init( Nodes );

			return _GetName( NodeRow, Buffer, NodeBuffer );
		}
		const content_ &GetValue(
			nrow__ NodeRow,
			buffer &Buffer ) const
		{
			node_buffer NodeBuffer;

			Buffer.Init( Contents );
			NodeBuffer.Init( Nodes );

			return _GetValue( NodeRow, Buffer, NodeBuffer );
		}
		const content_ &GetCompleteName(
			nrow__ NodeRow,
			content_ &Content ) const;
	};

	E_AUTO( registry )

	nrow__ Parse(
		xtf::extended_text_iflow__ &Flow,
		registry_ &Registry );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
