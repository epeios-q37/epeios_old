/*
	Header for the 'ids' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2002  Claude SIMON (csimon@epeios.org).

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

#ifndef IDS__INC
#define IDS__INC

#define IDS_NAME		"IDS"

#define	IDS_VERSION	"$Revision$"

#define IDS_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &IDSTutor;

#if defined( XXX_DBG ) && !defined( IDS_NODBG )
#define IDS_DBG
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
//D ID Store 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "stk.h"

namespace ids {
	//c Store of ids of type 'id__'. Use 'ID_STORE' rather than directly this class.
	template <typename id__> class ids_store_ 
	{
	public:
		struct s
		{
			stk::E_STACK_( id__ )::s Released;
			//r First unused ID.
			id__ FirstUnused;
		} &S_;
		//o Released IDs
		stk::E_STACK_( id__ ) Released;
		ids_store_( s &S )
		: S_( S ),
		  Released( S.Released )
		{}
		void reset( bso::bool__ P = true )
		{
			Released.reset( P );
			S_.FirstUnused = 0;
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MD )
		{
			Released.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Released.plug( MM );
		}
		ids_store_ &operator =( const ids_store_ &IS )
		{
			Released = S_.Released;
			S_.FirstUnused = IS.S_.FirstUnused;

			return *this;
		}
		//f Initialization with 'First' as first unused.
		void Init( id__ First = 0 )
		{
			S_.FirstUnused = First;

			Released.Init();
		}
		//f Return a new unused id. 'Released' is set at 'true' if the id is a released one.
		id__ New( bso::bool__ &Released )
		{
			id__ ID;

			if ( this->Released.Amount() ) {
				ID = this->Released.Pop();
				Released = true;
			} else {
				ID = S_.FirstUnused++;

				if ( S_.FirstUnused == 0 )
					ERRl();

				Released = false;
			}

			return ID;
		}
		//f Return a new unused id.
		id__ New( void )
		{
			bso::bool__ Released;

			return New( Released );
		}
		//f Released id 'ID'.
		void Release( id__ ID )
		{
#ifdef IDS_DBG
			if ( Released.Exists( ID ) )
				ERRu();
			if ( ID >= S_.FirstUnused )
				ERRu();
#endif
			Released.Push( ID );
		}
		//f Return the first unused id.
		id__ GetFirstUnused( void ) const
		{
			return S_.FirstUnused;
		}
		//f Return the amount of released IDs.
		epeios::size__ Amount( void ) const
		{
			return Released.Amount();
		}
		//f Return true' if 'ID' exists, false otherwize.
		bso::bool__ Exists( id__ ID ) const
		{
			return Released.ObjectExists( ID );
		}
	};

	AUTO1( ids_store )

	#define E_IDS_STORE_( t )	ids_store_<t>
	#define E_IDS_STORE( t )	ids_store<t>
	

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
