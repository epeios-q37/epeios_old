/*
	Header for the 'rgstry' library by Claude SIMON (csimon@epeios.org)
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
#include "cpe.h"
#include "xml.h"

#ifdef CPE__USE_VC_WORKAROUND
#	undef SearchPath
#endif

namespace rgstry {

	typedef str::string_	term_;
	typedef str::string		term;

	typedef term_			value_;
	typedef term			value;

	class path_item_ {
	public:
		struct s {
			term_::s TagName, AttributeName;
			value_::s AttributeValue;
		};
		term_ TagName, AttributeName;
		value_ AttributeValue;
		path_item_( s &S )
		: TagName( S.TagName ),
		  AttributeName( S.AttributeName ),
		  AttributeValue( S.AttributeValue )
		{}
		void reset( bso::bool__ P = true )
		{
			TagName.reset( P );
			AttributeName.reset( P );
			AttributeValue.reset( P);
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			TagName.plug( MM );
			AttributeName.plug( MM );
			AttributeValue.plug( MM );
		}
		path_item_ &operator =( const path_item_ &PI )
		{
			TagName = PI.TagName;
			AttributeName = PI.AttributeName;
			AttributeValue = PI.AttributeValue;

			return *this;
		}
		void Init( void )
		{
			TagName.Init();
			AttributeName.Init();
			AttributeValue.Init();
		}
	};

	E_AUTO( path_item );

	typedef ctn::E_XCONTAINER_( path_item_ ) _path_items_;

	class path_
	: public _path_items_
	{
	public:
		struct s
		: _path_items_::s
		{};
		path_( s &S )
		: _path_items_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			_path_items_::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			_path_items_::plug( MM );
		}
		_path_items_ &operator =( const _path_items_ &PI )
		{
			_path_items_::operator =( PI );

			return *this;
		}
		void Init( void )
		{
			_path_items_::Init();
		}
	};



	E_AUTO( path )

	epeios::row__ BuildPath(
		const term_ &Term,
		path_ &Path );

	E_ROW( trow__ );		// term row.

	typedef lstctn::E_LXMCONTAINERt_( term_, trow__ )	terms_;
	E_AUTO( terms )

	typedef ctn::E_XMCONTAINER_( term_ )	values_;
	E_AUTO( values )


	typedef ctn::E_CMITEMt( term_, trow__ )	term_buffer;

	struct entry__ {
		trow__ NameRow;
		trow__ ValueRow;
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
			trow__ NameRow;
			trow__ ValueRow;
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
			trow__ NameRow = NONE,
			nrow__ Parent = NONE )
		{
			S_.Parent = Parent;
			S_.NameRow = NameRow;

			Attributes.Init();
			Children.Init();
		}
		E_RODISCLOSE_( trow__, NameRow )
		E_RWDISCLOSE_( trow__, ValueRow )
		E_RWDISCLOSE_( nrow__, Parent )
	};

	typedef lstctn::E_LXCONTAINERt_( node_, nrow__ ) nodes_;
	E_AUTO( nodes )

	typedef ctn::E_CITEMt( node_, nrow__ )	node_buffer;

	class registry_ {
	private:
		void _AddEntry(
			const term_ &Name,
			const value_ &Value,
			erows_ &EntryRows )
		{
			entry__ Entry;

			Entry.NameRow = Terms.Add( Name );
			Entry.ValueRow = Terms.Add( Value );

			EntryRows.Append( Entries.Add( Entry ) );
		}
		const term_ &_GetTerm(
			trow__ TermRow,
			term_buffer &Buffer ) const
		{
			return Buffer( TermRow );
		}
		const term_ &_GetName(
			const entry__ &Entry,
			term_buffer &Buffer ) const
		{
			return _GetTerm( Entry.NameRow, Buffer );
		}
		const value_ &_GetValue(
			const entry__ &Entry,
			term_buffer &Buffer ) const
		{
			return _GetTerm( Entry.ValueRow, Buffer );
		}
		const term_ &_GetName(
			erow__ EntryRow,
			term_buffer &Buffer ) const
		{
			return _GetName( Entries( EntryRow ), Buffer );
		}
		const value_ &_GetValue(
			erow__ EntryRow,
			term_buffer &Buffer ) const
		{
			return _GetValue( Entries( EntryRow ), Buffer );
		}
		erow__ _SearchEntry(
			const term_ &Name,
			const erows_ &EntryRows ) const;
		erow__ _SearchEntry(
			const term_ &Name,
			const value_ &Value,
			const erows_ &EntryRows ) const
		{
			erow__ EntryRow = _SearchEntry( Name, EntryRows );

			if ( EntryRow != NONE ) {
				term_buffer Buffer;

				Buffer.Init( Terms );

				if ( _GetValue( EntryRow, Buffer ) != Value )
					EntryRow = NONE;
			}

			return EntryRow;
		}
		bso::bool__ _AttributeExists(
			const term_ &Name,
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
		const term_ &_GetName(
			nrow__ NodeRow,
			term_buffer &NameBuffer,
			node_buffer &NodeBuffer ) const
		{	
			return NameBuffer( _GetNode( NodeRow, NodeBuffer ).GetNameRow() );
		}
		const value_ &_GetValue(
			nrow__ NodeRow,
			term_buffer &ValueBuffer,
			node_buffer &NodeBuffer ) const
		{	
			static term Empty;
			trow__ Row = _GetNode( NodeRow, NodeBuffer ).GetValueRow();

			if ( Row != NONE )
				return ValueBuffer( Row );
			else {
				Empty.Init();
				return Empty;
			}
		}
		nrow__ _SearchNode(
			const term_ &Name,
			const nrows_ &NodeRows,
			epeios::row__ &Cursor ) const;
		nrow__ _SearchNode(
			const term_ &Name,
			nrow__ ParentNodeRow,
			epeios::row__ &Cursor ) const
		{
			node_buffer Buffer;

			Buffer.Init( Nodes );

			return _SearchNode( Name, Buffer( ParentNodeRow ).Children, Cursor );
		}
		nrow__ _SearchNode(
			const term_ &NodeName,
			const term_ &AttributeName,
			const value_ &AttributeValue,
			nrow__ ParentNodeRow,
			epeios::row__ &Cursor ) const;
		nrow__ _SearchChild(
			const path_item_ &Item,
			nrow__ ParentRow,
			epeios::row__ &Cursor ) const
		{
			if ( Item.AttributeName.Amount() != 0 )
				return SearchChild( Item.TagName, Item.AttributeName, Item.AttributeValue, ParentRow, Cursor );
			else
				return SearchChild( Item.TagName, ParentRow, Cursor );
		}
		nrow__ _SearchChild(
			const path_item_ &Item,
			nrow__ ParentRow ) const
		{
			epeios::row__ Cursor = NONE;

			return _SearchChild( Item, ParentRow, Cursor );
		}
		nrow__ _SearchChild(
			const path_item_ &Item,
			nrow__ Root,
			erow__ &AttributeEntryRow,
			epeios::row__ &Cursor ) const;
		nrow__ _SearchPath(
			const path_ &Path,
			epeios::row__ PathRow,
			nrow__ Row,
			erow__ &AttributeEntryRow,
			nrows_ &ResultTags,
			erows_ &ResultAttributes ) const;
		nrow__ _SearchPath(
			const path_ &Path,
			nrow__ ParentRow,
			erow__ &AttributeEntryRow ) const;
		nrow__ _SearchPath(
			const term_ &Term,
			nrow__ ParentRow,
			erow__ &AttributeEntryRow,
			epeios::row__ &PathErrorRow ) const;
		nrow__ _CreateChild(
			const path_item_ &Item,
			nrow__ Row )
		{
			Row = AddChild( Item.TagName, Row );

			if ( Item.AttributeName.Amount() != 0 )
				AddAttribute( Item.AttributeName, Item.AttributeValue, Row );

			return Row;
		}
		erow__ _GetAttribute(
			const term_ &Name,
			nrow__ NodeRow ) const
		{
			node_buffer Buffer;

			Buffer.Init( Nodes );

			return _SearchEntry( Name, _GetNode( NodeRow, Buffer ).Attributes );
		}
		void _Delete( erow__ Row )
		{
			entry__ Entry = Entries( Row );

			if ( Entry.NameRow != NONE )
				Terms.Delete( Entry.NameRow );

			if ( Entry.ValueRow != NONE )
				Terms.Delete( Entry.ValueRow );
		}
		void _Delete( const erows_ &Rows );
		void _Delete( nrow__ Row );
		void _Delete( const nrows_ &Rows );
		void _DumpAttributes(
			const erows_ &Rows,
			xml::writer_ &Writer ) const;
		void _DumpNode(
			nrow__ NodeRow,
			xml::writer_ &Writer,
			term_buffer &TermBuffer,
			node_buffer &NodeBuffer ) const
		{
			Writer.PushTag( _GetName( NodeRow, TermBuffer, NodeBuffer ) );	// 'PopTag' correspondant fait par méthode appelante.
			_DumpAttributes( _GetNode( NodeRow, NodeBuffer ).Attributes, Writer );

			const term_ &Value = _GetValue( NodeRow, TermBuffer, NodeBuffer );

			if ( Value.Amount() != 0 )
				Writer.PutValue( Value );

		}
		void _Dump(
			nrow__ Root,
			bso::bool__ RootToo,
			xml::writer_ &Writer,
			term_buffer &TermBuffer,
			node_buffer &NodeBuffer ) const;
	public:
		struct s {
			terms_::s Terms;
			entries_::s Entries;
			nodes_::s Nodes;
		};
		terms_ Terms;
		entries_ Entries;
		nodes_ Nodes;
		registry_( s &S )
		: Terms( S.Terms ),
		  Entries( S.Entries ),
		  Nodes( S.Nodes )
		{}
		void reset( bso::bool__ P = true )
		{
			Terms.reset( P );
			Entries.reset( P );
			Nodes.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Terms.plug( MM );
			Entries.plug( MM );
			Nodes.plug( MM );
		}
		registry_ &operator =( const registry_ &R )
		{
			Terms = R.Terms;
			Entries = R.Entries;
			Nodes = R.Nodes;

			return *this;
		}
		void Init( void )
		{
			Terms.Init();
			Entries.Init();
			Nodes.Init();
		}
		nrow__ CreateNode( const term_ &Term )
		{
			nrow__ Row = Nodes.New();

			Nodes( Row ).Init( Terms.Add( Term ) );

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
			const term_ &Name,
			nrow__ ParentRow )
		{
			nrow__ Row = Nodes.New();

			Nodes( Row ).Init( Terms.Add( Name ), ParentRow );

			Nodes( ParentRow ).Children.Append( Row );

			Nodes.Flush();

			return Row;
		}
		void AddAttribute(
			const term_ &Name,
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
			trow__ ValueRow = Nodes( NodeRow ).ValueRow();

			if ( ValueRow == NONE )
				ValueRow = Terms.New();

			Terms( ValueRow ) = Value;

			Nodes( NodeRow ).ValueRow() = ValueRow;

			Terms.Flush();
			Nodes.Flush();

		}
		nrow__ SearchChild(
			const term_ &Name,
			nrow__ ParentNodeRow,
			epeios::row__ &Cursor ) const
		{
			return _SearchNode( Name, ParentNodeRow, Cursor );
		}
		nrow__ SearchChild(
			const term_ &Name,
			nrow__ ParentNodeRow ) const
		{
			epeios::row__ Cursor = NONE;

			return _SearchNode( Name, ParentNodeRow, Cursor );
		}
		nrow__ SearchChild(
			const term_ &NodeName,
			const term_ &AttributeName,
			const value_ &AttributeValue,
			nrow__ ParentNodeRow,
			epeios::row__ &Cursor ) const
		{
			return _SearchNode( NodeName, AttributeName, AttributeValue, ParentNodeRow, Cursor );
		}
		nrow__ SearchChild(
			const term_ &NodeName,
			const term_ &AttributeName,
			const value_ &AttributeValue,
			nrow__ ParentNodeRow ) const
		{
			epeios::row__ Cursor = NONE;

			return _SearchNode( NodeName, AttributeName, AttributeValue, ParentNodeRow, Cursor );
		}
		nrow__ SearchPath(
			const term_ &PathString,
			nrow__ ParentRow,
			erow__ &AttributeEntryRow,
			epeios::row__ &PathErrorRow ) const
		{
			return _SearchPath( PathString, ParentRow, AttributeEntryRow, PathErrorRow );
		}
		nrow__ CreatePath(
			const path_ &Path,
			nrow__ ParentRow );
		nrow__ CreatePath(
			const term_ &Term,
			nrow__ ParentRow,
			epeios::row__ &PathErrorRow );
		const value_ &GetPathValue(
			const path_ &Path,
			nrow__ ParentRow,
			bso::bool__ &Exists,
			term_buffer &Buffer ) const;	// Nota : ne met 'Exists' à 'false' que lorque 'Path' n'existe pas.
		const value_ &GetPathValue(
			const term_ &Path,
			nrow__ ParentRow,
			bso::bool__ &Exists,
			epeios::row__ &PathErrorRow,
			term_buffer &Buffer ) const;	// Nota : ne met 'Exists' à 'false' que lorque 'Path' n'existe pas.
		const value_ &GetPathValue(
			const term_ &Path,
			nrow__ ParentRow,
			bso::bool__ &Exists,
			term_buffer &Buffer ) const	// Nota : ne met 'Exists' à 'false' que lorque 'Path' n'existe pas.
		{
			epeios::row__ PathErrorRow = NONE;

			const value_ &Value = GetPathValue( Path, ParentRow, Exists, PathErrorRow, Buffer );

			if ( PathErrorRow != NONE )
				ERRu();

			return Value;
		}
		const value_ &GetPathValue(
			const term_ &Path,
			nrow__ ParentRow,
			term_buffer &Buffer ) const	// Nota : ne met 'Exists' à 'false' que lorque 'Path' n'existe pas.
		{
			bso::bool__ Exists = true;

			const value_ &Value = GetPathValue( Path, ParentRow, Exists, Buffer );

			if ( !Exists )
				ERRu();

			return Value;
		}
		bso::bool__ GetPathValues(
			const path_ &Path,
			nrow__ ParentRow,
			values_ &Values ) const;
		bso::bool__ GetPathValues(
			const term_ &Path,
			nrow__ ParentRow,
			epeios::row__ &PathErrorRow,
			values_ &Values ) const;
		nrow__ SetPathValue(
			const path_ &Path,
			const value_ &Value,
			nrow__ ParentRow )
		{
			ParentRow = CreatePath( Path, ParentRow );

			SetValue( Value, ParentRow );

			return ParentRow;
		}
		nrow__ SetPathValue(
			const term_ &Path,
			const value_ &Value,
			nrow__ ParentRow,
			epeios::row__ &PathErrorRow );
		nrow__ SetPathValue(
			const term_ &Path,
			const value_ &Value,
			nrow__ ParentRow )
		{
			epeios::row__ PathErrorRow = NONE;

			nrow__ Result = SetPathValue( Path, Value, ParentRow, PathErrorRow );

			if ( PathErrorRow != NONE )
				ERRu();

			return Result;
		}
		nrow__ GetParent( nrow__ NodeRow ) const
		{
			node_buffer NodeBuffer;

			NodeBuffer.Init( Nodes );

			return _GetNode( NodeRow, NodeBuffer ).Parent();
		}
		const term_ &GetName(
			nrow__ NodeRow,
			term_buffer &Buffer ) const
		{
			node_buffer NodeBuffer;

			Buffer.Init( Terms );
			NodeBuffer.Init( Nodes );

			return _GetName( NodeRow, Buffer, NodeBuffer );
		}
		const value_ &GetValue(
			nrow__ NodeRow,
			term_buffer &Buffer ) const
		{
			node_buffer NodeBuffer;

			Buffer.Init( Terms );
			NodeBuffer.Init( Nodes );

			return _GetValue( NodeRow, Buffer, NodeBuffer );
		}
		const term_ &GetCompleteName(
			nrow__ NodeRow,
			term_ &Term,
			const char *Separator = ":" ) const;
		erow__ GetAttribute( 
			const term_ &Name,
			nrow__ NodeRow ) const
		{
			return _GetAttribute( Name, NodeRow );
		}
		bso::bool__ AttributeExists(
			const term_ &Name,
			nrow__ NodeRow ) const
		{
			node_buffer Buffer;

			return _GetAttribute( Name, NodeRow ) != NONE;
		}
		const term_ &GetName(
			erow__ EntryRow,
			term_buffer &Buffer ) const
		{
			Buffer.Init( Terms );

			return _GetName( EntryRow, Buffer );
		}
		const value_ &GetValue(
			erow__ EntryRow,
			term_buffer &Buffer ) const
		{
			Buffer.Init( Terms );

			return _GetValue( EntryRow, Buffer );
		}
		const value_ &GetAttributeValue(
			const term_ &Name,
			nrow__ NodeRow,
			term_buffer &Buffer ) const
		{
			erow__ EntryRow = GetAttribute( Name, NodeRow );

#ifdef RGSTRY_DBG
			if ( EntryRow == NONE )
				ERRu();
#endif
			Buffer.Init( Terms );

			return _GetValue( EntryRow, Buffer );
		}
		void Delete( nrow__ Row )
		{
			_Delete( Row );
		}
		bso::bool__ DeletePath(
			const path_ &Path,
			nrow__ ParentRow )
		{
			erow__ AttributeRow = NONE;

			nrow__ Result = _SearchPath( Path, ParentRow, AttributeRow );

			if ( AttributeRow != NONE )
				_Delete( AttributeRow );
			else if ( Result != NONE )
				_Delete( Result );

			return Result != NONE;
		}
		bso::bool__ DeletePath(
			const term_ &PathString,
			nrow__ ParentRow,
			epeios::row__ &PathErrorRow );
		bso::bool__ DeletePath(
			const term_ &PathString,
			nrow__ ParentRow )
		{
			epeios::row__ PathErrorRow = NONE;
			bso::bool__ Result = DeletePath( PathString, ParentRow, PathErrorRow );

			if ( PathErrorRow != NONE )
				ERRu();

			return Result;
		}
		nrow__ CreateNewRegistry( const term_ &Name )
		{
			return CreateNode( Name );
		}
		bso::bool__ PathExists(
			const path_ &Path,
			nrow__ ParentRow ) const
		{
			erow__ AttributeEntryRow = NONE;

			return _SearchPath( Path, ParentRow, AttributeEntryRow ) != NONE;
		}
		bso::bool__ PathExists(
			const term_ &Path,
			nrow__ ParentRow,
			epeios::row__ &PathErrorRow ) const;
		void Dump(
			nrow__ Root,
			bso::bool__ RootToo,
			bso::bool__ Indent,
			txf::text_oflow__ &Flow ) const;
	};

	E_AUTO( registry )

	nrow__ Parse(
		xtf::extended_text_iflow__ &Flow,
		registry_ &Registry,
		nrow__ Root,	// 'Root' peut être = 'NONE', auquel cas une nouvelle 'regsitry' est créee.
		xtf::location__ &ErrorLine,
		xtf::location__ &ErrorColumn );


	class overloaded_registry___
	{
	public:
		struct global {
			const registry_ *Registry;
			nrow__ Root;
		} Global;
		struct local {
			registry_ *Registry;
			nrow__ Root;
		} Local;
		void reset( bso::bool__ P = true )
		{
			Global.Registry = NULL;
			Global.Root = NONE;

			Local.Registry = NULL;
			Local.Root = NONE;
		}
		overloaded_registry___( void )
		{
			reset( false );
		}
		~overloaded_registry___( void )
		{
			reset();
		}
		nrow__ Init(
			const registry_ &Global,
			nrow__ Root,
			registry_ &Local )	// 'Global' et 'Local' peuvent être identiques.
		{
			term_buffer Buffer;

			this->Global.Registry = &Global;
			this->Global.Root = Root;

			this->Local.Registry = &Local;
			return this->Local.Root = Local.CreateNode( this->Global.Registry->GetName( Root, Buffer ) );
		}
		const value_ &GetPathValue(
			const term_ &Path,
			bso::bool__ &Exists,
			epeios::row__ &PathErrorRow,
			term_buffer &Buffer ) const;	// Nota : ne met 'Exists' à 'false' que lorque 'Path' n'existe pas.
		const value_ &GetPathValue(
			const term_ &Path,
			bso::bool__ &Exists,
			term_buffer &Buffer ) const	// Nota : ne met 'Exists' à 'false' que lorque 'Path' n'existe pas.
		{
			epeios::row__ PathErrorRow = NONE;

			const value_ &Value = GetPathValue( Path, Exists, PathErrorRow, Buffer );

			if ( PathErrorRow != NONE )
				ERRu();

			return Value;
		}
		bso::bool__ GetPathValues(
			const term_ &PathString,
			epeios::row__ &PathErrorRow,
			values_ &Values ) const;
		bso::bool__ GetPathValues(
			const term_ &PathString,
			values_ &Values ) const
		{
			epeios::row__ PathErrorRow = NONE;

			bso::bool__ Exists = GetPathValues( PathString, PathErrorRow, Values );

			if ( PathErrorRow != NONE )
				ERRu();

			return Exists;
		}
		void SetPathValue(
			const term_ &Path,
			const value_ &Value,
			epeios::row__ &PathErrorRow )
		{
			Local.Registry->SetPathValue( Path, Value, Local.Root, PathErrorRow );
		}
		void SetPathValue(
			const term_ &Path,
			const value_ &Value )
		{	
			epeios::row__ PathErrorRow = NONE;

			Local.Registry->SetPathValue( Path, Value, Local.Root, PathErrorRow );

			if ( PathErrorRow != NONE )
				ERRu();
		}
		void Delete( nrow__ Row )
		{
			Local.Registry->Delete( Row );
		}
		bso::bool__ PathExists(
			const rgstry::term_ &Path,
			epeios::row__ &PathErrorRow ) const;
		bso::bool__ PathExists( const rgstry::term_ &Path ) const
		{
			epeios::row__ PathErrorRow = NONE;

			bso::bool__ Result = PathExists( Path, PathErrorRow );

			if ( PathErrorRow != NONE )
				ERRu();

			return Result;
		}
 	};

	class overloaded_unique_registry___	// La base de registre de base et locale sont la même.
	: public overloaded_registry___
	{
	private:
		nrow__ _LocalRoot;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _LocalRoot != NONE )
					overloaded_registry___::Delete( _LocalRoot );
			}

			overloaded_registry___::reset( P );
			_LocalRoot = NONE;
		}
		overloaded_unique_registry___( void )
		{
			reset( false );
		}
		virtual ~overloaded_unique_registry___( void )
		{
			reset();
		}
		void Init(
			registry_ &Global,
			nrow__ Root )
		{
			reset();

			_LocalRoot = overloaded_registry___::Init( Global, Root, Global );
		}
	};
}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
