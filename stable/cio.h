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

# include "err.h"
# include "iof.h"
# include "txf.h"
# include "cpe.h"
# include "flx.h"

# ifdef CPE__T_LIBRARY
#  error "Cannot be used in a library !"
# endif

# ifndef CIO_NO_AUTOMATIC_INITIALIZATION	// Utilise dans le cadre d'un service Windows.
#  define CIO__AUTOMATIC_INITIALIZATION 	// Si modifié, modifié 'WINTOL' en conséquence !
# endif

namespace cio {

	extern iop::descriptor__ CInDescriptor, COutDescriptor, CErrDescriptor;

	// 'Thread-safe'.
	extern flx::relay_oflow_driver___ COutDriver;
	extern flx::relay_oflow_driver___ CErrDriver;
	extern flx::relay_iflow_driver___ CInDriver;

	class coutf___
	: public flw::oflow__
	{
	private:
		flw::datum__ _Cache[IOP__BUFFER_SIZE];
	public:
		void Init( flw::size__ AmountMax = FLW_SIZE_MAX )
		{
			oflow__::Init( COutDriver, _Cache, sizeof( _Cache ), AmountMax );
		}
	};

	class cout___
	: public txf::text_oflow__
	{
	private:
		coutf___ _CoutF;
	public:
		void reset( bso::bool__ P = true )
		{
			text_oflow__::reset( P );
			_CoutF.reset( P );
		}
		cout___( void )
		{
			reset( false );
		}
		~cout___( void )
		{
			reset();
		}
		void Init( flw::size__ AmountMax = FLW_SIZE_MAX )
		{
			_CoutF.Init( AmountMax );
			text_oflow__::Init( _CoutF );
		}
	};


	class cerrf___
	: public flw::oflow__
	{
	private:
		flw::datum__ _Cache[IOP__BUFFER_SIZE];
	public:
		void Init( flw::size__ AmountMax = FLW_SIZE_MAX )
		{
			oflow__::Init( CErrDriver, _Cache, sizeof( _Cache ), AmountMax ) ;
		}
	};

	class cerr___
	: public txf::text_oflow__
	{
	private:
		cerrf___ _CerrF;
	public:
		void reset( bso::bool__ P = true )
		{
			text_oflow__::reset( P );
			_CerrF.reset( P );
		}
		cerr___( void )
		{
			reset( false );
		}
		~cerr___( void )
		{
			reset();
		}
		void Init( flw::size__ AmountMax = FLW_SIZE_MAX )
		{
			_CerrF.Init( AmountMax );
			text_oflow__::Init( _CerrF );
		}
	};

	class cinf___
	: public flw::iflow__
	{
	public:
		void Init( flw::size__ AmountMax = FLW_SIZE_MAX )
		{
			iflow__::Init( CInDriver, AmountMax );
		}
	};

	class cin___
	: public txf::text_iflow__
	{
	private:
		cinf___ _CinF;
	public:
		void reset( bso::bool__ P = true )
		{
			text_iflow__::reset( P );
			_CinF.reset( P );
		}
		cin___( void )
		{
			reset( false );
		}
		~cin___( void )
		{
			reset();
		}
		void Init( flw::size__ AmountMax = FLW_SIZE_MAX )
		{
			_CinF.Init( AmountMax );
			text_iflow__::Init( _CinF );
		}
	};

	// 'thread UNsafe'.
	extern cout___ COut;
	extern cerr___ CErr;
	extern cin___ CIn;

	enum target__
	{
		tConsole,	// Lecture/écriture de/dans la console.
		tVoid,		// Lecture/écriture de/dans rien (utile pour les service Windows.
		t_amount,
		t_Undefined,
		t_Default = tConsole
	};

	void Initialize( target__ Target );
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
