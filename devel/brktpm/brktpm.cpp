/*
  'brktpm' library by Claude L. Simon (simon@epeios.org)
  Requires the 'brktpm' header file ('brktpm.h').
  Copyright (C) 2000 Claude L. SIMON (simon@epeios.org).

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

#define BRKTPM__COMPILATION

#include "brktpm.h"

class brktpmtutor
: public ttr_tutor
{
public:
	brktpmtutor( void )
	: ttr_tutor( BRKTPM_NAME )
	{
#ifdef BRKTPM_DBG
		Version = BRKTPM_VERSION "\b\bD $";
#else
		Version = BRKTPM_VERSION;
#endif
		Owner = BRKTPM_OWNER;
		Date = "$Date$";
	}
	virtual ~brktpmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace brktpm;

#define M1	255
#define M2	65535
#define M3	16777215

namespace {
	inline void PutSize_(
		bso__ulong Size,
		flw::oflow___ &Flow )
	{
		if ( Size >= M1 )
		{
			Flow.Put( M1 );
			Size -= M1;

			if ( Size >= M2 )
			{
				Flow.Put( M1 );
				Flow.Put( M1 );
				Size -= M2;

				if ( Size >= M3 )
				{
					Flow.Put( M1 );
					Flow.Put( M1 );
					Flow.Put( M1 );

					Size -= M3;

					Flow.Put( ( (flw::data__)( Size & ( 255 << 24 ) ) >> 24 ) );
				}
				Flow.Put( (flw::data__)( ( Size & ( 255 << 16 ) ) >> 16 ) );
			}
			Flow.Put( (flw::data__)( ( Size & ( 255 << 8 ) ) >> 8 ) );
		}
		Flow.Put( (flw::data__)( Size & 255 ) );
	}

	inline bso__ulong GetSize_( flw::iflow___ &IFlow )
	{
		bso__ulong Size = IFlow.Get();

		if ( Size == M1 )
		{
			Size += ( IFlow.Get() << 8 ) | IFlow.Get();

			if ( Size == ( M1 + M2 ) )
			{
				Size += ( IFlow.Get() << 16 ) | ( IFlow.Get() << 8 ) | IFlow.Get();

				if ( Size == ( M1 + M2 + M3 ) )
					Size += ( IFlow.Get() << 24 ) | ( IFlow.Get() << 16 ) | ( IFlow.Get() << 8 ) | IFlow.Get();
			}
		}

		return Size;
	}
}

namespace {
	template <typename s> inline void PutSet_(
		const s &S,
		flw::oflow___ &OFlow )
	{
		tym::row__ P = S.First();

		PutSize_( S.Amount(), OFlow );

		while( P != NONE ) {
			flw::Put( S( P ), OFlow );
			P = S.Next( P );
		}
	}

	template <typename s, typename i> inline void GetSet_(
		flw::iflow___ &IFlow,
		s &S )
	{
		i I;
		bso__ulong Size = GetSize_( IFlow );

		while( Size-- ) {
			flw::Get( IFlow, I );
			S.Add( I );
		}
	}
}

void brktpm::PutIds8(
	const ids8_ &Ids8,
	flw::oflow___ &OFlow )
{
	PutSet_( Ids8, OFlow );
}

void brktpm::GetIds8(
	flw::iflow___ &IFlow,
	ids8_ &Ids8 )
{
	GetSet_<ids8_, id8__>( IFlow, Ids8 );
}

void brktpm::PutIds16(
	const ids16_ &Ids16,
	flw::oflow___ &OFlow )
{
	PutSet_( Ids16, OFlow );
}

void brktpm::GetIds16(
	flw::iflow___ &IFlow,
	ids16_ &Ids16 )
{
	GetSet_<ids16_, id16__>( IFlow, Ids16 );
}

void brktpm::PutIds32(
	const ids32_ &Ids32,
	flw::oflow___ &OFlow )
{
	PutSet_( Ids32, OFlow );
}

void brktpm::GetIds32(
	flw::iflow___ &IFlow,
	ids32_ &Ids32 )
{
	GetSet_<ids32_, id32__>( IFlow, Ids32 );
}

namespace {
	namespace {
		namespace {
			template <typename t> inline void Put_(
				const item_<t> &I,
				flw::oflow___ &OFlow )
			{
				flw::Put( I.ID(), OFlow );
				OFlow << I.Value;
			}

			template <typename t> inline void Get_(
				flw::iflow___ &IFlow,
				item_<t> &I )
			{
				flw::Get( IFlow, I.S_.ID );
				IFlow >> I.Value;
			}

			inline void Put_(
				const str::string_ &String,
				flw::oflow___ &OFlow )
			{
				OFlow << String;
			}

			inline void Get_(
				flw::iflow___ &IFlow,
				string_ &String )
			{
				IFlow >> String;
			}

			inline void Put_(
				const binary_ &Binary,
				flw::oflow___ &OFlow )
			{
				PutSet_( Binary, OFlow );
			}

			inline void Get_(
				flw::iflow___ &IFlow,
				binary_ &Binary )
			{
				GetSet_<binary_, bso__raw>( IFlow, Binary );
			}

			inline void Put_(
				const command_detail_ &CD,
				flw::oflow___ &OFlow )
			{
				OFlow << CD.Name;
				PutSet_( CD.Casts, OFlow );
			}

			inline void Get_(
				flw::iflow___ &IFlow,
				command_detail_ &CD )
			{
				IFlow >> CD.Name;
				GetSet_<bch::E_BUNCH_( id8__ ), id8__>( IFlow, CD.Casts );
			}

		}

		template <typename c, typename i> inline void GenericPutContainer_(
			const c &C,
			flw::oflow___ &OFlow )
		{
			i Item;
			tym::row__ P = C.First();

			Item.Init( C );

			PutSize_( C.Amount(), OFlow );

			while ( P != NONE ) {
				Put_( Item( P ), OFlow );
				P = C.Next( P );
			}
		}
	}

	template <typename c, typename i> inline void PutContainer_(
		const c &C,
		flw::oflow___ &OFlow )
	{
		GenericPutContainer_<c, ctn::E_CITEM( i )>( C, OFlow );
	}

	template <typename c, typename i> inline void PutMContainer_(
		const c &C,
		flw::oflow___ &OFlow )
	{
		GenericPutContainer_<c, ctn::E_CMITEM( i )>( C, OFlow );
	}

	template <typename c, typename i> inline void GetContainer_(
		flw::iflow___ &IFlow,
		c &C )
	{
		i Item;
		bso__ulong Size = GetSize_( IFlow );

		while( Size-- ) {
			Item.Init();

			Get_( IFlow, Item );
			C.Add( Item );
		}
	}
}

void brktpm::PutStrings(
	const strings_ &Strings,
	flw::oflow___ &OFlow )
{
	PutMContainer_<strings_, string_>( Strings, OFlow );
}

void brktpm::GetStrings(
   flw::iflow___ &IFlow,
   strings_ &Strings )
{
	GetContainer_<strings_, string>( IFlow, Strings );
}

void brktpm::PutBinary(
	const binary_ &Binary,
	flw::oflow___ &OFlow )
{
	PutSet_( Binary, OFlow );
}

void brktpm::GetBinary(
   flw::iflow___ &IFlow,
   binary_ &Binary )
{
	GetSet_<binary_, bso__raw>( IFlow, Binary );
}

void brktpm::PutBinaries(
	const binaries_ &Binaries,
	flw::oflow___ &OFlow )
{
	PutMContainer_<binaries_, binary_>( Binaries, OFlow );
}

void brktpm::GetBinaries(
	flw::iflow___ &IFlow,
	binaries_ &Binaries )
{
	GetContainer_<binaries_, binary>( IFlow, Binaries );
}

void brktpm::PutItems8(
	const items8_ &Items8,
	flw::oflow___ &OFlow )
{
	PutMContainer_<items8_, item8_>( Items8, OFlow );
}

void brktpm::GetItems8(
   flw::iflow___ &IFlow,
   items8_ &Items8 )
{
	GetContainer_<items8_, item8>( IFlow, Items8 );
}

void brktpm::PutItems16(
	const items16_ &Items16,
	flw::oflow___ &OFlow )
{
	PutMContainer_<items16_, item16_>( Items16, OFlow );
}

void brktpm::GetItems16(
   flw::iflow___ &IFlow,
   items16_ &Items16 )
{
	GetContainer_<items16_, item16>( IFlow, Items16 );
}

void brktpm::PutItems32(
	const items32_ &Items32,
	flw::oflow___ &OFlow )
{
	PutMContainer_<items32_, item32_>( Items32, OFlow );
}

void brktpm::GetItems32(
   flw::iflow___ &IFlow,
   items32_ &Items32 )
{
	GetContainer_<items32_, item32>( IFlow, Items32 );
}

void brktpm::PutObjectsReferences(
	const objects_references_ &ObjectsReferences,
	flw::oflow___ &OFlow )
{
	PutSet_( ObjectsReferences, OFlow );
}

void brktpm::GetObjectsReferences(
	flw::iflow___ &IFlow,
	objects_references_ &ObjectsReferences )
{
	GetSet_<objects_references_, object_reference__>( IFlow, ObjectsReferences );
}

void brktpm::PutCommandsDetails(
	const commands_details_ &CommandsDetails,
	flw::oflow___ &OFlow )
{
	PutContainer_<commands_details_, command_detail_ >( CommandsDetails, OFlow );
}

void brktpm::GetCommandsDetails(
	flw::iflow___ &IFlow,
	commands_details_ &CommandsDetails )
{
	GetContainer_<commands_details_, command_detail >( IFlow, CommandsDetails );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class brktpmpersonnalization
: public brktpmtutor
{
public:
	brktpmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~brktpmpersonnalization( void )
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

static brktpmpersonnalization Tutor;

ttr_tutor &BRKTPMTutor = Tutor;
