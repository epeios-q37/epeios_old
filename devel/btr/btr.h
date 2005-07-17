/*
	Header for the 'btr' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
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

#define BTR_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &BTRTutor;

#if defined( XXX_DBG ) && !defined( BTR_NODBG )
#define BTR_DBG
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
//D Binary Tree 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bch.h"

namespace btr {
	//c Node. Internal use.
	class _node__
	{
	public:
		epeios::row_t__
			// Parent of the node.
			Parent,
			// Left children.
			Left,
			// Right children.
			Right;
		void reset( bso::bool__ = true )
		{
			Parent = Left = Right = NONE;
		}
		_node__( void )
		{
			reset( false );
		}
	};

	//t Nodes. Internal use.
	typedef bch::E_BUNCH_( _node__ ) _nodes_;

	class _nodes_manager_
	: public _nodes_
	{
	public:
		struct s
		: public _nodes_::s
		{};
		_nodes_manager_( s &S )
		: _nodes_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			_nodes_::reset( P );
		}
		//f Initialization.
		void Init( void )
		{
			_nodes_::Init();
		}
		void Prepare(
			epeios::row_t__ Start,
			epeios::row_t__ End );
		void Release(
			epeios::row_t__ Start,
			epeios::row_t__ End );
		//f Release father of 'Node'.
		void ReleaseParent( epeios::row_t__ Node )
		{
			_node__ Buffer = _nodes_::Get( Node );

			Buffer.Parent = NONE;
			_nodes_::Store( Buffer, Node );
		}
		//f Release left child of 'Node'.
		void ReleaseLeft( epeios::row_t__ Node )
		{
			_node__ Buffer = _nodes_::Get( Node );

			Buffer.Left = NONE;
			_nodes_::Store( Buffer, Node );
		}
		//f Release right child of 'Node'.
		void ReleaseRight( epeios::row_t__ Node )
		{
			_node__ Buffer = _nodes_::Get( Node );

			Buffer.Right = NONE;
			_nodes_::Store( Buffer, Node );
		}
		//f Return true if 'Node' has a father.
		bso::bool__ HasParent( epeios::row_t__ Node ) const
		{
			return _nodes_::Get( Node ).Parent != NONE;
		}
		//f Return true if 'Node' has left chid.
		bso::bool__ HasLeft( epeios::row_t__ Node ) const
		{
			return _nodes_::Get( Node ).Left != NONE;
		}
		//f Return true if 'Node' has right chid.
		bso::bool__ HasRight( epeios::row_t__ Node ) const
		{
			return _nodes_::Get( Node ).Right != NONE;
		}
		//f Return father of 'Node', or 'NONE' if nonde.
		epeios::row_t__ Parent( epeios::row_t__ Node ) const
		{
			return _nodes_::Get( Node ).Parent;
		}
		//f Return left child of 'Node', or 'NONE' if nonde.
		epeios::row_t__ Left( epeios::row_t__ Node ) const
		{
			return _nodes_::Get( Node ).Left;
		}
		//f Return right child of 'Node', or 'NONE' if nonde.
		epeios::row_t__ Right( epeios::row_t__ Node ) const
		{
			return _nodes_::Get( Node ).Right;
		}
		//f 'Left' becomes left child of 'Parent'.
		void BecomeLeft(
			epeios::row_t__ Left,
			epeios::row_t__ Parent )
		{
			_node__ GParent = _nodes_::Get( Parent ), GLeft = _nodes_::Get( Left );

			GParent.Left = Left;
			GLeft.Parent = Parent;

			_nodes_::Store( GParent, Parent );
			_nodes_::Store( GLeft, Left );
		}
		//f 'Right' becomes left child of 'Parent'.
		void BecomeRight(
			epeios::row_t__ Right,
			epeios::row_t__ Parent )
		{
			_node__ GParent = _nodes_::Get( Parent ), GRight = _nodes_::Get( Right );

			GParent.Right = Right;
			GRight.Parent = Parent;

			_nodes_::Store( GParent,Parent );
			_nodes_::Store( GRight, Right );
		}
/*		row_t__ TrouverAieulMaleAvecRight(
			row_t__ Depart,
			row_t__ Racine ) const;
*/		epeios::row_t__ ForceParent(
			epeios::row_t__ Node,
			epeios::row_t__  Parent )
		{
			_node__ Lien = _nodes_::Get( Node );
			epeios::row_t__ AncienParent = Lien.Parent;

			Lien.Parent = Parent;

			_nodes_::Store( Lien, Node );

			return AncienParent;
		}
		//f Return true if 'Node' is a left child.
		bso::bool__ IsLeft( epeios::row_t__ Node ) const
		{
			return HasParent( Node ) && Left( Parent( Node ) ) == Node;
		}
		//f Return true if 'Node' is a right.
		bso::bool__ IsRight( epeios::row_t__ Node ) const
		{
			return HasParent( Node ) && Right( Parent( Node ) ) == Node;
		}
		//f Return true if 'Node' is child.
		bso::bool__ IsChild( epeios::row_t__ Node ) const
		{
			return HasParent( Node );
		}
		//f Return true if 'Node' is parent.
		bso::bool__ IsParent( epeios::row_t__ Node ) const
		{
			return HasLeft( Node ) || HasRight( Node );
		}
		void PrintStructure(
			epeios::row_t__ Racine,
			txf::text_oflow__ &Flot ) const;
		//f Return true if 'Node' has a child.
		bso::bool__ HasChild( epeios::row_t__ Node ) const
		{
			return HasRight( Node ) || HasLeft( Node );
		}
	};



	/**********************************************/
	/* CLASSE DE GESTION D'UN CONTENAIRE D'ARBRES */
	/**********************************************/

	//c Binary tree.
	template <typename r> class binary_tree_
	{
	private:
		void Prepare_(
			epeios::row_t__ Start,
			epeios::row_t__ End )
		{
			Nodes.Prepare( Start, End );
		}
		void Release_(
			epeios::row_t__ Start,
			epeios::row_t__ End )
		{
			Nodes.Release( Start, End );
		}
		void ReleaseParent_( epeios::row_t__ Node )
		{
			Nodes.ReleaseParent( Node );
		}
		void ReleaseLeft_( epeios::row_t__ Node )
		{
			Nodes.ReleaseLeft( Node );
		}
		void ReleaseRight_( epeios::row_t__ Node )
		{
			Nodes.ReleaseRight( Node );
		}
		bso::bool__ HasParent_( epeios::row_t__ Node ) const
		{
			return Nodes.HasParent( Node );
		}
		bso::bool__ HasLeft_( epeios::row_t__ Node ) const
		{
			return Nodes.HasLeft( Node );
		}
		bso::bool__ HasRight_( epeios::row_t__ Node ) const
		{
			return Nodes.HasRight( Node );
		}
		epeios::row_t__ Parent_( epeios::row_t__ Node ) const
		{
			return Nodes.Parent( Node );
		}
		epeios::row_t__ Left_( epeios::row_t__ Node ) const
		{
			return Nodes.Left( Node );
		}
		epeios::row_t__ Right_( epeios::row_t__ Node ) const
		{
			return Nodes.Right( Node );
		}
		void BecomeLeft_(
			epeios::row_t__ Left,
			epeios::row_t__ Parent )
		{
			Nodes.BecomeLeft( Left, Parent );
		}
		void BecomeRight_(
			epeios::row_t__ Right,
			epeios::row_t__ Parent )
		{
			Nodes.BecomeRight( Right, Parent );
		}
/*
		epeios::row_t__ TrouverAieulMaleAvecRight_(
			epeios::row_t__ Depart,
			epeios::row_t__ Racine ) const
		{
			return Nodes.TrouverAieulMaleAvecRight( Depart, Racine );
		}
*/		epeios::row_t__ ForceParent_(
			epeios::row_t__ Node,
			epeios::row_t__  Parent )
		{
			return Nodes.ForceParent( Node, Parent );
		}
	public:
		struct s
		{
			_nodes_manager_::s Nodes;
		};
		// La table des liens.
		_nodes_manager_ Nodes;
		binary_tree_( s &S )
		: Nodes( S.Nodes )
		{}
		void reset( bool P = true )
		{
			Nodes.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Nodes.plug( M );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &M )
		{
			Nodes.plug( M );
		}
		// Operateur d'affectation.
		binary_tree_ &operator =( const binary_tree_ &O )
		{
			Nodes = O.Nodes;

			return *this;
		}
	/*	void ecrire( flo_sortie_ &F ) const
		{
			Nodes.ecrire( F );
		}
		void lire( flo_entree_ &F )
		{
			Nodes.lire( F );
		}
	*/	//f Initialization.
		void Init( void )
		{
			Nodes.Init();
		}
		//f Extent of the tree.
		epeios::size__ Extent( void ) const
		{
			return Nodes.Extent();
		}
		//f Amount of node in the tree.
		epeios::size__ Amount( void ) const
		{
			return Nodes.Amount();
		}
		//f Return parent of 'Node'.
		r Parent( r Node ) const
		{
			return Parent_( *Node );
		}
		//f Return left of 'Node'..
		r Left( r Node ) const
		{
			return Left_( *Node ) ;
		}
		//f Return right of 'Node'..
		r Right( r Node ) const
		{
			return Right_( *Node );
		}
		/* Elague 'Node'; 'Node' devient la racine de l'arbre
		et perd donc son père. */
		//f Cut 'Node'. 'Node' becomes a root.
		void Cut( r Node )
		{
			epeios::row_t__ Parent = Parent_( *Node );

			if ( HasLeft_( Parent ) && ( Left_( Parent ) == *Node ) )
				ReleaseLeft_( Parent );
			else
				ReleaseRight_( Parent );

			ReleaseParent_( *Node );
		}
		//f Return true if 'Child' is left of 'Parent'.
		bso::bool__ IsLeft(
			r Child,
			r Parent ) const
		{
			return Left_( *Parent ) == Child;
		}
		//f Return true if 'Child' is right of 'Parent'.
		bso::bool__ IsRight(
			r Child,
			r Parent ) const
		{
			return Right_( *Parent ) == Child;
		}
		//f Return true if 'Child' is child of 'Parent'.
		bso::bool__ IsChild(
			r Child,
			r Parent ) const
		{
			return ( Left_( *Parent ) == Child ) || ( Right_( *Parent ) == Child );
		}
		//f Return true if 'Parent' is parent of 'Child'.
		bso::bool__ IsParent(
			r Parent,
			r Child ) const
		{
			return Parent_( *Child ) == Parent;
		}
		//f Return true if 'Node' is a child.
		bso::bool__ IsLeft( r Node ) const
		{
			return HasParent_( *Node ) && Left_( Parent_( *Node ) ) == *Node;
		}
		//f Return true if 'Node' is a right.
		bso::bool__ IsRight( r Node ) const
		{
			return HasParent_( *Node ) && Right_( Parent_( *Node ) ) == *Node;
		}
		//f Return true if 'Node' is child.
		bso::bool__ IsChild( r Node ) const
		{
			return HasParent_( *Node );
		}
		//f Return true if 'Node' is parent.
		bso::bool__ IsParent( r Node ) const
		{
			return HasLeft_( *Node ) || HasRight_( *Node );
		}
		//f 'Parent' take 'Child' as left.
		void TakeLeft(
			r Parent,
			r Child )
		{
			BecomeLeft_( *Child, *Parent );
		}
		//f 'Parent' take 'Child' as right.
		void TakeRight(
			r Parent,
			r Child )
		{
			BecomeRight_( *Child, *Parent );
		}
		//f 'Child' becomes left of 'Parent'.
		void BecomeLeft(
			r Child,
			r Parent )
		{
			BecomeLeft_( *Child, *Parent );
		}
		//f 'Child' becomes right of 'Parent'.
		void BecomeRight(
			r Child,
			r Parent )
		{
			BecomeRight_( *Child, *Parent );
		}
		//f Allocate enough room to handle 'Size' node.
		void Allocate(
			epeios::size__ Size,
			aem::mode Mode = aem::mDefault )
		{
			if ( Size > Nodes.Amount() )
			{
				epeios::size__ PrecCapacite = Nodes.Amount();

				Nodes.Allocate( Size, Mode );
				Prepare_( PrecCapacite, Size - 1 );
			}
			else if ( Size < Nodes.Amount() )
			{
				Release_( Size, Nodes.Amount() - 1 );
				Nodes.Allocate( Size, Mode );
			}
		}
		//f Return true if 'Node' has right.
		bso::bool__ HasRight( r Node ) const
		{
			return HasRight_( *Node );
		}
		//f Return true if 'Node' has left.
		bso::bool__ HasLeft( r Node ) const
		{
			return HasLeft_( *Node );
		}
		//f Return true if 'Node' has a child.
		bso::bool__ HasChild( r Node ) const
		{
			return HasChild_( *Node ) || ALeft_( *Node );
		}
		//f Return true if 'Node' has a parent.
		bso::bool__ HasParent( r Node ) const
		{
			return HasParent_( *Node );
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
			txf::text_oflow__ &OFlow ) const
		{
			Nodes.PrintStructure( *Root, OFlow );
		}
		// Sert à parcourir l'arbre de racine 'Racine'. Retourne le noeud aprés 'Position'.
	/*	r Suivant(
			r Courant,
			r Racine ) const
		{
			row_t__ &Temp = Racine;

			if ( ALeft( Courant ) )
				Courant = Left( Courant );
			else if ( ARight( Courant ) )
				Courant = Right( Courant );
			else if ( ( Courant = TrouverAieulMaleAvecRight_( Courant, Racine ) ) != BTR_INEXISTANT )
					Courant = Right( Courant );

			return Courant;
		}
	*/
	};

	E_AUTO1( binary_tree )

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
