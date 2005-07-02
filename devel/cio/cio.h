/*
	Header for the 'cio' library by Claude SIMON (csimon@epeios.org)
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

#ifndef CIO__INC
#define CIO__INC

#define CIO_NAME		"CIO"

#define	CIO_VERSION	"$Revision$"

#define CIO_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &CIOTutor;

#if defined( XXX_DBG ) && !defined( CIO_NODBG )
#define CIO_DBG
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
//D Console Input/Output 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "iof.h"
#include "txf.h"
#include "cio.h"
#include "cpe.h"

namespace cio {

	extern iof::descriptor__ cind, coutd, cerrd;

	typedef iof::io_oflow__	_oflow__;
	typedef iof::io_iflow__	_iflow__;

	//o Standard output as a pure flow (not a text flow).
	extern _oflow__ unsafe_coutf;

	//o Error output as a pure flow (not a text flow).
	extern _oflow__ unsafe_cerrf;

	//o Standard input as a pure flow (not a text flow).
	extern _iflow__ unsafe_cinf;

	//o Standard output as a text flow.
	extern txf::text_oflow__ unsafe_cout;

	//o Error output as a text flow.
	extern txf::text_oflow__ unsafe_cerr;

	//o Standard input as a text flow.
	extern txf::text_iflow__ unsafe_cin;

	//o All written in this object is erased.
	extern txf::text_oflow__ unsafe_nul;

#ifndef CPE__MT
	_oflow__ &coutf = unsafe_coutf;
	_oflow__ &cerrf = unsafe_cerrf;
	_iflow__ &cinf  = unsafe_cinf;

	txf::text_oflow__ &cout = unsafe_cout;
	txf::text_oflow__ &cerr = unsafe_cerr;
	txf::text_iflow__ &cin  = unsafe_cin;
#else
	extern flw::mutex__ cinm, coutm, cerrm;

	class safe_coutf___
	: public _oflow__
	{
	public:
		safe_coutf___( void )
		: _oflow__( coutd, coutm )
#ifdef CPE__USE_VC_WORKAROUND
		  ,io_core__( coutd )
#endif
		{}
	};

	class safe_cout___
	: public txf::text_oflow__
	{
	private:
		safe_coutf___ _CoutF;
	public:
		safe_cout___( void )
		: text_oflow__( _CoutF )
		{}
	};


	class safe_cerrf___
	: public _oflow__
	{
	public:
		safe_cerrf___( void )
		: _oflow__( cerrd, cerrm )
#ifdef CPE__USE_VC_WORKAROUND
		  ,io_core__( cerrd )
#endif
		{}
	};

	class safe_cerr___
	: public txf::text_oflow__
	{
	private:
		safe_cerrf___ _CerrF;
	public:
		safe_cerr___( void )
		: text_oflow__( _CerrF )
		{}
	};

	class safe_cinf___
	: public _iflow__
	{
	public:
		safe_cinf___( void )
		: _iflow__( cind, cinm )
#ifdef CPE__USE_VC_WORKAROUND
		  ,io_core__( cind )
#endif
		{}
	};

	class safe_cin___
	: public txf::text_iflow__
	{
	private:
		safe_cinf___ _CinF;
	public:
		safe_cin___( void )
		: text_iflow__( _CinF )
		{}
	};
#endif
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
