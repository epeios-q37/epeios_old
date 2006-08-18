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
#include "bitbch.h"

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
			mdr::E_MEMORY_DRIVER__ &Pilote );
		bso::bool__ _IsBlack( r Node ) const
		{
			return !Colors( Node );
		}
		bso::bool__ _IsRed( r Node ) const
		{
			return Colors( Node );
		}
		void _BecomeBlack( r Node )
		{
			Colors.Store( false, Node );
		}
		void _BecomeRed( r Node )
		{
			Colors.Store( true, Node );
		}
		E_BTREEt_( r ) &_BaseTree( void )
		{
			return *this;
		}
		const E_BTREEt_( r ) &_BaseTree( void ) const
		{
			return *this;
		}
		void _InsertCase1( r Node )
		{
			if ( HasParent( Node )) {
				_BecomeRed( Node );
				_InsertCase2( Node );
			} else
				_BecomeBlack( Node );
		}
		void _InsertCase2( r Node)
		{
			if ( _IsRed( Parent( Node ) ) )
				_InsertCase3( Node );
		}
		void _InsertCase3( r Node )
		{
			r Parent, GrandParent, Uncle = this->Uncle( Node, Parent, GrandParent );

			if ( ( Uncle != NONE ) && ( _IsRed( Uncle ) ) ) {
				_BecomeBlack( Parent );
				_BecomeBlack( Uncle );
				_BecomeRed( GrandParent );
				_InsertCase1( GrandParent );
			} else
				_InsertCase4( Node );
		}

		void _InsertCase4( r Node)
		{
			r Parent, GrandParent = this->GrandParent( Node, Parent );

			if ( IsRight( Node ) && IsLeft( Parent ) ) {
				RotateLeft( Parent );
				Node = Left( Node );
			} else if ( IsLeft( Node ) && IsRight( Parent ) ) {
				RotateRight( Parent );
				Node = Right( Node );
			}

			_InsertCase5( Node );
		}

		void _InsertCase5( r Node )
		{
			r Parent, GrandParent = this->GrandParent( Node, Parent );

			if ( IsLeft( Node ) && IsLeft( Parent ) )
				RotateRight( GrandParent );
			else
				RotateLeft( GrandParent );
		}
	public:
		bitbch::E_BIT_BUNCHt_( r ) Colors;		
		struct s
		: public E_BTREEt_( r )::s
		{
			bitbch::E_BIT_BUNCHt_( r )::s Colors;
		};
		tree_index_( s &S )
		: E_BTREEt_( r )( S ),
		  Colors( S.Colors )
		{}
		void reset( bool P = true )
		{
			E_BTREEt_( r )::reset( P );
			Colors.reset( P );
		}
		void plug( mmm::multimemory_ &MM )
		{
			E_BTREEt_( r )::plug( MM );
			Colors.plug ( MM );
		}
/*
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			E_BTREEt_( r )::plug( MD );
		}
*/		tree_index_ &operator =( const tree_index_ &I )
		{
			E_BTREEt_( r )::operator =( I );
			Colors = I.Colors;

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
			Colors.Init();
		}
		void Allocate(
			epeios::size__ Size,
			aem::mode Mode = aem::mDefault )
		{
			_BaseTree().Allocate( Size, Mode );
			Colors.Allocate( Size, Mode );
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
		// 'Node' devient le fils gauche de 'Parent'.
		void BecomeLeft(
			r Node,
			r Parent,
			bso::bool__ AutoBalance = true )
		{
			_BaseTree().BecomeLeft( Node, Parent );

			if ( AutoBalance )
				_InsertCase1( Node );
		}
		// 'Node' devient le fils gauche de 'Parent'.
		void BecomeRight(
			r Node,
			r Parent,
			bso::bool__ AutoBalance = true )
		{
			_BaseTree().BecomeRight( Node, Parent );

			if ( AutoBalance )
				_InsertCase1( Node );
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
		//f Balances the tree which underlies the index. Return the new root.
		r Balance( r Root );
		//f Fill the index with the items in 'Queue' beginning at 'Head', using 'MD' as memory driver if != 'NULL'. Return the new root.
		r Fill(
			que::E_QUEUEt_( r ) &Queue,
			r Head,
			mdr::E_MEMORY_DRIVER__ &MD = MDR_INTERNAL_MEMORY_DRIVER )
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
		mdr::E_MEMORY_DRIVER__ &Pilote );

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
		mdr::E_MEMORY_DRIVER__ &Pilote )
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
		r _Current;
		const tree_index_<r> *Index_;
#ifdef IDXBTR_DBG
		void _Test( void ) const
		{
			if ( Index_== NULL )
				ERRu();

			if ( _Current == NONE )
				ERRu();
		}
#endif
		r _Handle( r Row )
		{
			if ( Row != NONE )
				_Current = Row;

			return Row;
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Current = NONE;
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

			_Current = Root;
		}
		//f Try to find an element greater then the current.
		r SearchGreater( void )
		{
#ifdef IDXBTR_DBG
			_Test();
#endif
			return _Handle( Index_->Right( _Current ) );
		}
		//f Try to find an element lesser then the current.
		r SearchLesser( void )
		{
#ifdef IDXBTR_DBG
			_Test();
#endif
			return _Handle( Index_->Left( _Current ) );
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
