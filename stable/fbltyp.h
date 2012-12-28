/*
	Header for the 'fbltyp' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#ifndef FBLTYP__INC
#define FBLTYP__INC

#define FBLTYP_NAME		"FBLTYP"

#define	FBLTYP_VERSION	"$Revision$"

#define FBLTYP_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLTYPTutor;

#if defined( XXX_DBG ) && !defined( FBLTYP_NODBG )
#define FBLTYP_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout TYPes 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "str.h"
#include "bitbch.h"

#ifdef CPE__VC
#	undef GetObject
#endif

#define FBLTYP_UNDEFINED_ID32	((fbltyp::id32__)BSO_ULONG_MAX)
#define FBLTYP_UNDEFINED_ID16	((fbltyp::id16__)BSO_USHORT_MAX)
#define FBLTYP_UNDEFINED_ID8	((fbltyp::id8__)BSO_UBYTE_MAX)

#define FBLTYP_ITEM( id, name )\
	typedef fbltyp::item_<id>	name##_;\
	E_AUTO( name )

#define FBLTYP_ITEMS( item, name )\
	typedef ctn::E_MCONTAINER_( item##_ )	name##_;\
	E_AUTO( name )

#define FBLTYP_XITEM( id, name )\
	typedef fbltyp::extended_item_<id>	name##_;\
	E_AUTO( name )

#define FBLTYP_XITEMS( item, name )\
	typedef ctn::E_CONTAINER_( item##_ )	name##_;\
	E_AUTO( name )

# define FBLTYP_MIMIC( stype, sname )	E_TMIMIC__( stype##__, sname##__ )

# define FBLTYP_MIMICS( stype, btype, sname, bname )\
	FBLTYP_MIMIC( stype, sname );\
	class bname##_\
	: public bch::E_BUNCH_( sname##__ )\
	{\
	public:\
		struct s\
		: public bch::E_BUNCH_( sname##__ )::s\
		{};\
		bname##_( s &S )\
		: bch::E_BUNCH_( sname##__ )( S )\
		{}\
		btype##_ &operator *( void )\
		{\
			return *(btype##_ *)this;\
		}\
		const btype##_ &operator *( void ) const\
		{\
			return *(const btype##_ *)this;\
		}\
	};\
	E_AUTO( bname )

# define FBLTYP_MIMICX( stype, btype, xtype, sname, bname, xname )\
	FBLTYP_MIMICS( stype, btype, sname, bname );\
	E_TMIMIC( xtype, xname );

# define FBLTYP_MIMIC_ID8( sname ) FBLTYP_MIMIC( fbltyp::id8, sname )
# define FBLTYP_MIMIC_ID16( sname ) FBLTYP_MIMIC( fbltyp::id16, sname )
# define FBLTYP_MIMIC_ID32( sname ) FBLTYP_MIMIC( fbltyp::id32, sname )

# define FBLTYP_MIMIC_IDS8( sname, bname ) FBLTYP_MIMICS( fbltyp::id8, fbltyp::ids8, sname, bname )
# define FBLTYP_MIMIC_IDS16( sname, bname ) FBLTYP_MIMICS( fbltyp::id16, fbltyp::ids16, sname, bname )
# define FBLTYP_MIMIC_IDS32( sname, bname ) FBLTYP_MIMICS( fbltyp::id32, fbltyp::ids32, sname, bname )

# define FBLTYP_MIMIC_XIDS8( sname, bname, xname ) FBLTYP_MIMICX( fbltyp::id8, fbltyp::ids8, fbltyp::xids8, sname, bname, xname )
# define FBLTYP_MIMIC_XIDS16( sname, bname, xname ) FBLTYP_MIMICX( fbltyp::id16, fbltyp::ids16, fbltyp::xids16, sname, bname, xname )
# define FBLTYP_MIMIC_XIDS32( sname, bname, xname ) FBLTYP_MIMICX( fbltyp::id32, fbltyp::ids32, fbltyp::xids32, sname, bname, xname )

# define FBLTYP_MIMIC_ULONG( sname ) FBLTYP_MIMIC( fbltyp::ulong, sname )
# define FBLTYP_MIMIC_ULONGS( sname, bname ) FBLTYP_MIMICS( fbltyp::ulong, fbltyp::ulongs, sname, bname )


namespace fbltyp {
	//t Value.
	typedef str::string_ value_;
	typedef str::string value;

	template <class id> class item_
	{
	public:
		value_ Value;
		struct s
		{
			value_::s Value;
			id ID;
		} &S_;
		item_( s &S )
		: S_( S ),
		  Value( S.Value )
		{}
		void reset( bool P = true )
		{
			Value.reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Value.plug( MD );
		}
		void plug( mmm::multimemory_ &M )
		{
			Value.plug( M );
		}
		item_ &operator =( const item_ &I )
		{
			Value = I.Value;
			S_.ID = I.S_.ID;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Value.Init();
		}
		//f Initialization with id 'ID' and value 'Value'.
		void Init(
			id ID,
			const value_ &Value )
		{
			this->Value.Init( Value );
			S_.ID = ID;
		}
		//f Return the value of the ID.
		id ID( void ) const
		{
			return S_.ID;
		}
		//f 'ID' becomes the ID.
		void ID( id ID ) 
		{
			S_.ID = ID;
		}
	};

	//f Values.
	typedef ctn::E_MCONTAINER_( value_ ) values_;
	E_AUTO( values )

	template <class id> class extended_item_
	{
	public:
		values_ Values;
		struct s
		{
			values_::s Values;
			id ID;
		} &S_;
		extended_item_( s &S )
		: S_( S ),
		  Values( S.Values )
		{}
		void reset( bool P = true )
		{
			Values.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Values.plug( M );
		}
		extended_item_ &operator =( const extended_item_ &I )
		{
			Values = I.Values;
			S_.ID = I.S_.ID;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Values.Init();
		}
		//f Return the value of the ID.
		id ID( void ) const
		{
			return S_.ID;
		}
		//f 'ID' becomes the ID.
		void ID( id ID ) 
		{
			S_.ID = ID;
		}
	};

	//t Name.
	typedef str::string_ name_;
	E_AUTO( name )

	template <class t, typename r> class detail_
	{
	public:
		name_ Name;
		bch::E_BUNCHt_( t, r ) Casts;
		struct s {
			name_::s Name;
			typename bch::E_BUNCHt_( t, r )::s Casts;
		};
		detail_( s &S )
		: Name( S.Name ),
		  Casts( S.Casts )
		{}
		void reset( bool P = true )
		{
			Name.reset( P );
			Casts.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Name.plug( M );
			Casts.plug( M );
		}
		detail_ &operator =( const detail_ &CI )
		{
			Name = CI.Name;
			Casts = CI.Casts;

			return *this;
		}
		// Initialization.
		void Init( void )
		{
			Name.Init();
			Casts.Init();
		}
	};

	E_AUTO2( detail )

	struct object_reference__
	{
		bso::ushort__ Type;
		bso::ushort__ Identifier;
	};

	typedef bso::ushort__	object_t__;
	E_TMIMIC__( object_t__, object__ );

	typedef bso::bool__ boolean_t__;
	typedef boolean_t__	boolean__;
	typedef bitbch::bit_bunch_<mdr::row__>	booleans_t_;
	E_TMIMIC( booleans_t, booleans );

	typedef bso::slong__ slong__;
	typedef bch::E_BUNCH_( slong__ ) slongs_t_;
	E_TMIMIC( slongs_t, slongs );

	typedef bso::ulong__	ulong__;
	typedef bch::E_BUNCH_( ulong__ ) ulongs_t_;
	E_TMIMIC( ulongs_t, ulongs );

	typedef bso::ubyte__ id8_t__;
	typedef id8_t__ id8__;
	typedef bch::E_BUNCH_( id8__ ) ids8_t_;
	E_TMIMIC( ids8_t, ids8 );
	typedef ctn::E_MCONTAINER_( ids8_ ) xids8_t_;
	E_TMIMIC( xids8_t, xids8 );

	typedef bso::ushort__ id16_t__;
	typedef id16_t__ id16__;
	typedef bch::E_BUNCH_( id16__ ) ids16_t_;
	E_TMIMIC( ids16_t, ids16 );
	typedef ctn::E_MCONTAINER_( ids16_ ) xids16_t_;
	E_TMIMIC( xids16_t, xids16 );

	typedef bso::ulong__ id32_t__;
	typedef id32_t__ id32__;
	typedef bch::E_BUNCH_( id32__ ) ids32_t_;
	E_TMIMIC( ids32_t, ids32 );
	typedef ctn::E_MCONTAINER_( ids32_ ) xids32_t_;
	E_TMIMIC( xids32_t, xids32 );

	typedef bso::char__ char_t__;
	typedef char_t__	char__;
	typedef str::string_	string_;
	typedef str::string		string;
	typedef ctn::E_MCONTAINER_( string_ )	strings_;
	typedef ctn::E_MCONTAINER( string_ )	strings;
	typedef ctn::E_CONTAINER_( strings_ )	xstrings_;
	typedef ctn::E_CONTAINER( strings_ )	xstrings;
	typedef bso::raw__	byte_t__;
	typedef byte_t__	byte__;
	typedef bch::E_BUNCH_( bso::raw__ ) binary_;
	typedef bch::E_BUNCH( bso::raw__ )	binary;
	typedef ctn::E_MCONTAINER_( binary_ ) binaries_;
	typedef ctn::E_MCONTAINER( binary_ ) binaries;
	FBLTYP_ITEM( id8__, item8 )
	FBLTYP_ITEMS( item8, items8 )
	FBLTYP_ITEM( id16__, item16 )
	FBLTYP_ITEMS( item16, items16 )
	FBLTYP_ITEM( id32__, item32 )
	FBLTYP_ITEMS( item32, items32 )
	FBLTYP_XITEM( id8__, xitem8 )
	FBLTYP_XITEMS( xitem8, xitems8 )
	FBLTYP_XITEM( id16__, xitem16 )
	FBLTYP_XITEMS( xitem16, xitems16 )
	FBLTYP_XITEM( id32__, xitem32 )
	FBLTYP_XITEMS( xitem32, xitems32 )
	/* Both below declaration are not part of the protocol. */
	typedef fbltyp::detail_<id8__, mdr::row__>	command_detail_	;
	typedef fbltyp::detail<id8__, mdr::row__>	command_detail;
	/**/
	typedef ctn::E_CONTAINER_( command_detail_ ) commands_details_;
	typedef ctn::E_CONTAINER( command_detail_ ) commands_details;
	typedef bch::E_BUNCH_( object_reference__ ) objects_references_;
	typedef bch::E_BUNCH( object_reference__ ) objects_references;

	template <typename t> inline t *_New( void )
	{
		t *P = new t;

		if ( P == NULL )
			ERRa();

		return P;
	}

	template <typename t> inline void _Delete( t *P )
	{
		delete P;
	}

	template <typename t> inline t *_SNew( void )
	{
		return _New<t>();
	}

	template <typename t> inline void _SDelete( t *P )
	{
		_Delete( P );
	}

	template <typename t> inline t *_DNew( void )
	{
		t *P = _New<t>();

		P->Init();

		return P;
	}

	template <typename t> inline void _DDelete( t *P )
	{
		_Delete( P );
	}

