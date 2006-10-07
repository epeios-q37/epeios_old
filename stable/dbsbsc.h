/*
	Header for the 'dbsbsc' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef DBSBSC__INC
#define DBSBSC__INC

#define DBSBSC_NAME		"DBSBSC"

#define	DBSBSC_VERSION	"$Revision$"

#define DBSBSC_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &DBSBSCTutor;

#if defined( XXX_DBG ) && !defined( DBSBSC_NODBG )
#define DBSBSC_DBG
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
//D DataBaSe BaSiCs 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "tol.h"
#include "mdr.h"
#include "mmm.h"

namespace dbsbsc {
	class delayed_initialization_	// Permet d'éviter certaines opérations coûteuses en temps à l'initialisation.
	{
	protected:
		virtual void DBSBSCCompleteInitialization( void ) = 0;
	public:
		struct s
		{
			bso::bool__ InitializationCompleted;
		} &S_;
		delayed_initialization_( s &S )
		: S_( S )
		{}
		void reset( bso::bool__ = true )
		{
			S_.InitializationCompleted = false;
		}
		void plug( mmm::E_MULTIMEMORY_ & )
		{
			// A des fins de standardisation.
		}
		void plug( mdr::E_MEMORY_DRIVER__ &)
		{
			// A des fins de standardisation.
		}
		delayed_initialization_ &operator =( const delayed_initialization_ &DI )
		{
			S_.InitializationCompleted = DI.S_.InitializationCompleted;

			return *this;
		}
		void Init( bso::bool__ Partial )
		{
			if ( !Partial )
				DBSBSCCompleteInitialization();

			S_.InitializationCompleted = !Partial;
		}
		void CompleteInitialization( void )
		{
			if ( S_.InitializationCompleted )
				ERRu();

			DBSBSCCompleteInitialization();

			S_.InitializationCompleted = true;
		}
		E_RODISCLOSE_( bso::bool__, InitializationCompleted );
	};


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
