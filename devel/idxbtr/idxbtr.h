/*
  Header for the 'idxbtr' library by Claude L. Simon (simon@epeios.org)
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
//C Claude L. SIMON (simon@epeios.org)
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

namespace idxbtr {

	using btr::binary_tree_;

	//c Tree-based index, fast sorting, but slow browsing.
	class tree_index_
	: public E_BTREE_
	{
	private:
		// Retourne le premier noeud sans fils à partir de 'Position' en descendant par les fils.
		tym::row__ NoeudSansFils_( tym::row__ Position ) const;
		// Retourne le premier noeud sans fille à partir de 'Position' en descendant par les fille.
		tym::row__ NoeudSansFille_( tym::row__ Position ) const;
		// Retourne le père du premier noeud qui est fils en remontant.
		tym::row__ PereFilsEnRemontant_( tym::row__ Position ) const;
		// Retourne le père du premier noeud qui est fille en remontant.
		tym::row__ PereFilleEnRemontant_( tym::row__ Position ) const;
		/* Equilibre l'arbre, sachant que l'ordre des éléments est donnée par
		la file 'File' de tête 'Tete' et que l'on doit utiliser la 'Pilote' si != NULL.
		Retourne la nouvelle racine de l'arbre. */
		tym::row__ Equilibrer_(
			que::E_QUEUE_ &Index,
			tym::row__ Premier,
			mdr::E_MEMORY_DRIVER_ &Pilote );
	public:
		struct s
		: public E_BTREE_::s
		{
			// La racine de l'arbre.
			tym::row__ Racine;
		} &S_;
		tree_index_( s &S )
		: S_( S ),
		  E_BTREE_( S )
		{}
		void reset( bool P = true )
		{
			E_BTREE_::reset( P );

			S_.Racine = NONE;
		}
		void plug( mmm::multimemory_ &MM )
		{
			E_BTREE_::plug( MM );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MD )
		{
			E_BTREE_::plug( MD );
		}
		tree_index_ &operator =( const tree_index_ &I )
		{
			E_BTREE_::operator =( I );

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

			E_BTREE_::Init();
		}
		//f Return the root of the tree. To use as the first node for the 'NextAvailable()' and 'PreviousAvailablbe()' functions.
		tym::row__ Root( void ) const
		{
			return S_.Racine;
		}
		//f Return true if index empty, false otherwise.
		bso__bool IsEmpty( void ) const
		{
			return S_.Racine == NONE;
		}
		//f 'Item' becomes the first item of the index, which must be empty.
		void Create( tym::row__ Item )
		{
			if ( !IsEmpty() )
				ERRu();

			S_.Racine = Item;
		}
		//f Return the first item of the index.
		tym::row__ First( void ) const
		{
			if ( S_.Racine == NONE )
				return NONE;
			else
				return NoeudSansFils_( S_.Racine );
		}
		//f Return the last item of the index.
		tym::row__ Last( void ) const
		{
			if ( S_.Racine == NONE )
				return NONE;
			else
				return NoeudSansFille_( S_.Racine );
		}
		//f Return the item next to 'Item'.
		tym::row__ Next( tym::row__ Item ) const
		{
			if ( E_BTREE_::HasRight( Item ) )
				return NoeudSansFils_( E_BTREE_::Right( Item ) );
			else
				if ( E_BTREE_::IsLeft( Item ) )
					return E_BTREE_::Parent( Item );
				else if ( E_BTREE_::IsRight( Item ) )
					return PereFilsEnRemontant_( Item );
				else
					return NONE;
		}
		//f Return the item previous to 'Item'.
		tym::row__ Previous( tym::row__ Position ) const
		{
			if ( E_BTREE_::HasLeft( Position ) )
				return NoeudSansFille_( E_BTREE_::Left( Position ) );
			else
				if ( E_BTREE_::IsRight( Position ) )
					return E_BTREE_::Parent( Position );
				else if ( E_BTREE_::IsLeft( Position ) )
					return PereFilleEnRemontant_( Position );
				else
					return NONE;
		}
		//f Remove 'Item'.
		void Remove( tym::row__ Item )
		{
			epeios::row__ Fils = E_BTREE_::Left( Item );
			epeios::row__ Fille = E_BTREE_::Right( Item );
			epeios::row__ Pere = E_BTREE_::Parent( Item );

			if ( Fils != NONE )
				E_BTREE_::Cut( Fils );

			if ( Fille != NONE )
				E_BTREE_::Cut( Fille );

			if ( E_BTREE_::IsLeft( Item ) )
			{
				E_BTREE_::Cut( Item );

				if ( Fils != NONE )
				{
					E_BTREE_::BecomeLeft( Fils, Pere );

					if ( Fille != NONE )
						E_BTREE_::BecomeRight( Fille, NoeudSansFille_( Fils ) );
				}
				else if ( Fille != NONE )
					E_BTREE_::BecomeLeft( Fille, Pere );
			}
			else if ( E_BTREE_::IsRight( Item ) )
			{
				E_BTREE_::Cut( Item );

				if ( Fille != NONE )
				{
					E_BTREE_::BecomeRight( Fille, Pere );

					if ( Fils != NONE )
						E_BTREE_::BecomeLeft( Fils, NoeudSansFils_( Fille ) );
				}
				else if ( Fils != NONE )
					E_BTREE_::BecomeRight( Fils, Pere );
			}
			else
			{
				if ( Fils != NONE )
				{
					E_BTREE_::BecomeRight( Fille, NoeudSansFille_( Fils ) );

					S_.Racine = Fils;
				}
				else
					S_.Racine = Fille;
			}
		}
		/*f Return false and put in 'Item' the next node of 'Item', or return
		true and let 'Item' unchanged if 'Item' next node is free. */
		bso__bool NextAvailable( tym::row__ &Item ) const
		{
			if ( E_BTREE_::HasRight( Item ) )
			{
				Item = Right( Item );
				return false;
			}
			else
				return true;
		}
		/*f Return false and put in 'Item' the previous node of 'Item', or return
		true and let 'Item' unchanged if 'Item' next node is free. */
		bso__bool PreviousAvailable( tym::row__ &Item ) const
		{
			if ( E_BTREE_::HasLeft( Item ) )
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
			tym::row__ New,
			tym::row__ Item )
		{
			E_BTREE_::BecomeRight( New, Item );
		}
		/*f 'New' becomes the previous item of 'Item'. 'PreviousAvailable( Item )' must
		return true to use this function. */
		void BecomePrevious(
			tym::row__ New,
			tym::row__ Item )
		{
			E_BTREE_::BecomeLeft( New, Item );
		}
		//f Balances the tree which underlies the index.
		void Balance( void );
		//f Fill the index with the items in 'Queue' beginning at 'Head', using 'MD' as memory driver if != 'NULL'.
		void Fill(
			que::E_QUEUE_ &Queue,
			tym::row__ Head,
			mdr::E_MEMORY_DRIVER_ &MD = *(mdr::E_MEMORY_DRIVER_ *)NULL )
		{
			Init();

			Allocate( Queue.Amount() );

			S_.Racine = Equilibrer_( Queue, Head, MD );
		}
		//f Print the tree structure of the index.
		void PrintStructure( txf::text_oflow___ &OStream ) const
		{
			E_BTREE_::PrintStructure( S_.Racine, OStream );
		}
	};

	AUTO( tree_index )
	
	#define E_IBTREE	tree_index
	#define E_IBTREE_	tree_index_
}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
