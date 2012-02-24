/*
	Header for the 'rgstry' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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
#include "xpp.h"

# define RGSTRY__TAG_MARKER	'%'

// Prédéclaration.
namespace lcl {
	struct rack__;
}

namespace rgstry {

	typedef str::string		_term;
	typedef str::string_	_term_;

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

	typedef lstctn::E_LXCONTAINERt_( node_, row__ ) nodes_;
	E_AUTO( nodes )

	typedef ctn::E_CITEMt( node_, row__ )	buffer;

	typedef epeios::row__ cursor__;

	class registry_ {
	private:
		const node_ &_GetNode(
			row__ Row,
			buffer &Buffer ) const
		{
			Buffer.Init( Nodes );

			return Buffer( Row );
		}
		nature__ _GetNature( row__ Row ) const
		{
			buffer Buffer;

			return _GetNode( Row, Buffer ).Nature();
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
		row__ _Search(
			const name_ &Name,
			const rows_ &Rows,
			cursor__ &Cursor ) const;
		row__ _Search(
			const name_ &Name,
			row__ Row,
			cursor__ &Cursor ) const
		{
			buffer Buffer;

			return _Search( Name, _GetNode( Row, Buffer ).Children, Cursor );
		}
		row__ _Search(
			nature__ Nature,
			const name_ &Name,
			const rows_ &Rows,
			cursor__ &Cursor ) const;
		row__ _Search(
			nature__ Nature,
			const name_ &Name,
			row__ Row,
			cursor__ &Cursor ) const
		{
			buffer Buffer;

			return _Search( Nature, Name, _GetNode( Row, Buffer ).Children, Cursor );
		}
		row__ _SearchKey(
			const name_ &Name,
			row__ Row,
			cursor__ &Cursor ) const
		{
			return _Search( nKey, Name, Row, Cursor );
		}
		row__ _SearchKey(
			const name_ &Name,
			row__ Row ) const
		{
			cursor__ Cursor = NONE;

			return _Search( nKey, Name, Row, Cursor );
		}
		row__ _SearchAttribute(
			const name_ &Name,
			row__ Row,
			cursor__ &Cursor ) const
		{
			return _Search( nAttribute, Name, Row, Cursor );
		}
		row__ _SearchAttribute(
			const name_ &Name,
			row__ Row ) const
		{
			cursor__ Cursor = NONE;

			return _Search( nAttribute, Name, Row, Cursor );
		}
		bso::bool__ _AttributeExists(
			const name_ &Name,
			row__ Row ) const
		{
			return _SearchAttribute( Name, Row ) != NONE;
		}
		row__ _SearchAttribute(
			const name_ &Name,
			const value_ &Value,
			row__ Row ) const
		{
			Row = _SearchAttribute( Name, Row );

			if ( Row != NONE ) {
				buffer Buffer;

				if ( _GetValue( Row, Buffer ) != Value )
					Row = NONE;
			}

			return Row;
		}
		bso::bool__ _AttributeExists(
			const name_ &Name,
			const value_ &Value,
			row__ Row ) const
		{
			return _SearchAttribute( Name, Value, Row ) != NONE;
		}
		row__ _Search(
			const name_ &KeyName,
			const name_ &AttributeName,
			const value_ &AttributeValue,
			row__ Row,
			cursor__ &Cursor ) const;
		row__ _Search(
			const path_item_ &Item,
			row__ Row,
			cursor__ &Cursor ) const
		{
			return _Search( Item.KeyName, Item.AttributeName, Item.AttributeValue, Row, Cursor );
		}
		row__ _Search(
			const path_item_ &Item,
			row__ Row ) const
		{
			cursor__ Cursor = NONE;

			return _Search( Item, Row, Cursor );
		}
		row__ _Search(
			const path_ &Path,
			epeios::row__ PathRow,
			row__ Row,
			rows_ &ResultRows ) const;
		row__ _Search(
			const path_ &Path,
			row__ Row ) const;
		row__ _Search(
			const str::string_ &PathString,
			row__ Row,
			epeios::row__ *PathErrorRow = NULL ) const;
		row__ _CreateWithoutFlush(
			nature__ Nature,
			const name_ &Name,
			const value_ &Value,
			row__ Row = NONE )
		{
			row__ NewRow = Nodes.New();

			Nodes( NewRow ).Init( Nature, Name, Value, Row );

			return NewRow;
		}
		row__ _CreateWithFlush(
			nature__ Nature,
			const name_ &Name,
			const value_ &Value,
			row__ Row = NONE )
		{
			Row = _CreateWithoutFlush( Nature, Name, Value, Row );

			Nodes.Flush();

			return Row;
		}
		row__ _CreateKey(
			const name_ &Name,
			row__ Row = NONE )
		{
			return _CreateWithFlush( nKey, Name, value(), Row );
		}
		row__ _Add( 
			nature__ Nature,
			const name_ &Name,
			const value_ &Value,
			row__ Row )
		{
			row__ NewRow = _CreateWithoutFlush( Nature, Name, Value, Row );

			Nodes( Row ).Children.Append( NewRow );

			Nodes.Flush();

			return NewRow;
		}
		row__ _AddKey(
			const name_ &Name,
			row__ Row )
		{
			return _Add( nKey, Name, value(), Row );
		}
		row__ _AddAttribute(
			const name_ &Name,
			const value_ &Value,
			row__ Row )
		{
			return _Add( nAttribute, Name, Value, Row );
		}
		row__ _Create(
			const path_item_ &Item,
			row__ Row )
		{
			if ( Item.KeyName.Amount() != 0 ) {
				Row = _AddKey( Item.KeyName, Row );

				if ( Item.AttributeName.Amount() != 0 )
					_AddAttribute( Item.AttributeName, Item.AttributeValue, Row );
				else
					if ( Item.AttributeValue.Amount() != 0 )
						ERRu();

			} else if ( Item.AttributeName.Amount() != 0 )
				Row = _AddAttribute( Item.AttributeName, Item.AttributeValue, Row );
			else
				ERRu();

			return Row;
		}
		void _Delete( row__ Row );
		void _Delete( const rows_ &Rows );
		void _DumpAttributes(
			row__ Row,
			xml::writer_ &Writer ) const;
		void _DumpNode(
			row__ Row,
			xml::writer_ &Writer,
			buffer &Buffer ) const
		{
			Writer.PushTag( Buffer( Row ).Name );	// 'PopTag' correspondant fait par méthode appelante.
			_DumpAttributes( Row, Writer );

			const value_ &Value = Buffer( Row ).Value;

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
			row__ Row )
		{
			return _AddKey( Name, Row );
		}
		row__ AddAttribute(
			const name_ &Name,
			const value_ &Value,
			row__ Row )
		{
			return _AddAttribute( Name, Value, Row );
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

			Nodes.Flush();
		}
		row__ Search(
			const path_ &Path,
			row__ Row ) const
		{
			return _Search( Path, Row );
		}
		row__ Search(
			const str::string_ &PathString,
			row__ Row,
			epeios::row__ *PathErrorRow = NULL ) const
		{
			return _Search( PathString, Row, PathErrorRow );
		}
		row__ Create(
			const path_ &Path,
			row__ Row );
		row__ Create(
			const str::string_ &PathString,
			row__ Row,
			epeios::row__ *PathErrorRow = NULL );
		nature__ GetNature( row__ Row ) const
		{
			return _GetNature( Row );
		}
		const value_ &GetValue(
			const path_ &Path,
			row__ Row,
			bso::bool__ *Missing,
			buffer &Buffer ) const;	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
		bso::bool__ GetValue(
			const path_ &Path,
			row__ Row,
			value_ &Value ) const
		{
			buffer Buffer;
			bso::bool__ Missing = false;

			Value.Append( GetValue( Path, Row, &Missing, Buffer ) );

			return !Missing;
		}
		const value_ &GetValue(
			const str::string_ &PathString,
			row__ Row,
			bso::bool__ *Missing,
			buffer &Buffer,
			epeios::row__ *PathErrorRow = NULL ) const;	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
		bso::bool__ GetValue(
			const str::string_ &PathString,
			row__ Row,
			value_ &Value ) const
		{
			buffer Buffer;
			bso::bool__ Missing = false;

			Value.Append( GetValue( PathString, Row, &Missing, Buffer ) );

			return !Missing;
		}
		const value_ &GetValue(
			const str::string_ &PathString,
			row__ Row,
			buffer &Buffer ) const
		{
			bso::bool__ Missing = false;

			const value_ &Value = GetValue( PathString, Row, &Missing, Buffer );

			if ( Missing )
				ERRu();

			return Value;
		}
		bso::bool__ GetValues(
			const path_ &Path,
			row__ Row,
			values_ &Values ) const;
		bso::bool__ GetValues(
			const str::string_ &PathString,
			row__ Row,
			values_ &Values,
			epeios::row__ *PathErrorRow = NULL ) const;
		row__ SetValue(
			const path_ &Path,
			const value_ &Value,
			row__ Row )
		{
			Row = Create( Path, Row );

			SetValue( Value, Row, false );

			return Row;
		}
		row__ SetValue(
			const str::string_ &PathString,
			const value_ &Value,
			row__ Row,
			epeios::row__ *PathErrorRow = NULL );
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
		row__ SearchAtribute( 
			const name_ &Name,
			row__ Row ) const
		{
			return _SearchAttribute( Name, Row );
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
			row__ ResultRow = _SearchAttribute( Name, Row );

#ifdef RGSTRY_DBG
			if ( Row == NONE )
				ERRu();
#endif
			return _GetValue( ResultRow, Buffer );
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
			row__ Row,
			epeios::row__ *PathErrorRow = NULL );
		row__ CreateRegistry( const name_ &Name )
		{
			return _CreateKey( Name );
		}
		bso::bool__ Exists(
			const path_ &Path,
			row__ Row ) const
		{
			return _Search( Path, Row ) != NONE;
		}
		bso::bool__ Exists(
			const str::string_ &PathString,
			row__ Row,
			epeios::row__ *PathErrorRow = NULL ) const;
		epeios::size__ Dump(
			row__ Root,
			bso::bool__ RootToo,
			xml::writer_ &Writer ) const;	// Retourne le nombre d'enfants.
		epeios::size__ Dump(
			row__ Root,
			bso::bool__ RootToo,
			xml::outfit__ Outfit,
			xml::encoding__ Encoding,
			txf::text_oflow__ &Flow ) const;	// Retourne le nombre d'enfants.
	};

	E_AUTO( registry )

# if 0
	struct error_details_
	{
	public:
		struct s {
			xml::coord__ Coord;
			epeios::row__ PathErrorRow;
			xpp::status__ XPPStatus;
			str::string_::s FileName;
		} &S_;
		str::string_ FileName;
		error_details_( s &S )
		: S_( S ),
		  FileName( S.FileName )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Coord.reset( P );
			S_.PathErrorRow = NONE;
			S_.XPPStatus = xpp::s_Undefined;

			S_.Coord.reset( P );
			FileName.reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			FileName.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			FileName.plug( MM );
		}
		error_details_ &operator =( const error_details_ &ED )
		{
			S_.Coord = ED.S_.Coord;
			S_.PathErrorRow = ED.S_.PathErrorRow;
			S_.XPPStatus = ED.S_.XPPStatus;

			FileName = ED.FileName;

			return *this;
		}
		void Init( void )
		{
			reset();

			S_.Coord.Init();
			FileName.Init();
		}
		E_RODISCLOSE_( xml::coord__, Coord );
		E_RODISCLOSE_( epeios::row__, PathErrorRow );
		E_RODISCLOSE_( xpp::status__, XPPStatus );
	};

	E_AUTO( error_details );
#endif

	row__ Parse(
		xtf::extended_text_iflow__ &XFlow,
		const xpp::criterions___ &Criterions,
		registry_ &Registry,
		row__ &Root,	// Peut être = 'NONE', auquel cas une nouvelle 'registry' est créee dont la racine est stockée dans ce paramètre.
		xpp::context___ &Context );

	inline row__ Parse(
		xtf::extended_text_iflow__ &XFlow,
		const xpp::criterions___ &Criterions,
		registry_ &Registry,
		row__ &Root	) // Peut être = 'NONE', auquel cas une nouvelle 'registry' est créee dont la racine est stockée dans ce paramètre.
	{
		row__ Row = NONE;
	ERRProlog
		xpp::context___ Context;
	ERRBegin
		Context.Init();

		Row = Parse( XFlow, Criterions, Registry, Root, Context );
	ERRErr
	ERREnd
	ERREpilog
		return Row;
	}

	enum status__ {
		sOK,
		sUnableToOpenFile,
		sParseError,	// Pas d'entrée dans le fichier de traduction : ce sont les traductions du 'parser__' qui sont utilisés.
		sUnableToFindRootPath,
		sRootPathError,
		s_amount,
		s_Undefined
	};

	const char *Label( status__ Status );

	typedef xpp::context___ _context___;

	struct context___
	: public _context___
	{
		status__ Status;
		epeios::row__ PathErrorRow;
		void reset( bso::bool__ P = true )
		{
			_context___::reset( P );

			Status = s_Undefined;
			PathErrorRow = NONE;
		}
		context___( void )
		{
			reset( false );

			Init();
		}
		~context___( void )
		{
			reset();
		}
		void Init( void )
		{
			_context___::Init();

			Status = s_Undefined;
			PathErrorRow = NONE;
		}
	};


	const str::string_ &GetTranslation(
		const context___ &Context,
		const lcl::rack__ &LocaleRack,
		str::string_ &Translation );

	status__ FillRegistry(
		xtf::extended_text_iflow__ &XFlow,
		const xpp::criterions___ &Criterions,
		const char *RootPath,
		rgstry::registry_ &Registry,
		rgstry::row__ &RegistryRoot,
		context___ &Context );

	inline status__ FillRegistry(
		xtf::extended_text_iflow__ &XFlow,
		const xpp::criterions___ &Criterions,
		const char *RootPath,
		rgstry::registry_ &Registry,
		rgstry::row__ &RegistryRoot )
	{
		status__ Status = s_Undefined;
	ERRProlog
		context___ Context;
	ERRBegin
		Context.Init();

		Status = FillRegistry( XFlow, Criterions, RootPath, Registry, RegistryRoot, Context );
	ERRErr
	ERREnd
	ERREpilog
		return Status;
	}

	status__ FillRegistry(
		const char *FileName,
		const xpp::criterions___ &Criterions,
		const char *RootPath,
		rgstry::registry_ &Registry,
		rgstry::row__ &RegistryRoot,
		context___ &Context );

	inline status__ FillRegistry(
		const char *FileName,
		const xpp::criterions___ &Criterions,
		const char *RootPath,
		rgstry::registry_ &Registry,
		rgstry::row__ &RegistryRoot )
	{
		status__ Status = s_Undefined;
	ERRProlog
		context___ Context;
	ERRBegin
		Context.Init();

		Status = FillRegistry( FileName, Criterions, RootPath, Registry, RegistryRoot, Context );
	ERRErr
	ERREnd
	ERREpilog
		return Status;
	}

# if 1	// Déprécié, destiné à disparaître. Utiliser 'multi_level_registry_'.
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
				LocalRoot = Local.CreateRegistry( this->Global.Registry->GetName( Root, Buffer ) );

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

				Root = Registry.CreateRegistry( this->Global.Registry->GetName( Global.Root, Buffer ) );
			}

			return Local.Root = Root;
		}
		const value_ &GetValue(
			const str::string_ &PathString,
			bso::bool__ *Missing,
			buffer &Buffer,
			epeios::row__ *PathErrorRow = NULL  ) const;	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
		bso::bool__ GetValue(
			const str::string_ &PathString,
			value_ &Value ) const
		{
			buffer Buffer;
			bso::bool__ Missing = false;

			Value.Append( GetValue( PathString, &Missing, Buffer ) );

			return !Missing;
		}
		bso::bool__ GetValues(
			const str::string_ &PathString,
			values_ &Values,
			epeios::row__ *PathErrorRow = NULL ) const;
		void SetValue(
			const str::string_ &PathString,
			const value_ &Value,
			epeios::row__ *PathErrorRow = NULL )
		{
			Local.Registry->SetValue( PathString, Value, Local.Root, PathErrorRow );
		}
		void Delete( row__ Row )
		{
			Local.Registry->Delete( Row );
		}
		bso::bool__ Exists(
			const str::string_ &Path,
			epeios::row__ *PathErrorRow = NULL ) const;
		void Search(
			const path_ &Path,
			row__ &GlobalRow,
			row__ &LocalRow ) const
		{
			GlobalRow = Global.Registry->Search( Path, Global.Root );
			LocalRow = Local.Registry->Search( Path, Local.Root );
		}
		void Search(
			const str::string_ &Path,
			row__ &GlobalRow,
			row__ &LocalRow,
			epeios::row__ *PathErrorRow = NULL ) const;
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
				Root = Global.CreateRegistry( name() );

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
# endif
	E_ROW( level__ );
#	define RGSTRY_UNDEFINED_LEVEL	NONE

	struct _entry__ {
		row__ Root;
		const registry_ *Registry;
		void reset( bso::bool__ = true )
		{
			Root = NONE;
			Registry = NULL;
		}
		void Init(
			row__ Root = NONE,
			const registry_ &Registry = *(const registry_ *)NULL )
		{
			this->Root = Root;
			this->Registry = &Registry;
		}
		_entry__(
			row__ Root = NONE,
			const registry_ &Registry = *(const registry_ *)NULL )
		{
			Init( Root, Registry );
		}
	};


	typedef bch::E_BUNCHt_( _entry__, level__ ) _entries_;
	typedef bch::E_BUNCHt_( time_t, level__ ) _timestamps_;

	// Registre multi-niveau
	class multi_level_registry_
	{
	private:
		void _Touch( level__ Level )
		{
			TimeStamps.Store( time( NULL ), Level );
		}
		level__ _RawCreateLevel( void )
		{
			level__ Level = TimeStamps.New();

			if ( Entries.Append( _entry__() ) != Level )
				ERRc();

			_Touch( Level );

			return Level;
		}
		level__ _RawAddLevel( const _entry__ &Entry )
		{
			level__ Level = RGSTRY_UNDEFINED_LEVEL;

			Entries.Store( Entry, Level = _RawCreateLevel() );

			return Level;
		}
		const _entry__ _GetEntry( level__ Level ) const
		{
			return Entries( Level );
		}
		const registry_ &_GetRegistry( level__ Level ) const
		{
			_entry__ Entry = _GetEntry( Level );

			if ( Entry.Registry == NULL )
				return EmbeddedRegistry;
			else
				return *Entry.Registry;
		}
		registry_ &_GetRegistry( level__ Level )
		{
			_entry__ Entry = _GetEntry( Level );

			if ( Entry.Registry != NULL )
				ERRc();

			return EmbeddedRegistry;
		}
		row__ _GetRoot( level__ Level ) const
		{
			return _GetEntry( Level ).Root;
		}
	public:
		struct s {
			registry_::s EmbeddedRegistry;
			_entries_::s Entries;
			_timestamps_::s TimeStamps;
		};
		registry_ EmbeddedRegistry;
		_entries_ Entries;
		_timestamps_ TimeStamps;
		multi_level_registry_( s &S )
		: EmbeddedRegistry( S.EmbeddedRegistry ),
		  Entries( S.Entries ),
		  TimeStamps( S.TimeStamps )
		{}
		void reset( bso::bool__ P = true )
		{
			EmbeddedRegistry.reset( P );
			Entries.reset( P );
			TimeStamps.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			EmbeddedRegistry.plug( MM );
			Entries.plug( MM );
			TimeStamps.plug( MM );
		}
		multi_level_registry_ &operator =( const multi_level_registry_ &MLR )
		{
			EmbeddedRegistry = MLR.EmbeddedRegistry;
			Entries = MLR.Entries;
			TimeStamps = MLR.TimeStamps;

			return *this;
		}
		void Init( void )
		{
			EmbeddedRegistry.Init();
			Entries.Init();
			TimeStamps.Init();
		}
		E_NAVt( Entries., level__ );
		const registry_ &GetRegistry( level__ Level ) const
		{
			return _GetRegistry( Level );
		}
		registry_ &GetRegistry( level__ Level )
		{
			return _GetRegistry( Level );
		}
		row__ GetRoot( level__ Level ) const
		{
			return _GetRoot( Level );
		}
		void SetRoot(
			level__ Level,
			row__ Root )
		{
			_entry__ Entry = Entries( Level );

			Entry.Root = Root;

			Entries.Store( Entry, Level );
		}
/*		level__ CreateImportedLevel(
			row__ Row,
			const registry_ &Registry )
		{
			return _RawCreateLevel( _entry__( Row, Registry ) );
		}
		level__ CreatedEmbeddedLevel( void )
		{
			return _RawCreateLevel( _entry__() );
		}
*/
		level__ AddImportedLevel(
			row__ Root,
			const registry_ &Registry )
		{
			return _RawAddLevel( _entry__( Root, Registry ) );
		}
		level__ AddEmbeddedLevel( const name_ &Name = name() )
		{
			return _RawAddLevel( _entry__( EmbeddedRegistry.CreateRegistry( Name ) ) );
		}
		void Add( const multi_level_registry_ &Registry )
		{
			level__ Level = Registry.First();

			while ( Level != RGSTRY_UNDEFINED_LEVEL ) {
				AddImportedLevel( Registry.GetRoot( Level ), Registry.GetRegistry( Level ) );

				Level = Registry.Next( Level );
			}
		}
		void Create(
			level__ Level,
			const str::string_ &Path )
		{
			_GetRegistry( Level ).Create( Path, _GetRoot( Level ) );
		}
		const value_ &GetValue(
			level__ Level,
			const path_ &Path,
			bso::bool__ *Missing,
			buffer &Buffer ) const	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
		{
			return _GetRegistry( Level ).GetValue( Path, _GetRoot( Level ), Missing, Buffer );
		}
		const value_ &GetValue(
			level__ Level,
			const str::string_ &PathString,
			bso::bool__ *Missing,
			buffer &Buffer,
			epeios::row__ *PathErrorRow = NULL  ) const	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
		{
			return _GetRegistry( Level ).GetValue( PathString, _GetRoot( Level ), Missing, Buffer, PathErrorRow );
		}
		const value_ &GetValue(
			const str::string_ &PathString,
			bso::bool__ *Missing,
			buffer &Buffer,
			epeios::row__ *PathErrorRow = NULL  ) const;	// Nota : ne met 'Missing' à 'true' que lorque 'Path' n'existe pas. Si 'Missing' est à 'true', aucune action n'est réalisée.
		bso::bool__ GetValue(
			level__ Level,
			const path_ &Path,
			value_ &Value ) const
		{
			return _GetRegistry( Level ).GetValue( Path, _GetRoot( Level ), Value );
		}
		bso::bool__ GetValue(
			level__ Level,
			const str::string_ &PathString,
			value_ &Value,
			epeios::row__ *PathErrorRow = NULL ) const
		{
			buffer Buffer;
			bso::bool__ Missing = false;

			Value.Append( GetValue( Level, PathString, &Missing, Buffer, PathErrorRow ) );

			return !Missing;
		}
		bso::bool__ GetValue(
			const str::string_ &PathString,
			value_ &Value,
			epeios::row__ *PathErrorRow = NULL ) const;
		bso::bool__ GetValue(
			const char *PathString,
			value_ &Value,
			epeios::row__ *PathErrorRow = NULL ) const
		{
			return GetValue( str::string( PathString ), Value, PathErrorRow );
		}
		bso::bool__ GetValues(
			level__ Level,
			const path_ &Path,
			values_ &Values ) const
		{
			return _GetRegistry( Level ).GetValues( Path, _GetRoot( Level ), Values );
		}
		bso::bool__ GetValues(
			level__ Level,
			const str::string_ &PathString,
			values_ &Values,
			epeios::row__ *PathErrorRow = NULL ) const
		{
			return _GetRegistry( Level ).GetValues( PathString, _GetRoot( Level ), Values, PathErrorRow );
		}
		bso::bool__ GetValues(
			const str::string_ &PathString,
			values_ &Values,
			epeios::row__ *PathErrorRow = NULL ) const;
		bso::bool__ GetValues(
			const char *PathString,
			values_ &Values,
			epeios::row__ *PathErrorRow = NULL ) const
		{
			return GetValues( str::string( PathString ), Values, PathErrorRow );
		}
		void SetValue(
			level__ Level,
			const str::string_ &PathString,
			const value_ &Value,
			epeios::row__ *PathErrorRow = NULL )
		{
			_GetRegistry( Level ).SetValue( PathString, Value, _GetRoot( Level ), PathErrorRow );

			_Touch( Level );
		}
		bso::bool__ SetValue(
			const str::string_ &PathString,
			const value_ &Value,
			epeios::row__ *PathErrorRow = NULL );	// Retourne 'false' si 'PathString' a déjà la valeur 'Value', 'true' sinon.
		bso::bool__ Delete(
			const path_ &Path,
			level__ Level )
		{
			if ( _GetRegistry( Level ).Delete( Path, _GetRoot( Level ) ) ) {
				_Touch( Level );
				return true;
			} else
				return false;
		}
		bso::bool__ Delete( 
			const str::string_ &PathString,
			level__ Level,
			epeios::row__ *PathErrorRow = NULL )	// Retourne 'false' si 'PathString' a déjà la valeur 'Value', 'true' sinon.
		{
			if ( _GetRegistry( Level ).Delete( PathString, _GetRoot( Level ), PathErrorRow ) ) {
				_Touch( Level );
				return true;
			} else
				return false;
		}
		bso::bool__ Delete( 
			const char *PathString,
			level__ Level,
			epeios::row__ *PathErrorRow = NULL )	// Retourne 'false' si 'PathString' a déjà la valeur 'Value', 'true' sinon.
		{
			return Delete( str::string( PathString ), Level, PathErrorRow );
		}
		bso::bool__ Delete(
			const str::string_ &PathString,
			epeios::row__ *PathErrorRow = NULL );	// Retourne 'false' si 'PathString' a déjà la valeur 'Value', 'true' sinon.
		bso::bool__ Delete(
			const char *PathString,
			epeios::row__ *PathErrorRow = NULL )	// Retourne 'false' si 'PathString' a déjà la valeur 'Value', 'true' sinon.
		{
			return Delete( str::string( PathString ), PathErrorRow );
		}
		bso::bool__ MoveTo(
			const str::string_ &Path,
			level__ Level );
		bso::bool__ MoveTo(
			const char *Path,
			level__ Level )
		{
			return MoveTo( str::string( Path ), Level );
		}
		row__ Search(
			level__ Level,
			const path_ &Path ) const
		{
			return _GetRegistry( Level ).Search( Path, _GetRoot( Level ) );
		}
		row__ Search(
			level__ Level,
			const str::string_ &PathString,
			epeios::row__ *PathErrorRow = NULL ) const
		{
			return _GetRegistry( Level ).Search( PathString, _GetRoot( Level ), PathErrorRow );
		}
		row__ Search(
			const str::string_ &PathString,
			epeios::row__ *PathErrorRow = NULL ) const;
		bso::bool__ Exists(
			level__ Level,
			const path_ &Path ) const
		{
			return Search( Level, Path ) != NONE;
		}
		bso::bool__ Exists(
			level__ Level,
			const str::string_ &PathString,
			epeios::row__ *PathErrorRow = NULL ) const
		{
			return Search( Level, PathString, PathErrorRow ) != NONE;
		}
		bso::bool__ Exists(
			const str::string_ &PathString,
			epeios::row__ *PathErrorRow = NULL ) const
		{
			return Search( PathString, PathErrorRow ) != NONE;
		}
		status__ Fill(
			level__ Level,
			xtf::extended_text_iflow__ &XFlow,
			const xpp::criterions___ &Criterions,
			const char *RootPath,
			context___ &Context )
		{
			status__ Status = s_Undefined;
			row__ Root = _GetRoot( Level );
			
			Status = FillRegistry( XFlow, Criterions, RootPath, _GetRegistry( Level ), Root, Context ); 

			Entries.Store( _entry__( Root ), Level );

			_Touch( Level );

			return Status;
		}
		status__ Fill(
			level__ Level,
			xtf::extended_text_iflow__ &XFlow,
			const xpp::criterions___ &Criterions,
			const char *RootPath )
		{
			status__ Status = s_Undefined;
			row__ Root = _GetRoot( Level );
			
			Status = FillRegistry( XFlow, Criterions, RootPath, _GetRegistry( Level ), Root ); 

			Entries.Store( _entry__( Root ), Level );

			_Touch( Level );

			return Status;
		}
		status__ Fill(
			level__ Level,
			const char *FileName,
			const xpp::criterions___ &Criterions,
			const char *RootPath,
			context___ &Context )
		{
			status__ Status = s_Undefined;
			row__ Root = _GetRoot( Level );
			
			Status = FillRegistry( FileName, Criterions, RootPath, _GetRegistry( Level ), Root, Context ); 

			Entries.Store( _entry__( Root ), Level );

			_Touch( Level );

			return Status;
		}
		status__ Fill(
			level__ Level,
			const char *FileName,
			const xpp::criterions___ &Criterions,
			const char *RootPath )
		{
			status__ Status = s_Undefined;
			row__ Root = _GetRoot( Level );
			
			Status = FillRegistry( FileName, Criterions, RootPath, _GetRegistry( Level ), Root ); 

			Entries.Store( _entry__( Root ), Level );

			_Touch( Level );

			return Status;
		}
		epeios::size__ Dump(
			level__ Level,
			bso::bool__ RootToo,
			xml::writer_ &Writer ) const
		{
			return _GetRegistry( Level ).Dump( _GetRoot( Level ), RootToo, Writer );
		}
		epeios::size__ Dump(
			level__ Level,
			bso::bool__ RootToo,
			xml::outfit__ Outfit,
			xml::encoding__ Encoding,
			txf::text_oflow__ &TFlow ) const
		{
			return _GetRegistry( Level ).Dump( _GetRoot( Level ), RootToo, Outfit, Encoding, TFlow );
		}
		time_t TimeStamp( level__ Level ) const
		{
			return TimeStamps( Level );
		}
	};

	E_AUTO( multi_level_registry )

	template <typename registry, typename path> inline str::_guint__ _GetUnsigned(
		const registry &Registry,
		const path &Path,
		str::_guint__ Default,
		bso::bool__ *Error,
		str::_guint__ Min,
		str::_guint__ Max )
	{
		str::_guint__ Value = Default;
	ERRProlog
		str::string RawValue;
		epeios::row__ GenericError = NONE;
	ERRBegin
		RawValue.Init();

		if ( Registry.GetValue( Path, RawValue, &GenericError ) )
			Value = str::_GenericUnsignedConversion( RawValue, 0, &GenericError, str::bAuto, Max );

		if ( ( GenericError != NONE ) || ( Value < Min ) ) {

			Value = Default;

			if ( Error != NULL )
				*Error = true;
			else
				ERRu();
		}

	ERRErr
	ERREnd
	ERREpilog
		return Value;
	}


	template <typename registry, typename path> inline str::_gsint__ _GetSigned(
		const registry &Registry,
		const path &Path,
		str::_gsint__ Default,
		bso::bool__ *Error,
		str::_gsint__ Min,
		str::_gsint__ Max )
	{
		str::_gsint__ Value = Default;
	ERRProlog
		str::string RawValue;
		epeios::row__ GenericError = NONE;
	ERRBegin
		RawValue.Init();

		if ( Registry.GetValue( Path, RawValue, &GenericError ) )
			Value = str::_GenericSignedConversion( RawValue, 0, &GenericError, str::bAuto, Min, Max );

		if ( ( GenericError != NONE ) ) {

			Value = Default;

			if ( Error != NULL )
				*Error = true;
			else
				ERRu();
		}

	ERRErr
	ERREnd
	ERREpilog
		return Value;
	}

