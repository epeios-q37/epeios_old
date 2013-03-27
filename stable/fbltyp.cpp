/*
	'fbltyp' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fbltyp' header file ('fbltyp.h').
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

#define FBLTYP__COMPILATION

#include "fbltyp.h"

class fbltyptutor
: public ttr_tutor
{
public:
	fbltyptutor( void )
	: ttr_tutor( FBLTYP_NAME )
	{
#ifdef FBLTYP_DBG
		Version = FBLTYP_VERSION "\b\bD $";
#else
		Version = FBLTYP_VERSION;
#endif
		Owner = FBLTYP_OWNER;
		Date = "$Date$";
	}
	virtual ~fbltyptutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace fbltyp;

static inline void PutSize_(
	bso::u32__ Size,
	flw::oflow__ &Flow )
{
	dtfptb::NewPutSize( Size, Flow );
}

static inline bso::u32__ GetSize_( flw::iflow__ &Flow )
{
	return dtfptb::NewGetSize( Flow);
}

namespace {
	template <typename s> inline void PutSet_(
		const s &S,
		flw::oflow__ &OFlow )
	{
		sdr::row__ P = S.First();

		PutSize_( S.Amount(), OFlow );

		while( P != NONE ) {
			flw::Put( S( P ), OFlow );
			P = S.Next( P );
		}
	}

	template <typename s, typename i> inline void GetSet_(
		flw::iflow__ &IFlow,
		s &S )
	{
		i I;
		bso::u32__ Size = GetSize_( IFlow );

		while( Size-- ) {
			flw::Get( IFlow, I );
			S.Append( I );
		}
	}
}

void fbltyp::PutIds8(
	const ids8_ &Ids8,
	flw::oflow__ &OFlow )
{
	PutSet_( Ids8, OFlow );
}

void fbltyp::PutS32s(
	const s32s_ &S32s,
	flw::oflow__ &OFlow )
{
	PutSet_( S32s, OFlow );
}

void fbltyp::GetS32s(
	flw::iflow__ &IFlow,
	s32s_ &S32s )
{
	GetSet_<s32s_, s32__>( IFlow, S32s );
}
void fbltyp::PutU32s(
	const u32s_ &U32s,
	flw::oflow__ &OFlow )
{
	PutSet_( U32s, OFlow );
}

void fbltyp::GetU32s(
	flw::iflow__ &IFlow,
	u32s_ &U32s )
{
	GetSet_<u32s_, u32__>( IFlow, U32s );
}
void fbltyp::GetIds8(
	flw::iflow__ &IFlow,
	ids8_ &Ids8 )
{
	GetSet_<ids8_, id8__>( IFlow, Ids8 );
}

void fbltyp::PutIds16(
	const ids16_ &Ids16,
	flw::oflow__ &OFlow )
{
	PutSet_( Ids16, OFlow );
}

void fbltyp::GetIds16(
	flw::iflow__ &IFlow,
	ids16_ &Ids16 )
{
	GetSet_<ids16_, id16__>( IFlow, Ids16 );
}

namespace {
	template <typename t> inline void Put_(
		const item_<t> &I,
		flw::oflow__ &OFlow )
	{
		flw::Put( I.ID(), OFlow );
		OFlow << I.Value;
	}

	template <typename t> inline void Get_(
		flw::iflow__ &IFlow,
		item_<t> &I )
	{
		flw::Get( IFlow, I.S_.ID );
		IFlow >> I.Value;
	}

	inline void Put_(
		const str::string_ &String,
		flw::oflow__ &OFlow )
	{
		PutSet_( String, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		string_ &String )
	{
		GetSet_<string_, bso::char__>( IFlow, String );
	}

	inline void Put_(
		const binary_ &Binary,
		flw::oflow__ &OFlow )
	{
		PutSet_( Binary, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		binary_ &Binary )
	{
		GetSet_<binary_, bso::raw__>( IFlow, Binary );
	}

	inline void Put_(
		const ids8_ &Ids8,
		flw::oflow__ &OFlow )
	{
		PutSet_( Ids8, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		ids8_ &Ids8 )
	{
		GetSet_<ids8_, id8__>( IFlow, Ids8 );
	}

	inline void Put_(
		const ids16_ &Ids16,
		flw::oflow__ &OFlow )
	{
		PutSet_( Ids16, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		ids16_ &Ids16 )
	{
		GetSet_<ids16_, id16__>( IFlow, Ids16 );
	}

	inline void Put_(
		const ids32_ &Ids32,
		flw::oflow__ &OFlow )
	{
		PutSet_( Ids32, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		ids32_ &Ids32 )
	{
		GetSet_<ids32_, id32__>( IFlow, Ids32 );
	}

	inline void Put_(
		const command_detail_ &CD,
		flw::oflow__ &OFlow )
	{
		OFlow << CD.Name;
		PutSet_( CD.Casts, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		command_detail_ &CD )
	{
		IFlow >> CD.Name;
		GetSet_<bch::E_BUNCH_( id8__ ), id8__>( IFlow, CD.Casts );
	}

	template <typename t> inline void Put_(
		const extended_item_<t> &I,
		flw::oflow__ &OFlow );

	void Put_(
		const strings_ &Strings,
		flw::oflow__ &OFlow );	// Predeclaration.

	template <typename c, typename i> inline void GenericPutContainer_(
		const c &C,
		flw::oflow__ &OFlow )
	{
		i Item;
		sdr::row__ P = C.First();

		Item.Init( C );

		PutSize_( C.Amount(), OFlow );

		while ( P != NONE ) {
			Put_( Item( P ), OFlow );
			P = C.Next( P );
		}
	}

	template <typename c, typename i> inline void PutContainer_(
		const c &C,
		flw::oflow__ &OFlow )
	{
		GenericPutContainer_<c, ctn::E_CITEM( i )>( C, OFlow );
	}

	template <typename c, typename i> inline void PutMContainer_(
		const c &C,
		flw::oflow__ &OFlow )
	{
		GenericPutContainer_<c, ctn::E_CMITEM( i )>( C, OFlow );
	}

	template <typename t> inline void Put_(
		const extended_item_<t> &I,
		flw::oflow__ &OFlow )
	{
		flw::Put( I.ID(), OFlow );
		PutMContainer_<strings_, string_>( I.Values, OFlow );
	}

	void Get_(
		flw::iflow__ &IFlow,
		strings_ &Strings );	// Predeclaration.

	template <typename t> inline void Get_(
		flw::iflow__ &IFlow,
		extended_item_<t> &I );	// Predeclaration.

	template <typename c, typename i> inline void GetContainer_(
		flw::iflow__ &IFlow,
		c &C )
	{
		i Item;
		bso::u32__ Size = GetSize_( IFlow );

		while( Size-- ) {
			Item.Init();

			Get_( IFlow, Item );
			C.Append( Item );
		}
	}

	template <typename t> inline void Get_(
		flw::iflow__ &IFlow,
		extended_item_<t> &I )
	{
		flw::Get( IFlow, I.S_.ID );
		GetContainer_<strings_, string>( IFlow, I.Values );
	}

	inline void Put_(
		const strings_ &Strings,
		flw::oflow__ &OFlow )
	{
		PutMContainer_<strings_, string_>( Strings, OFlow );
	}

	inline void Get_(
		flw::iflow__ &IFlow,
		strings_ &Strings )
	{
		GetContainer_<strings_, string>( IFlow, Strings );
	}

}

void fbltyp::PutXIds8(
	const xids8_ &XIds8,
	flw::oflow__ &OFlow )
{
	PutMContainer_<xids8_, ids8_>( XIds8, OFlow );
}

void fbltyp::GetXIds8(
	flw::iflow__ &IFlow,
	xids8_ &XIds8 )
{
	GetContainer_<xids8_, ids8>( IFlow, XIds8 );
}

void fbltyp::PutXIds16(
	const xids16_ &XIds16,
	flw::oflow__ &OFlow )
{
	PutMContainer_<xids16_, ids16_>( XIds16, OFlow );
}

void fbltyp::GetXIds16(
	flw::iflow__ &IFlow,
	xids16_ &XIds16 )
{
	GetContainer_<xids16_, ids16>( IFlow, XIds16 );
}

void fbltyp::PutBooleans(
	const booleans_ &Booleans,
	flw::oflow__ &OFlow )	// To optimize.
{
	PutSet_( Booleans, OFlow );
}

void fbltyp::GetBooleans(
	flw::iflow__ &IFlow,
	booleans_ &Booleans ) // To optimize.
{
	GetSet_<booleans_, bso::bool__>( IFlow, Booleans );
}

void fbltyp::PutIds32(
	const ids32_ &Ids32,
	flw::oflow__ &OFlow )
{
	Put_( Ids32, OFlow );
}

void fbltyp::GetIds32(
	flw::iflow__ &IFlow,
	ids32_ &Ids32 )
{
	Get_( IFlow, Ids32 );
}
void fbltyp::PutXIds32(
	const xids32_ &XIds32,
	flw::oflow__ &OFlow )
{
	PutMContainer_<xids32_, ids32_>( XIds32, OFlow );
}

void fbltyp::GetXIds32(
	flw::iflow__ &IFlow,
	xids32_ &XIds32 )
{
	GetContainer_<xids32_, ids32>( IFlow, XIds32 );
}

void fbltyp::PutStrings(
	const strings_ &Strings,
	flw::oflow__ &OFlow )
{
	Put_( Strings, OFlow );
}

void fbltyp::GetStrings(
   flw::iflow__ &IFlow,
   strings_ &Strings )
{
	Get_( IFlow, Strings );
}

void fbltyp::PutXStrings(
	const xstrings_ &XStrings,
	flw::oflow__ &OFlow )
{
	PutContainer_<xstrings_, strings_>( XStrings, OFlow );
}

void fbltyp::GetXStrings(
   flw::iflow__ &IFlow,
   xstrings_ &XStrings )
{
	GetContainer_<xstrings_, strings>( IFlow, XStrings );
}

void fbltyp::PutBinary(
	const binary_ &Binary,
	flw::oflow__ &OFlow )
{
	PutSet_( Binary, OFlow );
}

void fbltyp::GetBinary(
   flw::iflow__ &IFlow,
   binary_ &Binary )
{
	GetSet_<binary_, bso::raw__>( IFlow, Binary );
}

void fbltyp::PutBinaries(
	const binaries_ &Binaries,
	flw::oflow__ &OFlow )
{
	PutMContainer_<binaries_, binary_>( Binaries, OFlow );
}

void fbltyp::GetBinaries(
	flw::iflow__ &IFlow,
	binaries_ &Binaries )
{
	GetContainer_<binaries_, binary>( IFlow, Binaries );
}

void fbltyp::PutItem8(
	const item8_ Item8,
	flw::oflow__ &OFlow )
{
	Put_( Item8, OFlow );
}

void fbltyp::GetItem8(
   flw::iflow__ &IFlow,
   item8_ &Item8 )
{
	Get_( IFlow, Item8 );
}

void fbltyp::PutItems8(
	const items8_ &Items8,
	flw::oflow__ &OFlow )
{
	PutMContainer_<items8_, item8_>( Items8, OFlow );
}

void fbltyp::GetItems8(
   flw::iflow__ &IFlow,
   items8_ &Items8 )
{
	GetContainer_<items8_, item8>( IFlow, Items8 );
}

void fbltyp::PutItem16(
	const item16_ Item16,
	flw::oflow__ &OFlow )
{
	Put_( Item16, OFlow );
}

void fbltyp::GetItem16(
   flw::iflow__ &IFlow,
   item16_ &Item16 )
{
	Get_( IFlow, Item16 );
}

void fbltyp::PutItems16(
	const items16_ &Items16,
	flw::oflow__ &OFlow )
{
	PutMContainer_<items16_, item16_>( Items16, OFlow );
}

void fbltyp::GetItems16(
   flw::iflow__ &IFlow,
   items16_ &Items16 )
{
	GetContainer_<items16_, item16>( IFlow, Items16 );
}

void fbltyp::PutItem32(
	const item32_ Item32,
	flw::oflow__ &OFlow )
{
	Put_( Item32, OFlow );
}

void fbltyp::GetItem32(
   flw::iflow__ &IFlow,
   item32_ &Item32 )
{
	Get_( IFlow, Item32 );
}

void fbltyp::PutItems32(
	const items32_ &Items32,
	flw::oflow__ &OFlow )
{
	PutMContainer_<items32_, item32_>( Items32, OFlow );
}

void fbltyp::GetItems32(
   flw::iflow__ &IFlow,
   items32_ &Items32 )
{
	GetContainer_<items32_, item32>( IFlow, Items32 );
}

void fbltyp::PutXItems8(
	const xitems8_ &XItems8,
	flw::oflow__ &OFlow )
{
	PutContainer_<xitems8_, xitem8_>( XItems8, OFlow );
}

void fbltyp::GetXItems8(
   flw::iflow__ &IFlow,
   xitems8_ &XItems8 )
{
	GetContainer_<xitems8_, xitem8>( IFlow, XItems8 );
}

void fbltyp::PutXItems16(
	const xitems16_ &XItems16,
	flw::oflow__ &OFlow )
{
	PutContainer_<xitems16_, xitem16_>( XItems16, OFlow );
}

void fbltyp::GetXItems16(
   flw::iflow__ &IFlow,
   xitems16_ &XItems16 )
{
	GetContainer_<xitems16_, xitem16>( IFlow, XItems16 );
}

void fbltyp::PutXItems32(
	const xitems32_ &XItems32,
	flw::oflow__ &OFlow )
{
	PutContainer_<xitems32_, xitem32_>( XItems32, OFlow );
}

void fbltyp::GetXItems32(
   flw::iflow__ &IFlow,
   xitems32_ &XItems32 )
{
	GetContainer_<xitems32_, xitem32>( IFlow, XItems32 );
}

void fbltyp::PutObjectsReferences(
	const objects_references_ &ObjectsReferences,
	flw::oflow__ &OFlow )
{
	PutSet_( ObjectsReferences, OFlow );
}

void fbltyp::GetObjectsReferences(
	flw::iflow__ &IFlow,
	objects_references_ &ObjectsReferences )
{
	GetSet_<objects_references_, object_reference__>( IFlow, ObjectsReferences );
}

void fbltyp::PutCommandsDetails(
	const fbltyp::commands_details_ &CommandsDetails,
	flw::oflow__ &OFlow )
{
	PutContainer_<commands_details_, command_detail_ >( CommandsDetails, OFlow );
}

void fbltyp::GetCommandsDetails(
	flw::iflow__ &IFlow,
	fbltyp::commands_details_ &CommandsDetails )
{
	GetContainer_<commands_details_, command_detail>( IFlow, CommandsDetails );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fbltyppersonnalization
: public fbltyptutor
{
public:
	fbltyppersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fbltyppersonnalization( void )
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

static fbltyppersonnalization Tutor;

ttr_tutor &FBLTYPTutor = Tutor;
