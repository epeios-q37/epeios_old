/*
  Header for the 'prshct' library by Claude L. Simon (epeios@epeios.org)
  Copyright (C) 2000 Claude L. Simon 

  This file is part of the Epeios project (http://www.epeios.org/).
  

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

#ifndef PRSHCT__INC
#define PRSHCT__INC

#define PRSHCT_NAME		"PRSHCT"

#define	PRSHCT_VERSION	"$Revision$"	

#define PRSHCT_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &PRSHCTTutor;

#if defined( XXX_DBG ) && !defined( PRSHCT_NODBG )
#define PRSHCT_DBG 
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

#include "err.h"
#include "flw.h"
#include "txmhch.h"
#include "txmpgh.h"
#include "xtf.h"

struct prshct {
	//e Error codes.
	enum error {
		//i No error.
		rOK,
		//i Level error.
		rLevel,
		//i Syntax error.
		rSyntax
	};
};

/*f Parse 'IFlow' with marker 'Marker' and fill 'Text'. If 'ErrH' != 'err::hUsual',
you MUST give a 'Error'. */
POSITION__ PRSHCTParse(
	xtf::extended_text_iflow___ &IFlow,
	txmhch_hierarchical_text_ &Text,
	bso__char Marker,
	err::handle ErrH = err::hUsual,
	prshct::error &Error = *(prshct::error *)NULL );

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