# ifdef _MU
#  define RGSRTY__MS_BACKUP	_MU
# endif
	
#define _MU( name, type, min, max )\
	type Get##name(\
		const multi_level_registry_ &Registry,\
		type Default,\
		const tags_ &Tags = tags(),\
		bso::bool__ *Error = NULL,\
		type Min = min,\
		type Max = max )\
	{\
		type Value = 0;\
	ERRProlog\
		str::string Buffer;\
	ERRBegin\
		Buffer.Init();\
		Value = (type)_GetUnsigned( Registry, GetPath( Tags, Buffer ), Default, Error, Min, Max );\
	ERRErr\
	ERREnd\
	ERREpilog\
		return Value;\
	}


# ifdef _MS
#  define RGSRTY__MU_BACKUP	_MS
# endif

#define _MS( name, type, min, max )\
	type Get##name(\
		const multi_level_registry_ &Registry,\
		type Default,\
		const tags_ &Tags = tags(),\
		bso::bool__ *Error = NULL,\
		type Min = min,\
		type Max = max )\
	{\
		type Value = 0;\
	ERRProlog\
		str::string Buffer;\
	ERRBegin\
		Buffer.Init();\
		Value = (type)_GetSigned( Registry, GetPath( Tags, Buffer ), Default, Error, Min, Max );\
	ERRErr\
	ERREnd\
	ERREpilog\
		return Value;\
	}

	typedef values_	tags_;
	E_AUTO( tags );

	class entry___
	{
	private:
		void _GetParentPath(
			bso::bool__ NoTailingSlash,
			str::string_ &Path ) const
		{
			if ( _Parent != NULL ) {
				_Parent->GetPath( Path );

				if ( Path.Amount() != 0 ) {
					if ( Path( Path.Last() ) == '/' ) {
						if ( NoTailingSlash )
							Path.Remove( Path.Last() );
					} else if ( !NoTailingSlash )
						Path.Append( '/' );
				}
			}
		}
		const bso::bool__ _IsPostInitialized( void ) const
		{
			return ( ( _Path.Amount() != 0 )
				     || ( _RawPath == NULL )
					 || ( *_RawPath == 0 ) );
		}
		void _PostInitialize( void ) const;
		const str::string_ &_GetPath(
			const tags_ &Tags,
			str::string_ &Path ) const;
	private:
		const entry___ *_Parent;
		const char *_RawPath;
		mutable str::string _Path;
	public:
		void reset( bso::bool__ P = true )
		{
			_Parent = NULL;
			_RawPath = NULL;
			_Path.reset( P );
		}
		entry___(
			const char *Path = NULL,	// Non dupliqué !
			const entry___ &Parent = *(const entry___ *)NULL )
		{
			reset( false );
			Init( Path, Parent );
		}
		void Init(
			const char *Path = NULL,	// Non dupliqué !
			const entry___ &Parent = *(const entry___ *)NULL )
		{
			_Parent = &Parent;
			_RawPath = Path;

			_Path.Init();
		}
		const str::string_ &GetPath(
			const tags_ &Tags,
			str::string_ &Path ) const
		{
			if ( !_IsPostInitialized() )
				_PostInitialize();

			return _GetPath( Tags, Path );
		}
		const str::string_ &GetPath( str::string_ &Path ) const
		{
			return GetPath( tags(), Path );
		}
		bso::bool__ SetValue(
			multi_level_registry_ &Registry,
			const tags_ &Tags,
			const str::string_ &Value ) const
		{
			bso::bool__ Made = false;
		ERRProlog
			str::string Buffer;
		ERRBegin
			Buffer.Init();
			Made = Registry.SetValue( GetPath( Tags, Buffer ), Value );
		ERRErr
		ERREnd
		ERREpilog
			return Made;
		}
		bso::bool__ SetValue(
			multi_level_registry_ &Registry,
			const str::string_ &Value ) const
		{
			return SetValue( Registry, tags(), Value );
		}
		bso::bool__ GetValue(
			const multi_level_registry_ &Registry,
			const tags_ &Tags,
			str::string_ &Value ) const
		{
			bso::bool__ Made = false;
		ERRProlog
			str::string Buffer;
		ERRBegin
			Buffer.Init();
			Made = Registry.GetValue( GetPath( Tags, Buffer ), Value );
		ERRErr
		ERREnd
		ERREpilog
			return Made;
		}
		bso::bool__ GetValue(
			const multi_level_registry_ &Registry,
			str::string_ &Value ) const
		{
			return GetValue( Registry, tags(), Value );
		}
		bso::bool__ GetValues(
			const multi_level_registry_ &Registry,
			const tags_ &Tags,
			values_ &Values ) const
		{
			bso::bool__ Made = false;
		ERRProlog
			str::string Buffer;
		ERRBegin
			Buffer.Init();
			Made = Registry.GetValues( GetPath( Tags, Buffer ), Values );
		ERRErr
		ERREnd
		ERREpilog
			return Made;
		}
		bso::bool__ GetValues(
			const multi_level_registry_ &Registry,
			values_ &Values ) const
		{
			return GetValues( Registry, tags(), Values );
		}
# ifdef CPE__64_BITS_TYPES_ALLOWED
		_MU( ULL, bso::ullong__, BSO_ULLONG_MIN, BSO_ULLONG_MAX )
		_MS( SLL, bso::ullong__, BSO_SLLONG_MIN, BSO_SLLONG_MAX )
# endif
		_MU( UL, bso::ulong__, BSO_ULONG_MIN, BSO_ULONG_MAX )
		_MS( SL, bso::slong__, BSO_SLONG_MIN, BSO_SLONG_MAX )
		_MU( US, bso::ushort__, BSO_USHORT_MIN, BSO_USHORT_MAX )
		_MS( SS, bso::sshort__, BSO_SSHORT_MIN, BSO_SSHORT_MAX )
		_MU( UB, bso::ubyte__, BSO_UBYTE_MIN, BSO_UBYTE_MAX )
		_MS( SB, bso::sbyte__, BSO_SBYTE_MIN, BSO_SBYTE_MAX )
	};

