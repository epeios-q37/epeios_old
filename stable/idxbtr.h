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

	using btr::level__;

	//c Tree-based index, fast sorting, but slow browsing.
	template <typename r> class tree_index_
	{
	private:
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
		void _BecomeSameColor(
			r Source,
			r Target )
		{
			Colors.Store( Colors( Source ), Target );
		}
		void _SwapColors(
			r Node1,
			r Node2 )
		{
			bso::bool__ Buffer = Colors( Node1 );

			Colors.Store( Colors( Node2 ), Node1 );

			Colors.Store( Buffer, Node2 );
		}
		r _InsertCase1(
			r Node,
			r Root )
		{
//			return Root;

			if ( BaseTree.HasParent( Node ) ) {
				_BecomeRed( Node );
				Root = _InsertCase2( Node, Root );
			} else
				_BecomeBlack( Node );

			return Root;
		}
		r _InsertCase2(
			r Node,
			r Root )
		{
#ifdef IDXBTR_DBG
			if ( !_IsBlack( Root ) )
				ERRu();
#endif
			if ( _IsRed( BaseTree.Parent( Node ) ) )
				Root = _InsertCase3( Node, Root );

			return Root;
		}
		r _InsertCase3(
			r Node,
			r Root )
		{
			r Parent, GrandParent, Uncle = BaseTree.Uncle( Node, Parent, GrandParent );

			if ( ( Uncle != NONE ) && ( _IsRed( Uncle ) ) ) {
				_BecomeBlack( Parent );
				_BecomeBlack( Uncle );
				_BecomeRed( GrandParent );
				Root = _InsertCase1( GrandParent, Root );
			} else
				Root = _InsertCase4( Node, Root );

			return Root;
		}
		r _InsertCase4(
			r Node,
			r Root )
		{
			r Parent, GrandParent = BaseTree.GrandParent( Node, Parent );

			if ( BaseTree.IsRight( Node ) && BaseTree.IsLeft( Parent ) ) {
				if ( Parent == Root )
					Root = BaseTree.RotateLeft( Parent );
				else
					BaseTree.RotateLeft( Parent );
				Node = BaseTree.Left( Node );
			} else if ( BaseTree.IsLeft( Node ) && BaseTree.IsRight( Parent ) ) {
				if ( Parent == Root )
					Root = BaseTree.RotateRight( Parent );
				else
					BaseTree.RotateRight( Parent );

				Node = BaseTree.Right( Node );
			}

			return _InsertCase5( Node, Root );
		}
		r _InsertCase5(
			r Node,
			r Root )
		{
			r Parent, GrandParent = BaseTree.GrandParent( Node, Parent );

			_BecomeBlack( Parent );
			_BecomeRed( GrandParent );

			if ( BaseTree.IsLeft( Node ) && BaseTree.IsLeft( Parent ) ) {
				if ( GrandParent == Root )
					Root = BaseTree.RotateRight( GrandParent );
				else
					BaseTree.RotateRight( GrandParent );
			} else {
				if ( GrandParent == Root )
					Root = BaseTree.RotateLeft( GrandParent );
				else
					BaseTree.RotateLeft( GrandParent );
			}

			return Root;

		}
		void _DeleteCase1( r Node )
		{
			if ( BaseTree.HasParent( Node ))
				_DeleteCase2( Node );
		}
		void _DeleteCase2( r Node )
		{
			r Parent = NONE;
			r Sibling = BaseTree.Sibling( Node, Parent );

			if ( _IsRed( Sibling ) ) {
				_BecomeRed( Parent );
				_BecomeBlack( Sibling );

				if ( BaseTree.IsLeft( Node ) )
					BaseTree.RotateLeft( Parent );
				else
					BaseTree.RotateRight( Parent );
			}

			_DeleteCase3( Node );
		}
		void _DeleteCase3( r Node )
		{
			r Parent = NONE;
			r Sibling = BaseTree.Sibling( Node, Parent );

			if ( _IsBlack( Parent ) &&
				 _IsBlack( Sibling ) &&
				 _IsBlack( BaseTree.Left( Sibling ) ) &&
				 _IsBlack( BaseTree.Right( Sibling ) ) )
			{
				_BecomeRed( Sibling );
				_DeleteCase1( Parent );
			}
			else
				_DeleteCase4( Node );
		}	
		void _DeleteCase4( r Node )
		{
			r Parent = NONE;
			r Sibling = BaseTree.Sibling( Node, Parent );

			if ( _IsRed( Parent ) &&
				 _IsBlack( Sibling ) &&
				 _IsBlack( BaseTree.Left( Sibling ) ) &&
				 _IsBlack( BaseTree.Right( Sibling ) ) )
			{
				_BecomeRed( Sibling ),
				_BecomeBlack( Parent );
			}
			else
				_DeleteCase5( Node );
		}
		void _DeleteCase5( r Node )
		{
			r Parent = NONE;
			r Sibling = BaseTree.Sibling( Node, Parent );

			if ( BaseTree.IsLeft( Node ) &&
				 _IsBlack( Sibling ) &&
				 _IsBlack( BaseTree.Left( Sibling ) ) &&
				 _IsRed( BaseTree.Right( Sibling ) ) )
			{
				_BecomeRed( Sibling );
				_BecomeBlack( BaseTree.Left( Sibling ) );
				BaseTree.RotateRight( Sibling );
			}
			if ( BaseTree.IsRight( Node ) &&
				 _IsBlack( Sibling ) &&
				 _IsRed( BaseTree.Left( Sibling ) ) &&
				 _IsBlack( BaseTree.Right( Sibling ) ) )
			{
				_BecomeRed( Sibling );
				_BecomeBlack( BaseTree.Right( Sibling ) );
				BaseTree.RotateLeft( Sibling );
			}

			_DeleteCase6( Node );
		}
		void _DeleteCase6( r Node )
		{
			r Parent = NONE;
			r Sibling = BaseTree.Sibling( Node, Parent );

			_BecomeSameColor( Parent, Sibling );
			_BecomeBlack( Parent );

			if ( BaseTree.IsLeft( Node ) ) {
				/* Here, sibling(n)->right->color == RED */
				_BecomeBlack( BaseTree.Right( Sibling ) );
				BaseTree.RotateLeft( Parent );
			}
			else
			{
				/* Here, sibling(n)->left->color == RED */
				_BecomeBlack( BaseTree.Left( Sibling ) );
				BaseTree.RotateRight( Parent );
			}
		}
		r _SearchMostLeftNode( r Node ) const
		{
			level__ Level;

			return BaseTree.SearchMostLeftNode( Node, Level );
		}
		r _SearchMostRightNode( r Node ) const
		{
			level__ Level;

			return BaseTree.SearchMostRightNode( Node, Level );
		}
	public:
		btr::E_BTREEt_( r ) BaseTree;
		bitbch::E_BIT_BUNCHt_( r ) Colors;		
		struct s
		{
			typename btr::E_BTREEt_( r )::s BaseTree;
			typename bitbch::E_BIT_BUNCHt_( r )::s Colors;
		};
		tree_index_( s &S )
		: BaseTree( S.BaseTree ),
		  Colors( S.Colors )
		{}
		void reset( bool P = true )
		{
			BaseTree.reset( P );
			Colors.reset( P );
		}
		void plug( mmm::multimemory_ &MM )
		{
			BaseTree.plug( MM );
			Colors.plug ( MM );
		}
/*
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			E_BTREEt_( r )::plug( MD );
		}
*/		tree_index_ &operator =( const tree_index_ &I )
		{
			BaseTree = I.BaseTree;
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
			BaseTree.Init();
			Colors.Init();
		}
		void Allocate(
			epeios::size__ Size,
			aem::mode Mode = aem::mDefault )
		{
			BaseTree.Allocate( Size, Mode );
			Colors.Allocate( Size, Mode );
		}
		//f Return the first item of the index.
		r First( r Root ) const
		{
			return _SearchMostLeftNode( Root );
		}
		//f Return the last item of the index.
		r Last( r Root ) const
		{
			return _SearchMostRightNode( Root );
		}
		//f Return the item next to 'Item'.
		r Next( r Item ) const
		{
			if ( BaseTree.HasRight( Item ) )
				return _SearchMostLeftNode( BaseTree.Right( Item ) );
			else
				if ( BaseTree.IsLeft( Item ) )
					return BaseTree.Parent( Item );
				else if ( BaseTree.IsRight( Item ) )
					return BaseTree.ParentOfFirstLeftNode( Item );
				else
					return NONE;
		}
		//f Return the item previous to 'Item'.
		r Previous( r Position ) const
		{
			if ( BaseTree.HasLeft( Position ) )
				return NoeudSansFille_( BaseTree.Left( Position ) );
			else
				if ( BaseTree.IsRight( Position ) )
					return BaseTree.Parent( Position );
				else if ( BaseTree.IsLeft( Position ) )
					return BaseTree.SearchFirstRightFather( Position );
				else
					return NONE;
		}
		mdr::size__ Amount( void ) const
		{
			return BaseTree.Amount();
		}
#if 1
		r Delete(
			r Node,
			r Root )
		{
			if ( BaseTree.HasBothChildren( Node ) ) {
				r NewNode = NONE;

				if ( *Node & 1 )	// Petit générateur aléatoire.
					NewNode = _SearchMostLeftNode( BaseTree.Right( Node ) );
				else
					NewNode = _SearchMostRightNode( BaseTree.Left( Node ) );

				if ( Root == Node )
					Root = NewNode;

				BaseTree.SwapNodes( Node, NewNode );

				Node = NewNode;
			}

			if ( BaseTree.HasChildren( Node ) ) {

				r NewNode = NONE;

				if ( BaseTree.HasLeft( Node ) )
					NewNode = BaseTree.Left( Node );
				else
					NewNode = BaseTree.Right( Node );

				if ( Node == Root )
					Root = NewNode;

				BaseTree.SwapNodes( Node, NewNode );
			}

			if ( Node == Root )
				Root = NONE;

			if ( !BaseTree.HasChildren( Node ) ) {
				_DeleteCase1( Node );
				BaseTree.Cut( Node );
			} else {
				r Child = BaseTree.HasLeft( Node ) ? BaseTree.Left( Node ) : BaseTree.Right( Node );

				BaseTree.Cut( Child );

				BaseTree.SwapTrees( Child, Node );	// Sans que les couleurs suivent.

				if ( _IsBlack( Node ) ) {
					if ( _IsRed( Child ) )
						_BecomeBlack( Child );
					else
						_DeleteCase1( Child );
				}
			}

			return Root;
		}
#else
		//f Remove 'Item'. Return the new root.
		r Delete(
			r Item,
			r Root )
		{
			r Fils = BaseTree.Left( Item );
			r Fille = BaseTree.Right( Item );
			r Pere = BaseTree.Parent( Item );

			if ( Fils != NONE )
				BaseTree.Cut( Fils );

			if ( Fille != NONE )
				BaseTree.Cut( Fille );

			if ( BaseTree.IsLeft( Item ) )
			{
				BaseTree.Cut( Item );

				if ( Fils != NONE )
				{
					BaseTree.BecomeLeft( Fils, Pere );

					if ( Fille != NONE )
						BaseTree.BecomeRight( Fille, _SearchMostRightNode( Fils ) );
				}
				else if ( Fille != NONE )
					BaseTree.BecomeLeft( Fille, Pere );
			}
			else if ( BaseTree.IsRight( Item ) )
			{
				BaseTree.Cut( Item );

				if ( Fille != NONE )
				{
					BaseTree.BecomeRight( Fille, Pere );

					if ( Fils != NONE )
						BaseTree.BecomeLeft( Fils, _SearchMostLeftNode( Fille ) );
				}
				else if ( Fils != NONE )
					BaseTree.BecomeRight( Fils, Pere );
			}
			else
			{
				if ( Fils != NONE )
				{
					if ( Fille != NONE )
						BaseTree.BecomeRight( Fille, _SearchMostRightNode( Fils ) );

					return Fils;
				}
				else
					return Fille;
			}

			return Root;
		}
#endif
		//f Mark 'Row' as greater then 'Current'. 'Current' must be the result as a search with 'seeker_'.
		r BecomeGreater(
			r Row,
			r Current,
			r Root )
		{
			BaseTree.BecomeRight( Row, Current );

			return _InsertCase1( Row, Root );
		}
		//f Mark 'Row' as lesser then 'Current'. 'Current' must be the result as a search with 'seeker_'.
		r BecomeLesser(
			r Row,
			r Current,
			r Root )
		{
			BaseTree.BecomeLeft( Row, Current );

			return _InsertCase1( Row, Root );
		}
		void BecomeRoot( r Node )
		{
#ifdef IDXBTR_DBG
			if ( BaseTree.HasParent( Node ) )
				ERRu();
#endif
			_InsertCase1( Node, Node );
		}
		r GetLesser( r Node ) const
		{
			return BaseTree.Left( Node );
		}
		r GetGreater( r Node ) const
		{
			return BaseTree.Right( Node );
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
			BaseTree.PrintStructure( Root, OStream );
		}
		friend epeios::row_t__ Equilibrer_(
			tree_index_<epeios::row__> &Tree,
			que::E_QUEUE_ &File,
			epeios::row_t__ Premier,
			mdr::E_MEMORY_DRIVER__ &Pilote );
	};

	E_AUTO1( tree_index )

	#define E_IBTREEt( r )	tree_index< r >
	#define E_IBTREEt_( r )	tree_index_< r >

	#define E_IBTREE	E_IBTREEt( epeios::row__ )
	#define E_IBTREE_	E_IBTREEt_( epeios::row__ )

	epeios::row_t__ Balance_(
		E_IBTREE_ &Tree,
		epeios::row_t__ Root );

	epeios::row_t__ Equilibrer_(
		E_IBTREE_ &Tree,
		que::E_QUEUE_ &File,
		epeios::row_t__ Premier,
		mdr::E_MEMORY_DRIVER__ &Pilote );

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
			return _Handle( Index_->GetGreater( _Current ) );
		}
		//f Try to find an element lesser then the current.
		r SearchLesser( void )
		{
#ifdef IDXBTR_DBG
			_Test();
#endif
			return _Handle( Index_->GetLesser( _Current ) );
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
