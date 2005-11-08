/*
	Header for the 'cio' library by Claude SIMON (csimon@epeios.org)
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

#ifdef CPE__LIBRARY
#	error "Cannot be used in a library !"
#endif

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

#ifndef CPE__MT
#	define coutf	unsafe_coutf
#	define cerrf	unsafe_cerrf
#	define cinf		unsafe_cinf

#	define 	cout	unsafe_cout
#	define	cerr	unsafe_cerr
#	define	cin		unsafe_cin
#else
	extern flw::mutex__ cinm, coutm, cerrm;

	class aware_coutf___
	: public _oflow__
	{
	public:
		aware_coutf___( void )
		{
			_oflow__::Init(  coutd, coutm );
		}
	};

	class aware_cout___
	: public txf::text_oflow__
	{
	private:
		aware_coutf___ _CoutF;
	public:
		aware_cout___( void )
		: text_oflow__( _CoutF )
		{}
	};


	class aware_cerrf___
	: public _oflow__
	{
	public:
		aware_cerrf___( void )
		{
			_oflow__::Init( cerrd, cerrm );
		}
	};

	class aware_cerr___
	: public txf::text_oflow__
	{
	private:
		aware_cerrf___ _CerrF;
	public:
		aware_cerr___( void )
		: text_oflow__( _CerrF )
		{}
	};

	class aware_cinf___
	: public _iflow__
	{
	public:
		aware_cinf___( void )
		{
			_iflow__::Init( cind, cinm );
		}
	};

	class aware_cin___
	: public txf::text_iflow__
	{
	private:
		aware_cinf___ _CinF;
	public:
		aware_cin___( void )
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
