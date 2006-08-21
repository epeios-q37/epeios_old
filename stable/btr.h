/*
	Header for the 'btr' library by Claude SIMON (csimon@epeios.org)
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

#if 1

namespace btr {
	typedef bso::ubyte__ level__;

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
	private:
		/* Fait pointer le, parent s'il existe, de 'OldNode' sur 'NewNode', 'Node' contenant
		les caratéristiques de 'OldNode'. */
		void _SwapParent(
			const _node__&Node,
			epeios::row_t__ OldNode,
			epeios::row_t__ NewNode )
		{
			if ( Node.Parent != NONE )
				if ( IsLeft( OldNode ) )
					BecomeLeft( NewNode, Node.Parent );
				else
					BecomeRight( NewNode, Node.Parent );
		}
		/* Fait pointer les enfants, s'ils existents, de 'OldNode' sur 'NewNode', 'Node' contenant
		les caratéristiques de 'OldNode'. */
		void _SwapChildren(
			const _node__&Node,
			epeios::row_t__ OldNode,
			epeios::row_t__ NewNode )
		{
			if ( Node.Parent != NONE )
				if ( IsLeft( OldNode ) )
					BecomeLeft( NewNode, Node.Parent );
				else
					BecomeRight( NewNode, Node.Parent );
		}
		void _SwapRelated(
			const _node__&Node,
			epeios::row_t__ OldNode,
			epeios::row_t__ NewNode )
		{
			_SwapParent( Node, OldNode, NewNode );
			_SwapChildren( Node, OldNode, NewNode );
		}
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
		// Retourne le premier noeud sans fils gauche à partir de 'Node' en descendant par les fils gauche.
		epeios::row_t__ SearchMostLeftNode(
			epeios::row_t__ Node,
			level__ &Level ) const;
		// Retourne le premier noeud sans noeid gauche à partir de 'Node' en descendant par les noeuds gauche.
		epeios::row_t__ SearchMostRightNode(
			epeios::row_t__ Node,
			level__ &Level ) const;
		void PrintStructure(
			epeios::row_t__ Racine,
			txf::text_oflow__ &Flot ) const;
		//f Return true if 'Node' has a child.
		bso::bool__ HasChildren( epeios::row_t__ Node ) const
		{
			return HasRight( Node ) || HasLeft( Node );
		}
		bso::bool__ HasBothChildren( epeios::row_t__ Node ) const
		{
			return HasRight( Node ) && HasLeft( Node );
		}
		// Echange 'Node1' avec 'Node2', c'est-à-dire qu'il echangent leur parent, et fils gauche et droit respectif.
		void SwapNodes(
			epeios::row_t__ Node1,
			epeios::row_t__ Node2 )
		{
			_node__ N1, N2;

			N1 = Get( Node1 );
			N2 = Get( Node2 );

			_SwapRelated( N1, Node1, Node2 );
			_SwapRelated( N2, Node2, Node1 );

			Store( N2, Node1 );
			Store( N1, Node2 );
		}
		// Echange l'abre de racine 'Node1' avec l'abre de racine 'Node2'.
		void SwapTrees(
			epeios::row_t__ Node1,
			epeios::row_t__ Node2 )
		{
			_node__ N1, N2;

			N1 = Get( Node1 );
			N2 = Get( Node2 );

			_SwapParent( N1, Node1, Node2 );
			_SwapParent( N2, Node2, Node1 );

			Store( N2, Node1 );
			Store( N1, Node2 );
		}
		// Retourne le parent du premier noeud qui est fils en remontant.
		epeios::row_t__ ParentOfFirstLeftNode( epeios::row_t__ Node ) const;
		// Retourne le père du premier noeud qui est fille en remontant.
		epeios::row_t__ ParentOfFirstRightNode( epeios::row_t__ Node ) const;
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
		void plug( mdr::E_MEMORY_DRIVER__ &M )
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
		// Retourne le grand-parent de 'Node', ou 'NONE' si inexistant. Sotcke dans 'Parent' le parent de 'Node', si existant.
		r GrandParent(
			r Node,
			r &Parent ) const
		{
			if ( ( Parent = this->Parent( Node ) ) != NONE )
				return this->Parent( Parent );
			else
				return NONE;
		}
		// Retourne le grand-parent de 'Node', ou 'NONE' si inexistant
		r GrandParent( r Node ) const
		{
			r Parent;

			return GrandParent( Node, Parent );
		}
		/* Retourne l'oncle de 'Node', ou 'NONE' si inexistant. Stocke dans 'Parent' et 'GrandParent' respectivement 
		le parent et le grand-parent de 'Node', s'ils existent. ATTENTION : 'Parent' et 'GrandParent' ne doivent PAS pointer
		sur la même variable. */
		r Uncle(
			r Node,
			r &Parent,
			r &GrandParent ) const
		{
			if ( ( GrandParent = this->GrandParent( Node, Parent ) ) == NONE )
				return NONE;

			if ( IsLeft( Parent ) )
				return Right( GrandParent );
			else
				return Left( GrandParent );
		}
		/* Retourne l'oncle de 'Node', ou 'NONE' si inexistant. Stocke dans 'Parent' et 'GrandParent' respectivement 
		le parent et le grand-parent de 'Node', d'ils existent. */
		r Uncle( r Node ) const
		{
			r Parent, GrandParent;

			return Uncle( Node, Parent, GrandParent );
		}
		// Retourne le frère, 's'il existe, de 'Node'. Stocke, s'il existe, le parent de 'Node' dans 'PArent'
		r Sibling(
			r Node,
			r &Parent ) const
		{
			Parent = this->Parent( Node );

			if ( IsLeft( Node ) )
				return Right( Parent );
			else if ( IsRight( Node ) )
				return Left ( Parent );
			else
				return NONE;
		}
		// Echange 'Node1' avec 'Node2', c'est-à-dire qu'il echangent leur parent, et enfants respectifs.
		void SwapNodes(
			r Node1,
			r Node2 )
		{
			Nodes.SwapNodes( *Node1, *Node2 );
		}
		// Echange l'arbre de racine 'Node1' avec l'arbre de racine 'Node2'.
		void SwapTrees(
			r Node1,
			r Node2 )
		{
			Nodes.SwapTrees( *Node1, *Node2 );
		}
		// Rotation à droite avec 'Node' come pivot, qui DOIT avoir un fils gauche. Retourne le remplaçant de 'Node'.
		r RotateRight( r Node )
		{
			r A, B, C, P, &D = Node;
			bso::bool__ IsLeftFlag = false;

			P = Parent( D );

			if ( P != NONE ) {
				IsLeftFlag = IsLeft( D );
				Cut( D );
			}

			B = Left( D );

#ifdef BTR_DBG
			if ( B == NONE )
				ERRu();
#endif

			A = Left( B );
			C = Right( B );

			Cut( B );

			if ( A != NONE )
				Cut( A );

			if ( C != NONE )
				Cut( C );

			if ( P != NONE )
				if ( IsLeftFlag )
					BecomeLeft( B, P );
				else
					BecomeRight( B, P );

			BecomeRight( D, B );

			if ( A != NONE )
				BecomeLeft( A, B );

			if ( C != NONE )
				BecomeLeft( C, D );

			return B;
		}
		// Rotation à gauche avec 'Node' come pivot, qui DOIT avoir un fils droit.. Retourne le remplaçant de 'Node'.
		r RotateLeft( r Node )
		{
			r E, D, C, P, &B = Node;
			bso::bool__ IsLeftFlag = false;

			P = Parent( B );

			if ( P != NONE ) {
				IsLeftFlag = IsLeft( B );
				Cut( B );
			}

			D = Right( B );

#ifdef BTR_DBG
			if ( D == NONE )
				ERRu();
#endif

			E = Right( D );
			C = Left( B );

			Cut( D );

			if ( E != NONE )
				Cut( E );

			if ( C != NONE )
				Cut( C );

			if ( P != NONE )
				if ( IsLeftFlag )
					BecomeLeft( D, P );
				else
					BecomeRight( D, P );

			BecomeLeft( B, D );

			if ( E != NONE )
				BecomeRight( E, D );

			if ( C != NONE )
				BecomeRight( C, B );

			return D;
		}
		/* Elague 'Node'; 'Node' devient la racine de l'arbre
		et perd donc son père. */
		//f Cut 'Node'. 'Node' becomes a root.
		void Cut( r Node )
		{
			epeios::row_t__ Parent = Parent_( *Node );

			if ( HasLeft_( Parent ) && ( Left_( Parent ) == *Node ) )
				ReleaseLeft_( Parent );
			
			if ( HasRight_( Parent ) && ( Right_( Parent ) == *Node ) )
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
		//f Return true if 'Node' has a child.
		bso::bool__ HasChildren( r Node ) const
		{
			return Nodes.HasChildren( *Node );
		}
		bso::bool__ HasBothChildren( r Node ) const
		{
			return Nodes.HasBothChildren( *Node );
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
		r SearchMostLeftNode(
			r Node,
			level__ &Level ) const
		{
			return Nodes.SearchMostLeftNode( *Node, Level );
		}
		r SearchMostRightNode(
			r Node,
			level__ &Level ) const
		{
			return Nodes.SearchMostRightNode( *Node, Level );
		}
		// Retourne le père du premier noeud qui est fils en remontant.
		r ParentOfFirstLeftNode( r Node ) const
		{
			return Nodes.ParentOfFirstLeftNode( *Node );
		}
		// Retourne le père du premier noeud qui est fille en remontant.
		r ParentOfFirstRightNode( r Node ) const
		{
			return Nodes.ParentOfFirstRightNode( *Node );
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

	#define E_BTREEt_( r )	binary_tree_< r >
	#define E_BTREEt( r )	binary_tree< r >

	#define E_BTREE_	E_BTREEt_( epeios::row__ )
	#define E_BTREE		E_BTREEt( epeios::row__ )
}


#else	//Version portable ; pb avec gcc.
namespace btr {
	//c Node. Internal use.
	template <typename r_t> class _node__
	{
	public:
		r_t
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

	// Prédéclaration.
	template <typename r_t, typename nodes> class _nodes_manager_;

	typedef bch::E_BUNCH_( btr::_node__<epeios::row_t__> ) _nodes_;

	void Release(
		_nodes_manager_<epeios::row_t__, _nodes_> &Nodes,
		epeios::row_t__ Start,
		epeios::row_t__ End );

	void Prepare(
		_nodes_manager_<epeios::row_t__, _nodes_> &Nodes,
		epeios::row_t__ Start,
		epeios::row_t__ End );


	template <typename r_t, typename nodes> class _nodes_manager_
	: public nodes
	{
	public:
		struct s
		: public nodes::s
		{};
		_nodes_manager_( s &S )
		: nodes( S )
		{}
		void reset( bso::bool__ P = true )
		{
			nodes::reset( P );
		}
		//f Initialization.
		void Init( void )
		{
			nodes::Init();
		}
		void Prepare(
			r_t Start,
			r_t End )
		{
			btr::Prepare( *this, Start, End );
		}
		void Release(
			r_t Start,
			r_t End )
		{
			btr::Release( *this, Start, End );
		}
		//f Release father of 'Node'.
		void ReleaseParent( r_t Node )
		{
			_node__<r_t> Buffer = nodes::Get( Node );

			Buffer.Parent = NONE;
			nodes::Store( Buffer, Node );
		}
		//f Release left child of 'Node'.
		void ReleaseLeft( r_t Node )
		{
			_node__<r_t> Buffer = nodes::Get( Node );

			Buffer.Left = NONE;
			nodes::Store( Buffer, Node );
		}
		//f Release right child of 'Node'.
		void ReleaseRight( r_t Node )
		{
			_node__<r_t> Buffer = nodes::Get( Node );

			Buffer.Right = NONE;
			nodes::Store( Buffer, Node );
		}
		//f Return true if 'Node' has a father.
		bso::bool__ HasParent( r_t Node ) const
		{
			return nodes::Get( Node ).Parent != NONE;
		}
		//f Return true if 'Node' has left chid.
		bso::bool__ HasLeft( r_t Node ) const
		{
			return nodes::Get( Node ).Left != NONE;
		}
		//f Return true if 'Node' has right chid.
		bso::bool__ HasRight( r_t Node ) const
		{
			return nodes::Get( Node ).Right != NONE;
		}
		//f Return father of 'Node', or 'NONE' if nonde.
		r_t Parent( r_t Node ) const
		{
			return nodes::Get( Node ).Parent;
		}
		//f Return left child of 'Node', or 'NONE' if nonde.
		r_t Left( r_t Node ) const
		{
			return nodes::Get( Node ).Left;
		}
		//f Return right child of 'Node', or 'NONE' if nonde.
		r_t Right( r_t Node ) const
		{
			return nodes::Get( Node ).Right;
		}
		//f 'Left' becomes left child of 'Parent'.
		void BecomeLeft(
			r_t Left,
			r_t Parent )
		{
			_node__<r_t> GParent = nodes::Get( Parent ), GLeft = nodes::Get( Left );

			GParent.Left = Left;
			GLeft.Parent = Parent;

			nodes::Store( GParent, Parent );
			nodes::Store( GLeft, Left );
		}
		//f 'Right' becomes left child of 'Parent'.
		void BecomeRight(
			r_t Right,
			r_t Parent )
		{
			_node__<r_t> GParent = nodes::Get( Parent ), GRight = nodes::Get( Right );

			GParent.Right = Right;
			GRight.Parent = Parent;

			nodes::Store( GParent,Parent );
			nodes::Store( GRight, Right );
		}
/*		row_t__ TrouverAieulMaleAvecRight(
			row_t__ Depart,
			row_t__ Racine ) const;
*/		r_t ForceParent(
			r_t Node,
			r_t  Parent )
		{
			_node__<r_t> Lien = nodes::Get( Node );
			r_t AncienParent = Lien.Parent;

			Lien.Parent = Parent;

			nodes::Store( Lien, Node );

			return AncienParent;
		}
		//f Return true if 'Node' is a left child.
		bso::bool__ IsLeft( r_t Node ) const
		{
			return HasParent( Node ) && Left( Parent( Node ) ) == Node;
		}
		//f Return true if 'Node' is a right.
		bso::bool__ IsRight( r_t Node ) const
		{
			return HasParent( Node ) && Right( Parent( Node ) ) == Node;
		}
		//f Return true if 'Node' is child.
		bso::bool__ IsChild( r_t Node ) const
		{
			return HasParent( Node );
		}
		//f Return true if 'Node' is parent.
		bso::bool__ IsParent( r_t Node ) const
		{
			return HasLeft( Node ) || HasRight( Node );
		}
		void PrintStructure(
			r_t Racine,
			txf::text_oflow__ &Flot ) const;
		//f Return true if 'Node' has a child.
		bso::bool__ HasChild( r_t Node ) const
		{
			return HasRight( Node ) || HasLeft( Node );
		}
	};

	/**********************************************/
	/* CLASSE DE GESTION D'UN CONTENAIRE D'ARBRES */
	/**********************************************/

	//c Binary tree.
	template <typename r, typename r_t> class binary_tree_
	{
	private:
		void Prepare_(
			r_t Start,
			r_t End )
		{
			Nodes.Prepare( Start, End );
		}
		void Release_(
			r_t Start,
			r_t End )
		{
			Nodes.Release( Start, End );
		}
		void ReleaseParent_( r_t Node )
		{
			Nodes.ReleaseParent( Node );
		}
		void ReleaseLeft_( r_t Node )
		{
			Nodes.ReleaseLeft( Node );
		}
		void ReleaseRight_( r_t Node )
		{
			Nodes.ReleaseRight( Node );
		}
		bso::bool__ HasParent_( r_t Node ) const
		{
			return Nodes.HasParent( Node );
		}
		bso::bool__ HasLeft_( r_t Node ) const
		{
			return Nodes.HasLeft( Node );
		}
		bso::bool__ HasRight_( r_t Node ) const
		{
			return Nodes.HasRight( Node );
		}
		r_t Parent_( r_t Node ) const
		{
			return Nodes.Parent( Node );
		}
		r_t Left_( r_t Node ) const
		{
			return Nodes.Left( Node );
		}
		r_t Right_( r_t Node ) const
		{
			return Nodes.Right( Node );
		}
		void BecomeLeft_(
			r_t Left,
			r_t Parent )
		{
			Nodes.BecomeLeft( Left, Parent );
		}
		void BecomeRight_(
			r_t Right,
			r_t Parent )
		{
			Nodes.BecomeRight( Right, Parent );
		}
/*
		r_t TrouverAieulMaleAvecRight_(
			r_t Depart,
			r_t Racine ) const
		{
			return Nodes.TrouverAieulMaleAvecRight( Depart, Racine );
		}
*/		r_t ForceParent_(
			r_t Node,
			r_t  Parent )
		{
			return Nodes.ForceParent( Node, Parent );
		}
	public:
		struct s
		{
			_nodes_manager_<r_t,bch::E_BUNCH_( _node__<r_t> )>::s Nodes;
		};
		// La table des liens.
		_nodes_manager_<r_t,bch::E_BUNCH_( _node__<r_t> )> Nodes;
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
		void plug( mdr::E_MEMORY_DRIVER__ &M )
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
		// Retourne le grand-parent de 'Node', ou 'NONE' si inexistant. Sotcke dans 'Parent' le parent de 'Node', si existant.
		r GrandParent(
			r Node,
			r &Parent ) const
		{
			if ( ( Parent = Parent_( *Node ) ) != NONE )
				return Parent_( Parent );
			else
				return NONE;
		}
		// Retourne le grand-parent de 'Node', ou 'NONE' si inexistant
		r GrandParent( r Node ) const
		{
			r Parent;

			return GranParent( Node, Parent );
		}
		/* Retourne l'oncle de 'Node', ou 'NONE' si inexistant. Stocke dans 'Parent' et 'GrandParent' respectivement 
		le parent et le grand-parent de 'Node', s'ils existent. ATTENTION : 'Parent' et 'GrandParent' ne doivent PAS pointer
		sur la même variable. */
		r Uncle(
			r Node,
			r &Parent,
			r &GrandParent ) const
		{
			if ( ( GrandParent = GrandParent( Node, Parent ) ) == NONE )
				return NONE;

			if ( IsLeft( Parent, GrandParent ) )
				return Right( GrandParent );
			else
				return Left( GrandParent );
		}
		/* Retourne l'oncle de 'Node', ou 'NONE' si inexistant. Stocke dans 'Parent' et 'GrandParent' respectivement 
		le parent et le grand-parent de 'Node', d'ils existent. */
		r Uncle( r Node ) const
		{
			r Parent, GrandParent;

			return Uncle( Node, Parent, GrandParent );
		}
		/* Elague 'Node'; 'Node' devient la racine de l'arbre
		et perd donc son père. */
		//f Cut 'Node'. 'Node' becomes a root.
		void Cut( r Node )
		{
			r_t Parent = Parent_( *Node );

			if ( HasLeft_( Parent ) && ( Left_( Parent ) == *Node ) )
				ReleaseLeft_( Parent );
			
			if ( HasRight_( Parent ) && ( Right_( Parent ) == *Node ) )
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
		// Rotation à droite avec 'Node' come pivot, qui DOIT avoir un fils gauche.
		void RotateRight( r Node )
		{
			r A, B, C, P, D = &Node;
			bso::bool__ IsLeft = false;

			Parent = Parent( D );

			if ( Parent != NONE ) {
				IsLeft = IsLeft( D );
				Cut( D );
			}

			B = Left( D );

#ifdef BTR_DBG
			if ( B == NONE )
				ERRu();
#endif

			A = Left( B );
			C = Right( B );

			Cut( B );

			if ( A != NONE )
				Cut( A );

			if ( C != NONE )
				Cut( C );

			if ( Parent != NONE )
				if ( IsLeft )
					BecomeLeft( B, Parent );
				else
					BecomRight( B, Parent );

			BecomeRight( D, B );

			if ( A != NONE )
				BecomeLeft( A, B );

			if ( C != NONE )
				BecomeLeft( C, D );
		}
		// Rotation à droite avec 'Node' come pivot, qui DOIT avoir un fils droit.
		void RotateRight( r Node )
		{
			r E, D, C, P, B = &Node;
			bso::bool__ IsLeft = false;

			P = P( B );

			if ( P != NONE ) {
				IsLeft = IsLeft( B );
				Cut( B );
			}

			D = Right( B );

#ifdef BTR_DBG
			if ( D == NONE )
				ERRu();
#endif

			E = Right( D );
			C = Left( B );

			Cut( D );

			if ( E != NONE )
				Cut( E );

			if ( C != NONE )
				Cut( C );

			if ( P != NONE )
				if ( IsLeft )
					BecomeLeft( D, Parent );
				else
					BecomRight( D, Parent );

			BecomeLeft( B, D );

			if ( E != NONE )
				BecomeRight( E, D );

			if ( C != NONE )
				BecomeRight( C, B );
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

	E_AUTO2( binary_tree )

	#define E_BTREEt_( r )	binary_tree_<r,epeios::row_t__>
	#define E_BTREEt( r )	binary_tree<r,epeios::row_t__>

	#define E_BTREE_	E_BTREEt_( epeios::row__ )
	#define E_BTREE		E_BTREEt( epeios::row__ )

	#define E_PBTREEt_( r )	binary_tree_<r,epeios::p_row_t__>
	#define E_PBTREEt( r )	binary_tree<r,epeios::p_row_t__>

	#define E_PBTREE_		E_PBTREEt_( epeios::p_row__ )
	#define E_PBTREE		E_PBTREEt( epeios::p_row__ )
}
#endif

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
