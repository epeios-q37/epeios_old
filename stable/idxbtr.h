/*
	Header for the 'idxbtr' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2004 Claude SIMON (csimon@epeios.org).

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

#ifndef IDXBTR__INC
#define IDXBTR__INC

#define IDXBTR_NAME		"IDXBTR"

#define	IDXBTR_VERSION	"$Revision$"

#define IDXBTR_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &IDXBTRTutor;

#if defined( XXX_DBG ) && !defined( IDXBTR_NODBG )
#define IDXBTR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D InDeX Binary TRee 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D InDeX Binary TRee
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "btr.h"
#include "que.h"
#include "stk.h"

namespace idxbtr {

	using btr::binary_tree_;

	//c Tree-based index, fast sorting, but slow browsing.
	template <typename r> class tree_index_
	: public E_BTREEt_( r )
	{
	private:
		// Retourne le premier noeud sans fils à partir de 'Position' en descendant par les fils.
		r NoeudSansFils_( r Position ) const;
		// Retourne le premier noeud sans fille à partir de 'Position' en descendant par les fille.
		r NoeudSansFille_( r Position ) const;
		// Retourne le père du premier noeud qui est fils en remontant.
		r PereFilsEnRemontant_( r Position ) const;
		// Retourne le père du premier noeud qui est fille en remontant.
		r PereFilleEnRemontant_( r Position ) const;
		/* Equilibre l'arbre, sachant que l'ordre des éléments est donnée par
		la file 'File' de tête 'Tete' et que l'on doit utiliser la 'Pilote' si != NULL.
		Retourne la nouvelle racine de l'arbre. */
		r Equilibrer_(
			que::E_QUEUEt_( r ) &Index,
			r Premier,
			mdr::E_MEMORY_DRIVER_ &Pilote );
	public:
		struct s
		: public E_BTREEt_( r )::s
		{};
		tree_index_( s &S )
		: E_BTREEt_( r )( S )
		{}
		void reset( bool P = true )
		{
			E_BTREEt_( r )::reset( P );
		}
		void plug( mmm::multimemory_ &MM )
		{
			E_BTREEt_( r )::plug( MM );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MD )
		{
			E_BTREEt_( r )::plug( MD );
		}
		tree_index_ &operator =( const tree_index_ &I )
		{
			E_BTREEt_( r )::operator =( I );

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
			E_BTREEt_( r) ::Init();
		}
		//f Return the first item of the index.
		r First( r Root ) const
		{
			return NoeudSansFils_( Root );
		}
		//f Return the last item of the index.
		r Last( r Root ) const
		{
				return NoeudSansFille_( Root );
		}
		//f Return the item next to 'Item'.
		r Next( r Item ) const
		{
			if ( E_BTREEt_( r )::HasRight( Item ) )
				return NoeudSansFils_( E_BTREEt_( r )::Right( Item ) );
			else
				if ( E_BTREEt_( r )::IsLeft( Item ) )
					return E_BTREEt_( r )::Parent( Item );
				else if ( E_BTREEt_( r )::IsRight( Item ) )
					return PereFilsEnRemontant_( Item );
				else
					return NONE;
		}
		//f Return the item previous to 'Item'.
		r Previous( r Position ) const
		{
			if ( E_BTREEt_( r )::HasLeft( Position ) )
				return NoeudSansFille_( E_BTREEt_( r )::Left( Position ) );
			else
				if ( E_BTREEt_( r )::IsRight( Position ) )
					return E_BTREEt_( r )::Parent( Position );
				else if ( E_BTREEt_( r )::IsLeft( Position ) )
					return PereFilleEnRemontant_( Position );
				else
					return NONE;
		}
		//f Remove 'Item'. Return the new root.
		r Delete(
			r Item,
			r Root )
		{
			r Fils = E_BTREEt_( r )::Left( Item );
			r Fille = E_BTREEt_( r )::Right( Item );
			r Pere = E_BTREEt_( r )::Parent( Item );

			if ( Fils != NONE )
				E_BTREEt_( r )::Cut( Fils );

			if ( Fille != NONE )
				E_BTREEt_( r )::Cut( Fille );

			if ( E_BTREEt_( r )::IsLeft( Item ) )
			{
				E_BTREEt_( r )::Cut( Item );

				if ( Fils != NONE )
				{
					E_BTREEt_( r )::BecomeLeft( Fils, Pere );

					if ( Fille != NONE )
						E_BTREEt_( r )::BecomeRight( Fille, NoeudSansFille_( Fils ) );
				}
				else if ( Fille != NONE )
					E_BTREEt_( r )::BecomeLeft( Fille, Pere );
			}
			else if ( E_BTREEt_( r )::IsRight( Item ) )
			{
				E_BTREEt_( r )::Cut( Item );

				if ( Fille != NONE )
				{
					E_BTREEt_( r )::BecomeRight( Fille, Pere );

					if ( Fils != NONE )
						E_BTREEt_( r )::BecomeLeft( Fils, NoeudSansFils_( Fille ) );
				}
				else if ( Fils != NONE )
					E_BTREEt_( r )::BecomeRight( Fils, Pere );
			}
			else
			{
				if ( Fils != NONE )
				{
					if ( Fille != NONE )
						E_BTREEt_( r )::BecomeRight( Fille, NoeudSansFille_( Fils ) );

					return Fils;
				}
				else
					return Fille;
			}

			return Root;
		}
		//f Mark 'Row' as greater then 'Current'. 'Current' must be the result as a search with 'seeker_'.
		void MarkAsGreater(
			r Row,
			r Current )
		{
#ifdef IDXBTR_DBG
			if ( HasRight( Current ) )
				ERRu();
#endif
			BecomeRight( Row, Current );
		}
		//f Mark 'Row' as lesser then 'Current'. 'Current' must be the result as a search with 'seeker_'.
		void MarkAsLesser(
			r Row,
			r Current )
		{
#ifdef IDXBTR_DBG
			if ( HasLeft( Current ) )
				ERRu();
#endif
			BecomeLeft( Row, Current );
		}
		//f Mark 'Row' as same then 'Current'. 'Current' must be the result as a search with 'seeker_'.
		void MarkAsSame(
			r Row,
			r Current )
		{
			if ( *Row & 1 )	// Little random generator :-).
				BecomeLeft( Row, NoeudSansFils_( Current ) );
			else
				BecomeRight( Row, NoeudSansFille_( Current ) );
		}
		//f Balances the tree which underlies the index. Return the new root.
		r Balance( r Root );
		//f Fill the index with the items in 'Queue' beginning at 'Head', using 'MD' as memory driver if != 'NULL'. Return the new root.
		r Fill(
			que::E_QUEUEt_( r ) &Queue,
			r Head,
			mdr::E_MEMORY_DRIVER_ &MD = *(mdr::E_MEMORY_DRIVER_ *)NULL )
		{
			Init();

			Allocate( Queue.Amount() );

			return Equilibrer_( Queue, Head, MD );
		}
		//f Print the tree structure of the index.
		void PrintStructure(
			txf::text_oflow__ &OStream,
			r Root ) const
		{
			E_BTREEt_( r )::PrintStructure( Root, OStream );
		}
	};

	E_AUTO1( tree_index )

	#define E_IBTREEt( r )	tree_index< r >
	#define E_IBTREEt_( r )	tree_index_< r >

	#define E_IBTREE	E_IBTREEt( epeios::row__ )
	#define E_IBTREE_	E_IBTREEt_( epeios::row__ )

	epeios::row_t__ NoeudSansFils_(
		const E_IBTREE_ &Tree,
		epeios::row_t__ Position );

	epeios::row_t__ NoeudSansFille_(
		const E_IBTREE_ &Tree,
		epeios::row_t__ Position );

	epeios::row_t__ PereFilsEnRemontant_( 
		const E_IBTREE_ &Tree,
		epeios::row_t__ Position );

	epeios::row_t__ PereFilleEnRemontant_( 
		const E_IBTREE_ &Tree,
		epeios::row_t__ Position );

	epeios::row_t__ Balance_(
		E_IBTREE_ &Tree,
		epeios::row_t__ Root );

	epeios::row_t__ Equilibrer_(
		E_IBTREE_ &Tree,
		que::E_QUEUE_ &File,
		epeios::row_t__ Premier,
		mdr::E_MEMORY_DRIVER_ &Pilote );

	template <typename r> inline r tree_index_<r>::NoeudSansFils_( r Position ) const
	{
		return idxbtr::NoeudSansFils_( *(const E_IBTREE_ *)this, *Position );
	}

	template <typename r> inline r tree_index_<r>::NoeudSansFille_( r Position ) const
	{
		return idxbtr::NoeudSansFille_( *(const E_IBTREE_ *)this, *Position );
	}

	template <typename r> inline r tree_index_<r>::PereFilsEnRemontant_( r Position ) const
	{
		return idxbtr::PereFilsEnRemontant_( *(const E_IBTREE_ *)this, *Position );
	}

	template <typename r> inline r tree_index_<r>::PereFilleEnRemontant_( r Position ) const
	{
		return idxbtr::PereFilleEnRemontant_( *(const E_IBTREE_ *)this, *Position );
	}

	template <typename r> inline r tree_index_<r>::Equilibrer_(
		que::E_QUEUEt_( r ) &Index,
		r Premier,
		mdr::E_MEMORY_DRIVER_ &Pilote )
	{
		return idxbtr::Equilibrer_( *(E_IBTREE_ *)this, *(que::E_QUEUE_ *)&Index, *Premier, Pilote );
	}

	template <typename r> inline r tree_index_<r>::Balance( r Root )
	{
		return idxbtr::Balance_( *(E_IBTREE_ *)this, *Root );
	}

	//c To seek in a tree index.
	template <typename r> class tree_seeker__
	{
	private:
		r Current_;
		const tree_index_<r> *Index_;
#ifdef IDXBTR_DBG
		void _Test( void ) const
		{
			if ( Index_== NULL )
				ERRu();

			if ( Current_ == NONE )
				ERRu();
		}
#endif
		r _Handle( r Row )
		{
			if ( Row != NONE )
				Current_ = Row;

			return Row;
		}
	public:
		void reset( bso::bool__ = true )
		{
			Current_ = NONE;
			Index_ = NULL;
		}
		tree_seeker__( void )
		{
			reset( false );
		}
		~tree_seeker__( void )
		{
			reset( true );
		}
		//f Initialisation with index 'Index'. 'Index' must not be empty.
		void Init(
			const tree_index_<r> &Index,
			r Root )
		{
			reset( true );

			Index_ = &Index;

			Current_ = Root;
		}
		//f Try to find an element greater then the current.
		r SearchGreater( void )
		{
#ifdef IDXBTR_DBG
			_Test();
#endif
			return _Handle( Index_->Right( Current_ ) );
		}
		//f Try to find an element lesser then the current.
		r SearchLesser( void )
		{
#ifdef IDXBTR_DBG
			_Test();
#endif
			return _Handle( Index_->Left( Current_ ) );
		}
		E_RODISCLOSE__( r, Current )
	};

	//d A seeker.
	#define E_TSEEKERt__( r )	tree_seeker__<r>
	#define E_TSEEKER__			tree_seeker__<epeios::row__>


}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
