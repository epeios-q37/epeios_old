/*
  Header for the 'btr' library by Claude L. Simon (csimon@webmails.com)
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@webmails.com) 

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

#ifndef BTR__INC
#define BTR__INC

#define BTR_NAME		"BTR"

#define	BTR_VERSION	"$Revision$"	

#define BTR_OWNER		"the Epeios project (http://epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &BTRTutor;

#if defined( XXX_DBG ) && !defined( BTR_NODBG )
#define BTR_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (csimon@webmails.com)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bch.h"

namespace btr {
	using namespace epeios;

	// La généalogie d'un noeud.
	class genealogie__
	{
	public:
		row_t__
			Pere,
			// reference du pere
			Fils,
			// reference du fils (fils droit)
			Fille;
			// reference de la fille (fils gauche)
		void reset( bool = true )
		{
			Pere = Fils = Fille = NONE;
		}
		// virginisation
		genealogie__( void )
		{
			reset( false );
		}
		// constructeur
	};

	typedef bch::E_BUNCH_( genealogie__ ) genealogies_;

	class liens_
	: public genealogies_
	{
	public:
		struct s
		: public genealogies_::s
		{};
		liens_( s &S )
		: genealogies_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			genealogies_::reset( P );
		}
		//f Initialization.
		void Init( void )
		{
			genealogies_::Init();
		}
		// retourne le nombre de liens
		void Preparer(
			row_t__ Debut,
			row_t__ Fin );
		// prépare les liens de 'Debut' à 'Fin'
		void Liberer(
			row_t__ Debut,
			row_t__ Fin );
		// libère les liens de 'Debut' à 'Fin'
		void InvaliderPere( row_t__ Noeud )
		{
			genealogie__ Genealogie = genealogies_::Read( Noeud );

			Genealogie.Pere = NONE;
			genealogies_::Write( Genealogie, Noeud );
		}
		// invalide le pere
		void InvaliderFils( row_t__ Noeud )
		{
			genealogie__ Genealogie = genealogies_::Read( Noeud );

			Genealogie.Fils = NONE;
			genealogies_::Write( Genealogie, Noeud );
		}
		// invalide le fils
		void InvaliderFille( row_t__ Noeud )
		{
			genealogie__ Genealogie = genealogies_::Read( Noeud );

			Genealogie.Fille = NONE;
			genealogies_::Write( Genealogie, (row_t__)Noeud );
		}
		// invalide la fille
		bso::bool__ APere( row_t__ Element ) const
		{
			return genealogies_::Read( Element ).Pere != NONE;
		}
		// retourne != 0 si 'Element' a un pere, 0 sinon
		bso::bool__ AFils( row_t__ Element ) const
		{
			return genealogies_::Read( Element ).Fils != NONE;
		}
		// retourne != 0 si 'Element' a un fils, 0 sinon
		bso::bool__ AFille( row_t__ Element ) const
		{
			return genealogies_::Read( Element ).Fille != NONE;
		}
		// retourne != 0 si 'Element' a une fille, 0 sinon
		row_t__ Pere( row_t__ Element ) const
		{
			return genealogies_::Read( Element ).Pere;
		}
		// retourne le pere de 'Element'
		row_t__ Fils( row_t__ Element ) const
		{
			return genealogies_::Read( Element ).Fils;
		}
		// retourne le fils de 'Element'
		row_t__ Fille( row_t__ Element ) const
		{
			return genealogies_::Read( Element ).Fille;
		}
		// retourne la fille de 'Element'
		void AdopterFils(
			row_t__ Pere,
			row_t__ Fils )
		{
			genealogie__ GPere = genealogies_::Read( Pere ), GFils = genealogies_::Read( Fils );

			GPere.Fils = Fils;
			GFils.Pere = Pere;

			genealogies_::Write( GPere, Pere );
			genealogies_::Write( GFils, Fils );
		}
		// 'Fils' devient fils de 'Pere'
		void AdopterFille(
			row_t__ Pere,
			row_t__ Fille )
		{
			genealogie__ GPere = genealogies_::Read( Pere ), GFille = genealogies_::Read( Fille );

			GPere.Fille = Fille;
			GFille.Pere = Pere;

			genealogies_::Write( GPere,Pere );
			genealogies_::Write( GFille, Fille );
		}
		// 'Fille' devient fille de 'Pere'
		/* Remonte l'arbre 'Racine' à partir de 'Depart', dont la présence d'une fille
		n'est pas testée, jusqu'à trouver un aieul mâle avec une fille. */
		row_t__ TrouverAieulMaleAvecFille(
			row_t__ Depart,
			row_t__ Racine ) const;
		row_t__ ForceParent(
			row_t__ Node,
			row_t__  Parent )
		{
			genealogie__ Lien = genealogies_::Read( Node );
			row_t__ AncienPere = Lien.Pere;

			Lien.Pere = Parent;

			genealogies_::Write( Lien, Node );

			return AncienPere;
		}
		//f Return true if 'Node' is a child.
		bso::bool__ IsLeft( row_t__ Node ) const
		{
			return APere( Node ) && Fils( Pere( Node ) ) == Node;
		}
		//f Return true if 'Node' is a right.
		bso::bool__ IsRight( row_t__ Node ) const
		{
			return APere( Node ) && Fille( Pere( Node ) ) == Node;
		}
		//f Return true if 'Node' is child.
		bso::bool__ IsChild( row_t__ Node ) const
		{
			return APere( Node );
		}
		//f Return true if 'Node' is parent.
		bso::bool__ IsParent( row_t__ Node ) const
		{
			return AFils( Node ) || AFille( Node );
		}
		void PrintStructure(
			row_t__ Racine,
			txf::text_oflow___ &Flot ) const;
		//f Return true if 'Node' has right.
		bso::bool__ HasRight( row_t__ Node ) const
		{
			return AFille( Node );
		}
		//f Return true if 'Node' has left.
		bso::bool__ HasLeft( row_t__ Node ) const
		{
			return AFils( Node );
		}
		//f Return true if 'Node' has a child.
		bso::bool__ HasChild( row_t__ Node ) const
		{
			return AFille( Node ) || AFils( Node );
		}
		//f Return true if 'Node' has a parent.
		bso::bool__ HasParent( row_t__ Node ) const
		{
			return APere( Node );
		}
	};



	/**********************************************/
	/* CLASSE DE GESTION D'UN CONTENAIRE D'ARBRES */
	/**********************************************/

	//c Binary tree.
	template <typename r> class binary_tree_
	{
	private:
		// retourne le nombre de liens
		void Preparer_(
			epeios::row_t__ Debut,
			epeios::row_t__ Fin )
		{
			Liens.Preparer( Debut, Fin );
		}
		// prépare les liens de 'Debut' à 'Fin'
		void Liberer_(
			epeios::row_t__ Debut,
			epeios::row_t__ Fin )
		{
			Liens.Liberer( Debut, Fin );
		}
		// libère les liens de 'Debut' à 'Fin'
		void InvaliderPere_( epeios::row_t__ Noeud )
		{
			Liens.InvaliderPere( Noeud );
		}
		// invalide le pere
		void InvaliderFils_( epeios::row_t__ Noeud )
		{
			Liens.InvaliderFils( Noeud );
		}
		// invalide le fils
		void InvaliderFille_( epeios::row_t__ Noeud )
		{
			Liens.InvaliderFille( Noeud );
		}
		// invalide la fille
		bso::bool__ APere_( epeios::row_t__ Element ) const
		{
			return Liens.APere( Element );
		}
		// retourne != 0 si 'Element' a un pere, 0 sinon
		bso::bool__ AFils_( epeios::row_t__ Element ) const
		{
			return Liens.AFils( Element );
		}
		// retourne != 0 si 'Element' a un fils, 0 sinon
		bso::bool__ AFille_( epeios::row_t__ Element ) const
		{
			return Liens.AFille( Element );
		}
		// retourne != 0 si 'Element' a une fille, 0 sinon
		epeios::row_t__ Pere_( epeios::row_t__ Element ) const
		{
			return Liens.Pere( Element );
		}
		// retourne le pere de 'Element'
		epeios::row_t__ Fils_( epeios::row_t__ Element ) const
		{
			return Liens.Fils( Element );
		}
		// retourne le fils de 'Element'
		epeios::row_t__ Fille_( epeios::row_t__ Element ) const
		{
			return Liens.Fille( Element );
		}
		// retourne la fille de 'Element'
		void AdopterFils_(
			epeios::row_t__ Pere,
			epeios::row_t__ Fils )
		{
			Liens.AdopterFils( Pere, Fils );
		}
		// 'Fils' devient fils de 'Pere'
		void AdopterFille_(
			epeios::row_t__ Pere,
			epeios::row_t__ Fille )
		{
			Liens.AdopterFille( Pere, Fille );
		}
		// 'Fille' devient fille de 'Pere'
		/* Remonte l'arbre 'Racine' à partir de 'Depart', dont la présence d'une fille
		n'est pas testée, jusqu'à trouver un aieul mâle avec une fille. */
		epeios::row_t__ TrouverAieulMaleAvecFille_(
			epeios::row_t__ Depart,
			epeios::row_t__ Racine ) const
		{
			return Liens.TrouverAieulMaleAvecFille( Depart, Racine );
		}
		epeios::row_t__ ForceParent_(
			epeios::row_t__ Node,
			epeios::row_t__  Parent )
		{
			return Liens.ForceParent( Node, Parent );
		}
	public:
		struct s
		{
			liens_::s Liens;
		};
		// La table des liens.
		liens_ Liens;
		binary_tree_( s &S )
		: Liens( S.Liens )
		{}
		void reset( bool P = true )
		{
			Liens.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Liens.plug( M );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &M )
		{
			Liens.plug( M );
		}
		// Operateur d'affectation.
		binary_tree_ &operator =( const binary_tree_ &O )
		{
			Liens = O.Liens;

			return *this;
		}
	/*	void ecrire( flo_sortie_ &F ) const
		{
			Liens.ecrire( F );
		}
		void lire( flo_entree_ &F )
		{
			Liens.lire( F );
		}
	*/	//f Initialization.
		void Init( void )
		{
			Liens.Init();
		}
		//f Extent of the tree.
		epeios::size__ Extent( void ) const
		{
			return Liens.Extent();
		}
		//f Amount of node in the tree.
		epeios::size__ Amount( void ) const
		{
			return Liens.Amount();
		}
		//f Return parent of 'Node'.
		r Parent( r Node ) const
		{
			return Pere_( *Node );
		}
		//f Return left of 'Node'..
		r Left( r Node ) const
		{
			return Fils_( *Node ) ;
		}
		//f Return right of 'Node'..
		r Right( r Node ) const
		{
			return Fille_( *Node );
		}
		/* Elague 'Noeud'; 'Noeud' devient la racine de l'arbre
		et perd donc son père. */
		//f Cut 'Node'. 'Node' becomes a root.
		void Cut( r Node )
		{
			epeios::row_t__ Pere = Pere_( *Node );

			if ( AFils_( Pere ) && ( Fils_( Pere ) == *Node ) )
				InvaliderFils_( Pere );
			else
				InvaliderFille_( Pere );

			InvaliderPere_( *Node );
		}
		//f Return true if 'Child' is left of 'Parent'.
		bso::bool__ IsLeft(
			r Child,
			r Parent ) const
		{
			return Fils_( *Parent ) == Child;
		}
		//f Return true if 'Child' is right of 'Parent'.
		bso::bool__ IsRight(
			r Child,
			r Parent ) const
		{
			return Fille_( *Parent ) == Child;
		}
		//f Return true if 'Child' is child of 'Parent'.
		bso::bool__ IsChild(
			r Child,
			r Parent ) const
		{
			return ( Fils_( *Parent ) == Child ) || ( Fille_( *Parent ) == Child );
		}
		//f Return true if 'Parent' is parent of 'Child'.
		bso::bool__ IsParent(
			r Parent,
			r Child ) const
		{
			return Pere_( *Child ) == Parent;
		}
		//f Return true if 'Node' is a child.
		bso::bool__ IsLeft( r Node ) const
		{
			return APere_( *Node ) && Fils_( Pere_( *Node ) ) == *Node;
		}
		//f Return true if 'Node' is a right.
		bso::bool__ IsRight( r Node ) const
		{
			return APere_( *Node ) && Fille_( Pere_( *Node ) ) == *Node;
		}
		//f Return true if 'Node' is child.
		bso::bool__ IsChild( r Node ) const
		{
			return APere_( *Node );
		}
		//f Return true if 'Node' is parent.
		bso::bool__ IsParent( r Node ) const
		{
			return AFils_( *Node ) || AFille_( *Node );
		}
		//f 'Parent' take 'Child' as left.
		void TakeLeft(
			r Parent,
			r Child )
		{
			AdopterFils_( *Parent, *Child );
		}
		//f 'Parent' take 'Child' as right.
		void TakeRight(
			r Parent,
			r Child )
		{
			AdopterFille_( *Parent, *Child );
		}
		//f 'Child' becomes left of 'Parent'.
		void BecomeLeft(
			r Child,
			r Parent )
		{
			AdopterFils_( *Parent, *Child );
		}
		//f 'Child' becomes right of 'Parent'.
		void BecomeRight(
			r Child,
			r Parent )
		{
			AdopterFille_( *Parent, *Child );
		}
		//f Allocate enough room to handle 'Size' node.
		void Allocate(
			epeios::size__ Size,
			aem::mode Mode = aem::mDefault )
		{
			if ( Size > Liens.Amount() )
			{
				epeios::size__ PrecCapacite = Liens.Amount();

				Liens.Allocate( Size, Mode );
				Preparer_( PrecCapacite, Size - 1 );
			}
			else if ( Size < Liens.Amount() )
			{
				Liberer_( Size, Liens.Amount() - 1 );
				Liens.Allocate( Size, Mode );
			}
		}
		//f Return true if 'Node' has right.
		bso::bool__ HasRight( r Node ) const
		{
			return AFille_( *Node );
		}
		//f Return true if 'Node' has left.
		bso::bool__ HasLeft( r Node ) const
		{
			return AFils_( *Node );
		}
		//f Return true if 'Node' has a child.
		bso::bool__ HasChild( r Node ) const
		{
			return AFille_( *Node ) || AFils_( *Node );
		}
		//f Return true if 'Node' has a parent.
		bso::bool__ HasParent( r Node ) const
		{
			return APere_( *Node );
		}
		//f Force the parent from 'Node' to 'Parent'. Return the previous parent.
		r ForceParent(
			r Node,
			r Parent )
		{
			return ForceParent_( *Node, *Parent );
		}
	/*	// Ecrit dans 'Flot' l'arbre de racine l'élément à 'Position'.
		void EcrireDansFlot(
			flo_sortie_portable_ &Flot,
			r Position ) const;
		/* Lecture de l'abre contenu dans 'Flot'. La valeur retournée correspond à
		la position de la racine. */
		//	r LireDeFlot( flo_entree_portable_ &Flot );
		//f Print to 'OFfow' the structure of the tree whith root 'Root'.
		void PrintStructure(
			r Root,
			txf::text_oflow___ &OFlow ) const
		{
			Liens.PrintStructure( *Root, OFlow );
		}
		// Sert à parcourir l'arbre de racine 'Racine'. Retourne le noeud aprés 'Position'.
	/*	r Suivant(
			r Courant,
			r Racine ) const
		{
			row_t__ &Temp = Racine;

			if ( AFils( Courant ) )
				Courant = Fils( Courant );
			else if ( AFille( Courant ) )
				Courant = Fille( Courant );
			else if ( ( Courant = TrouverAieulMaleAvecFille_( Courant, Racine ) ) != BTR_INEXISTANT )
					Courant = Fille( Courant );

			return Courant;
		}
	*/
	};

	AUTOt( binary_tree )

	#define E_BTREEt_( r )	binary_tree_<r>
	#define E_BTREEt( r )	binary_tree<r>

	#define E_BTREE_	E_BTREEt_( epeios::row__ )
	#define E_BTREE		E_BTREEt( epeios::row__ )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
