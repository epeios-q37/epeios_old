/*
	'uym' library by Claude SIMON (csimon@epeios.org)
	Requires the 'uym' header file ('uym.h').
	Copyright (C) 2000-2003 Claude SIMON (csimon@epeios.org).

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

#define UYM__COMPILATION

#include "uym.h"

class uymtutor
: public ttr_tutor
{
public:
	uymtutor( void )
	: ttr_tutor( UYM_NAME )
	{
#ifdef UYM_DBG
		Version = UYM_VERSION "\b\bD $";
#else
		Version = UYM_VERSION;
#endif
		Owner = UYM_OWNER;
		Date = "$Date$";
	}
	virtual ~uymtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace uym;

bso::sign__ uym::Compare(
	const untyped_memory_ &E1,
	const untyped_memory_ &E2,
	row__ DebutE1,
	row__ DebutE2,
	size__ Nombre )
{
	bso::sign__ Resultat;
ERRProlog
	datum__ *Tampon = NULL;
ERRBegin
	if ( Nombre )
	{
		fam::size__ Taille = Nombre > 30000 ? 60000 : Nombre * 2;
		Tampon = (datum__ *)fam::FAM.Allocate( Taille, 4 );

		if ( !Tampon )
		{
			DebutE1 += Nombre;
			DebutE2 += Nombre;

			while( Nombre && ( E1.Get( DebutE1 - Nombre ) == E2.Get( DebutE2 - Nombre ) ) )
				Nombre--;

			if ( Nombre )
				Resultat = ( ( E1.Get( DebutE1 - Nombre ) > E2.Get( DebutE2 - Nombre ) ) ? 1 : -1 );
			else
			{
				datum__ C1 = E1.Get( 0 ),  C2 = E2.Get( 0 );

				Resultat = C1 == C2 ? 0 : C1 > C2 ? 1 : -1;
			}
		}
		else
		{
			datum__ *T1 = Tampon;
			datum__ *T2 = Tampon + ( Taille /= 2 );


			do
			{
				if ( Taille > Nombre )
					Taille = Nombre;

				E1.Recall( DebutE1, Taille, T1 );
				E2.Recall( DebutE2, Taille, T2 );

				DebutE1 += Taille;
				DebutE2 += Taille;
				Nombre -= Taille;

				Resultat = memcmp( T1, T2, Taille );
			}
			while ( ( Resultat == 0 ) && ( Nombre != 0 ) );
		}
	}
	else
		Resultat =  0;
ERRErr
ERREnd
	if ( Tampon )
		fam::FAM.Free( Tampon );
ERREpilog
	return Resultat;
}

void uym::_Copy(
	const class untyped_memory_ &Source,
	row__ PosSource,
	class untyped_memory_ &Dest,
	row__ PosDest,
	size__ Nombre,
	datum__ *Tampon,
	bsize__ TailleTampon )
{
	if ( PosSource >= PosDest )
	{
		while( Nombre > TailleTampon )
		{
			Source.Recall( PosSource, TailleTampon, Tampon );
			Dest.Store( Tampon, TailleTampon, PosDest );

			PosSource += TailleTampon;
			PosDest += TailleTampon;
			Nombre -= TailleTampon;
		}

		if ( Nombre )
		{
			Source.Recall( PosSource, Nombre, Tampon );
			Dest.Store( Tampon, (bsize__)Nombre, PosDest );
		}
	}
	else
	{
		PosDest += Nombre;
		PosSource += Nombre;

		while( Nombre > TailleTampon )
		{
			Source.Recall( PosSource - TailleTampon, TailleTampon, Tampon );
			Dest.Store( Tampon, TailleTampon, PosDest - TailleTampon );

			PosSource -= TailleTampon;
			PosDest -= TailleTampon;
			Nombre -= TailleTampon;
		}

		if ( Nombre )
		{
			Source.Recall( PosSource - Nombre, Nombre, Tampon );
			Dest.Store( Tampon, (bsize__)Nombre, PosDest - Nombre );
		}
	}
}


/* Duplique 'Objet' 'Nombre' fois à partir de 'Position'
sachant qu'il est de taille 'Taille'. */
void untyped_memory_::Store(
	const datum__ *Objet,
	bsize__ Taille,
	row__ Position,
	size__ Nombre )
{
	while ( Nombre-- )
	{
		Store( Objet, Taille, Position );
		Position += Taille;
	}
}

row__ untyped_memory_::Search(
	const datum__ *Objet,
	size_t Taille,
	row__ Debut,
	row__ Fin ) const
{
	row__ Retour = NONE;
ERRProlog
	fam::size__ TailleTampon = Taille * ( ( Fin - Debut ) / Taille );
	datum__ *Tampon = NULL;
	bool Trouve = false;
ERRBegin

	if ( ( Tampon = (datum__ *)fam::FAM.Allocate( TailleTampon, Taille ) ) != NULL )
	{
		while( ( !Trouve ) && ( ( Debut + Taille ) <= Fin ) )
		{
			size_t PositionDansTampon = 0;

			if ( ( Debut + TailleTampon ) > Fin )
				TailleTampon = Taille * ( ( Fin - Debut ) / Taille );

			Recall( Debut, TailleTampon, Tampon );

			while( ( !Trouve ) && ( PositionDansTampon < TailleTampon ) )
			{
				Trouve = !memcmp( Objet, (char *)Tampon + PositionDansTampon, Taille );
				PositionDansTampon += Taille;
			}

			Debut += TailleTampon;
		}

		Debut -= TailleTampon;
	}
	else
	{
		while( ( !Trouve ) && ( ( Debut + Taille ) <= Fin ) )
		{
			size_t PositionRelative = 0;

			while ( ( PositionRelative < Taille )
					&& ( Get( Debut + PositionRelative )
						 == Objet[PositionRelative] ) )
			{};

			if ( PositionRelative == Taille )
				Trouve = true;
			else
				Debut += Taille;
		}
	}

	if ( Trouve )
		Retour = Debut;
ERRErr
ERREnd
	if ( Tampon )
		fam::FAM.Free( Tampon );
ERREpilog
	return Retour;
}


