/*
	Header for the 'iof' library by Claude SIMON (csimon@epeios.org)
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

#ifndef IOF__INC
#define IOF__INC

#define IOF_NAME		"IOF"

#define	IOF_VERSION	"$Revision$"

#define IOF_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &IOFTutor;

#if defined( XXX_DBG ) && !defined( IOF_NODBG )
#define IOF_DBG
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
//D Input/Output Flows 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "cpe.h"
#include "flf.h"
#include "flw.h"

#ifdef IOF_BUFFER_SIZE
#	define IOF__BUFFER_SIZE	IOF_BUFFER_SIZE
#else
#	define IOF__BUFFER_SIZE	1024
#endif

#ifdef IOF_USE_STANDARD_IO
#	define IOF__USE_STANDARD_IO
#elif defined( CPE__MS )
#	define IOF__USE_LOWLEVEL_IO
#elif defined( CPE__UNIX )
#	define IOF__USE_LOWLEVEL_IO
#elif defined( CPE__MAC )
//#	define IO__USE_LOWLEVEL_IO	// Because not implemented yet for Mac.
#	define	IOF__USE_STANDARD_IO
#else
#	define	IOF__USE_STANDARD_IO
#endif

#ifdef IOF__USE_STANDARD_IO
#	include "cslio.h"
#elif defined( IOF__USE_LOWLEVEL_IO )
#	include "llio.h"
#else
#	error "Undefined I/O enviroment !"
#endif

namespace iof {
#ifdef IOF__USE_LOWLEVEL_IO
	typedef llio::lowlevel_input__		input__;
	typedef llio::lowlevel_output__		output__;
	typedef llio::lowlevel_io__			io__;
	using llio::descriptor__;
	using llio::amount__;
#	define IOF_UNDEFINED_DESCRIPTOR	LLIO_UNDEFINED_DESCRIPTOR
#elif defined( IOF__USE_STANDARD_IO )
	typedef cslio::standard_input__		input__;
	typedef cslio::standard_output__		output__;
	typedef cslio::standard_io__			io__;
	using cslio::descriptor__;
	using cslio::amount__;
#	define IOF_UNDEFINED_DESCRIPTOR	CSLIO_UNDEFINED_DESCRIPTOR
#else
#	error "Undefined I/O enviroment !"
#endif

	class io_oflow_functions___
	: public output__,
	  public flf::oflow_functions___
	{
	protected:
		flf::bsize__ FLFWrite(
			const flf::datum__ *Tampon,
			flf::bsize__ Minimum,
			flf::bsize__ Demande )
		{
#ifdef IOF_DBG
			if ( ( Tampon == NULL ) && ( Minimum || Demande ) )
				ERRu();
#endif
			flf::bsize__ Written = 0;

			while ( Written < Minimum )
				Written += output__::Write( Tampon, Demande - Written );

			return Written;
		}
		void FLFSynchronize( void )
		{
			Flush();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			output__::reset( P );
			oflow_functions___::reset( P );
		}
		io_oflow_functions___( void )
		{
			reset( false );
		}
		virtual ~io_oflow_functions___( void )
		{
			reset();
		}
		void Init( descriptor__ D )
		{
			output__::Init( D );
			oflow_functions___::Init();
		}
	};

	class io_oflow___
	: public flw::oflow__
	{
	private:
		io_oflow_functions___ _Functions;
		flw::datum__ _Cache[IOF__BUFFER_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			oflow__::reset( P );
		}
		io_oflow___( amount__ AmountMax )
		: oflow__( _Functions, _Cache, sizeof( _Cache ), AmountMax )
		{
			reset( false );
		}
		virtual ~io_oflow___( void )
		{
			reset();
		}
		void Init( descriptor__ D )
		{
			_Functions.Init( D );
		}
	};

	class io_iflow_functions___
	: public input__,
	  public flf::iflow_functions___
	{
	protected:
		flf::bsize__ FLFRead(
			flf::bsize__ Minimum,
			flf::datum__ *Tampon,
			flf::bsize__ Desire )
		{
	#ifdef STF_DBG
			if( Tampon == NULL )
				ERRu();
	#endif
			flf::bsize__ NombreLus = 0;

			while ( !OnEOF() && ( NombreLus < Minimum ) )
				NombreLus += input__::Read( Desire - NombreLus, Tampon );

			return NombreLus;
		}
		void FLFDismiss( void )
		{}
	public:
		void reset( bso::bool__ P = true )
		{
			input__::reset( P );
			iflow_functions___::reset( P );
		}
		io_iflow_functions___( void )
		{
			reset( false );
		}
		virtual ~io_iflow_functions___( void )
		{
			reset();
		}
		void Init( descriptor__ D )
		{
			input__::Init( D );
			iflow_functions___::Init();
		}
	};

	class io_iflow___
	: public flw::iflow__
	{
	private:
		flw::datum__ _Cache[IOF__BUFFER_SIZE];
		io_iflow_functions___ _Functions;
	public:
		void reset( bso::bool__ P = true )
		{
			_Functions.reset( P );
			iflow__::reset( P );
		}
		io_iflow___( amount__ AmountMax )
		: iflow__( _Functions, _Cache, sizeof( _Cache ), AmountMax )
		{
			reset( false );
		}
		virtual ~io_iflow___( void )
		{
			reset();
		}
		void Init( descriptor__ D )
		{
			_Functions.Init( D );
		}
	};

	class io_flow_functions___
	: public output__,
	  public input__,
	  public flf::ioflow_functions___
	{
	protected:
		flf::bsize__ FLFWrite(
			const flf::datum__ *Tampon,
			flf::bsize__ Minimum,
			flf::bsize__ Demande )
		{
#ifdef IOF_DBG
			if ( ( Tampon == NULL ) && ( Minimum || Demande ) )
				ERRu();
#endif
			flf::bsize__ Written = 0;

			while ( Written < Minimum )
				Written += output__::Write( Tampon, Demande - Written );

			return Written;
		}
		void FLFSynchronize( void )
		{
			Flush();
		}
		flf::bsize__ FLFRead(
			flf::bsize__ Minimum,
			flf::datum__ *Tampon,
			flf::bsize__ Desire )
		{
	#ifdef STF_DBG
			if( Tampon == NULL )
				ERRu();
	#endif
			flf::bsize__ NombreLus = 0;

			while ( !OnEOF() && ( NombreLus < Minimum ) )
				NombreLus += input__::Read( Desire - NombreLus, Tampon );

			return NombreLus;
		}
		void FLFDismiss( void )
		{}
	public:
		void Init(  descriptor__ D  )
		{
			input__::Init( D );
			output__::Init( D );
		}

	};

	class io_flow___
	: public io__,
	  public flw::ioflow__
	{
	private:
		io_flow_functions___ _Functions;
		flw::datum__ _Cache[2 * IOF__BUFFER_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			io__::reset( P );
			ioflow__::reset( P );
		}
		io_flow___( amount__ AmountMax )
		: ioflow__( _Functions, _Cache, sizeof( _Cache ), AmountMax )
		{
			reset( false );
		}
		virtual ~io_flow___( void )
		{
			reset();
		}
		void Init(
			descriptor__ D,
			amount__ AmountMax )
		{
			io__::Init( D );
			_Functions.Init( D );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
