/*
  Header for the 'idxque' library by Claude SIMON (csimon@epeios.org)
  Copyright (C) 2000-2002 Claude SIMON (csimon@epeios.org) 

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

#ifndef IDXQUE__INC
#define IDXQUE__INC

#define IDXQUE_NAME		"IDXQUE"

#define	IDXQUE_VERSION	"$Revision$"	

#define IDXQUE_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &IDXQUETutor;

#if defined( XXX_DBG ) && !defined( IDXQUE_NODBG )
#define IDXQUE_DBG 
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
/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D InDeX QUEue
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "txf.h"
#include "que.h"
#include "stk.h"

//d The end of the queue.
#define IDXQUE_END	NONE

namespace idxque {
	using namespace que;
	using que::managed_queue_;	// VC++ has problem without this, despite line above.

	//c Queue-based index, fast browsing, but slow sorting.
	template <typename r> class queue_index_
	: public E_MQUEUEt_( r )
	{
	public:
		struct s
		: public E_MQUEUEt_( r )::s
		{};
	// fonctions
		queue_index_( s &S )
		: E_MQUEUEt_( r )( S )
		{}
		void reset( bool P = true )
		{
			E_MQUEUEt_( r )::reset( P );
		}
		void plug( mmm::multimemory_ &MM )
		{
			E_MQUEUEt_( r )::plug( MM );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MD )
		{
			E_MQUEUEt_( r )::plug( MD );
		}
		queue_index_ &operator =( const queue_index_ &T )
		{
			E_MQUEUEt_( r )::operator =( T );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			E_MQUEUEt_( r )::Init();
		}
		/*f Dump to the stack 'Stack' the queue beginning at 'Begin' and in the 'Direction ' direction.. */
		void Dump(
			stk::E_STACK_( r ) &Stack,
			r Begin,
			idxque::direction Direction )
		{
			E_MQUEUEt_( r )::Dump( Stack, Begin, Direction );
		}
		//f Item is marked as greater then 'Node'.
		void MarkAsGreater(
			r Item,
			r Node )
		{
			E_MQUEUEt_( r )::BecomeNext( Item, Node );
		}
		//f Item is marked as lesser then 'Node'.
		void MarkAsLesser(
			r Item,
			r Node )
		{
			E_MQUEUEt_( r )::BecomePrevious( Item, Node );
		}
		//f Item is marked as same then 'Node'.
		void MarkAsSame(
			r Item,
			r Node )
		{
			E_MQUEUEt_( r )::BecomeNext( Item, Node );
		}
	};

	AUTOt( queue_index )
	
	#define E_IQUEUEt_( r )	queue_index_<r>
	#define E_IQUEUEt( r )	queue_index<r>
	
	#define E_IQUEUE_	E_IQUEUEt_( epeios::row__ )
	#define E_IQUEUE	E_IQUEUEt( epeios::row__ )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
