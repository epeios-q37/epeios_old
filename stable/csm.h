/*
  Header for the 'csm' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org) 

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
  along with this program; if not, go to http://www.fsf.org/
  or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef CSM__INC
#define CSM__INC

#define CSM_NAME		"CSM"

#define	CSM_VERSION	"$Revision$"	

#define CSM_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &CSMTutor;

#if defined( XXX_DBG ) && !defined( CSM_NODBG )
#define CSM_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (simon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "salcsm.h"
#include "srv.h"

//d An alias for the user client process function.
#define CSMCP	SALCSMCP
//d An alias for the user client initialization function.
#define CSMCI	SALCSMCI
//d An alias for the user client ending function.
#define CSMCE	SALCSMCE
//d An alias for the user server process function.
#define CSMSP	SALCSMSP
//d An alias for the user server initialization function.
#define CSMSI	SALCSMSI
//d An alias for the user server endinf function.
#define CSMSE	SALCSMSE

namespace csm {
	using namespace salcsm;
	//c To manage client/server operations.
	class manager___
	: public functions__
	{
	public:
		void reset( bso__bool = true )
		{}
		manager___( void )
		{
			reset( false );
		}
		~manager___( void )			
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{
			reset();
		}
		//f Process.
		void Process(
			srv::service__ Service,
			err::handle Handle = err::hUsual );
	};
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
