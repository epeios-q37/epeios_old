/*
	'btr' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'btr' header file ('btr.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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
		Version = BTR_VERSION "\b\bD $";
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

#include "txf.h"

using namespace mdr;

#if 1
void btr::_nodes_manager_::Release(
	row_t__ Start,
	row_t__ End )
{
	do {
		if ( HasParent( Start ) )
		{
			if ( IsLeft( Start ) )
				ReleaseLeft( _nodes_::Get( Start ).Parent );
			else
				ReleaseRight( _nodes_::Get( Start ).Parent );

			ReleaseParent( Start );
		}

		if ( HasLeft( Start ) )
		{
			ReleaseParent( _nodes_::Get( Start ).Left );
			ReleaseLeft( Start );
		}

		if ( HasRight( Start ) )
		{
			ReleaseParent( _nodes_::Get( Start ).Right );
			ReleaseRight( Start );
		}
	}
	while( ++Start <= End );
}

void btr::_nodes_manager_::Prepare(
	row_t__ Start,
	row_t__ End )
{
	_node__ Node;

	do {
		Store( Node, Start );
	}
	while( ++Start <= End );
}


row_t__ btr::_nodes_manager_::SearchMostLeftNode(
	row_t__ Node,
	level__ &Level ) const
{
	Level = 0;

	while( HasLeft( Node ) ) {
		Node = Left( Node );
		Level++;
	}

	return Node;
}

// Retourne le premier noeud sans fille � partir de 'Position' en descendant par les fille.
row_t__ btr::_nodes_manager_::SearchMostRightNode(
	row_t__ Node,
	level__ &Level ) const
{
	while( HasRight( Node ) ) {
		Node = Right( Node );
		Level++;
	}

	return Node;
}

// Retourne le p�re du premier noeud qui est fils en remontant.
row_t__ btr::_nodes_manager_::ParentOfFirstLeftNode( row_t__ Node ) const
{
	while( !IsLeft( Node )
		   && HasParent( Node ) )
		Node = Parent( Node );

	if ( IsLeft( Node ) )
		return Parent( Node );
	else
		return NONE;
}

// Retourne le p�re du premier noeud qui est fille en remontant.
row_t__ btr::_nodes_manager_::ParentOfFirstRightNode( row_t__ Node ) const
{
	while( !IsRight( Node )
			&& HasParent( Node ) )
		Node = Parent( Node );

	if ( IsRight( Node ) )
		return Parent( Node );
	else
		return NONE;
}


// Ecrit dans 'Flot' l'arbre de racine l'�l�ment � 'Position'.
/*
void arbre_binaire::EcrireDansFlot(
	flo_sortie_portable_ &Flot,
	mbs__position Position ) const
{
	mbs__position_ Courant = Position();
	fdm__bool DoitBoucler = true, RemonteeRight = false, RemonteeLeft = false;

	Flot << begin << tag << 'R' << data;

	ABBEcrire( Flot, Courant );

	while ( DoitBoucler )
	{
		if ( !RemonteeRight && !RemonteeLeft && HasRight( Courant ) )
		{
			Courant = Right_( Courant );
			Flot << tag << 'f' << data;
			ABBEcrire( Flot, Courant );
		}
		else if ( !RemonteeLeft && HasLeft( Courant ) )
		{
			RemonteeRight = 0;

			Courant = Left_( Courant );

			Flot << tag << 'F' << data;
			ABBEcrire( Flot, Courant );
		}
		else if ( Courant != Position() )
		{
			if ( EstRight( Courant ) )
			{
				RemonteeLeft = 0;
				RemonteeRight = 1;
			}
			else
			{
				RemonteeLeft = 1;
				RemonteeRight = 0;
			}

			Courant = Parent_( Courant );

			Flot << tag << 'P' << data << nothing;
		}
		else
			DoitBoucler = 0;
	}

	Flot << end;
}
*/

