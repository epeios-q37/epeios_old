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

	using namespace iodef;

	using flw::oflow__;
	using flw::iflow__;
	using flw::ioflow__;

	class io_oflow__
	: public output__,
	  public oflow__
	{
	private:
		flw::datum__ Cache_[IOF__BUFFER_SIZE];
	protected:
		flw::size__ FLWWrite(
			const flw::datum__ *Tampon,
			flw::size__ Minimum,
			flw::size__ Demande,
			bool Synchronize )
		{
#ifdef IOF_DBG
			if ( ( Tampon == NULL ) && ( Minimum || Demande ) )
				ERRu();
#endif
			flw::size__ Written = 0;

			while ( Written < Minimum )
				Written += output__::Write( Tampon, Demande - Written );

			if ( Synchronize )
				Flush();

			return Written;
		}
	public:
		io_oflow__(
			descriptor__ D = IOF_UNDEFINED_DESCRIPTOR,
			amount__ AmountMax = FLW_AMOUNT_MAX )
		: output__( D ),
		  oflow__( Cache_, sizeof( Cache_ ), AmountMax ),
		  io_core__( D )
		{}
	};

	class io_iflow__
	: public input__,
	  public iflow__
	{
	private:
		flw::datum__ Cache_[IOF__BUFFER_SIZE];
	private:
		flw::amount__ _HandleAmount(
			flw::amount__ Minimum,
			flw::datum__ *Tampon,
			flw::amount__ Desire,
			flw::amount__ AmountRead )
		{
			if ( AmountRead < Minimum )
			{
				if ( !OnEOF() )
					ERRd();
				else
					AmountRead += iflow__::HandleEOFD( Tampon + AmountRead, Desire - AmountRead );

				if ( AmountRead < Minimum )
					ERRd();
			}

			return AmountRead;
		}
	protected:
		flw::size__ FLWRead(
			flw::size__ Minimum,
			flw::datum__ *Tampon,
			flw::size__ Desire )
		{
	#ifdef STF_DBG
			if( Tampon == NULL )
				ERRu();
	#endif
			flw::amount__ NombreLus = 0;

			while ( !OnEOF() && ( NombreLus < Minimum ) )
				NombreLus += input__::Read( Desire - NombreLus, Tampon );

			return _HandleAmount( Minimum, Tampon, Desire, NombreLus );
		}
	public:
		io_iflow__(
			descriptor__ D =IOF_UNDEFINED_DESCRIPTOR,
			amount__ AmountMax = FLW_AMOUNT_MAX )
		: input__( D ),
		  iflow__( Cache_, sizeof( Cache_ ), AmountMax ),
		  io_core__( D )
		{}
	};

	class io_ioflow__
	: public io__,
	  public ioflow__
	{
	private:
		flw::datum__ Cache_[2 * IOF__BUFFER_SIZE];
	private:
		flw::amount__ _HandleAmount(
			flw::amount__ Minimum,
			flw::datum__ *Tampon,
			flw::amount__ Desire,
			flw::amount__ AmountRead )
		{
			if ( AmountRead < Minimum )
			{
				if ( !OnEOF() )
					ERRd();
				else
					AmountRead += iflow__::HandleEOFD( Tampon + AmountRead, Desire - AmountRead );

				if ( AmountRead < Minimum )
					ERRd();
			}

			return AmountRead;
		}
	protected:
		flw::size__ FLWRead(
			flw::size__ Minimum,
			flw::datum__ *Tampon,
			flw::size__ Desire )
		{
	#ifdef STF_DBG
			if( Tampon == NULL )
				ERRu();
	#endif
			flw::amount__ NombreLus = 0;

			while ( !OnEOF() && ( NombreLus < Minimum ) )
				NombreLus += input__::Read( Desire - NombreLus, Tampon );

			return _HandleAmount( Minimum, Tampon, Desire, NombreLus );
		}
		flw::size__ FLWWrite(
			const flw::datum__ *Tampon,
			flw::size__ Minimum,
			flw::size__ Demande,
			bool Synchronize )
		{
#ifdef IOF_DBG
			if ( ( Tampon == NULL ) && ( Minimum || Demande ) )
				ERRu();
#endif
			flw::size__ Written = 0;

			while ( Written < Minimum )
				Written += output__::Write( Tampon, Demande - Written );

			if ( Synchronize )
				Flush();

			return Written;
		}
	public:
		io_ioflow__(
			descriptor__ D = IOF_UNDEFINED_DESCRIPTOR,
			amount__ AmountMax = FLW_AMOUNT_MAX )
		: io__( D ),
		  ioflow__( Cache_, sizeof( Cache_ ), AmountMax ),
		  io_core__( D )
		{}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
