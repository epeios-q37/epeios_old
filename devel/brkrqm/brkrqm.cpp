/*
  'brkrqm' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'brkrqm' header file ('brkrqm.h').
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

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
  along with this program; if not, go to http://www.fsf.org or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#define BRKRQM__COMPILATION

#include "brkrqm.h"

class brkrqmtutor
: public ttr_tutor
{
public:
	brkrqmtutor( void )
	: ttr_tutor( BRKRQM_NAME )
	{
#ifdef BRKRQM_DBG
		Version = BRKRQM_VERSION " (DBG)";
#else
		Version = BRKRQM_VERSION;
#endif
		Owner = BRKRQM_OWNER;
		Date = "$Date$";
	}
	virtual ~brkrqmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace brkrqm;

#include <fstream.h>

void brkrqm::description_::Init(
	const char *Name,
	const cast *Casts )
{
	int i = 0;
	cast Cast = cInvalid;
	bso__bool Array = false;

	this->Name.Init();
	this->Casts.Init();

	this->Name = Name;

	do
	{
		if ( Cast == cArray ) {
			if ( Array )
				ERRu();
			Array = true;
		} else if ( Cast == cEnd )
			Array = false;
		Cast = Casts[i++];
		this->Casts.Add( Cast );
	} while ( Array || ( Cast != cEnd ) );

#ifdef RQM_DBG
	if ( Array )
		ERRc();
#endif

	while ( ( ( Cast = Casts[i++] ) != cEnd ) || Array ) {
		if ( Cast == cArray ) {
			if ( Array )
				ERRu();
			Array = true;
		} else if ( Cast == cEnd )
			Array = false;

		this->Casts.Add( Cast );
	}

#ifdef RQM_DBG
	if ( Array != 0 )
		ERRc();
#endif

}

POSITION__ brkrqm::descriptions_::Position( const description_ &Description ) const
{
	POSITION__ Position = CONTAINER_( description_ )::First();
ERRProlog
	CITEM( description_ ) D;
ERRBegin

	D.Init( *this );

	while( ( Position != NONE )
		    && ( ( D( Position ).Name != Description.Name )
		         || ( D( Position ).Casts != Description.Casts ) ) )
		Position = CONTAINER_( description_ )::Next( Position );
ERRErr
ERREnd
ERREpilog
	return Position;
}

// Return cast of name 'CastName', or 'cInvalid' if non-existent.
cast brkrqm::GetCast( const str_string_ &CastName )
{
	cast Retour = cEnd;
ERRProlog
	int Id = c_amount;
	str_string S;
ERRBegin
	S.Init();

	while( --Id && ( S = CastsNames[Id], S != CastName ) );

	Retour = (cast)Id;
ERRErr
ERREnd
ERREpilog
	return Retour;
}

// Ajoute dans 'Flot' le paramètre 'Valeur' de caste 'Caste'.
void brkrqm::AddValue(
	cast Cast,
	const void *Valeur,
	flw::oflow___ &Flot )
{
	brkrqm::AddCast( Cast, Flot );

	switch( Cast ) {
	case cObject:
		Flot.Put( Valeur, sizeof( object__ ) );
		break;
	case cType:
		Flot.Put( Valeur, sizeof( type__ ) );
		break;
	case cCasts:
	{
		const SET_( cast__ ) &Casts = *( const SET_( cast__ ) * )Valeur;
		POSITION__ Position = Casts.First();

		while ( Position  != NONE )
		{
			flw::Put( Casts( Position ), Flot );
			Position = Casts.Next( Position );
		}

		Flot.Put( 0 );
		break;
	}
	case cCommand:
		Flot.Put( Valeur, sizeof( command__ ) );
		break;
	case cBoolean:
		Flot.Put( Valeur, sizeof( bso__bool ) );
		break;
	case cId8:
		Flot.Put( Valeur, sizeof( bso__ubyte ) );
		break;
	case cId16:
		Flot.Put( Valeur, sizeof( bso__ushort ) );
		break;
	case cId32:
		Flot.Put( Valeur, sizeof( bso__ulong ) );
		break;
	case cByte:
		Flot.Put( Valeur, sizeof( bso__raw ) );
		break;
	case cBinary:
	{
		const SET_( bso__raw ) &D = *(const SET_( bso__raw ) *)Valeur;
		SIZE__ Nombre = D.Amount();

		if ( Nombre >= 255 )
		{
			Flot.Put( 255U );
			Nombre -= 255;

			if ( Nombre >= 65535 )
			{
				Flot.Put( 255U );
				Flot.Put( 255U );
				Nombre -= 65535;

				if ( Nombre >= 16777215 )
				{
					Flot.Put( 255U );
					Flot.Put( 255U );
					Flot.Put( 255U );

					Nombre -= 16777215;

					Flot.Put( ( (flw::data__)( Nombre & ( 255 << 24 ) ) >> 24 ) );
				}
				Flot.Put( (flw::data__)( ( Nombre & ( 255 << 16 ) ) >> 16 ) );
			}
			Flot.Put( (flw::data__)( ( Nombre & ( 255 << 8 ) ) >> 8 ) );
		}
		Flot.Put( (flw::data__)( Nombre & 255 ) );

		while( Nombre < D.Amount() )
			Flot.Put( D( Nombre++ ) );

		break;
	}
	case cChar:
		Flot.Put( Valeur, sizeof( bso__char ) );
		break;
	case cString:
	{
		const str_string_ &S =*(const str_string_ *)Valeur;
		POSITION__ Position = 0;

		while ( Position < S.Amount() )
			Flot.Put( S( Position++ ) );

		Flot.Put( 0 );
		break;
	}
	case cPointer:
		Flot.Put( &Valeur, sizeof( void * ) );
		break;
	default:
		ERRu();	//r Case inconnue.
		break;
	}
}

void brkrqm::request_manager___::GetValue_(
	  cast Cast,
	  void *Valeur )
{
	switch ( Cast ) {
	case cObject:
	{
		Channel_->Get( sizeof( object__ ), Valeur );
		break;
	}
	case cType:
	{
		Channel_->Get( sizeof( type__ ), Valeur );
		break;
	}
	case cCasts:
	{
		cast__ Cast = cInvalid;
		SET_( cast__ ) &Casts = *( SET_( cast__ ) *)Valeur;
		bso__bool Fin = false;
		bso__bool Array = false;

		flw::Get( *Channel_, Cast );

		do
		{
			if ( Cast == cArray ) {
				if ( Array )
					ERRc();
				else
					Array = true;
			}
					
		
			if ( Cast == cEnd )
				if ( Array )
					Array = false;
				else
					Fin = true;

			Casts.Add( Cast );
			flw::Get( *Channel_, Cast );
		}
		while( ( Cast != cEnd ) || !Fin || Array);

		break;
	}
	case cCommand:
	{
		Channel_->Get( sizeof( command__ ), Valeur );
		break;
	}
	case cBoolean:
	{
		Channel_->Get( sizeof( bso__bool ), Valeur );
		break;
	}
	case cId8:
	{
		Channel_->Get( sizeof( bso__ubyte ), Valeur );
		break;
	}
	case cId16:
	{
		Channel_->Get( sizeof( bso__ushort ), Valeur );
		break;
	}
	case cId32:
	{
		Channel_->Get( sizeof( bso__ulong ), Valeur );
		break;
	}
	case cByte:
	{
		Channel_->Get( sizeof( bso__raw ), Valeur );
		break;
	}
	case cBinary:
	{
		SET_( bso__raw ) &Binaire = *(SET_( bso__raw ) *)Valeur;
		bso__ulong Nombre = Channel_->Get();

		if ( Nombre == 255 )
		{
			Nombre += ( Channel_->Get() << 8 ) | Channel_->Get();

			if ( Nombre == 65790 )
			{
				Nombre += ( Channel_->Get() << 16 ) | ( Channel_->Get() << 8 ) | Channel_->Get();

				if ( Nombre == 16843005 )
				{
					Nombre += ( Channel_->Get() << 24 ) | ( Channel_->Get() << 16 ) | ( Channel_->Get() << 8 ) | Channel_->Get();

					if ( Nombre < 16843005 )
						ERRu();
				}
			}
		}

		while( Nombre-- )
			Binaire.Add( Channel_->Get() );

		break;
	}
	case cChar:
	{
		Channel_->Get( sizeof( bso__char ), Valeur );
		break;
	}
	case cString:
	{
			str_string_ &Chaine = *( str_string_ *)Valeur;
			bso__char C;

		while ( C = Channel_->Get() )
			Chaine.Add( C );

		break;
	}
	case cPointer:
	{
		Channel_->Get( sizeof( void * ), Valeur );
		break;
	}
	default:
		ERRu();
		break;
	}
}

void brkrqm::request_manager___::GetValue(
	cast Cast,
	void *Valeur )
{
	if ( Array_ != NONE )
		ERRb();

	if ( GetCast_() != Cast )
		ERRb();

	GetValue_( Cast, Valeur );

	if ( ( Cast_ = GetCast_() ) == cArray )
	{
		Array_ = Position_;
		Cast_ = cInvalid;
	}
}

bso__bool brkrqm::request_manager___::GetArrayValue(
	cast Cast,
	void *Valeur )
{
	bso__bool NotLast = true;

	if ( GetCast_() != Cast )
		ERRu();

	GetValue_( Cast, Valeur );

	if ( Array_ != NONE )
	{
		if ( ( Cast_ = GetCast_() ) == cEnd )
		{
			Array_ = NONE;
			Cast_ = cInvalid;

			NotLast = false;
		}
	} else
		ERRb();

	if ( ( Cast_ = GetCast_() ) == cArray )
	{
		Array_ = Position_;
		Cast_ = cInvalid;
	}

	return NotLast;
}

POSITION__ brkrqm::request_manager___::FindEndOfArray_( POSITION__ P )
{
	while( ( P != NONE ) && ( Description_->Casts.Read( P ) != cEnd ) )
		P = Description_->Casts.Next( P );
		
#ifdef BRKRQM_DBG
	if ( P == NONE )
		ERRc();
#endif

	return P;
}

namespace {
	struct a_ {
		int i;
	};

	struct b_
	: public a_
	{};

	struct bv_
	: public a_
	{
		virtual ~bv_( void ) {}
	};

	template <class t> static inline void Test_( t &T )
	{
		void *P = &T;
		a_ &A = *(a_ *)P;

		if ( &A.i != &T.i )
			ERRt();
	}

	static inline void Test_( void )
	{
	ERRProlog
		b_ B;
		bv_ BV;
	ERRBegin

		Test_( B );
	#if !defined( CPE__VC )
		/* 'VC++' is the only compiler I know which doesn't pass the following test.
		For this compiler, it doesn't matter.
		If another compiler doesn't pass this test, that's very few chance that this
		library doesn't work properly, but it can happen. If so, tell me.
		*/
		Test_( BV );
	#endif
	ERRErr
	ERREnd
	ERREpilog
	}
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class brkrqmpersonnalization
: public brkrqmtutor
{
public:
	brkrqmpersonnalization( void )
	{
	ERRFProlog
	ERRFBegin
		/* I assume that the compiler must have a behaviour
		that is not guaranted by the standard. Than I verify
		if the compiler has this behaviour. */
		Test_();
	ERRFErr
		cout << '\t' << ERRMessage() << '\a' << endl;
		exit( EXIT_FAILURE );	// Due to the fact that 'main()'.isn't yet called.
	ERRFEnd
	ERRFEpilog
	}
	~brkrqmpersonnalization( void )
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

static brkrqmpersonnalization Tutor;

ttr_tutor &BRKRQMTutor = Tutor;
