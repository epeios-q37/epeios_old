/*
  'uym' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'uym' header file ('uym.h').
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
		Version = UYM_VERSION " (DBG)";
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

bso__sbyte UYMCompare(
	const uym_basic_memory_ &E1,
	const uym_basic_memory_ &E2,
	uym__position DebutE1,
	uym__position DebutE2,
	uym__size Nombre )
{
	bso__sbyte Resultat;
ERRProlog
	uym__data *Tampon = NULL;
ERRBegin
	if ( Nombre )
	{
		fam__size Taille = Nombre > 30000 ? 60000 : Nombre * 2;
		Tampon = (uym__data *)FAM.Allocate( Taille, 4 );

		if ( !Tampon )
		{
			DebutE1 += Nombre;
			DebutE2 += Nombre;

			while( Nombre && ( E1.Read( DebutE1 - Nombre ) == E2.Read( DebutE2 - Nombre ) ) )
				Nombre--;

			if ( Nombre )
				Resultat = ( E1.Read( DebutE1 - Nombre ) > E2.Read( DebutE2 - Nombre ) ) ? 1 : -1;
			else
			{
				uym__data C1 = E1.Read( 0 ),  C2 = E2.Read( 0 );

				Resultat = (  C1 == C2 ? 0 : C1 > C2 ? 1 : -1 );
			}
		}
		else
		{
			uym__data *T1 = Tampon;
			uym__data *T2 = Tampon + ( Taille /= 2 );


			do
			{
				if ( Taille > Nombre )
					Taille = Nombre;

				E1.Read( DebutE1, Taille, T1 );
				E2.Read( DebutE2, Taille, T2 );

				DebutE1 += Taille;
				DebutE2 += Taille;
				Nombre -= Taille;

				Resultat = memcmp( T1, T2, Taille );
			}
			while ( !Resultat && Nombre );
		}
	}
	else
		Resultat =  0;
ERRErr
ERREnd
	if ( Tampon )
		FAM.Free( Tampon );
ERREpilog
	return Resultat;
}

void UYMCopy_(
	const class uym_basic_memory_ &Source,
	uym__position PosSource,
	class uym_basic_memory_ &Dest,
	uym__position PosDest,
	uym__size Nombre,
	uym__data *Tampon,
	uym__bsize TailleTampon )
{
	if ( PosSource >= PosDest )
	{
		while( Nombre > TailleTampon )
		{
			Source.Read( PosSource, TailleTampon, Tampon );
			Dest.Write( Tampon, TailleTampon, PosDest );

			PosSource += TailleTampon;
			PosDest += TailleTampon;
			Nombre -= TailleTampon;
		}

		if ( Nombre )
		{
			Source.Read( PosSource, Nombre, Tampon );
			Dest.Write( Tampon, Nombre, PosDest );
		}
	}
	else
	{
		PosDest += Nombre;
		PosSource += Nombre;

		while( Nombre > TailleTampon )
		{
			Source.Read( PosSource - TailleTampon, TailleTampon, Tampon );
			Dest.Write( Tampon, TailleTampon, PosDest - TailleTampon );

			PosSource -= TailleTampon;
			PosDest -= TailleTampon;
			Nombre -= TailleTampon;
		}

		if ( Nombre )
		{
			Source.Read( PosSource - Nombre, Nombre, Tampon );
			Dest.Write( Tampon, Nombre, PosDest - Nombre );
		}
	}
}


/* Duplique 'Objet' 'Nombre' fois à partir de 'Position'
sachant qu'il est de taille 'Taille'. */
void uym_basic_memory_::Fill(
	const uym__data *Objet,
	uym__bsize Taille,
	uym__size Nombre,
	uym__position Position )
{
	while ( Nombre-- )
	{
		Write( Objet, Taille, Position );
		Position += Taille;
	}
}

