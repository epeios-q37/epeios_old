/*
  Header for the 'btr' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org) 

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

#ifndef BTR__INC
#define BTR__INC

#define BTR_NAME		"BTR"

#define	BTR_VERSION	"$Revision$"	

#define BTR_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &BTRTutor;

#if defined( XXX_DBG ) && !defined( BTR_NODBG )
#define BTR_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (simon@epeios.org)
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

	// La généalogie d'un noeud.
	class genealogie__
	{
	public:
		tym::row__
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



	/**********************************************/
	/* CLASSE DE GESTION D'UN CONTENAIRE D'ARBRES */
	/**********************************************/

	//c Binary tree.
	class binary_tree_
	{
		// Fonction appelée lors de l'écriture dans 'Flot' de l'élément à la position 'Position'.
	/*	virtual void BTRWrite(
			flo_sortie_portable_ &Flot,
			tym::row__ Position ) const{ ERRu(); }
	*/	/* Fonction appelée lors de la lecture d'un élément à partir de 'Flot'.
		Doit créer un nouvel élément et retourner sa position. */
	//	virtual tym::row__ BTRRead( flo_entree_portable_ &Flot ){ ERRU(); return 0; }
	private:
	// fonction
		tym::size__ Nombre_( void ) const
		{
			return Liens.Amount();
		}
		// retourne le nombre de liens
		void Preparer_(
			tym::row__ Debut,
			tym::row__ Fin );
		// prépare les liens de 'Debut' à 'Fin'
		void Liberer_(
			tym::row__ Debut,
			tym::row__ Fin );
		// libère les liens de 'Debut' à 'Fin'
		void InvaliderPere_( tym::row__ Noeud )
		{
			genealogie__ Genealogie = Liens( Noeud );

			Genealogie.Pere = NONE;
			Liens.Write( Genealogie, Noeud );
		}
		// invalide le pere
		void InvaliderFils_( tym::row__ Noeud )
		{
			genealogie__ Genealogie = Liens( (tym::row__)Noeud );

			Genealogie.Fils = NONE;
			Liens.Write( Genealogie, (tym::row__)Noeud );
		}
		// invalide le fils
		void InvaliderFille_( tym::row__ Noeud )
		{
			genealogie__ Genealogie = Liens( (tym::row__)Noeud );

			Genealogie.Fille = NONE;
			Liens.Write( Genealogie, (tym::row__)Noeud );
		}
		// invalide la fille
		bso__bool APere_( tym::row__ Element ) const
		{
			return Liens( Element ).Pere != NONE;
		}
		// retourne != 0 si 'Element' a un pere, 0 sinon
		bso__bool AFils_( tym::row__ Element ) const
		{
			return Liens( Element ).Fils != NONE;
		}
		// retourne != 0 si 'Element' a un fils, 0 sinon
		bso__bool AFille_( tym::row__ Element ) const
		{
			return Liens( Element ).Fille != NONE;
		}
		// retourne != 0 si 'Element' a une fille, 0 sinon
		tym::row__ Pere_( tym::row__ Element ) const
		{
			return Liens( (tym::row__)Element ).Pere;
		}
		// retourne le pere de 'Element'
		tym::row__ Fils_( tym::row__ Element ) const
		{
			return Liens( Element ).Fils;
		}
		// retourne le fils de 'Element'
		tym::row__ Fille_( tym::row__ Element ) const
		{
			return Liens( Element ).Fille;
		}
		// retourne la fille de 'Element'
		void AdopterFils_(
			tym::row__ Pere,
			tym::row__ Fils )
		{
			genealogie__ GPere = Liens( Pere ), GFils = Liens( Fils );

			GPere.Fils = Fils;
			GFils.Pere = Pere;

			Liens.Write( GPere, Pere );
			Liens.Write( GFils, Fils );
		}
		// 'Fils' devient fils de 'Pere'
		void AdopterFille_(
			tym::row__ Pere,
			tym::row__ Fille )
		{
			genealogie__ GPere = Liens( Pere ), GFille = Liens( Fille );

			GPere.Fille = Fille;
			GFille.Pere = Pere;

			Liens.Write( GPere,(tym::row__)Pere );
			Liens.Write( GFille, (tym::row__)Fille );
		}
		// 'Fille' devient fille de 'Pere'
		/* Remonte l'arbre 'Racine' à partir de 'Depart', dont la présence d'une fille
		n'est pas testée, jusqu'à trouver un aieul mâle avec une fille. */
		tym::row__ TrouverAieulMaleAvecFille_(
			tym::row__ Depart,
			tym::row__ Racine ) const;
		tym::row__ ForceParent_(
			tym::row__ Node,
			tym::row__  Parent )
		{
			genealogie__ Lien = Liens( Node );
			tym::row__ AncienPere = Lien.Pere;

			Lien.Pere = Parent;

			Liens.Write( Lien, Node );

			return AncienPere;
		}
	public:
		struct s
		{
			bch::E_BUNCH_( genealogie__ )::s Liens;
		};
		// La table des liens.
		bch::E_BUNCH_( genealogie__ ) Liens;
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

			if ( Nombre_() )
				Preparer_( 0, Nombre_() - 1 );
		}
		//f Extent of the tree.
		tym::size__ Extent( void ) const
		{
			return Liens.Extent();
		}
		//f Amount of node in the tree.
		tym::size__ Amount( void ) const
		{
			return Liens.Amount();
		}
		//f Return parent of 'Node'.
		tym::row__ Parent( tym::row__ Node ) const
		{
			return Pere_( Node );
		}
		//f Return left of 'Node'..
		tym::row__ Left( tym::row__ Node ) const
		{
			return Fils_( Node ) ;
		}
		//f Return right of 'Node'..
		tym::row__ Right( tym::row__ Node ) const
		{
			return Fille_( Node );
		}
		/* Elague 'Noeud'; 'Noeud' devient la racine de l'arbre
		et perd donc son père. */
		//f Cut 'Node'. 'Node' becomes a root.
		void Cut( tym::row__ Node )
		{
			tym::row__ Pere = Pere_( Node );

			if ( AFils_( Pere ) && ( Fils_( Pere ) == Node ) )
				InvaliderFils_( Pere );
			else
				InvaliderFille_( Pere );

			InvaliderPere_( Node );
		}
		//f Return true if 'Child' is left of 'Parent'.
		bso__bool IsLeft(
			tym::row__ Child,
			tym::row__ Parent ) const
		{
			return Fils_( Parent ) == Child;
		}
		//f Return true if 'Child' is right of 'Parent'.
		bso__bool IsRight(
			tym::row__ Child,
			tym::row__ Parent ) const
		{
			return Fille_( Parent ) == Child;
		}
		//f Return true if 'Child' is child of 'Parent'.
		bso__bool IsChild(
			tym::row__ Child,
			tym::row__ Parent ) const
		{
			return ( Fils_( Parent ) == Child ) || ( Fille_( Parent ) == Child );
		}
		//f Return true if 'Parent' is parent of 'Child'.
		bso__bool IsParent(
			tym::row__ Parent,
			tym::row__ Child ) const
		{
			return Pere_( Child ) == Parent;
		}
		//f Return true if 'Node' is a child.
		bso__bool IsLeft( tym::row__ Node ) const
		{
			return APere_( Node ) && Fils_( Pere_( Node ) ) == Node;
		}
		//f Return true if 'Node' is a right.
		bso__bool IsRight( tym::row__ Node ) const
		{
			return APere_( Node ) && Fille_( Pere_( Node ) ) == Node;
		}
		//f Return true if 'Node' is child.
		bso__bool IsChild( tym::row__ Node ) const
		{
			return APere_( Node );
		}
		//f Return true if 'Node' is parent.
		bso__bool IsParent( tym::row__ Node ) const
		{
			return AFils_( Node ) || AFille_( Node );
		}
		//f 'Parent' take 'Child' as left.
		void TakeLeft(
			tym::row__ Parent,
			tym::row__ Child )
		{
			AdopterFils_( Parent, Child );
		}
		//f 'Parent' take 'Child' as right.
		void TakeRight(
			tym::row__ Parent,
			tym::row__ Child )
		{
			AdopterFille_( Parent, Child );
		}
		//f 'Child' becomes left of 'Parent'.
		void BecomeLeft(
			tym::row__ Child,
			tym::row__ Parent )
		{
			AdopterFils_( Parent, Child );
		}
		//f 'Child' becomes right of 'Parent'.
		void BecomeRight(
			tym::row__ Child,
			tym::row__ Parent )
		{
			AdopterFille_( Parent, Child );
		}
		//f Allocate enough room to handle 'Size' node.
		void Allocate(
			tym::size__ Size,
			bso__bool Adjust = false )
		{
			if ( Size > Nombre_() )
			{
				tym::size__ PrecCapacite = Nombre_();

				Liens.Allocate( Size, Adjust );
				Preparer_( PrecCapacite, Size - 1 );
			}
			else if ( Size < Nombre_() )
			{
				Liberer_( Size, Nombre_() - 1 );
				Liens.Allocate( Size, Adjust );
			}
		}
		//f Return true if 'Node' has right.
		bso__bool HasRight( tym::row__ Node ) const
		{
			return AFille_( Node );
		}
		//f Return true if 'Node' has left.
		bso__bool HasLeft( tym::row__ Node ) const
		{
			return AFils_( Node );
		}
		//f Return true if 'Node' has a child.
		bso__bool HasChild( tym::row__ Node ) const
		{
			return AFille_( Node ) || AFils_( Node );
		}
		//f Return true if 'Node' has a parent.
		bso__bool HasParent( tym::row__ Node ) const
		{
			return APere_( Node );
		}
		//f Force the parent from 'Node' to 'Parent'. Return the previous parent.
		tym::row__ ForceParent(
			tym::row__ Node,
			tym::row__ Parent )
		{
			return ForceParent_( Node, Parent );
		}
	/*	// Ecrit dans 'Flot' l'arbre de racine l'élément à 'Position'.
		void EcrireDansFlot(
			flo_sortie_portable_ &Flot,
			tym::row__ Position ) const;
		/* Lecture de l'abre contenu dans 'Flot'. La valeur retournée correspond à
		la position de la racine. */
		//	tym::row__ LireDeFlot( flo_entree_portable_ &Flot );
		//f Print to 'OFfow' the structure of the tree whith root 'Root'.
		void PrintStructure(
			tym::row__ Root,
			txf::text_oflow___ &OFlow ) const;
		// Sert à parcourir l'arbre de racine 'Racine'. Retourne le noeud aprés 'Position'.
	/*	tym::row__ Suivant(
			tym::row__ Courant,
			tym::row__ Racine ) const
		{
			tym::row__ &Temp = Racine;

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

	AUTO( binary_tree )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
