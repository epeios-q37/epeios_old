/*
	Header for the 'fil' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2001, 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef FIL__INC
#define FIL__INC

#define FIL_NAME		"FIL"

#define	FIL_VERSION	"$Revision$"

#define FIL_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &FILTutor;

#if defined( XXX_DBG ) && !defined( FIL_NODBG )
#define FIL_DBG
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
//D FILe 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

//D FILe. File management.

#include "iof.h"
#include "flw.h"

#ifdef FIL_FLOW_BUFFER_SIZE
#	define FIL__FLOW_BUFFER_SIZE FIL_FLOW_BUFFER_SIZE
#else
#	define FIL__FLOW_BUFFER_SIZE	100
#endif

namespace fil
{
	//e Open mode.
	enum mode {
		//i Unknow.
		mUnknow,
		//i Ecrase le contenu du fichier.
		mRemove,
		//i Ajoute à la fin du fichier.
		mAppend,
		//i Amount of mode.
		m_amount
	};
 	//e Status.
	enum status {
		//i Unknow,
		sUnknow,
		//i Success.
		sSuccess,
		//i Failure.
		sFailure,
		//i Amount of status.
		s_amount
	};

	typedef flw::iflow__	_iflow__;

	//c A file as standard input flow.
	class file_iflow___
	: public _iflow__
	{
	private:
		flw::datum__ Cache_[FIL__FLOW_BUFFER_SIZE];
		iof::file_io___ File_;
		flw::amount__ _HandleAmount(
			flw::amount__ Minimum,
			flw::datum__ *Tampon,
			flw::amount__ Desire,
			flw::amount__ AmountRead )
		{
			if ( AmountRead < Minimum )
			{
				if ( !File_.EOF() )
					ERRd();
				else
					AmountRead += _iflow__::HandleEOFD( Tampon + AmountRead, Desire - AmountRead );

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
#ifdef FIL_DBG
			if( Tampon == NULL )
				ERRu();
#endif
			flw::amount__ NombreLus = 0;

			if ( !File_.EOF() )
				NombreLus = File_.Read( Desire, Tampon );

			return _HandleAmount( Minimum, Tampon, Desire, NombreLus );
		}
public:
		void reset( bool P = true )
		{
			_iflow__::reset( P );
			File_.reset( P );
		}
		file_iflow___( void )
		{
			reset( false );
		}
		virtual ~file_iflow___( void )
		{
			reset( true );
		}
		/*f Initialization with the file named 'FileName'. Correct the '/' or '\' if 'Handle' = 'hCorrect'.
		Return 'sFailure' if initialization failes, and 'ErrHandle' set to 'err::hUsual', 'sSuccess' otherwise. */
		status Init(
			const char *FileName,
			err::handle ErrHandle = err::hUsual );
	};

	typedef flw::oflow__	_oflow__;

	//c A file as standard output flow.
	class file_oflow___
	: public _oflow__
	{
	private:
		iof::file_io___ File_;
		flw::datum__ Cache_[FIL__FLOW_BUFFER_SIZE];
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
			if ( (flw::size__)File_.Write( Tampon, Nombre ) < Nombre )
				ERRd();

			if ( Synchronize )
				File_.Flush();

			return Nombre;
		}
	public:
		void reset( bool P = true )
		{
			_oflow__::reset( P );
			File_.reset( P );

		}
		file_oflow___( void )
		{
			reset( false );
		}
		virtual ~file_oflow___( void )
		{
			reset( true );
		}
		/*f Initialization with the file named 'FileName'. If this file already exists, 'Mode' is was happen with it.
		If 'Handle' = 'hCorrect', corrects the '\' or '/' in file name.
		Return 'sFailure' if initialization failes, and 'ErrHandle' set to 'err::hUsual', 'sSuccess' otherwise. */
		status Init(
			const char *FileName,
			fil::mode Mode = fil::mRemove,
			err::handle ErrHandle = err::hUsual );
		/*f Initialization with the file named 'FileName'. If this file already exists, 'Mode' is was happen with it.
		If 'Handle' = 'hCorrect', corrects the '\' or '/' in file name.
		Return 'sFailure' if initialization failes, and 'ErrHandle' set to 'err::hUsual', 'sSuccess' otherwise. */
		status Init(
			const char *Nom,
			err::handle ErrHandle,
			mode Mode = fil::mRemove )
		{
			return Init( Nom, Mode, ErrHandle );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