#define FBLTYP_S( name, type )\
	inline type *New##name( void )\
	{\
		return _SNew<type>();\
	}\
	inline void Delete##name( type *P )\
	{\
		_SDelete( P );\
	}\

#define FBLTYP_D( name, type )\
	inline type *New##name( void )\
	{\
		return _DNew<type>();\
	}\
	inline void Delete##name( type *P )\
	{\
		_DDelete( P );\
	}\


	//f Put 'Object' into 'OFlow'.
	inline void PutObject(
		object__ Object,
		flw::oflow__ &OFlow )
	{
		flw::Put( Object, OFlow );
	}

	//f Return 'Object' from 'IFlow'.
	inline void GetObject(
		flw::iflow__ &IFlow,
		object__ &Object )
	{
		flw::Get( IFlow, Object );
	}

	FBLTYP_S( Object, object__ )
	
	//f Put 'Boolean' into 'OFlow'.
	inline void PutBoolean(
		boolean__ Boolean,
		flw::oflow__ &OFlow )
	{
		flw::Put( Boolean, OFlow );
	}

	//f Return 'Boolean' from 'IFlow'.
	inline void GetBoolean(
		flw::iflow__ &IFlow,
		boolean__ &Boolean )
	{
		flw::Get( IFlow, Boolean );
	}

	FBLTYP_S( Boolean, boolean__ )
	
	//f Put 'Booleans' into 'OFlow'.
	void PutBooleans(
		const booleans_ &Booleans,
		flw::oflow__ &OFlow );

	//f Return 'Booleans' from 'IFlow'.
	void GetBooleans(
		flw::iflow__ &IFlow,
		booleans_ &Booleans );

	FBLTYP_D( Booleans, booleans )

	//f Put 'Slong' into 'Flow'.
	inline void PutSLong(
		slong__ SLong,
		flw::oflow__ &OFlow )
	{
		dtfptb::FittedPutSLong( SLong, OFlow );
	}

	//f Get 'SLong' from 'IFlow'.
	inline void GetSLong(
		flw::iflow__ &IFlow,
		slong__ &SLong )
	{
		SLong = dtfptb::FittedGetSLong( IFlow );
	}
	
	FBLTYP_S( SLong, slong__ )

	//f Put 'Slongs' into 'Flow'.
	void PutSLongs(
		const slongs_ &SLongs,
		flw::oflow__ &OFlow );

	//f Get 'SLongs' from 'IFlow'.
	void GetSLongs(
		flw::iflow__ &IFlow,
		slongs_ &SLongs );

	FBLTYP_D( SLongs, slongs )

	//f Put 'Ulong' into 'Flow'.
	inline void PutULong(
		ulong__ ULong,
		flw::oflow__ &OFlow )
	{
		dtfptb::FittedPutULong( ULong, OFlow );
	}

	//f Get 'ULong' from 'IFlow'.
	inline void GetULong(
		flw::iflow__ &IFlow,
		ulong__ &ULong )
	{
		ULong = dtfptb::FittedGetULong( IFlow );
	}
	
	FBLTYP_S( ULong, ulong__ )

	//f Put 'Ulongs' into 'Flow'.
	void PutULongs(
		const ulongs_ &ULongs,
		flw::oflow__ &OFlow );

	//f Get 'ULongs' from 'IFlow'.
	void GetULongs(
		flw::iflow__ &IFlow,
		ulongs_ &ULongs );

	FBLTYP_D( ULongs, ulongs )

	//f Put 'Id8' into 'OFlow'.
	inline void PutId8(
		id8__ Id8,
		flw::oflow__ &OFlow )
	{
		flw::Put( Id8, OFlow );
	}

	//f Return 'Id8' from 'IFlow'.
	inline void GetId8(
		flw::iflow__ &IFlow,
		id8__ &Id8 )
	{
		flw::Get( IFlow, Id8 );
	}
	
	FBLTYP_S( Id8, id8__ )

	//f Put 'Ids8' into 'OFlow'.
	void PutIds8(
		const ids8_ &Ids8,
		flw::oflow__ &OFlow );

	//f Get 'Ids8' from 'OFlow'.
	void GetIds8(
		flw::iflow__ &IFlow,
		ids8_ &Ids8 );

	FBLTYP_D( Ids8, ids8 )

	//f Put 'XIds8' into 'OFlow'.
	void PutXIds8(
		const xids8_ &XIds8,
		flw::oflow__ &OFlow );

	//f Get 'XIds8' from 'OFlow'.
	void GetXIds8(
		flw::iflow__ &IFlow,
		xids8_ &XIds8 );

	FBLTYP_D( XIds8, xids8 )

	//f Put 'Id16' into 'OFlow'.
	inline void PutId16(
		id16__ Id16,
		flw::oflow__ &OFlow )
	{
		flw::Put( Id16, OFlow );
	}

	//f Return 'Id16' from 'IFlow'.
	inline void GetId16(
		flw::iflow__ &IFlow,
		id16__ &Id16 )
	{
		flw::Get( IFlow, Id16 );
	}
	
	FBLTYP_S( Id16, id16__ )

	//f Put 'Ids16' into 'OFlow'.
	void PutIds16(
		const ids16_ &Ids16,
		flw::oflow__ &OFlow );

	//f Get 'Ids16' from 'OFlow'.
	void GetIds16(
		flw::iflow__ &IFlow,
		ids16_ &Ids16 );

	FBLTYP_D( Ids16, ids16 )

	//f Put 'XIds16' into 'OFlow'.
	void PutXIds16(
		const xids16_ &XIds16,
		flw::oflow__ &OFlow );

	//f Get 'XIds16' from 'OFlow'.
	void GetXIds16(
		flw::iflow__ &IFlow,
		xids16_ &XIds16 );

	FBLTYP_D( XIds16, xids16 )

	//f Put 'Id32' into 'OFlow'.
	inline void PutId32(
		id32__ Id32,
		flw::oflow__ &OFlow )
	{
		flw::Put( Id32, OFlow );
	}

	//f Return 'Id32' from 'IFlow'.
	inline void GetId32(
		flw::iflow__ &IFlow,
		id32__ &Id32 )
	{
		flw::Get( IFlow, Id32 );
	}
	
	FBLTYP_S( Id32, id32__ )

	//f Put 'Ids32' into 'OFlow'.
	void PutIds32(
		const ids32_ &Ids32,
		flw::oflow__ &OFlow );

	//f Get 'Ids32' from 'OFlow'.
	void GetIds32(
		flw::iflow__ &IFlow,
		ids32_ &Ids32 );

	FBLTYP_D( Ids32, ids32 )

	//f Put 'XIds32' into 'OFlow'.
	void PutXIds32(
		const xids32_ &XIds32,
		flw::oflow__ &OFlow );

	//f Get 'XIds32' from 'OFlow'.
	void GetXIds32(
		flw::iflow__ &IFlow,
		xids32_ &XIds32 );

	FBLTYP_D( XIds32, xids32 )

	//f Put 'Char' into 'OFlow'.
	inline void PutChar(
		char__ Char,
		flw::oflow__ &OFlow )
	{
		flw::Put( Char, OFlow );
	}

	//f Return 'Char' from 'IFlow'.
	inline void GetChar(
		flw::iflow__ &IFlow,
		char__ &Char )
	{
		flw::Get( IFlow, Char );
	}

	FBLTYP_S( Char, char__ )

	//f Put 'String' into 'OFlow'.
	inline void PutString(
		const string_ &String,
		flw::oflow__ &OFlow )
	{
		OFlow << String;
	}

	//f Get 'String' from 'IFlow'.
	inline void GetString(
		flw::iflow__ &IFlow,
		string_ &String )
	{
		IFlow >> String;
	}

	FBLTYP_D( String, string )

	//f Put 'Strings' into 'OFlow'.
	void PutStrings(
		const strings_ &Strings,
		flw::oflow__ &OFlow );

	//f Get 'Strings' from 'IFlow'.
	void GetStrings(
		flw::iflow__ &IFlow,
		strings_ &Strings );

	FBLTYP_D( Strings, strings )

	//f Put 'XStrings' into 'OFlow'.
	void PutXStrings(
		const xstrings_ &XStrings,
		flw::oflow__ &OFlow );

	//f Get 'XStrings' from 'IFlow'.
	void GetXStrings(
		flw::iflow__ &IFlow,
		xstrings_ &Strings );

	FBLTYP_D( XStrings, xstrings )

	//f Put 'Byte' into 'OFlow'.
	inline void PutByte(
		byte__ Byte,
		flw::oflow__ &OFlow )
	{
		flw::Put( Byte, OFlow );
	}

	//f Return 'Byte' from 'IFlow'.
	inline void GetByte(
		flw::iflow__ &IFlow,
		byte__ &Byte )
	{
		flw::Get( IFlow, Byte );
	}
	
	FBLTYP_S( Byte, byte__ )

	//f Put 'Binary' into 'OFlow'.
	void PutBinary(
		const binary_ &Binary,
		flw::oflow__ &OFlow );

	//f Get 'Binary' from 'IFlow'.
	void GetBinary(
		flw::iflow__ &IFlow,
		binary_ &Binary );

	FBLTYP_D( Binary, binary )

	//f Put 'Binaries' into 'OFlow'.
	void PutBinaries(
		const binaries_ &Binaries,
		flw::oflow__ &OFlow );

	//f Get 'Binaries' from 'IFlow'.
	void GetBinaries(
		flw::iflow__ &IFlow,
		binaries_ &Binaries );

	FBLTYP_D( Binaries, binaries )

	//f Put 'Item8' into 'OFlow'.
	void PutItem8(
		const item8_ Item8,
		flw::oflow__ &OFlow );

	//f Get 'Items8' from 'IFlow'.
	void GetItem8(
	   flw::iflow__ &IFlow,
	   item8_ &Item8 );

	FBLTYP_D( Items8, items8 )

	//f Put 'Items8' into 'OFlow'.
	void PutItems8(
		const items8_ &Items8,
		flw::oflow__ &OFlow );

	//f Get 'Items8' from 'IFlow'.
	void GetItems8(
		flw::iflow__ &IFlow,
		items8_ &Items8 );

	FBLTYP_D( Items16, items16 )

	//f Put 'Item16' into 'OFlow'.
	void PutItem16(
		const item16_ Item16,
		flw::oflow__ &OFlow );

	//f Get 'Item16' from 'IFlow'.
	void GetItem16(
	   flw::iflow__ &IFlow,
	   item16_ &Item16 );

	FBLTYP_D( Item16, item16 )

	//f Put 'Items16' into 'OFlow'.
	void PutItems16(
		const items16_ &Items16,
		flw::oflow__ &OFlow );

	//f Get 'Items16' from 'IFlow'.
	void GetItems16(
		flw::iflow__ &IFlow,
		items16_ &Items16 );

	FBLTYP_D( items16, items16 )

	//f Put 'Item32' into 'OFlow'.
	void PutItem32(
		const item32_ Item32,
		flw::oflow__ &OFlow );

	//f Get 'Item32' from 'IFlow'.
	void GetItem32(
	   flw::iflow__ &IFlow,
	   item32_ &Item32 );

	FBLTYP_D( Item32, items32 )

	//f Put 'Items32' into 'OFlow'.
	void PutItems32(
		const items32_ &Items32,
		flw::oflow__ &OFlow );

	//f Get 'Items32' from 'IFlow'.
	void GetItems32(
		flw::iflow__ &IFlow,
		items32_ &Items32 );

	FBLTYP_D( Items32, items32 )

	//f Put 'XItems8' into 'OFlow'.
	void PutXItems8(
		const xitems8_ &XItems8,
		flw::oflow__ &OFlow );

	//f Get 'XItems8' from 'IFlow'.
	void GetXItems8(
		flw::iflow__ &IFlow,
		xitems8_ &XItems8 );

	FBLTYP_D( XItems8, xitems8 )

	//f Put 'XItems16' into 'OFlow'.
	void PutXItems16(
		const xitems16_ &XItems16,
		flw::oflow__ &OFlow );

	//f Get 'XItems16' from 'IFlow'.
	void GetXItems16(
		flw::iflow__ &IFlow,
		xitems16_ &XItems16 );

	FBLTYP_D( XItems16, xitems16 )

	//f Put 'XItems32' into 'OFlow'.
	void PutXItems32(
		const xitems32_ &XItems32,
		flw::oflow__ &OFlow );

	//f Get 'XItems32' from 'IFlow'.
	void GetXItems32(
		flw::iflow__ &IFlow,
		xitems32_ &XItems32 );

	FBLTYP_D( XItems32, xitems32 )

	//f Put 'CommandsItems' into 'OFlow'.
	void PutCommandsDetails(
		const commands_details_ &CommandsDetails,
		flw::oflow__ &OFlow );

	//f Get 'CommandsDetails' from 'IFlow'.
	void GetCommandsDetails(
		flw::iflow__ &IFlow,
		commands_details_ &CommandsDetails );

	FBLTYP_D( CommandsDetails, commands_details )

	//f Put 'ObjectsReferences' into 'OFlow'.
	void PutObjectsReferences(
		const objects_references_ &ObjectsReferences,
		flw::oflow__ &OFlow );

	//f Get 'ObjectsReferences' from 'IFlow'.
	void GetObjectsReferences(
		flw::iflow__ &IFlow,
		objects_references_ &ObjectsReferences );

	FBLTYP_D( ObjectsReferences, objects_references )


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