/* Lecture de l'abre contenu dans 'Flot'. La valeur retourn�e correspond �
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
			Courant = Parent_( Courant );
			Flot >> nothing;
		}
		else if ( *Flot.LastTag() == 'F' )
		{
			Nouveau = ABBLire( Flot )();
			AdopterLeft( Courant, Nouveau );
			Courant = Nouveau;
		}
		else if ( *Flot.LastTag() == 'f' )
		{
			Nouveau = ABBLire( Flot )();
			AdopterRight( Courant, Nouveau );
			Courant = Nouveau;
		}
	}

	return Courant;
}
*/


static void AfficherBlancs_(
	txf::text_oflow__ &Flot,
	bso::ulong__ Nombre )
{
	do {
		Flot.Put( '.' );
	} while ( Nombre-- );
}

//f Affiche la structure de l'arbre dans 'Flot'. A des fins de deboggage.
void btr::_nodes_manager_::PrintStructure(
	row_t__ Racine,
	txf::text_oflow__ &Flot ) const
{
	bso::ulong__ Niveau = 0;
	bso::bool__ Left = false, Right = false;
	row_t__ Courant = Racine;

	Flot << '*';

	do {
		if ( Left )
		{
			Left = Right = false;

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
					Left = true;
				else
					Right = true;

				Courant = Parent( Courant );
				Niveau--;
			}
		}
		else if ( Right )
		{
			Left = Right = false;

			if ( IsLeft( Courant ) )
				Left = true;
			else
				Right = true;

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
			Left = true;
			Niveau--;
		}
		else if ( IsRight( Courant ) )
		{
			Courant = Parent( Courant );
			Right = true;
			Niveau--;
		}
	} while ( Niveau || Left );

	Flot << txf::nl << txf::commit;
}

/* Remonte l'arbre 'Racine' � partir de 'Depart', dont la pr�sence d'une fille
n'est pas test�e, jusqu'� trouver un aieul avec une fille. */
/*
mbs__position abb_arbre_binaire_::TrouverAieulMaleAvecRight_(
	mbs__position Depart,
	mbs__position Racine ) const
{
	mbs__position &Courant = Depart;
	fdm__bool EtaitRight = false;

	do {
		EtaitRight = EstRight( Courant );
		Courant = Parent( Courant );
	} while( ( EtaitRight || !HasRight( Courant ) )
			 && ( Courant != Racine ) );

	if ( ( Courant == Racine ) && EtaitRight ) 
		Courant = ABB_INEXISTANT;

	return Courant;
}
*/
#else // Version portable.
template <typename r_t, typename nodes> static void Release_(
	nodes &Nodes,
	row_t__ Start,
	row_t__ End )
{
	do {
		if ( Nodes.HasParent( Start ) )
		{
			if ( Nodes.IsLeft( Start ) )
				Nodes.ReleaseLeft( Nodes.Get( Start ).Parent );
			else
				Nodes.ReleaseRight( Nodes.Get( Start ).Parent );

			Nodes.ReleaseParent( Start );
		}

		if ( Nodes.HasLeft( Start ) )
		{
			Nodes.ReleaseParent( Nodes.Get( Start ).Left );
			Nodes.ReleaseLeft( Start );
		}

		if ( Nodes.HasRight( Start ) )
		{
			Nodes.ReleaseParent( Nodes.Get( Start ).Right );
			Nodes.ReleaseRight( Start );
		}
	}
	while( ++Start <= End );
}

template <typename r_t, typename nodes> static void Prepare_(
	nodes &Nodes,
	row_t__ Start,
	row_t__ End )
{
	do {
		Nodes.ReleaseParent( Start );
		Nodes.ReleaseRight( Start );
		Nodes.ReleaseLeft( Start );
	}
	while( ++Start <= End );
}

void btr::Release(
	_nodes_manager_<row_t__, _nodes_> &Nodes,
	row_t__ Start,
	row_t__ End )
{
	Release_< row_t__,_nodes_manager_<row_t__, _nodes_> >( Nodes, Start, End );
}

void btr::Prepare(
	_nodes_manager_<row_t__, _nodes_> &Nodes,
	row_t__ Start,
	row_t__ End )
{
	Prepare_< row_t__,_nodes_manager_<row_t__, _nodes_> >( Nodes, Start, End );
}


