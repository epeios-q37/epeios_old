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
//D Input/Output and Files 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "cpe.h"

#ifdef IOF__USE_STANDARD_IO
#	define IOF__USE_STANDARD_IO
#elif defined( CPE__MS )
#	define IOF__USE_LOWLEVEL_IO
#elif defined( CPE__UNIX )
#	define IOF__USE_LOWLEVEL_IO
#elif defined( CPE__UNIX )
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
	typedef llio::lowlevel_io__		io__;
	typedef llio::file_lowlevel_io___	file_io___;
	using llio::amount__;
#elif defined( IOF__USE_STANDARD_IO )
	typedef sclio::standard_io__		io__;
	typedef sclio::file_standard_io___	file_io___;
	using sclio::amount__;
#else
#	error "Undefined I/O enviroment !"
#endif

	using namespace iodef;

	using flw::oflow__;
	using flw::iflow__;
	
	//c A stream output flow driver.
	class file_oflow__
	: public oflow__
	{
	private:
		io__ IO_;
		flw::datum__ Cache_[FLF__FLOW_BUFFER_SIZE];
	protected:
		virtual flw::size__ FLWWrite(
			const flw::datum__ *Tampon,
			flw::size__ Nombre,
			flw::size__,
			bool Synchronize )
		{
#ifdef STF_DBG
			if ( ( Tampon == NULL ) && Nombre )
				ERRu();
#endif
			if ( fwrite( Tampon, 1, Nombre, File_ ) < Nombre )
				ERRd();

			if ( Synchronize )
				fflush( File_ );

			return Nombre;
		}
	public:
		void reset( bool P = true )
		{
			oflow__::reset( P );
		}
		file_oflow__( FILE *&File )
		: oflow__(),
		  File_( File )
		{
			reset( false );
		}
		virtual ~file_oflow__( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( flw::amount__ AmountMax = FLW_AMOUNT_MAX )
		{
			oflow__::Init( Cache_, sizeof( Cache_ ), AmountMax );
		}
	};


	class file_iflow__
	: public iflow__
	{
	private:
		flw::datum__ Cache_[FLF__FLOW_BUFFER_SIZE];
		// Le stream en question.
		FILE *&File_;
		flw::amount__ _HandleAmount(
			flw::amount__ Minimum,
			flw::datum__ *Tampon,
			flw::amount__ Desire,
			flw::amount__ AmountRead )
		{
			if ( AmountRead < Minimum )
			{
				if ( !feof( File_ ) )
					ERRd();
				else
					AmountRead += iflow__::HandleEOFD( Tampon + AmountRead, Desire - AmountRead );

				if ( AmountRead < Minimum )
					ERRd();
			}

			return AmountRead;
		}
	protected:
	virtual flw::size__ FLWRead(
		flw::size__ Minimum,
		flw::datum__ *Tampon,
		flw::size__ Desire )
	{
#ifdef STF_DBG
		if( Tampon == NULL )
			ERRu();
#endif
		flw::amount__ NombreLus = 0;

		if ( !feof( File_ ) )
			NombreLus = fread( Tampon, 1, Desire, File_ );

		return _HandleAmount( Minimum, Tampon, Desire, NombreLus );
	}
	public:
		void reset( bool P = true )
		{
			iflow__::reset( P );
		}
		file_iflow__( FILE *&File )
		: iflow__(),
		  File_( File )
		{
			reset( false );
		}
		virtual ~file_iflow__( void )
		{
			reset( true );
		}
		//f Initialisation.
		void Init( flw::amount__ AmountMax = FLW_AMOUNT_MAX )
		{
			iflow__::Init( Cache_, sizeof( Cache_ ), AmountMax );
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
