/*
  Header for the 'thrstr' library by Claude L. Simon (epeios@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org) 

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
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef THRSTR__INC
#define THRSTR__INC

#define THRSTR_NAME		"THRSTR"

#define	THRSTR_VERSION	"$Revision$"	

#define THRSTR_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &THRSTRTutor;

#if defined( XXX_DBG ) && !defined( THRSTR_NODBG )
#define THRSTR_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (epeios@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D PLEASE COMPLETE
/* End addendum to automatic documentation generation part. */

#error "Obsolete library. Integrated in the 'MTK' library."

#include "err.h"
#include "flw.h"
#include "mtx.h"
#include "mtk.h"

//d The value of an invalid handler.
#define THRSTR_INVALID	NULL

namespace thrstr {
	struct data {
			mtx::mutex_handler__ Data;
			mtx::mutex_handler__ Thread;
			mtx::mutex_handler__ Store;
			bso__bool Continue;
			void *UP;
			bso__ulong Amount;
			mtx::mutex_handler__ AmountMutex;
			mtk__routine Routine;
	};
	
	typedef data* thread_store_handler__;
	
	//f Create a thread_store_handler which deals with 'Routine'.
	thread_store_handler__ Create( mtk__routine Routine );

	//f Delete 'Handler'.	
	void Close( thread_store_handler__ Handler );

	
	//c A thread store.
	class thread_store___ {
	private:
		thread_store_handler__ Handler_;
		mtx::mutex___ Data_;
		mtx::mutex___ Thread_;
		mtx::mutex___ Store_;
	public:
		void reset( bso__bool P = true )
		{
			Data_.reset( P );
			Thread_.reset( P );
			Store_.reset( P );
			Handler_ = THRSTR_INVALID;
		}
		thread_store___( void )
		{
			reset( false );
		}
		~thread_store___( void )
		{
			reset( true );
		}
		//f Initialinf with thread store handler 'Handler'.	
		void Init( thread_store_handler__ Handler )
		{
			Handler_ = Handler;
			
			Data_.Init( Handler->Data );
			Thread_.Init( Handler->Thread );
			Store_.Init( Handler->Store );
		}
		//f Launch a routine with 'UP' as user pointer.
		void Launch( void *UP );
	};		
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
