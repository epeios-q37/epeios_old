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

#ifdef CPE__T_LIBRARY
#	error "Cannot be used in a library !"
#endif

#ifndef CIO_NO_AUTOMATIC_INITIALIZATION	// Usefull for a Windows service.
#	define CIO__AUTOMATIC_INITIALIZATION
#endif

namespace cio {

	extern iop::descriptor__ cind, coutd, cerrd;

	//o Standard output as a pure flow (not a text flow).
	extern iof::io_oflow_functions___ _coutf;

	//o Error output as a pure flow (not a text flow).
	extern iof::io_oflow_functions___ _cerrf;

	//o Standard input as a pure flow (not a text flow).
	extern iof::io_iflow_functions___ _cinf;

	class aware_coutf___
	: public flw::oflow__
	{
	private:
		flw::datum__ _Cache[IOP__BUFFER_SIZE];
	public:
		aware_coutf___( flw::size__ AmountMax = FLW_SIZE_MAX )
		: oflow__( _coutf, _Cache, sizeof( _Cache ), AmountMax ) 
		{}
		void Init( void )
		{}
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
	: public flw::oflow__
	{
	private:
		flw::datum__ _Cache[IOP__BUFFER_SIZE];
	public:
		aware_cerrf___( flw::size__ AmountMax = FLW_SIZE_MAX )
		: oflow__( _cerrf, _Cache, sizeof( _Cache ), AmountMax ) 
		{}
		void Init( void )
		{}
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
	: public flw::iflow__
	{
	private:
		flw::datum__ _Cache[IOP__BUFFER_SIZE];
	public:
		aware_cinf___( flw::size__ AmountMax = FLW_SIZE_MAX )
		: iflow__( _cinf, _Cache, sizeof( _Cache ), AmountMax ) 
		{}
		void Init( void )
		{}
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

#ifndef CPE__T_MT
	extern aware_coutf___ coutf;
	extern aware_cerrf___ cerrf;
	extern aware_cinf___ cinf;

	extern aware_cout___ cout;
	extern aware_cerr___ cerr;
	extern aware_cin___ cin;
#endif

	void Initialize( void );
	/* Peform the initialization needed by this library if not
	automatically done (defining of 'CIO_NO_AUTOMATIC_INITIALIZATION' ). */

	bso::bool__ IsInitialized( void );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
