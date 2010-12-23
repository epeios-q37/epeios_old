/*
	Header for the 'cio' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

#define CIO_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &CIOTutor;

#if defined( XXX_DBG ) && !defined( CIO_NODBG )
#define CIO_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
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
	// Si modifié, modifier 'WINTOL' en conséquence !
#endif

namespace cio {

	extern iop::descriptor__ cind, coutd, cerrd;

	//o Standard output as a pure flow (not a text flow).
	extern iof::io_oflow_functions___ _coutf;

	//o Error output as a pure flow (not a text flow).
	extern iof::io_oflow_functions___ _cerrf;

	//o Standard input as a pure flow (not a text flow).
	extern iof::io_iflow_functions___ _cinf;

	class unsafe_coutf___
	: public flw::oflow__
	{
	private:
		flw::datum__ _Cache[IOP__BUFFER_SIZE];
	public:
		unsafe_coutf___( flw::size__ AmountMax = FLW_SIZE_MAX )
		: oflow__( _coutf, _Cache, sizeof( _Cache ), AmountMax ) 
		{}
		void Init( void )
		{}
	};

	class unsafe_cout___
	: public txf::text_oflow__
	{
	private:
		unsafe_coutf___ _CoutF;
	public:
		void Init( void )
		{
			_CoutF.Init();
			text_oflow__::Init( _CoutF );
		}
	};


	class unsafe_cerrf___
	: public flw::oflow__
	{
	private:
		flw::datum__ _Cache[IOP__BUFFER_SIZE];
	public:
		unsafe_cerrf___( flw::size__ AmountMax = FLW_SIZE_MAX )
		: oflow__( _cerrf, _Cache, sizeof( _Cache ), AmountMax ) 
		{}
		void Init( void )
		{}
	};

	class unsafe_cerr___
	: public txf::text_oflow__
	{
	private:
		unsafe_cerrf___ _CerrF;
	public:
		void Init( void )
		{
			_CerrF.Init();
			text_oflow__::Init( _CerrF );
		}

	};

	class unsafe_cinf___
	: public flw::iflow__
	{
	private:
		flw::datum__ _Cache[IOP__BUFFER_SIZE];
	public:
		unsafe_cinf___( flw::size__ AmountMax = FLW_SIZE_MAX )
		: iflow__( _cinf, _Cache, sizeof( _Cache ), AmountMax ) 
		{}
		void Init( void )
		{}
	};

	class unsafe_cin___
	: public txf::text_iflow__
	{
	private:
		unsafe_cinf___ _CinF;
	public:
		void Init( void )
		{
			_CinF.Init();
			text_iflow__::Init( _CinF );
		}
	};

#ifndef CPE__T_MT
	extern unsafe_coutf___ coutf;
	extern unsafe_cerrf___ cerrf;
	extern unsafe_cinf___ cinf;

	extern unsafe_cout___ cout;
	extern unsafe_cerr___ cerr;
	extern unsafe_cin___ cin;
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
