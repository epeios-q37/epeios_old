/*
  'btr' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'btr' header file ('btr.h').
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

#define BTR__COMPILATION

#include "btr.h"

class btrtutor
: public ttr_tutor
{
public:
	btrtutor( void )
	: ttr_tutor( BTR_NAME )
	{
#ifdef BTR_DBG
		Version = BTR_VERSION " (DBG)";
#else
		Version = BTR_VERSION;
#endif
		Owner = BTR_OWNER;
		Date = "$Date$";
	}
	virtual ~btrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#define liens			btr_liens
#define arbre_binaire	btr_binary_tree_
#define element			btr_element
#define index			btr__index


void arbre_binaire::Liberer_(
	POSITION__ Debut,
	POSITION__ Fin )
{
	do {
		if ( APere_( Debut ) )
		{
			if ( IsLeft( Debut ) )
				InvaliderFils_( Liens( Debut ).Pere );
			else
				InvaliderFille_( Liens( Debut ).Pere );

			InvaliderPere_( Debut );
		}

		if ( AFils_( Debut ) )
		{
			InvaliderPere_( Liens( Debut ).Fils );
			InvaliderFils_( Debut );
		}

		if ( AFille_( Debut ) )
		{
			InvaliderPere_( Liens( Debut ).Fille );
			InvaliderFille_( Debut );
		}
	}
	while( ++Debut <= Fin );
}

void arbre_binaire::Preparer_(
	POSITION__ Debut,
	POSITION__ Fin )
{
	do {
		InvaliderPere_( Debut );
		InvaliderFille_( Debut );
		InvaliderFils_( Debut );
	}
	while( ++Debut <= Fin );
}


// Ecrit dans 'Flot' l'arbre de racine l'élément à 'Position'.
/*
void arbre_binaire::EcrireDansFlot(
	flo_sortie_portable_ &Flot,
	mbs__position Position ) const
{
	mbs__position_ Courant = Position();
	fdm__bool DoitBoucler = true, RemonteeFille = false, RemonteeFils = false;

	Flot << begin << tag << 'R' << data;

	ABBEcrire( Flot, Courant );

	while ( DoitBoucler )
	{
		if ( !RemonteeFille && !RemonteeFils && AFille( Courant ) )
		{
			Courant = Fille_( Courant );
			Flot << tag << 'f' << data;
			ABBEcrire( Flot, Courant );
		}
		else if ( !RemonteeFils && AFils( Courant ) )
		{
			RemonteeFille = 0;

			Courant = Fils_( Courant );

			Flot << tag << 'F' << data;
			ABBEcrire( Flot, Courant );
		}
		else if ( Courant != Position() )
		{
			if ( EstFille( Courant ) )
			{
				RemonteeFils = 0;
				RemonteeFille = 1;
			}
			else
			{
				RemonteeFils = 1;
				RemonteeFille = 0;
			}

			Courant = Pere_( Courant );

			Flot << tag << 'P' << data << nothing;
		}
		else
			DoitBoucler = 0;
	}

	Flot << end;
}
*/

/* Lecture de l'abre contenu dans 'Flot'. La valeur retournée correspond à
la position de la racine. */
/*
mbs__position abb_arbre_binaire_::LireDeFlot( flo_entree_portable_ &Flot )
{
	mbs__position_ Courant, Nouveau;
	fdm__ubyte Type;

	Flot >> begin >> tag >> data;

	if ( *Flot.LastTag() != 'R' )
		ERRF();

	Courant = ABBLire( Flot )();

	while( !Flot.End() )
	{
		Flot >> tag >> data;

		if ( *Flot.LastTag() == 'P' )
		{
			Courant = Pere_( Courant );
			Flot >> nothing;
		}
		else if ( *Flot.LastTag() == 'F' )
		{
			Nouveau = ABBLire( Flot )();
			AdopterFils( Courant, Nouveau );
			Courant = Nouveau;
		}
		else if ( *Flot.LastTag() == 'f' )
		{
			Nouveau = ABBLire( Flot )();
			AdopterFille( Courant, Nouveau );
			Courant = Nouveau;
		}
	}

	return Courant;
}
*/


static void AfficherBlancs_(
	txf::text_oflow___ &Flot,
	bso__ulong Nombre )
{
	do {
		Flot.Put( '.' );
	} while ( Nombre-- );
}

//f Affiche la structure de l'arbre dans 'Flot'. A des fins de deboggage.
void btr_binary_tree_::PrintStructure(
	POSITION__ Racine,
	txf::text_oflow___ &Flot ) const
{
	bso__ulong Niveau = 0;
	bso__bool Fils = false, Fille = false;
	POSITION__ Courant = Racine;

	Flot << '*';

	do {
		if ( Fils )
		{
			Fils = Fille = false;

			if ( HasRight( Courant ) )
			{
				Courant = this->Right( Courant );

				Flot << txf::nl;
				AfficherBlancs_( Flot, Niveau );
				Flot << '*';

				Niveau++;
			}
			else
			{
				if ( IsLeft( Courant ) )
					Fils = true;
				else
					Fille = true;

				Courant = Parent( Courant );
				Niveau--;
			}
		}
		else if ( Fille )
		{
			Fils = Fille = false;

			if ( IsLeft( Courant ) )
				Fils = true;
			else
				Fille = true;

			Courant = Parent( Courant );
			Niveau--;
		}
		else if ( HasLeft( Courant ) )
		{
			Courant = this->Left( Courant );

			Flot << '*';
			Niveau++;
		}
		else if ( HasRight( Courant ) )
		{
			Courant = this->Right( Courant );
			Flot << txf::nl;
			AfficherBlancs_( Flot, Niveau );
			Flot << '*';
			Niveau++;
		}
		else if ( IsLeft( Courant ) )
		{
			Courant = Parent( Courant );
			Fils = true;
			Niveau--;
		}
		else if ( IsRight( Courant ) )
		{
			Courant = Parent( Courant );
			Fille = true;
			Niveau--;
		}
	} while ( Niveau || Fils );

	Flot << txf::nl << txf::sync;
}

/* Remonte l'arbre 'Racine' à partir de 'Depart', dont la présence d'une fille
n'est pas testée, jusqu'à trouver un aieul avec une fille. */
/*
mbs__position abb_arbre_binaire_::TrouverAieulMaleAvecFille_(
	mbs__position Depart,
	mbs__position Racine ) const
{
	mbs__position &Courant = Depart;
	fdm__bool EtaitFille = false;

	do {
		EtaitFille = EstFille( Courant );
		Courant = Pere( Courant );
	} while( ( EtaitFille || !AFille( Courant ) )
			 && ( Courant != Racine ) );

	if ( ( Courant == Racine ) && EtaitFille ) 
		Courant = ABB_INEXISTANT;

	return Courant;
}
*/




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class btrpersonnalization
: public btrtutor
{
public:
	btrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~btrpersonnalization( void )
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

static btrpersonnalization Tutor;

ttr_tutor &BTRTutor = Tutor;
