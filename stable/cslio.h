/*
	Header for the 'cslio' library by Claude SIMON (csimon@epeios.org)
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

#ifndef CSLIO__INC
#define CSLIO__INC

#define CSLIO_NAME		"CSLIO"

#define	CSLIO_VERSION	"$Revision$"

#define CSLIO_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &CSLIOTutor;

#if defined( XXX_DBG ) && !defined( CSLIO_NODBG )
#define CSLIO_DBG
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
//D C Standard Library Input/Output 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "bso.h"
#include <stdio.h>

namespace cslio {
	typedef size_t	amount__;

	typedef FILE *descriptor__;

#define CSLIO_UNDEFINED_DESCRIPTOR	NULL

	class io_core__ {
	protected:
		descriptor__ D_;
		void _Test( void ) const
		{
#ifdef CSLIO_DBG
			if ( D_ == CSLIO_UNDEFINED_DESCRIPTOR )
				ERRu();
#endif
		}
	public:
		io_core__( descriptor__ D )
		: D_( D )
		{}
		void Seek( long Offset )
		{
			_Test();

			if ( fseek( D_, Offset, SEEK_SET ) != Offset )
				ERRx();
		}
		void operator()( descriptor__ D )
		{
			D_ = D;
		}
	};


	class standard_input__
	: public virtual io_core__
	{
	public:
		standard_input__( descriptor__ D = CSLIO_UNDEFINED_DESCRIPTOR )
		: io_core__( D )
		{}
		size_t Read(
			amount__ Amount,
			void *Buffer )
		{
			_Test();

			return fread( Buffer, 1, Amount, D_ );
		}
		bso::bool__ OnEOF( void )
		{
			_Test();

			return feof( D_ ) != 0;
		}
	};

	class standard_output__
	: public virtual io_core__
	{
	public:
		standard_output__( descriptor__ D = CSLIO_UNDEFINED_DESCRIPTOR )
		: io_core__( D )
		{}
		size_t Write(
			const void *Buffer,
			amount__ Amount )
		{
			_Test();

			return fwrite( Buffer, 1, Amount, D_ );
		}
		void Flush( void )
		{
			_Test();

			fflush( D_ );
		}
	};

	class standard_io__
	: public standard_output__,
	  public standard_input__
	{
	public:
		standard_io__( descriptor__ D = CSLIO_UNDEFINED_DESCRIPTOR )
		: standard_output__( D ),
		  standard_input__( D ),
		  io_core__( D )
		{}
	};

#if 0
	class file_standard_io___
	: public standard_io__
	{
	private:
		FILE *FD_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( FD_ != NULL )
					if ( fclose( FD_ ) != 0 )
						ERRx();
			}

			FD_ = NULL;
		}
		file_standard_io___( void )
		: standard_io__( FD_ )
		{
			reset( false );
		}
		~file_standard_io___( void )
		{
			reset();
		}
		status__ Init(
			const char *FileName,
			mode__ Mode )
		{
			char Flags[4]="b";

			reset();

			switch ( Mode ) {
			case mRemove:
				strcat( Flags, "w+" );
				break;
			case mAppend:
				strcat( Flags, "a+" );
				break;
			case mReadWrite:
				strcat( Flags, "r+" );
				break;
			case mReadOnly:
				strcat( Flags, "r" );
				break;
			default:
				ERRu();
				break;
			}

			if ( ( FD_ = fopen( FileName, Flags ) ) ==  NULL )
				return sFailure;
			else
				return sSuccess;
		}
	};
#endif

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
