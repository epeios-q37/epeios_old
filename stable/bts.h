/*
	Header for the 'bts' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2003  Claude SIMON (csimon@epeios.org).

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

#ifndef BTS__INC
#define BTS__INC

#define BTS_NAME		"BTS"

#define	BTS_VERSION	"$Revision$"

#define BTS_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &BTSTutor;

#if defined( XXX_DBG ) && !defined( BTS_NODBG )
#define BTS_DBG
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
//D Binary Tree Seeker 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "btr.h"

namespace bts {
	//e Node types.
	enum type__ {
		//i Unknow type.
		tUnknow,
		//i Root node.
		tRoot,
		//i Parent node.
		tParent,
		//i Left node.
		tLeft,
		//i Right node.
		tRight,
		//i Amount node types.
		t_amount
	};

	typedef epeios::row_t__	_row__;

	//c Base binary tree seeker. Internal use.
	class _binary_tree_seeker__
	{
	private:
		const btr::E_BTREE_ *Tree_;
		_row__ Root_, Current_;
		type__ Type_;
		// Return the first node, starting at 'Node' and following the left childs, which have no left child.
		_row__ NodeWithoutLeft_( _row__ Node );
		// Return the first node starting at 'Node'.
		_row__ First_( _row__ Node );
	public:
		void reset( bso::bool__ P = true )
		{
			Tree_ = NULL;

			Root_, Current_ = NONE;

			Type_ = tUnknow;
		}
		~_binary_tree_seeker__( void )
		{
			reset( false );
		}
		//f Initialization with tree 'Tree' with root node 'Root' using browse method 'Method'.
		void Init(
			const btr::E_BTREE_ &Tree,
			_row__ Root )
		{
			Tree_ = &Tree;
			Root_ = Current_ = Root;
			Type_ = tRoot;
		}
		//f Return first node.
		_row__ First( void )
		{
			Current_ = First_( Root_ );

			if ( Tree_->IsLeft( Current_ ) )
				Type_ = tLeft;
			else
				Type_ = tRight;	// Even if 'Current_' is the root node.

			return Current_;
		}
		//f Return last node.
		_row__ Last( void )
		{
			Current_ = Root_;

			if ( Tree_->IsParent( Root_ ) )
				Type_ = tParent;
			else
				Type_ = tLeft;

			return Current_;
		}
		//f Return next node, or 'NONE' if none.
		_row__ Next( void )
		{
			_row__ Candidate = NONE;

			switch ( Type_ ) {
			case tRoot:
				Candidate = First();
				break;
			case tParent:
				if ( !Tree_->HasParent( Current_ ) ) {
					Candidate = NONE;
					break;
				} else if ( Tree_->IsRight( Current_ ) ) {
					Candidate = *Tree_->Parent( Current_ );
					Type_ = tParent;
					break;
				}
			case tLeft:
				Candidate = *Tree_->Parent( Current_ );

				if ( Tree_->HasRight( Candidate ) ) {
					Candidate = First_( *Tree_->Right( Candidate ) );

					if ( Tree_->IsRight( Candidate ) )
						Type_ = tRight;
					else
						Type_ = tLeft;
				} else
					Type_ = tParent;
				break;
			case tRight:
				Candidate = *Tree_->Parent( Current_ );
				Type_ = tParent;
				break;
			default:
				ERRc();
				break;
			}

			Current_ = Candidate;

			return Current_;
		}
		//f Return the current node, which is what is returned by the las 'First()' od 'Next()' function called.
		_row__ GetCurrent( void )
		{
			return Current_;
		}
		//f Return the type of the node. Significatif only if 'GetCurrent()' != NONE.
		type__ GetType( void )
		{
			return Type_;
		}
	};

	//c Binary tree seeker.
	template <typename r> class binary_tree_seeker__
	: private _binary_tree_seeker__
	{
	public:
		//f Initialisation with tree 'Tree' with root node 'Root'.
		void Init(
			const btr::E_BTREEt_( r ) &Tree,
			r Row )
		{
			_binary_tree_seeker__::Init( *(const btr::E_BTREE_ *)&Tree, *Row );
		}
		//f Return the first node.
		r First( void )
		{
			return _binary_tree_seeker__::First();
		}
		//f Return the next node.
		r Next( void )
		{
			return _binary_tree_seeker__::Next();
		}
		//f Return the current node, which is what is returned by the las 'First()' od 'Next()' function called.
		r GetCurrent( void )
		{
			return _binary_tree_seeker__::GetCurrent();
		}
		//f Return the type of the node. Significatif only if 'GetCurrent()' != NONE.
		type__ GetType( void )
		{
			return _binary_tree_seeker__::GetType();
		}
	};

	#define E_BTREE_SEEKERt__( r )	binary_tree_seeker__< r >
	#define E_BTREE_SEEKER__		E_BTREE_SEEKERt__( epeios::row__ )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
