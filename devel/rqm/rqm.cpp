/*
  'rqm' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'rqm' header file ('rqm.h').
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

#define RQM__COMPILATION

#include "rqm.h"

class rqmtutor
: public ttr_tutor
{
public:
	rqmtutor( void )
	: ttr_tutor( RQM_NAME )
	{
#ifdef RQM_DBG
		Version = RQM_VERSION " (DBG)";
#else
		Version = RQM_VERSION;
#endif
		Owner = RQM_OWNER;
		Date = "$Date$";
	}
	virtual ~rqmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include <fstream.h>

const char *(&RQMCastsNames)[rqm::c_amount] = broker0::CastsNames;

void rqm_description_::Init(
	const char *Name,
	rqm::cast Cast,
	... )
{
	va_list L;

	this->Name.Init();
	Casts.Init();

	this->Name = Name;

	Casts.Add( Cast );

	va_start( L, Cast );

	while ( Cast != rqm::cEnd ) {
		Cast = va_arg( L, rqm::cast );
		Casts.Add( Cast );
	}

	while ( ( Cast = va_arg( L, rqm::cast ) ) != rqm::cEnd )
		Casts.Add( Cast );

	va_end( L );
}

void rqm_description_::Init(
	const char *Name,
	const rqm::cast *Casts )
{
	int i = 0;
	rqm::cast Caste;

	this->Name.Init();
	this->Casts.Init();

	this->Name = Name;

	do
	{
		Caste = Casts[i++];
		this->Casts.Add( Caste );
	} while ( Caste != rqm::cEnd );

	while ( ( Caste = Casts[i++] ) != rqm::cEnd )
		this->Casts.Add( Caste );
}


POSITION__ rqm_descriptions_::Add(
	const char *Name,
	rqm::cast Cast,
	... )
{
	POSITION__ Position = CONTAINER_( rqm_description_ )::New();
	va_list L;
	rqm_description_ &Description = CONTAINER_( rqm_description_ )::operator()( Position );

	Description.Init();
	Description.Name = Name;

	Description.Add( Cast );

	va_start( L, Cast );

	while ( Cast != rqm::cEnd ) {
		Cast = va_arg( L, rqm::cast );
		Description.Add( Cast );
	}

	while ( ( Cast = va_arg( L, rqm::cast ) ) != rqm::cEnd )
		Description.Add( Cast );

	va_end( L );

	CONTAINER_( rqm_description_ )::Sync();

	return Position;
}

POSITION__ rqm_descriptions_::Position( const char *Name ) const
{
	POSITION__ Position = CONTAINER_( rqm_description_ )::First();
ERRProlog
	CITEM( rqm_description_ ) Description;
ERRBegin

	Description.Init( *this );

	while( ( Position != NONE ) && ( Description( Position ).Name != str_string( Name ) ) )
		Position = CONTAINER_( rqm_description_ )::Next( Position );
ERRErr
ERREnd
ERREpilog
	return Position;
}

POSITION__ rqm_descriptions_::Position( const rqm_description_ &Description ) const
{
	POSITION__ Position = CONTAINER_( rqm_description_ )::First();
ERRProlog
	CITEM( rqm_description_ ) D;
ERRBegin

	D.Init( *this );

	while( ( Position != NONE )
		    && ( ( D( Position ).Name != Description.Name )
		         || ( D( Position ).Casts != Description.Casts ) ) )
		Position = CONTAINER_( rqm_description_ )::Next( Position );
ERRErr
ERREnd
ERREpilog
	return Position;
}

// Return the id of the cast 'Cast', or 'agr::cInvalid' if non-existent.
rqm::cast RQMIdCaste( const str_string_ &Caste )
{
	rqm::cast Retour = rqm::cEnd;
ERRProlog
	rqm__cast Id = rqm::c_amount;
	str_string S;
ERRBegin
	S.Init();

	while( --Id && ( S = RQMCastsNames[Id], S != Caste ) );

	Retour = (rqm::cast)(int)Id;
ERRErr
ERREnd
ERREpilog
	return Retour;
}

// Ajoute dans 'Flot' le paramètre 'Valeur' de caste 'Caste'.
void RQMAddValue(
	rqm::cast Caste,
	const void *Valeur,
	flw::oflow___ &Flot )
{
	RQMAddCast( Caste, Flot );

	switch( Caste ) {
	case rqm::cObject:
		Flot.Put( Valeur, sizeof( rqm__object ) );
		break;
	case rqm::cType:
		Flot.Put( Valeur, sizeof( rqm__type ) );
		break;
	case rqm::cCasts:
	{
		const SET_( rqm__cast ) &Castes = *( const SET_( rqm__cast ) * )Valeur;
		POSITION__ Position = 0;
		rqm__cast C;

		while ( Position < Castes.Amount() )
		{
			C = Castes( Position++ );
			Flot.Put( &C, sizeof( C ) );
		}

		Flot.Put( 0 );
		break;
	}
	case rqm::cCommand:
		Flot.Put( Valeur, sizeof( rqm__command ) );
		break;
	case rqm::cBoolean:
		Flot.Put( Valeur, sizeof( bso__bool ) );
		break;
	case rqm::cId8:
		Flot.Put( Valeur, sizeof( bso__ubyte ) );
		break;
	case rqm::cId16:
		Flot.Put( Valeur, sizeof( bso__ushort ) );
		break;
	case rqm::cId32:
		Flot.Put( Valeur, sizeof( bso__ulong ) );
		break;
	case rqm::cByte:
		Flot.Put( Valeur, sizeof( bso__raw ) );
		break;
	case rqm::cBinary:
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
	case rqm::cChar:
		Flot.Put( Valeur, sizeof( bso__char ) );
		break;
	case rqm::cString:
	{
		const str_string_ &S =*(const str_string_ *)Valeur;
		POSITION__ Position = 0;

		while ( Position < S.Amount() )
			Flot.Put( S( Position++ ) );

		Flot.Put( 0 );
		break;
	}
	case rqm::cPointer:
		Flot.Put( &Valeur, sizeof( void * ) );
		break;
	default:
		ERRu();	//r Case inconnue.
		break;
	}
}

void rqm__request_manager_::GetValue_(
	  rqm::cast Caste,
	  void *Valeur )
{
	switch ( Caste ) {
	case rqm::cObject:
	{
		Entree_->Get( sizeof( rqm__object ), Valeur );
		break;
	}
	case rqm::cType:
	{
		Entree_->Get( sizeof( rqm__type ), Valeur );
		break;
	}
	case rqm::cCasts:
	{
		rqm__cast Caste;
		SET_( rqm__cast ) &Castes = *( SET_( rqm__cast ) *)Valeur;
		bso__bool Fin = false;

		flw::Get( *Entree_, Caste );

		do
		{
			if ( Caste == rqm::cEnd )
				Fin = true;

			Castes.Add( Caste );
			flw::Get( *Entree_, Caste );
		}
		while( ( Caste != rqm::cEnd ) || !Fin );

		break;
	}
	case rqm::cCommand:
	{
		Entree_->Get( sizeof( rqm__command ), Valeur );
		break;
	}
	case rqm::cBoolean:
	{
		Entree_->Get( sizeof( bso__bool ), Valeur );
		break;
	}
	case rqm::cId8:
	{
		Entree_->Get( sizeof( bso__ubyte ), Valeur );
		break;
	}
	case rqm::cId16:
	{
		Entree_->Get( sizeof( bso__ushort ), Valeur );
		break;
	}
	case rqm::cId32:
	{
		Entree_->Get( sizeof( bso__ulong ), Valeur );
		break;
	}
	case rqm::cByte:
	{
		Entree_->Get( sizeof( bso__raw ), Valeur );
		break;
	}
	case rqm::cBinary:
	{
		SET_( bso__raw ) &Binaire = *(SET_( bso__raw ) *)Valeur;
		bso__ulong Nombre = Entree_->Get();

		if ( Nombre == 255 )
		{
			Nombre += ( Entree_->Get() << 8 ) | Entree_->Get();

			if ( Nombre == 65790 )
			{
				Nombre += ( Entree_->Get() << 16 ) | ( Entree_->Get() << 8 ) | Entree_->Get();

				if ( Nombre == 16843005 )
				{
					Nombre += ( Entree_->Get() << 24 ) | ( Entree_->Get() << 16 ) | ( Entree_->Get() << 8 ) | Entree_->Get();

					if ( Nombre < 16843005 )
						ERRu();
				}
			}
		}

		while( Nombre-- )
			Binaire.Add( Entree_->Get() );

		break;
	}
	case rqm::cChar:
	{
		Entree_->Get( sizeof( bso__char ), Valeur );
		break;
	}
	case rqm::cString:
	{
			str_string_ &Chaine = *( str_string_ *)Valeur;
			bso__char C;

		while ( C = Entree_->Get() )
			Chaine.Add( C );

		break;
	}
	case rqm::cPointer:
	{
		Entree_->Get( sizeof( void * ), Valeur );
		break;
	}
	default:
		ERRu();
		break;
	}
}

/*f Place dans 'Valeur' le contenu du prochain paramètre contenu dans la requête
sachant qu'il est de caste 'Caste'. Un contrôle est fait sur la caste.*/
bso__bool rqm__request_manager_::GetValue(
	rqm::cast Caste,
	void *Valeur )
{
	bso__bool Return = false;

	if ( GetCast_() != Caste )
		ERRu();

	GetValue_( Caste, Valeur );

	if ( Multi_ )
	{
		if ( ( Caste_ = GetCast_() ) == rqm::cEnd )
		{
			Multi_ = false;
			Caste_ = rqm::cInvalid;

			Return = true;
		}
	}

	if ( ( Caste_ = GetCast_() ) == rqm::cMulti )
	{
		Multi_ = true;
		Caste_ = rqm::cInvalid;
	}

	return Return;
}


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

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class rqmpersonnalization
: public rqmtutor
{
public:
	rqmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
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
	~rqmpersonnalization( void )
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

static rqmpersonnalization Tutor;

ttr_tutor &RQMTutor = Tutor;
