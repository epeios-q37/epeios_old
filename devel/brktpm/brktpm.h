/*
  Header for the 'brktpm' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (simon@epeios.org) 

  This file is part of the Epeios (http://www.epeios.org/) project.
  

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

#ifndef BRKTPM__INC
#define BRKTPM__INC

#define BRKTPM_NAME		"BRKTPM"

#define	BRKTPM_VERSION	"$Revision$"	

#define BRKTPM_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &BRKTPMTutor;

#if defined( XXX_DBG ) && !defined( BRKTPM_NODBG )
#define BRKTPM_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (simon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D BRoKer TyPe Manager.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "str.h"
#include "ctn.h"

#define BRKTPM_ITEM( id, name )\
	typedef brktpm::item_<id>	name##_;\
	AUTO( name )

#define BRKTPM_ITEMS( item, name )\
	typedef ctn::E_XMCONTAINER_( item##_ )	name##_;\
	AUTO( name )

namespace brktpm {

	template <class id> class item_
	{
	public:
		str::string_ Value;
		struct s
		{
			str::string_::s Value;
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
		void plug( mdr::E_MEMORY_DRIVER_ &MD )
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

	template <class t, typename r> class detail_
	{
	public:
		str::string_ Name;
		bch::E_BUNCHt_( t, r ) Casts;
		struct s {
			str::string_::s Name;
			bch::E_BUNCHt_( t, r )::s Casts;
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

	AUTO1( detail )

	struct object_reference__
	{
		bso__ushort Type;
		bso__ushort Identifier;
	};

	typedef bso__ushort	object_t__;
	TYPEDEF( object_t__, object__ );
	typedef bso__bool boolean_t__;
	typedef boolean_t__	boolean__;
	typedef bso__ubyte id8_t__;
	TYPEDEF( id8_t__,	id8__ );
	typedef bch::E_BUNCH_( id8__ ) ids8_;
	typedef bch::E_BUNCH( id8__ ) ids8;
	typedef bso__ushort id16_t__;
	TYPEDEF( id16_t__,	id16__ );
	typedef bch::E_BUNCH_( id16__ ) ids16_;
	typedef bch::E_BUNCH( id16__ ) ids16;
	typedef bso__ulong id32_t__;
	TYPEDEF( id32_t__,	id32__ );
	typedef bch::E_BUNCH_( id32__ ) ids32_;
	typedef bch::E_BUNCH( id32__ ) ids32;
	typedef bso__char char_t__;
	typedef char_t__	char__;
	typedef str::string_	string_;
	typedef str::string	string;
	typedef ctn::E_XMCONTAINER_( string_ )	strings_;
	typedef ctn::E_XMCONTAINER( string_ )	strings;
	typedef bso__raw	byte_t__;
	typedef byte_t__	byte__;
	typedef bch::E_BUNCH_( bso__raw ) binary_;
	typedef bch::E_BUNCH( bso__raw )	binary;
	typedef ctn::E_XMCONTAINER_( binary_ ) binaries_;
	typedef ctn::E_XMCONTAINER( binary_ ) binaries;
	BRKTPM_ITEM( bso__ubyte, item8 )
	BRKTPM_ITEMS( item8, items8 )
	BRKTPM_ITEM( bso__ushort, item16 )
	BRKTPM_ITEMS( item16, items16 )
	BRKTPM_ITEM( bso__ulong, item32 )
	BRKTPM_ITEMS( item32, items32 )
	/* Both below declaration are not part of the protocol. */
	typedef brktpm::detail_<id8__, epeios::row__>	command_detail_	;
	typedef brktpm::detail<id8__, epeios::row__>	command_detail;
	/**/
	typedef ctn::E_XCONTAINER_( command_detail_ ) commands_details_;
	typedef ctn::E_XCONTAINER( command_detail_ ) commands_details;
	typedef bch::E_BUNCH_( object_reference__ ) objects_references_;
	typedef bch::E_BUNCH( object_reference__ ) objects_references;

	//f Put 'Object' into 'OFlow'.
	inline void PutObject(
		object__ Object,
		flw::oflow___ &OFlow )
	{
		flw::Put( Object, OFlow );
	}

	//f Return 'Object' from 'IFlow'.
	inline void GetObject(
		flw::iflow___ &IFlow,
		object__ &Object )
	{
		flw::Get( IFlow, Object );
	}
	
	//f Put 'Boolean' into 'OFlow'.
	inline void PutBoolean(
		boolean__ Boolean,
		flw::oflow___ &OFlow )
	{
		flw::Put( Boolean, OFlow );
	}

	//f Return 'Boolean' from 'IFlow'.
	inline void GetBoolean(
		flw::iflow___ &IFlow,
		boolean__ &Boolean )
	{
		flw::Get( IFlow, Boolean );
	}
	
	//f Put 'Id8' into 'OFlow'.
	inline void PutId8(
		id8__ Id8,
		flw::oflow___ &OFlow )
	{
		flw::Put( Id8, OFlow );
	}

	//f Return 'Id8' from 'IFlow'.
	inline void GetId8(
		flw::iflow___ &IFlow,
		id8__ &Id8 )
	{
		flw::Get( IFlow, Id8 );
	}
	
	//f Put 'Ids8' into 'OFlow'.
	void PutIds8(
		const ids8_ &Ids8,
		flw::oflow___ &OFlow );

	//f Get 'Ids8' from 'OFlow'.
	void GetIds8(
		flw::iflow___ &IFlow,
		ids8_ &Ids8 );

	//f Put 'Id16' into 'OFlow'.
	inline void PutId16(
		id16__ Id16,
		flw::oflow___ &OFlow )
	{
		flw::Put( Id16, OFlow );
	}

	//f Return 'Id16' from 'IFlow'.
	inline void GetId16(
		flw::iflow___ &IFlow,
		id16__ &Id16 )
	{
		flw::Get( IFlow, Id16 );
	}
	
	//f Put 'Ids16' into 'OFlow'.
	void PutIds16(
		const ids16_ &Ids16,
		flw::oflow___ &OFlow );

	//f Get 'Ids16' from 'OFlow'.
	void GetIds16(
		flw::iflow___ &IFlow,
		ids16_ &Ids16 );

	//f Put 'Id32' into 'OFlow'.
	inline void PutId32(
		id32__ Id32,
		flw::oflow___ &OFlow )
	{
		flw::Put( Id32, OFlow );
	}

	//f Return 'Id32' from 'IFlow'.
	inline void GetId32(
		flw::iflow___ &IFlow,
		id32__ &Id32 )
	{
		flw::Get( IFlow, Id32 );
	}
	
	//f Put 'Ids32' into 'OFlow'.
	void PutIds32(
		const ids32_ &Ids32,
		flw::oflow___ &OFlow );

	//f Get 'Ids32' from 'OFlow'.
	void GetIds32(
		flw::iflow___ &IFlow,
		ids32_ &Ids32 );

	//f Put 'Char' into 'OFlow'.
	inline void PutChar(
		char__ Char,
		flw::oflow___ &OFlow )
	{
		flw::Put( Char, OFlow );
	}

	//f Return 'Char' from 'IFlow'.
	inline void GetChar(
		flw::iflow___ &IFlow,
		char__ &Char )
	{
		flw::Get( IFlow, Char );
	}

	//f Put 'String' into 'OFlow'.
	inline void PutString(
		const string_ &String,
		flw::oflow___ &OFlow )
	{
		OFlow << String;
	}

	//f Get 'String' from 'IFlow'.
	inline void GetString(
		flw::iflow___ &IFlow,
		string_ &String )
	{
		IFlow >> String;
	}

	//f Put 'Strings' into 'OFlow'.
	void PutStrings(
		const strings_ &Strings,
		flw::oflow___ &OFlow );

	//f Get 'Strings' from 'IFlow'.
	void GetStrings(
		flw::iflow___ &IFlow,
		strings_ &Strings );

	//f Put 'Byte' into 'OFlow'.
	inline void PutByte(
		byte__ Byte,
		flw::oflow___ &OFlow )
	{
		flw::Put( Byte, OFlow );
	}

	//f Return 'Byte' from 'IFlow'.
	inline void GetByte(
		flw::iflow___ &IFlow,
		byte__ &Byte )
	{
		flw::Get( IFlow, Byte );
	}
	
	//f Put 'Binary' into 'OFlow'.
	void PutBinary(
		const binary_ &Binary,
		flw::oflow___ &OFlow );

	//f Get 'Binary' from 'IFlow'.
	void GetBinary(
		flw::iflow___ &IFlow,
		binary_ &Binary );

	//f Put 'Binaries' into 'OFlow'.
	void PutBinaries(
		const binaries_ &Binaries,
		flw::oflow___ &OFlow );

	//f Get 'Binaries' from 'IFlow'.
	void GetBinaries(
		flw::iflow___ &IFlow,
		binaries_ &Binaries );

	//f Put 'Items8' into 'OFlow'.
	void PutItems8(
		const items8_ &Items8,
		flw::oflow___ &OFlow );

	//f Get 'Items8' from 'IFlow'.
	void GetItems8(
		flw::iflow___ &IFlow,
		items8_ &Items8 );

	//f Put 'Items16' into 'OFlow'.
	void PutItems16(
		const items16_ &Items16,
		flw::oflow___ &OFlow );

	//f Get 'Items16' from 'IFlow'.
	void GetItems16(
		flw::iflow___ &IFlow,
		items16_ &Items16 );

	//f Put 'Items32' into 'OFlow'.
	void PutItems32(
		const items32_ &Items32,
		flw::oflow___ &OFlow );

	//f Get 'Items32' from 'IFlow'.
	void GetItems32(
		flw::iflow___ &IFlow,
		items32_ &Items32 );

	//f Put 'CommandsItems' into 'OFlow'.
	void PutCommandsDetails(
		const commands_details_ &CommandsDetails,
		flw::oflow___ &OFlow );

	//f Get 'CommandsDetails' from 'IFlow'.
	void GetCommandsDetails(
		flw::iflow___ &IFlow,
		commands_details_ &CommandsDetails );

	//f Put 'ObjectsReferences' into 'OFlow'.
	void PutObjectsReferences(
		const objects_references_ &ObjectsReferences,
		flw::oflow___ &OFlow );

	//f Get 'ObjectsReferences' from 'IFlow'.
	void GetObjectsReferences(
		flw::iflow___ &IFlow,
		objects_references_ &ObjectsReferences );


};

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