# undef _MU
# undef _MS

# ifdef RGSRTY__MS_BACKUP
#  define _MS RGSRTY__MS_BACKUP
# endif


#ifdef _M
#	define RGSRTY__M_BACKUP	_M
#endif

#define _M( name, type, min, max )\
	template <typename registry, typename path> inline type Get##name(\
		const registry &Registry,\
		const path &Path,\
		type Default,\
		bso::bool__ *Error = NULL,\
		type Min = min,\
		type Max = max )\
	{\
		return (type)_GetUnsigned( Registry, Path, Default, Error, Min, Max );\
	}

#ifdef CPE__64_BITS_TYPES_ALLOWED
	_M( ULL, bso::ullong__, BSO_ULLONG_MIN, BSO_ULLONG_MAX )
#endif
	_M( UL, bso::ulong__, BSO_ULONG_MIN, BSO_ULONG_MAX )
	_M( US, bso::ushort__, BSO_USHORT_MIN, BSO_USHORT_MAX )
	_M( UB, bso::ubyte__, BSO_UBYTE_MIN, BSO_UBYTE_MAX )

# undef _M

# ifdef RGSRTY__M_BACKUP
#  define _M RGSRTY__M_BACKUP
# endif


#ifdef _M
#	define RGSRTY__M_BACKUP	_M
#endif

#define _M( name, type, min, max )\
	template <typename registry, typename path> inline type Get##name(\
		const registry &Registry,\
		const path &Path,\
		type Default,\
		bso::bool__ *Error = NULL,\
		type Min = min,\
		type Max = max )\
	{\
		return (type)_GetSigned( Registry, Path, Default, Error, Min, Max );\
	}

#ifdef CPE__64_BITS_TYPES_ALLOWED
	_M( SLL, bso::ullong__, BSO_SLLONG_MIN, BSO_SLLONG_MAX )
#endif
	_M( SL, bso::slong__, BSO_SLONG_MIN, BSO_SLONG_MAX )
	_M( SS, bso::sshort__, BSO_SSHORT_MIN, BSO_SSHORT_MAX )
	_M( SB, bso::sbyte__, BSO_SBYTE_MIN, BSO_SBYTE_MAX )


# undef _M

# ifdef RGSRTY__M_BACKUP
# define _M RGSRTY__M_BACKUP
# endif

}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
