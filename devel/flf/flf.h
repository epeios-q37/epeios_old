/*
	Header for the 'flf' library by Claude SIMON (csimon@epeios.org)
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

#ifndef FLF__INC
#define FLF__INC

#define FLF_NAME		"FLF"

#define	FLF_VERSION	"$Revision$"

#define FLF_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &FLFTutor;

#if defined( XXX_DBG ) && !defined( FLF_NODBG )
#define FLF_DBG
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
//D FiLe Flow 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include <stdio.h>

#ifdef FLF_FLOW_BUFFER_SIZE
//d Buffer size for a stream output flow. 100 by default.
#	define FLF__FLOW_BUFFER_SIZE FLF_FLOW_BUFFER_SIZE
#else
#	define FLF__FLOW_BUFFER_SIZE	100
#endif


namespace flf {
	using flw::oflow__;
	using flw::iflow__;
	
	//c A stream output flow driver.
	class file_oflow__
	: public oflow__
	{
	private:
		FILE *&File_;
		flw::datum__ Cache_[FLF__FLOW_BUFFER_SIZE];
	protected:
		virtual flw::amount__ FLWPut(
			const flw::datum__ *Tampon,
			flw::amount__ Nombre,
			flw::amount__,
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
		void Init( void )
		{
			oflow__::Init( Cache_, sizeof( Cache_ ) );
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
	virtual flw::amount__ FLWGet(
		flw::amount__ Minimum,
		flw::datum__ *Tampon,
		flw::amount__ Desire )
	{
#ifdef STF_DBG
		if( Tampon == NULL )
			ERRu();
#endif
		flw::amount__ NombreLus = 0;

		if ( !feof( File_ ) )
		{
			if ( ( NombreLus = fread( Tampon, 1, Desire, File_ ) ) != Desire )
				if ( NombreLus < Minimum )
					ERRd();
		}

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
		void Init( void )
		{
			iflow__::Init( Cache_, sizeof( Cache_ ) );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
