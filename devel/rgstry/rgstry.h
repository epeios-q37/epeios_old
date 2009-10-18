/*
	Header for the 'rgstry' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

#ifndef RGSTRY__INC
#define RGSTRY__INC

#define RGSTRY_NAME		"RGSTRY"

#define	RGSTRY_VERSION	"$Revision$"

#define RGSTRY_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &RGSTRYTutor;

#if defined( XXX_DBG ) && !defined( RGSTRY_NODBG )
#define RGSTRY_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
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

	E_TYPEDEF_( str::string,	_term );

	typedef _term_			name_;
	typedef _term			name;

	typedef _term_			value_;
	typedef _term			value;

	class path_item_ {
	public:
		struct s {
			name_::s KeyName, AttributeName;
			value_::s AttributeValue;
		};
		name_ KeyName, AttributeName;
		value_ AttributeValue;
		path_item_( s &S )
		: KeyName( S.KeyName ),
		  AttributeName( S.AttributeName ),
		  AttributeValue( S.AttributeValue )
		{}
		void reset( bso::bool__ P = true )
		{
			KeyName.reset( P );
			AttributeName.reset( P );
			AttributeValue.reset( P);
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			KeyName.plug( MM );
			AttributeName.plug( MM );
			AttributeValue.plug( MM );
		}
		path_item_ &operator =( const path_item_ &PI )
		{
			KeyName = PI.KeyName;
			AttributeName = PI.AttributeName;
			AttributeValue = PI.AttributeValue;

			return *this;
		}
		void Init( void )
		{
			KeyName.Init();
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
		const str::string_ &PathString,
		path_ &Path );

	typedef ctn::E_XMCONTAINER_( value_ )	values_;
	E_AUTO( values )

	E_ROW( row__ );
	typedef bch::E_BUNCH_( row__ ) rows_;
	E_AUTO( rows );

	// Nature du noeud.
	enum nature__ 
	{
		nKey,
		nAttribute,
		n_amount,
		n_Undefined
	};

	class node_ {
	public:
		struct s {
			nature__ Nature;
			row__ ParentRow;
			name_::s Name;
			value_::s Value;
			rows_::s Children;
		} &S_;
		name_ Name;
		value_ Value;
		rows_ Children;
		node_( s &S )
		: S_( S ),
		  Name( S.Name ),
		  Value( S.Value ),
		  Children( S.Children )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Nature = n_Undefined;
			S_.ParentRow = NONE;

			Name.reset( P );
			Value.reset( P );
			Children.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Name.plug( MM );
			Value.plug( MM );
			Children.plug( MM );
		}
		node_ &operator =( const node_ &N )
		{
			S_.Nature = N.S_.Nature;
			S_.ParentRow = N.S_.ParentRow;

			Name = N.Name;
			Value = N.Value;
			Children = N.Children;
		}
		void Init( void )
		{
			reset();

			Name.Init();
			Value.Init();
			Children.Init();
		}
		void Init(
			nature__ Nature,
			const name_ &Name,
			const value_ &Value,
			row__ ParentRow = NONE )
		{
			reset();

			S_.ParentRow = ParentRow;
			S_.Nature = Nature;

			this->Name = Name;
			this->Value = Value;

			Children.Init();
		}
		void Init(
			nature__ Nature,
			const name_ &Name,
			row__ ParentRow = NONE )
		{
			Init( Nature, Name, value(), ParentRow );
		}
		E_RWDISCLOSE_( row__, ParentRow )
		E_RODISCLOSE_( nature__, Nature );
	};

	typedef lstctn::E_LXMCONTAINERt_( node_, row__ ) nodes_;
	E_AUTO( nodes )

	typedef ctn::E_CMITEMt( node_, row__ )	buffer;

	class registry_ {
	private:
		const node_ &_GetNode(
			row__ Row,
			buffer &Buffer ) const
		{
			Buffer.Init( Nodes );

			return Buffer( Row );
		}
		const name_ &_GetName(
			row__ Row,
			buffer &Buffer ) const
		{
			return _GetNode( Row, Buffer ).Name;
		}
		const value_ &_GetValue(
			row__ Row,
			buffer &Buffer ) const
		{
			return _GetNode( Row, Buffer ).Value;
		}
		row__ _FlatSearch(
			const name_ &Name,
			const rows_ &Rows,
			epeios::row__ &Cursor ) const;
		row__ _FlatSearch(
			const name_ &Name,
			row__ ParentRow,
			epeios::row__ &Cursor ) const
		{
			buffer Buffer;

			return _FlatSearch( Name, _GetNode( ParentRow, Buffer ).Children, Cursor );
		}
		row__ _FlatSearch(
			nature__ Nature,
			const name_ &Name,
			const rows_ &Rows ) const;
		row__ _FlatSearch(
			nature__ Nature,
			const name_ &Name,
			row__ ParentRow ) const
		{
			buffer Buffer;

			return _FlatSearch( Nature, Name, _GetNode( ParentRow, Buffer ).Children );
		}
		row__ _GetAttributeRow(
			const name_ &Name,
			row__ Row ) const
		{
			return _FlatSearch( nAttribute, Name, Row );
		}
		bso::bool__ _AttributeExists(
			const name_ &Name,
			row__ Row ) const
		{
			return _GetAttributeRow( Name, Row ) != NONE;
		}
		bso::bool__ _AttributeExists(
			const name_ &Name,
			const value_ &Value,
			row__ Row ) const
		{
			row__ ResultRow = _GetAttributeRow( Name, Row );

			if ( ResultRow != NONE ) {
				buffer Buffer;

				if ( _GetValue( ResultRow, Buffer ) != Value )
					ResultRow = NONE;
			}

			return ResultRow != NONE;
		}
		row__ _Search(
			const name_ &KeyName,
			const name_ &AttributeName,
			const value_ &AttributeValue,
			row__ ParentRow,
			epeios::row__ &Cursor ) const;
		row__ _Search(
			const path_item_ &Item,
			row__ ParentRow ) const
		{
			epeios::row__ Cursor = NONE;

			return _Search( Item, ParentRow, Cursor );
		}
		row__ _Search(
			const path_item_ &Item,
			row__ Root,
			epeios::row__ &Cursor ) const;
		row__ _Search(
			const path_ &Path,
			epeios::row__ PathRow,
			row__ Row,
			rows_ &ResultRows ) const;
		row__ _Search(
			const path_ &Path,
			row__ ParentRow ) const;
		row__ _Search(
			const str::string_ &PathString,
			row__ ParentRow,
			epeios::row__ &PathErrorRow ) const;
		row__ _Create(
			nature__ Nature,
			const name_ &Name,
			const value_ &Value,
			row__ ParentRow = NONE )
		{
			row__ Row = Nodes.New();

			Nodes( Row ).Init( Nature, Name, Value, ParentRow );

			Nodes.Flush();

			return Row;
		}
		row__ _CreateKey(
			const name_ &Name,
			row__ ParentRow = NONE )
		{
			return _Create( nKey, Name, value(), ParentRow );
		}
		row__ _Add( 
			nature__ Nature,
			const name_ &Name,
			const value_ &Value,
			row__ ParentRow )
		{
			row__ Row = _Create( nKey, Name, Value, ParentRow );

			Nodes( ParentRow ).Children.Append( Row );

			return Row;
		}
		row__ _AddKey(
			const name_ &Name,
			row__ ParentRow )
		{
			return _Add( nKey, Name, value(), ParentRow );
		}
		row__ _AddAttribute(
			const name_ &Name,
			const value_ &Value,
			row__ ParentRow )
		{
			return _Add( nAttribute, Name, Value, ParentRow );
		}
		row__ _Create(
			const path_item_ &Item,
			row__ Row )
		{
			Row = _AddKey( Item.KeyName, Row );

			if ( Item.AttributeName.Amount() != 0 )
				_AddAttribute( Item.AttributeName, Item.AttributeValue, Row );

			return Row;
		}
		void _Delete( row__ Row );
		void _Delete( const rows_ &Rows );
		void _DumpAttributes(
			row__ Row,
			xml::writer_ &Writer ) const;
		void _DumpNode(
			row__ Row,
			xml::writer_ &Writer ) const
		{
			buffer Buffer;

			Writer.PushTag( _GetName( Row, Buffer ) );	// 'PopTag' correspondant fait par méthode appelante.
			_DumpAttributes( Row, Writer );

			const value_ &Value = _GetValue( Row, Buffer );

			if ( Value.Amount() != 0 )
				Writer.PutValue( Value );

		}
		epeios::size__ _Dump(
			row__ Root,
			bso::bool__ RootToo,
			xml::writer_ &Writer,
			buffer &Buffer ) const;	// Retourne le nombre d'enfants.
	public:
		struct s {
			nodes_::s Nodes;
		};
		nodes_ Nodes;
		registry_( s &S )
		: Nodes( S.Nodes )
		{}
		void reset( bso::bool__ P = true )
		{
			Nodes.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Nodes.plug( MM );
		}
		registry_ &operator =( const registry_ &R )
		{
			Nodes = R.Nodes;

			return *this;
		}
		void Init( void )
		{
			Nodes.Init();
		}
		void Add(
			row__ Row,
			row__ ParentRow )
		{
			Nodes( ParentRow ).Children.Append( Row );

#ifdef RGSTRY_DBG
			if ( Nodes( Row ).ParentRow() != NONE )
				ERRu();
#endif
			Nodes( Row ).ParentRow() = ParentRow;

			Nodes.Flush();
		}
		row__ AddKey(
			const name_ &Name,
			row__ ParentRow )
		{
			return _AddKey( Name, ParentRow );
		}
		void SetValue(
			const value_ &Value,
			row__ Row,
			bso::bool__ Append )
		{
			if ( Append )
				Nodes( Row ).Value.Append( Value );
			else
				Nodes( Row ).Value.Init( Value );
		}
		row__ Search(
			const name_ &Name,
			row__ ParentRow,
			epeios::row__ &Cursor ) const
		{
			return _Search( Name, ParentRow, Cursor );
		}
		row__ Search(
			const name_ &Name,
			row__ ParentRow ) const
		{
			epeios::row__ Cursor = NONE;

			return _Search( Name, ParentRow, Cursor );
		}
		row__ Search(
			const name_ &NodeName,
			const name_ &AttributeName,
			const value_ &AttributeValue,
			row__ ParentRow,
			epeios::row__ &Cursor ) const
		{
			return _Search( NodeName, AttributeName, AttributeValue, ParentRow, Cursor );
		}
		row__ Search(
			const name_ &KeyName,
			const name_ &AttributeName,
			const value_ &AttributeValue,
			row__ ParentRow ) const
		{
			epeios::row__ Cursor = NONE;

			return _Search( KeyName, AttributeName, AttributeValue, ParentRow, Cursor );
		}
		row__ Search(
			const str::string_ &PathString,
			row__ ParentRow,
			epeios::row__ &PathErrorRow ) const
		{
			return _Search( PathString, ParentRow, PathErrorRow );
		}
		row__ Create(
			const path_ &Path,
			row__ ParentRow );
		row__ Create(
			const str::string_ &PathString,
			row__ ParentRow,
			epeios::row__ &PathErrorRow );
		const value_ &GetValue(
			const path_ &Path,
			row__ ParentRow,
			bso::bool__ *Missing,
			buffer &Buffer ) const;	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas.
		const value_ &GetValue(
			const str::string_ &PathString,
			row__ ParentRow,
			bso::bool__ *Missing,
			epeios::row__ &PathErrorRow,
			buffer &Buffer ) const;	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas.
		const value_ &GetValue(
			const str::string_ &PathString,
			row__ ParentRow,
			bso::bool__ *Missing,
			buffer &Buffer ) const	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas.
		{
			epeios::row__ PathErrorRow = NONE;

			const value_ &Value = GetValue( PathString, ParentRow, Missing, PathErrorRow, Buffer );

			if ( PathErrorRow != NONE )
				ERRu();

			return Value;
		}
		bso::bool__ GetValue(
			const str::string_ &PathString,
			row__ ParentRow,
			value_ &Value ) const
		{
			buffer Buffer;
			bso::bool__ Missing = false;

			Value = GetValue( PathString, ParentRow, &Missing, Buffer );

			return !Missing;
		}
		const value_ &GetValue(
			const str::string_ &PathString,
			row__ ParentRow,
			buffer &Buffer ) const
		{
			bso::bool__ Missing = false;

			const value_ &Value = GetValue( PathString, ParentRow, &Missing, Buffer );

			if ( Missing )
				ERRu();

			return Value;
		}
		bso::bool__ GetPathValues(
			const path_ &Path,
			row__ ParentRow,
			values_ &Values ) const;
		bso::bool__ GetValues(
			const str::string_ &PathString,
			row__ ParentRow,
			epeios::row__ &PathErrorRow,
			values_ &Values ) const;
		row__ SetValue(
			const path_ &Path,
			const value_ &Value,
			row__ ParentRow )
		{
			ParentRow = Create( Path, ParentRow );

			SetValue( Value, ParentRow, false );

			return ParentRow;
		}
		row__ SetValue(
			const str::string_ &PathString,
			const value_ &Value,
			row__ ParentRow,
			epeios::row__ &PathErrorRow );
		row__ SetValue(
			const str::string_ &PathString,
			const value_ &Value,
			row__ ParentRow )
		{
			epeios::row__ PathErrorRow = NONE;

			row__ Result = SetValue( PathString, Value, ParentRow, PathErrorRow );

			if ( PathErrorRow != NONE )
				ERRu();

			return Result;
		}
		row__ GetParentRow( row__ Row ) const
		{
			buffer Buffer;

			return _GetNode( Row, Buffer ).ParentRow();
		}
		const name_ &GetName(
			row__ Row,
			buffer &Buffer ) const
		{
			return _GetName( Row, Buffer );
		}
		const value_ &GetValue(
			row__ Row,
			buffer &Buffer ) const
		{
			return _GetValue( Row, Buffer );
		}
		const str::string_ &GetCompleteName(
			row__ Row,
			str::string_ &Name,
			const char *Separator = ":" ) const;
		row__ GetAttributeRow( 
			const name_ &Name,
			row__ Row ) const
		{
			return _GetAttributeRow( Name, Row );
		}
		bso::bool__ AttributeExists(
			const name_ &Name,
			row__ Row ) const
		{
			return _AttributeExists( Name, Row );
		}
		const value_ &GetAttributeValue(
			const name_ &Name,
			row__ Row,
			buffer &Buffer ) const
		{
			row__ AttributeRow = _GetAttributeRow( Name, Row );

#ifdef RGSTRY_DBG
			if ( Row == NONE )
				ERRu();
#endif
			return _GetValue( AttributeRow, Buffer );
		}
		void Delete( row__ Row )
		{
			_Delete( Row );
		}
		bso::bool__ Delete(
			const path_ &Path,
			row__ Row )
		{
			row__ ResultRow = _Search( Path, Row );

			if ( ResultRow != NONE ) {
				_Delete( ResultRow );
				return true;
			} else
				return false;
		}
		bso::bool__ Delete(
			const str::string_ &PathString,
			row__ ParentRow,
			epeios::row__ &PathErrorRow );
		bso::bool__ DeletePath(
			const str::string_ &PathString,
			row__ ParentRow )
		{
			epeios::row__ PathErrorRow = NONE;
			bso::bool__ Result = Delete( PathString, ParentRow, PathErrorRow );

			if ( PathErrorRow != NONE )
				ERRu();

			return Result;
		}
		row__ CreateNewRegistry( const name_ &Name )
		{
			return _CreateKey( Name );
		}
		bso::bool__ Exists(
			const path_ &Path,
			row__ ParentRow ) const
		{
			return _Search( Path, ParentRow ) != NONE;
		}
		bso::bool__ Exists(
			const str::string_ &PathString,
			row__ ParentRow,
			epeios::row__ &PathErrorRow ) const;
		epeios::size__ Dump(
			row__ Root,
			bso::bool__ RootToo,
			bso::bool__ Indent,
			txf::text_oflow__ &Flow ) const;	// Retourne le nombre d'enfants.
	};
	E_AUTO( registry )

	class xcoord_ {
	public:
		struct s {
			xml::coord__ Coord;
			str::string_::s File;
		} &S_;
		str::string_ File;
		xcoord_( s &S )
		: S_( S ),
		  File( S.File )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Coord.reset( P );
			File.reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			File.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			File.plug( MM );
		}
		xcoord_ &operator =( const xcoord_ &XC )
		{
			S_.Coord = XC.S_.Coord;

			File = XC.File;

			return *this;
		}
		void Init( void )
		{
			reset();

			S_.Coord.Init();
			File.Init();
		}
		E_RWDISCLOSE_( xml::coord__, Coord );
	};

	E_AUTO( xcoord );

	row__ Parse(
		xtf::extended_text_iflow__ &Flow,
		const str::string_ &Directory,
		registry_ &Registry,
		row__ Root,	// 'Root' peut être = 'NONE', auquel cas une nouvelle 'registry' est créee.
		xml::extended_status__ &Status,	// Si valeur retournée == NONE, alors contient le code d'erreur.
		xcoord_ &ErrorCoord );

	inline row__ Parse(
		xtf::extended_text_iflow__ &Flow,
		const str::string_ &Directory,
		registry_ &Registry,
		row__ Root	) // 'Root' peut être = 'NONE', auquel cas une nouvelle 'registry' est créee.
	{
		row__ Result = NONE;
	ERRProlog
		xcoord Dummy;
		xml::extended_status__ Status = xml::xs_Undefined;
	ERRBegin
		Dummy.Init();

		Result = Parse( Flow, Directory, Registry, Root, Status, Dummy );
	ERRErr
	ERREnd
	ERREpilog
		return Result;
	}

	class overloaded_registry___
	{
	public:
		struct global {
			const registry_ *Registry;
			row__ Root;
		} Global;
		struct local {
			registry_ *Registry;
			row__ Root;
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
		row__ Init(
			const registry_ &Global,
			row__ Root,
			registry_ &Local,	// 'Global' et 'Local' peuvent être identiques.
			row__ LocalRoot )	// Si égal à NONE et 'Local' != 'NULL', est crée et retourné.
		{
			buffer Buffer;

			this->Global.Registry = &Global;
			this->Global.Root = Root;

			this->Local.Registry = &Local;

			if ( ( &Local != NULL ) && ( LocalRoot == NONE ) )
				LocalRoot = Local.CreateNewRegistry( this->Global.Registry->GetName( Root, Buffer ) );

			return this->Local.Root = LocalRoot;

		}
		void Init(
			const registry_ &Global,
			row__ Root )
		{
			Init( Global, Root, *(registry_ *)NULL, NONE );
		}
		row__ SetLocal(
			registry_ &Registry,	// Si == 'NULL', on prend le 'Global'.
			row__ Root )	// Si == 'NONE' est crée et retourné.
		{
			if ( ( &Global.Registry == NULL ) || ( Global.Root == NONE ) )
				ERRu();

			if ( &Registry == NULL )
				ERRu();

			Local.Registry = &Registry;

			if ( Root == NONE ) {
				buffer Buffer;

				Root = Registry.CreateNewRegistry( this->Global.Registry->GetName( Global.Root, Buffer ) );
			}

			return Local.Root = Root;
		}
		const value_ &GetValue(
			const str::string_ &PathString,
			bso::bool__ *Missing,
			epeios::row__ &PathErrorRow,
			buffer &Buffer ) const;	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas.
		const value_ &GetValue(
			const str::string_ &PathString,
			bso::bool__ *Missing,
			buffer &Buffer ) const	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas.
		{
			epeios::row__ PathErrorRow = NONE;

			const value_ &Value = GetValue( PathString, Missing, PathErrorRow, Buffer );

			if ( PathErrorRow != NONE )
				ERRu();

			return Value;
		}
		bso::bool__ GetValue(
			const str::string_ &PathString,
			value_ &Value ) const
		{
			buffer Buffer;
			bso::bool__ Missing = false;

			Value = GetValue( PathString, &Missing, Buffer );

			return !Missing;
		}
		bso::bool__ GetValues(
			const str::string_ &PathString,
			epeios::row__ &PathErrorRow,
			values_ &Values ) const;
		bso::bool__ GetValues(
			const str::string_ &PathString,
			values_ &Values ) const
		{
			epeios::row__ PathErrorRow = NONE;

			bso::bool__ Exists = GetValues( PathString, PathErrorRow, Values );

			if ( PathErrorRow != NONE )
				ERRu();

			return Exists;
		}
		void SetValue(
			const str::string_ &PathString,
			const value_ &Value,
			epeios::row__ &PathErrorRow )
		{
			Local.Registry->SetValue( PathString, Value, Local.Root, PathErrorRow );
		}
		void SetValue(
			const str::string_ &PathString,
			const value_ &Value )
		{	
			epeios::row__ PathErrorRow = NONE;

			Local.Registry->SetValue( PathString, Value, Local.Root, PathErrorRow );

			if ( PathErrorRow != NONE )
				ERRu();
		}
		void Delete( row__ Row )
		{
			Local.Registry->Delete( Row );
		}
		bso::bool__ Exists(
			const str::string_ &Path,
			epeios::row__ &PathErrorRow ) const;
		bso::bool__ Exists( const str::string_ &PathString ) const
		{
			epeios::row__ PathErrorRow = NONE;

			bso::bool__ Result = Exists( PathString, PathErrorRow );

			if ( PathErrorRow != NONE )
				ERRu();

			return Result;
		}
 	};

	class overloaded_unique_registry___	// La base de registre de base et locale sont la même.
	: public overloaded_registry___
	{
	private:
		row__ _LocalRoot;
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
		row__ Init(
			registry_ &Global,
			row__ Root )	// Si == 'NONE', est crée et retourné.
		{
			reset();

			if ( Root == NONE )
				Root = Global.CreateNewRegistry( name() );

			_LocalRoot = overloaded_registry___::Init( Global, Root, *(registry_ *)NULL, NONE );

			return Root;
		}
		row__ Init(
			registry_ &Global,
			row__ Root,
			row__ LocalRoot ) // Si égal à NONE, est crée et retourné.
		{
			reset();

			if ( LocalRoot == NONE )
				LocalRoot = _LocalRoot = overloaded_registry___::Init( Global, Root, Global, LocalRoot );
			else
				overloaded_registry___::Init( Global, Root, Global, LocalRoot );

			return LocalRoot;
		}
		row__ SetLocal(
			registry_ &Registry,
			row__ Root )	// Si 'Root' == 'NONE'
		{
			if ( Root == NONE )
				Root = _LocalRoot = overloaded_registry___::SetLocal( Registry, Root );
			else
				overloaded_registry___::SetLocal( Registry, Root );


			return Root;
		}
	};
}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