void untyped_memory_::Store(
	const untyped_memory_ &Source,
	size__ Amount,
	row__ Position,
	row__ Offset )
{
ERRProlog
	datum__ *Tampon = NULL;
ERRBegin

	if ( Amount > UYM_MIN_BUFFER_SIZE )
	{
		fam::size__ Taille = ( Amount > UYM_MAX_BUFFER_SIZE ? UYM_MAX_BUFFER_SIZE : Amount );
		Tampon = (datum__ *)fam::FAM.Allocate( Taille, ( Taille > UYM_MIN_BUFFER_SIZE ? UYM_MIN_BUFFER_SIZE : Taille ) );

		if ( Tampon )
			_Copy( Source, Position, *this, Offset, Amount, Tampon, Taille );
		else
		{
			datum__ Buffer[UYM_MIN_BUFFER_SIZE];
			_Copy( Source, Position, *this, Offset, Amount, Buffer, sizeof( Buffer ) );
		}

	}
	else
	{
		datum__ Buffer[UYM_MIN_BUFFER_SIZE];
		_Copy( Source, Position, *this, Offset, Amount, Buffer, sizeof( Buffer ) );
	}
ERRErr
ERREnd
	if ( Tampon )
		fam::FAM.Free( Tampon );
ERREpilog
}


void untyped_memory_::write(
	row__ Position,
	size__ Quantite,
	flw::oflow___ &OFlow ) const
{
ERRProlog
	datum__ TamponSecurite[UYM_MIN_BUFFER_SIZE];
	datum__ *Tampon = TamponSecurite;
	size_t Taille = sizeof( TamponSecurite );
ERRBegin
	if ( Quantite > UYM_MIN_BUFFER_SIZE )
	{
		Taille = ( Quantite > UYM_MAX_BUFFER_SIZE ? UYM_MAX_BUFFER_SIZE : Quantite );
		Tampon = (datum__ *)fam::FAM.Allocate( Taille, ( Taille > UYM_MIN_BUFFER_SIZE ? UYM_MIN_BUFFER_SIZE : Taille ) );

		if ( !Tampon )
		{
			Tampon = TamponSecurite;
			Taille = sizeof( TamponSecurite );
		}
	}

	while( Quantite )
	{
		if ( Quantite < Taille )
			Taille = Quantite;

		Recall( Position, Taille, Tampon );
#ifdef UYM_DBG
	if ( Taille > FLW_AMOUNT_MAX )
		ERRc(),
#endif
		OFlow.Put( Tampon, (flw::amount__)Taille );

		Quantite -= Taille;
		Position += Taille;
	}
ERRErr
ERREnd
	if ( ( Tampon != TamponSecurite ) && ( Tampon != NULL ) )
		fam::FAM.Free( Tampon );
ERREpilog
}

void untyped_memory_::read(
	flw::iflow___ &IFlow,
	row__ Position,
	size__ Quantite )
{
ERRProlog
	datum__ TamponSecurite[UYM_MIN_BUFFER_SIZE];
	datum__ *Tampon = TamponSecurite;
	size_t Taille = sizeof( TamponSecurite );
ERRBegin
	if ( Quantite > UYM_MIN_BUFFER_SIZE )
	{
		Taille = ( Quantite > UYM_MAX_BUFFER_SIZE ? UYM_MAX_BUFFER_SIZE : Quantite );
		Tampon = (datum__ *)fam::FAM.Allocate( Taille, ( Taille > UYM_MIN_BUFFER_SIZE ? UYM_MIN_BUFFER_SIZE : Taille ) );

		if ( !Tampon )
		{
			Tampon = TamponSecurite;
			Taille = sizeof( TamponSecurite );
		}
	}

	while( Quantite )
	{
		if ( Quantite < Taille )
			Taille = Quantite;

#ifdef UYM_DBG
	if ( Taille > FLW_AMOUNT_MAX )
		ERRc(),
#endif
		IFlow.Get( (flw::amount__)Taille, Tampon );

		Store( Tampon, Taille, Position );

		Quantite -= Taille;
		Position += Taille;
	}
ERRErr
ERREnd
	if ( ( Tampon != TamponSecurite ) && ( Tampon != NULL ) )
		fam::FAM.Free( Tampon );
ERREpilog
}

void uym::_Fill(
	const datum__ *Object,
	bsize__ Size,
	size__ Count,
	row__ Position,
	datum__ *Data )
{
	while( Count )
		memcpy( Data + Size * Count + Position, Object, Size );
}

#if 0
row__ uym::_Position(
	const datum__ *Objet,
	bsize__ Size,
	row__ Begin,
	row__ End,
	const datum__ *Data )
{
	while( ( Begin < End ) && ( memcmp( Data + Begin, Objet, Size ) ) )
		Begin += Size;

	if ( Begin > End )
		return NONE;
	else
		return Begin;
}
#endif


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class uympersonnalization
: public uymtutor
{
public:
	uympersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~uympersonnalization( void )
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

static uympersonnalization Tutor;

ttr_tutor &UYMTutor = Tutor;
