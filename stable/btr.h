/*
  Header for the 'btr' library by Claude L. Simon (epeios@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org) 

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
  along with this program; if not, go to http://www.fsf.org or write to the
  
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
//C Claude L. SIMON (epeios@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "set.h"

// La généalogie d'un noeud.
class btr__genealogie
{
public:
	POSITION__
		Pere,
		// reference du pere
		Fils,
		// reference du fils (fils droit)
		Fille;
		// reference de la fille (fils gauche)
	void ppr( void )
	{
		Pere = Fils = Fille = NONE;
	}
	// préparation d'un objet vierge
	void prg( void )
	{
		ppr();
	}
	// virginisation
	btr__genealogie( void )
	{
		ppr();
	}
	// constructeur
/*	~btr_liens( void )
	{
		prg();
	}
*/	// virginisation
	// exceptionellement; pour éviter l'apparition de 'warnings'
};



/**********************************************/
/* CLASSE DE GESTION D'UN CONTENAIRE D'ARBRES */
/**********************************************/

//c Binary tree.
class btr_binary_tree_
{
	// Fonction appelée lors de l'écriture dans 'Flot' de l'élément à la position 'Position'.
/*	virtual void BTRWrite(
		flo_sortie_portable_ &Flot,
		POSITION__ Position ) const{ ERRu(); }
*/	/* Fonction appelée lors de la lecture d'un élément à partir de 'Flot'.
	Doit créer un nouvel élément et retourner sa position. */
//	virtual POSITION__ BTRRead( flo_entree_portable_ &Flot ){ ERRU(); return 0; }
private:
// fonction
	SIZE__ Nombre_( void ) const
	{
		return Liens.Amount();
	}
	// retourne le nombre de liens
	void Preparer_(
		POSITION__ Debut,
		POSITION__ Fin );
	// prépare les liens de 'Debut' à 'Fin'
	void Liberer_(
		POSITION__ Debut,
		POSITION__ Fin );
	// libère les liens de 'Debut' à 'Fin'
	void InvaliderPere_( POSITION__ Noeud )
	{
		btr__genealogie Genealogie = Liens( Noeud );

		Genealogie.Pere = NONE;
		Liens.Write( Genealogie, Noeud );
	}
	// invalide le pere
	void InvaliderFils_( POSITION__ Noeud )
	{
		btr__genealogie Genealogie = Liens( (POSITION__)Noeud );

		Genealogie.Fils = NONE;
		Liens.Write( Genealogie, (POSITION__)Noeud );
	}
	// invalide le fils
	void InvaliderFille_( POSITION__ Noeud )
	{
		btr__genealogie Genealogie = Liens( (POSITION__)Noeud );

		Genealogie.Fille = NONE;
		Liens.Write( Genealogie, (POSITION__)Noeud );
	}
	// invalide la fille
	bso__bool APere_( POSITION__ Element ) const
	{
		return Liens( Element ).Pere != NONE;
	}
	// retourne != 0 si 'Element' a un pere, 0 sinon
	bso__bool AFils_( POSITION__ Element ) const
	{
		return Liens( Element ).Fils != NONE;
	}
	// retourne != 0 si 'Element' a un fils, 0 sinon
	bso__bool AFille_( POSITION__ Element ) const
	{
		return Liens( Element ).Fille != NONE;
	}
	// retourne != 0 si 'Element' a une fille, 0 sinon
	POSITION__ Pere_( POSITION__ Element ) const
	{
		return Liens( (POSITION__)Element ).Pere;
	}
	// retourne le pere de 'Element'
	POSITION__ Fils_( POSITION__ Element ) const
	{
		return Liens( Element ).Fils;
	}
	// retourne le fils de 'Element'
	POSITION__ Fille_( POSITION__ Element ) const
	{
		return Liens( Element ).Fille;
	}
	// retourne la fille de 'Element'
	void AdopterFils_(
		POSITION__ Pere,
		POSITION__ Fils )
	{
		btr__genealogie GPere = Liens( Pere ), GFils = Liens( Fils );

		GPere.Fils = Fils;
		GFils.Pere = Pere;

		Liens.Write( GPere, Pere );
		Liens.Write( GFils, Fils );
	}
	// 'Fils' devient fils de 'Pere'
	void AdopterFille_(
		POSITION__ Pere,
		POSITION__ Fille )
	{
		btr__genealogie GPere = Liens( Pere ), GFille = Liens( Fille );

		GPere.Fille = Fille;
		GFille.Pere = Pere;

		Liens.Write( GPere,(POSITION__)Pere );
		Liens.Write( GFille, (POSITION__)Fille );
	}
	// 'Fille' devient fille de 'Pere'
	/* Remonte l'arbre 'Racine' à partir de 'Depart', dont la présence d'une fille
	n'est pas testée, jusqu'à trouver un aieul mâle avec une fille. */
	POSITION__ TrouverAieulMaleAvecFille_(
		POSITION__ Depart,
		POSITION__ Racine ) const;
	POSITION__ ForceParent_(
		POSITION__ Node,
		POSITION__  Parent )
	{
		btr__genealogie Lien = Liens( Node );
		POSITION__ AncienPere = Lien.Pere;
		
		Lien.Pere = Parent;
		
		Liens.Write( Lien, Node );
		
		return AncienPere;
	}
public:
	struct s
	{
		SET_( btr__genealogie )::s Liens;
	};
	// La table des liens.
	SET_( btr__genealogie ) Liens;
	btr_binary_tree_( s &S )
	: Liens( S.Liens )
	{}
	void reset( bool P = true )
	{
		Liens.reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		Liens.plug( M );
	}
	void plug( MEMORY_DRIVER_ &M )
	{
		Liens.plug( M );
	}
	// Operateur d'affectation.
	btr_binary_tree_ &operator =( const btr_binary_tree_ &O )
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
	SIZE__ Extent( void ) const
	{
		return Liens.Extent();
	}
	//f Amount of node in the tree.
	SIZE__ Amount( void ) const
	{
		return Liens.Amount();
	}
	//f Return parent of 'Node'.
	POSITION__ Parent( POSITION__ Node ) const
	{
		return Pere_( Node );
	}
	//f Return left of 'Node'..
	POSITION__ Left( POSITION__ Node ) const
	{
		return Fils_( Node ) ;
	}
	//f Return right of 'Node'..
	POSITION__ Right( POSITION__ Node ) const
	{
		return Fille_( Node );
	}
	/* Elague 'Noeud'; 'Noeud' devient la racine de l'arbre
	et perd donc son père. */
	//f Cut 'Node'. 'Node' becomes a root.
	void Cut( POSITION__ Node )
	{
		POSITION__ Pere = Pere_( Node );

		if ( AFils_( Pere ) && ( Fils_( Pere ) == Node ) )
			InvaliderFils_( Pere );
		else
			InvaliderFille_( Pere );

		InvaliderPere_( Node );
	}
	//f Return true if 'Child' is left of 'Parent'.
	bso__bool IsLeft(
		POSITION__ Child,
		POSITION__ Parent ) const
	{
		return Fils_( Parent ) == Child;
	}
	//f Return true if 'Child' is right of 'Parent'.
	bso__bool IsRight(
		POSITION__ Child,
		POSITION__ Parent ) const
	{
		return Fille_( Parent ) == Child;
	}
	//f Return true if 'Child' is child of 'Parent'.
	bso__bool IsChild(
		POSITION__ Child,
		POSITION__ Parent ) const
	{
		return ( Fils_( Parent ) == Child ) || ( Fille_( Parent ) == Child );
	}
	//f Return true if 'Parent' is parent of 'Child'.
	bso__bool IsParent(
		POSITION__ Parent,
		POSITION__ Child ) const
	{
		return Pere_( Child ) == Parent;
	}
	//f Return true if 'Node' is a child.
	bso__bool IsLeft( POSITION__ Node ) const
	{
		return APere_( Node ) && Fils_( Pere_( Node ) ) == Node;
	}
	//f Return true if 'Node' is a right.
	bso__bool IsRight( POSITION__ Node ) const
	{
		return APere_( Node ) && Fille_( Pere_( Node ) ) == Node;
	}
	//f Return true if 'Node' is child.
	bso__bool IsChild( POSITION__ Node ) const
	{
		return APere_( Node );
	}
	//f Return true if 'Node' is parent.
	bso__bool IsParent( POSITION__ Node ) const
	{
		return AFils_( Node ) || AFille_( Node );
	}
	//f 'Parent' take 'Child' as left.
	void TakeLeft(
		POSITION__ Parent,
		POSITION__ Child )
	{
		AdopterFils_( Parent, Child );
	}
	//f 'Parent' take 'Child' as right.
	void TakeRight(
		POSITION__ Parent,
		POSITION__ Child )
	{
		AdopterFille_( Parent, Child );
	}
	//f 'Child' becomes left of 'Parent'.
	void BecomeLeft(
		POSITION__ Child,
		POSITION__ Parent )
	{
		AdopterFils_( Parent, Child );
	}
	//f 'Child' becomes right of 'Parent'.
	void BecomeRight(
		POSITION__ Child,
		POSITION__ Parent )
	{
		AdopterFille_( Parent, Child );
	}
	//f Allocate enough room to handle 'Size' node.
	void Allocate( SIZE__ Size )
	{
		if ( Size > Nombre_() )
		{
			SIZE__ PrecCapacite = Nombre_();

			Liens.Allocate( Size );
			Preparer_( PrecCapacite, Size - 1 );
		}
		else if ( Size < Nombre_() )
		{
			Liberer_( Size, Nombre_() - 1 );
			Liens.Allocate( Size );
		}
	}
	//f Return true if 'Node' has right.
	bso__bool HasRight( POSITION__ Node ) const
	{
		return AFille_( Node );
	}
	//f Return true if 'Node' has left.
	bso__bool HasLeft( POSITION__ Node ) const
	{
		return AFils_( Node );
	}
	//f Return true if 'Node' has a child.
	bso__bool HasChild( POSITION__ Node ) const
	{
		return AFille_( Node ) || AFils_( Node );
	}
	//f Return true if 'Node' has a parent.
	bso__bool HasParent( POSITION__ Node ) const
	{
		return APere_( Node );
	}
	//f Force the parent from 'Node' to 'Parent'. Return the previous parent.
	POSITION__ ForceParent(
		POSITION__ Node,
		POSITION__ Parent )
	{
		return ForceParent_( Node, Parent );
	}
/*	// Ecrit dans 'Flot' l'arbre de racine l'élément à 'Position'.
	void EcrireDansFlot(
		flo_sortie_portable_ &Flot,
		POSITION__ Position ) const;
	/* Lecture de l'abre contenu dans 'Flot'. La valeur retournée correspond à
	la position de la racine. */
	//	POSITION__ LireDeFlot( flo_entree_portable_ &Flot );
	//f Print to 'OFfow' the structure of the tree whith root 'Root'.
	void PrintStructure(
		POSITION__ Root,
		txf::text_oflow___ &OFlow ) const;
	// Sert à parcourir l'arbre de racine 'Racine'. Retourne le noeud aprés 'Position'.
/*	POSITION__ Suivant(
		POSITION__ Courant,
		POSITION__ Racine ) const
	{
		POSITION__ &Temp = Racine;

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

AUTO( btr_binary_tree )


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
