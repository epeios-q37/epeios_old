/*
  'bst' library by Claude L. Simon (simon@epeios.org)
  Requires the 'bst' header file ('bst.h').
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org).

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

#define BST__COMPILATION

#include "bst.h"

class bsttutor
: public ttr_tutor
{
public:
	bsttutor( void )
	: ttr_tutor( BST_NAME )
	{
#ifdef BST_DBG
		Version = BST_VERSION "\b\bD $";
#else
		Version = BST_VERSION;
#endif
		Owner = BST_OWNER;
		Date = "$Date$";
	}
	virtual ~bsttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

bso__bool ComparerBitABit_(
	bst_bits_set_ &E1,
	bst_bits_set_ &E2,
	BSIZE__ Nombre,
	POSITION__ P1,
	POSITION__ P2 )
{
	bso__bool Resultat = true;

	while ( Resultat && Nombre-- )
		Resultat = ( E1.Read( Nombre + P1 ) == E2.Read( Nombre + P2 ) );

	return Resultat;
}

/*
void bst_bits_::Dup( bst_bits_ &O )
{
	POSITION__ Position = O.S_->Capacite;

	Allouer( Position );

	while( Position-- )
		Ecrire( O.Lire( Position ), Position );
}
*/
/* Stocke dans 'D' 'O1' & 'O2', tous étant de taille 'Taille'.
'D' peut être 'O1' ou 'O2'. Usage interne. */
void BSTEt_(
	const bst__receptacle *O1,
	const bst__receptacle *O2,
	bst__receptacle *D,
	BSIZE__ Taille )
{
	while( Taille-- )
		D[Taille] = O1[Taille] & O2[Taille];
}

/* Stocke dans 'D' 'O1' | 'O2', tous étant de taille 'Taille'.
'D' peut être 'O1' ou 'O2'. Usage interne. */
void BSTOu_(
	const bst__receptacle *O1,
	const bst__receptacle *O2,
	bst__receptacle *D,
	BSIZE__ Taille )
{
	while( Taille-- )
		D[Taille] = O1[Taille] | O2[Taille];
}


/* Stocke dans 'D' ~'O' tou deux de taille 'Taille'. 'O' peut être 'D'.
Usage interne. */
void BSTNon_(
	const bst__receptacle *O,
	bst__receptacle *D,
	BSIZE__ Taille )
{
	while( Taille-- )
		D[Taille] = ~O[Taille];
}

/* Stocke dans 'D' 'O1' ^ 'O2', tous étant de taille 'Taille'.
'D' peut être 'O1' ou 'O2'. Usage interne. */
void BSTDff_(
	const bst__receptacle *O1,
	const bst__receptacle *O2,
	bst__receptacle *D,
	BSIZE__ Taille )
{
	while( Taille-- )
		D[Taille] = O1[Taille] ^ O2[Taille];
}

BSIZE__ BSTCompter_(
	const bst__receptacle *O,
	BSIZE__ Taille )
{
	BSIZE__ Nombre = 0;

	while ( Taille-- )
		if ( O[Taille] )
			for( int i = 0; i < BST_NB_BITS_RECEPTACLE; i++ )
				if ( O[Taille] & ( 1 << i ) )
					Nombre++;

	return Nombre;
}


struct pointeur
{
	const bst__receptacle *&Pointeur;
	bst__receptacle Read( POSITION__ Position ) const
	{
		return Pointeur[Position];
	}
	void Write(
		const bst__receptacle &,
		POSITION__ )
	{
		ERRu();
	}
	pointeur( const bst__receptacle *&P )
	: Pointeur( P ){}
};


POSITION__ BSTSuivant_(
	const bst__receptacle *O,
	BSIZE__ Taille,
	bso__bool Valeur,
	POSITION__ Courant )
{
	POSITION__ Limite = Taille * BST_NB_BITS_RECEPTACLE;
	pointeur P( O );

	while( ( ++Courant < Limite )
			 && ( bst__fonctions<pointeur>::Lire( Courant, P ) != Valeur ) );


	if ( Courant == Limite )
		return NONE;
	else
		return Courant;
}

POSITION__ BSTPrecedent_(
	const bst__receptacle *O,
	bso__bool Valeur,
	POSITION__ Courant )
{
	pointeur P( O );

	while( ( bst__fonctions<pointeur>::Lire( --Courant, P ) != Valeur ) && Courant );

	if ( !Courant && bst__fonctions<pointeur>::Lire( 0, P ) != Valeur )
		return NONE;
	else
		return Courant;
}

txf::text_oflow___ &operator <<(
	txf::text_oflow___ &Flot,
	bst_bits_set_ &M )
{
	for( POSITION__ P = 0; P < M.Size(); P++ )
		if ( M.Read( P ) )
			Flot << '*';
		else
			Flot << '-';

	return Flot;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class bstpersonnalization
: public bsttutor
{
public:
	bstpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~bstpersonnalization( void )
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

static bstpersonnalization Tutor;

ttr_tutor &BSTTutor = Tutor;