uym__position uym_basic_memory_::Position(
	const uym__data *Objet,
	size_t Taille,
	uym__position Debut,
	uym__position Fin )
{
	uym__position Retour = NONE;
ERRProlog
	fam__size TailleTampon = Taille * ( ( Fin - Debut ) / Taille );
	uym__data *Tampon = NULL;
	bool Trouve = false;
ERRBegin

	if ( ( Tampon = (uym__data *)FAM.Allocate( TailleTampon, Taille ) ) != NULL )
	{
		while( ( !Trouve ) && ( ( Debut + Taille ) <= Fin ) )
		{
			size_t PositionDansTampon = 0;

			if ( ( Debut + TailleTampon ) > Fin )
				TailleTampon = Taille * ( ( Fin - Debut ) / Taille );

			Read( Debut, TailleTampon, Tampon );

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
					&& ( Read( Debut + PositionRelative )
						 == ((char *)Objet)[PositionRelative] ) );

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
		FAM.Free( Tampon );
ERREpilog
	return Retour;
}


void uym_basic_memory_::Write(
	const uym_basic_memory_ &Source,
	uym__size Quantity,
	uym__position Position,
	uym__position Offset )
{
ERRProlog
	uym__data *Tampon = NULL;
ERRBegin

	if ( Quantity > UYM_MIN_BUFFER_SIZE )
	{
		fam__size Taille = ( Quantity > UYM_MAX_BUFFER_SIZE ? UYM_MAX_BUFFER_SIZE : Quantity );
		Tampon = (uym__data *)FAM.Allocate( Taille, ( Taille > UYM_MIN_BUFFER_SIZE ? UYM_MIN_BUFFER_SIZE : Taille ) );

		if ( Tampon )
			UYMCopy_( Source, Position, *this, Offset, Quantity, Tampon, Taille );
		else
		{
			uym__data Buffer[UYM_MIN_BUFFER_SIZE];
			UYMCopy_( Source, Position, *this, Offset, Quantity, Buffer, sizeof( Buffer ) );
		}

	}
	else
	{
		uym__data Buffer[UYM_MIN_BUFFER_SIZE];
		UYMCopy_( Source, Position, *this, Offset, Quantity, Buffer, sizeof( Buffer ) );
	}
ERRErr
ERREnd
	if ( Tampon )
		FAM.Free( Tampon );
ERREpilog
}


void uym_basic_memory_::write(
	uym__position Position,
	uym__size Quantite,
	flw::oflow___ &OFlow ) const
{
ERRProlog
	uym__data TamponSecurite[UYM_MIN_BUFFER_SIZE];
	uym__data *Tampon = TamponSecurite;
	size_t Taille = sizeof( TamponSecurite );
ERRBegin
	if ( Quantite > UYM_MIN_BUFFER_SIZE )
	{
		Taille = ( Quantite > UYM_MAX_BUFFER_SIZE ? UYM_MAX_BUFFER_SIZE : Quantite );
		Tampon = (uym__data *)FAM.Allocate( Taille, ( Taille > UYM_MIN_BUFFER_SIZE ? UYM_MIN_BUFFER_SIZE : Taille ) );

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

		Read( Position, Taille, Tampon );
		OFlow.Put( Tampon, Taille );

		Quantite -= Taille;
		Position += Taille;
	}
ERRErr
ERREnd
	if ( ( Tampon != TamponSecurite ) && ( Tampon != NULL ) )
		FAM.Free( Tampon );
ERREpilog
}

void uym_basic_memory_::read(
	flw::iflow___ &IFlow,
	uym__position Position,
	uym__size Quantite )
{
ERRProlog
	uym__data TamponSecurite[UYM_MIN_BUFFER_SIZE];
	uym__data *Tampon = TamponSecurite;
	size_t Taille = sizeof( TamponSecurite );
ERRBegin
	if ( Quantite > UYM_MIN_BUFFER_SIZE )
	{
		Taille = ( Quantite > UYM_MAX_BUFFER_SIZE ? UYM_MAX_BUFFER_SIZE : Quantite );
		Tampon = (uym__data *)FAM.Allocate( Taille, ( Taille > UYM_MIN_BUFFER_SIZE ? UYM_MIN_BUFFER_SIZE : Taille ) );

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

		IFlow.Get( Taille, Tampon );

		Write( Tampon, Taille, Position );

		Quantite -= Taille;
		Position += Taille;
	}
ERRErr
ERREnd
	if ( ( Tampon != TamponSecurite ) && ( Tampon != NULL ) )
		FAM.Free( Tampon );
ERREpilog
}

//f Duplicate 'Count' times 'Object' of size 'Size' to 'Position'.
void uym__memory_core_::Fill(
	const uym__data *Object,
	uym__bsize Size,
	uym__size Count,
	uym__position Position )
{
	while( Count )
		memcpy( Data_ + Size * Count + Position, Object, Size );
}

//f Return the position from 'Object' of size 'Size' between 'Begin' and 'End' (excluded) o 'NONE' if non-existant.
uym__position uym__memory_core_::Position(
	const uym__data *Objet,
	uym__bsize Size,
	uym__position Begin,
	uym__position End )
{
	while( ( Begin < End ) && ( memcmp( Data_ + Begin, Objet, Size ) ) )
		Begin += Size;

	if ( Begin > End )
		return NONE;
	else
		return Begin;
}


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
