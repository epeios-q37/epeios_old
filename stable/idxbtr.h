/*
  Header for the 'idxbtr' library by Claude L. Simon (epeios@epeios.org)
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

#ifndef IDXBTR__INC
#define IDXBTR__INC

#define IDXBTR_NAME		"IDXBTR"

#define	IDXBTR_VERSION	"$Revision$"	

#define IDXBTR_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &IDXBTRTutor;

#if defined( XXX_DBG ) && !defined( IDXBTR_NODBG )
#define IDXBTR_DBG 
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
#include "btr.h"
#include "que.h"
#include "stk.h"

/* structure de description d'un arbre.
Est utilisé lors du rééquilibrage d'un arbre. Usage interne. */
struct idxbtr__desc
{
	// Racine de l'arbre. N'a pas de fils droit. L'arbre de fils gauche est complet.
	POSITION__ Racine;
	// Niveau de l'arbre.
	SIZE__ Niveau;
};

//c Tree-based index, fast sorting, but slow browsing.
class idxbtr_tree_index_
: public btr_binary_tree_
{
private:
	// Retourne le premier noeud sans fils à partir de 'Position' en descendant par les fils.
	POSITION__ NoeudSansFils_( POSITION__ Position ) const;
	// Retourne le premier noeud sans fille à partir de 'Position' en descendant par les fille.
	POSITION__ NoeudSansFille_( POSITION__ Position ) const;
	// Retourne le père du premier noeud qui est fils en remontant.
	POSITION__ PereFilsEnRemontant_( POSITION__ Position ) const;
	// Retourne le père du premier noeud qui est fille en remontant.
	POSITION__ PereFilleEnRemontant_( POSITION__ Position ) const;
	/* Equilibre l'arbre, sachant que l'ordre des éléments est donnée par
	la file 'File' de tête 'Tete' et que l'on doit utiliser la 'Pilote' si != NULL.
	Retourne la nouvelle racine de l'arbre. */
	POSITION__ Equilibrer_(
		que_queue_ &Index,
		POSITION__ Premier,
		MEMORY_DRIVER_ &Pilote );
public:
	struct s
	: public btr_binary_tree_::s
	{
		// La racine de l'arbre.
		POSITION__ Racine;
	} &S_;
	idxbtr_tree_index_( s &S )
	: S_( S ),
	  btr_binary_tree_( S )
	{}
	void reset( bool P = true )
	{
		btr_binary_tree_::reset( P );

		S_.Racine = NONE;
	}
	void plug( mmm_multimemory_ &MM )
	{
		btr_binary_tree_::plug( MM );
	}
	void plug( MEMORY_DRIVER_ &MD )
	{
		btr_binary_tree_::plug( MD );
	}
	idxbtr_tree_index_ &operator =( const idxbtr_tree_index_ &I )
	{
		btr_binary_tree_::operator =( I );

		S_.Racine = I.S_.Racine;

		return *this;
	}
/*	void ecrire( flo_sortie_ &Flot ) const
	{
		FLOEcrire( S_.Racine, Flot );
		bts_binary_tree_::ecrire( Flot );
	}
	void lire( flo_entree_ &Flot )
	{
		FLOLire( Flot, S_.Racine );
		bts_binary_tree_::lire( Flot );
	}
*/	//f Initialization.
	void Init( void )
	{
		S_.Racine = NONE;

		btr_binary_tree_::Init();
	}
	//f Return the root of the tree. To use as the first node for the 'NextAvailable()' and 'PreviousAvailablbe()' functions.
	POSITION__ Root( void ) const
	{
		return S_.Racine;
	}
	//f Return true if index empty, false otherwise.
	bso__bool IsEmpty( void ) const
	{
		return S_.Racine == NONE;
	}
	//f 'Item' becomes the first item of the index, which must be empty.
	void Create( POSITION__ Item )
	{
		if ( !IsEmpty() )
			ERRu();

		S_.Racine = Item;
	}
	//f Return the first item of the index.
	POSITION__ First( void ) const
	{
		if ( S_.Racine == NONE )
			return NONE;
		else
			return NoeudSansFils_( S_.Racine );
	}
	//f Return the last item of the index.
	POSITION__ Last( void ) const
	{
		if ( S_.Racine == NONE )
			return NONE;
		else
			return NoeudSansFille_( S_.Racine );
	}
	//f Return the item next to 'Item'.
	POSITION__ Next( POSITION__ Item ) const
	{
		if ( btr_binary_tree_::HasRight( Item ) )
			return NoeudSansFils_( btr_binary_tree_::Right( Item ) );
		else
			if ( btr_binary_tree_::IsLeft( Item ) )
				return btr_binary_tree_::Parent( Item );
			else if ( btr_binary_tree_::IsRight( Item ) )
				return PereFilsEnRemontant_( Item );
			else
				return NONE;
	}
	//f Return the item previous to 'Item'.
	POSITION__ Previous( POSITION__ Position ) const
	{
		if ( btr_binary_tree_::HasLeft( Position ) )
			return NoeudSansFille_( btr_binary_tree_::Left( Position ) );
		else
			if ( btr_binary_tree_::IsRight( Position ) )
				return btr_binary_tree_::Parent( Position );
			else if ( btr_binary_tree_::IsLeft( Position ) )
				return PereFilleEnRemontant_( Position );
			else
				return NONE;
	}
	//f Remove 'Item'.
	void Remove( POSITION__ Item )
	{
		POSITION__ Fils = btr_binary_tree_::Left( Item );
		POSITION__ Fille = btr_binary_tree_::Right( Item );
		POSITION__ Pere = btr_binary_tree_::Parent( Item );

		if ( Fils != NONE )
			btr_binary_tree_::Cut( Fils );

		if ( Fille != NONE )
			btr_binary_tree_::Cut( Fille );

		if ( btr_binary_tree_::IsLeft( Item ) )
		{
			btr_binary_tree_::Cut( Item );

			if ( Fils != NONE )
			{
				btr_binary_tree_::BecomeLeft( Fils, Pere );

				if ( Fille != NONE )
					btr_binary_tree_::BecomeRight( Fille, NoeudSansFille_( Fils ) );
			}
			else if ( Fille != NONE )
				btr_binary_tree_::BecomeLeft( Fille, Pere );
		}
		else if ( btr_binary_tree_::IsRight( Item ) )
		{
			btr_binary_tree_::Cut( Item );

			if ( Fille != NONE )
			{
				btr_binary_tree_::BecomeRight( Fille, Pere );

				if ( Fils != NONE )
					btr_binary_tree_::BecomeLeft( Fils, NoeudSansFils_( Fille ) );
			}
			else if ( Fils != NONE )
				btr_binary_tree_::BecomeRight( Fils, Pere );
		}
		else
		{
			if ( Fils != NONE )
			{
				btr_binary_tree_::BecomeRight( Fille, NoeudSansFille_( Fils ) );

				S_.Racine = Fils;
			}
			else
				S_.Racine = Fille;
		}
	}
	/*f Return false and put in 'Item' the next node of 'Item', or return
	true and let 'Item' unchanged if 'Item' next node is free. */
	bso__bool NextAvailable( POSITION__ &Item ) const
	{
		if ( btr_binary_tree_::HasRight( Item ) )
		{
			Item = Right( Item );
			return false;
		}
		else
			return true;
	}
	/*f Return false and put in 'Item' the previous node of 'Item', or return
	true and let 'Item' unchanged if 'Item' next node is free. */
	bso__bool PreviousAvailable( POSITION__ &Item ) const
	{
		if ( btr_binary_tree_::HasLeft( Item ) )
		{
			Item = Left( Item );
			return false;
		}
		else
			return true;
	}
	/*f 'New' becomes the next item of 'Item'. 'NextAvailable( Item )' must
	return true to use this function. */
	void BecomeNext(
		POSITION__ New,
		POSITION__ Item )
	{
		btr_binary_tree_::BecomeRight( New, Item );
	}
	/*f 'New' becomes the previous item of 'Item'. 'PreviousAvailable( Item )' must
	return true to use this function. */
	void BecomePrevious(
		POSITION__ New,
		POSITION__ Item )
	{
		btr_binary_tree_::BecomeLeft( New, Item );
	}
	//f Balances the tree which underlies the index.
	void Balance( void );
	//f Fill the index with the items in 'Queue' beginning at 'Head', using 'MD' as memory driver if != 'NULL'.
	void Fill(
		que_queue_ &Queue,
		POSITION__ Head,
		MEMORY_DRIVER_ &MD = *(MEMORY_DRIVER_ *)NULL )
	{
		Init();

		Allocate( Queue.Amount() );

		S_.Racine = Equilibrer_( Queue, Head, MD );
	}
	//f Print the tree structure of the index.
	void PrintStructure( txf::text_oflow___ &OStream ) const
	{
		btr_binary_tree_::PrintStructure( S_.Racine, OStream );
	}
};

AUTO( idxbtr_tree_index )



/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