// Ecrit dans 'Flot' l'arbre de racine l'�l�ment � 'Position'.
/*
void arbre_binaire::EcrireDansFlot(
	flo_sortie_portable_ &Flot,
	mbs__position Position ) const
{
	mbs__position_ Courant = Position();
	fdm__bool DoitBoucler = true, RemonteeRight = false, RemonteeLeft = false;

	Flot << begin << tag << 'R' << data;

	ABBEcrire( Flot, Courant );

	while ( DoitBoucler )
	{
		if ( !RemonteeRight && !RemonteeLeft && HasRight( Courant ) )
		{
			Courant = Right_( Courant );
			Flot << tag << 'f' << data;
			ABBEcrire( Flot, Courant );
		}
		else if ( !RemonteeLeft && HasLeft( Courant ) )
		{
			RemonteeRight = 0;

			Courant = Left_( Courant );

			Flot << tag << 'F' << data;
			ABBEcrire( Flot, Courant );
		}
		else if ( Courant != Position() )
		{
			if ( EstRight( Courant ) )
			{
				RemonteeLeft = 0;
				RemonteeRight = 1;
			}
			else
			{
				RemonteeLeft = 1;
				RemonteeRight = 0;
			}

			Courant = Parent_( Courant );

			Flot << tag << 'P' << data << nothing;
		}
		else
			DoitBoucler = 0;
	}

	Flot << end;
}
*/

/* Lecture de l'abre contenu dans 'Flot'. La valeur retourn�e correspond �
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
			Courant = Parent_( Courant );
			Flot >> nothing;
		}
		else if ( *Flot.LastTag() == 'F' )
		{
			Nouveau = ABBLire( Flot )();
			AdopterLeft( Courant, Nouveau );
			Courant = Nouveau;
		}
		else if ( *Flot.LastTag() == 'f' )
		{
			Nouveau = ABBLire( Flot )();
			AdopterRight( Courant, Nouveau );
			Courant = Nouveau;
		}
	}

	return Courant;
}
*/


static void AfficherBlancs_(
	txf::text_oflow__ &Flot,
	bso::ulong__ Nombre )
{
	do {
		Flot.Put( '.' );
	} while ( Nombre-- );
}

//f Affiche la structure de l'arbre dans 'Flot'. A des fins de deboggage.
/*
void btr::_nodes_manager_::PrintStructure(
	row_t__ Racine,
	txf::text_oflow__ &Flot ) const
{
	bso::ulong__ Niveau = 0;
	bso::bool__ Left = false, Right = false;
	row_t__ Courant = Racine;

	Flot << '*';

	do {
		if ( Left )
		{
			Left = Right = false;

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
					Left = true;
				else
					Right = true;

				Courant = Parent( Courant );
				Niveau--;
			}
		}
		else if ( Right )
		{
			Left = Right = false;

			if ( IsLeft( Courant ) )
				Left = true;
			else
				Right = true;

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
			Left = true;
			Niveau--;
		}
		else if ( IsRight( Courant ) )
		{
			Courant = Parent( Courant );
			Right = true;
			Niveau--;
		}
	} while ( Niveau || Left );

	Flot << txf::nl << txf::sync;
}
*/
/* Remonte l'arbre 'Racine' � partir de 'Depart', dont la pr�sence d'une fille
n'est pas test�e, jusqu'� trouver un aieul avec une fille. */
/*
mbs__position abb_arbre_binaire_::TrouverAieulMaleAvecRight_(
	mbs__position Depart,
	mbs__position Racine ) const
{
	mbs__position &Courant = Depart;
	fdm__bool EtaitRight = false;

	do {
		EtaitRight = EstRight( Courant );
		Courant = Parent( Courant );
	} while( ( EtaitRight || !HasRight( Courant ) )
			 && ( Courant != Racine ) );

	if ( ( Courant == Racine ) && EtaitRight ) 
		Courant = ABB_INEXISTANT;

	return Courant;
}
*/
#endif


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
