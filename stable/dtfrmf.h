/*
  Header for the 'dtfrmf' library by Claude L. Simon (epeios@epeios.org)
  Copyright (C) 2000 IDEALX (http://www.IDEALX.com/) 

  This file is part of an IDEALX (http://www.IDEALX.org/) project
  which uses Epeios (http://www.epeios.org/) technology.

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

#ifndef DTFRMF__INC
#define DTFRMF__INC

#define DTFRMF_NAME		"DTFRMF"

#define	DTFRMF_VERSION	"$Revision$"	

#define DTFRMF_OWNER		"IDEALX (http://www.IDEALX.com/)"

#include "ttr.h"

extern class ttr_tutor &DTFRMFTutor;

#if defined( XXX_DBG ) && !defined( DTFRMF_NODBG )
#define DTFRMF_DBG 
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
//D Data Transfert ReMote Features.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "dtfbsc.h"
#include "flx.h"

namespace dtfrmf {
	using namespace dtfbsc;
	
		//f Put 'Short' in 'Flow'.
	inline void PutShort(
		bso__ushort Short,
		flw::oflow___ &Flow )
	{
		PutByte( Short & 0xff, Flow );
		PutByte( Short >> 8, Flow );
	}

	//f Return short in 'Flow'
	inline bso__ushort GetShort( flw::iflow___ &Flow )
	{
		return GetByte( Flow ) + ( GetByte( Flow ) << 8 );
	}

	//f Put 'Long' in 'Flow'.
	inline void PutLong(
		bso__ulong Long,
		flw::oflow___ &Flow )
	{
		PutShort( (bso__ushort)( Long & 0xffff ), Flow );
		PutShort( (bso__ushort)( Long >> 16 ), Flow );
	}

	//f Return long in 'Flow'
	inline bso__ulong GetLong( flw::iflow___ &Flow )
	{
		return GetShort( Flow ) + ( GetShort( Flow ) << 16 );
	}
	
	//f Encapsulates 'Data' in flow (put size and then the contents of data as is).
	void Encapsulate(
		const flx::set_ &Data,
		flw::oflow___ &Flow );
		
	//f Decapsulate what in 'Flow' into 'Data'.
	void Decapsulate(
		flw::iflow___ &Flow,
		flx::set_ &Data );
		
		
};

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
