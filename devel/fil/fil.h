/*
  Header for the 'fil' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org) 

  This file is part of the Epeios (http://www.epeios.org/) project.
  

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

#define FIL_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &FILTutor;

#if defined( XXX_DBG ) && !defined( FIL_NODBG )
#define FIL_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (simon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

//D FILe. File management.

#include "err.h"
#include "stf.h"
#include <fstream.h>

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

	using stf::istream_iflow___;

	//c A file as standard input flow.
	class file_iflow___
	: public istream_iflow___
	{
	private:
		ifstream Stream_;
	public:
		void reset( bool P = true )
		{
			istream_iflow___::reset( P );

			if ( P )
				Stream_.close();

			Stream_.clear();
		}
		file_iflow___( void )
		: istream_iflow___( Stream_ )
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

	using stf::ostream_oflow___;

	//c A file as standard output flow.
	class file_oflow___
	: public ostream_oflow___
	{
	private:
		ofstream Stream_;
	public:
		void reset( bool P = true )
		{
			ostream_oflow___::reset( P );

			if ( P )
				Stream_.close();

			Stream_.clear();
		}
		file_oflow___( void )
		: ostream_oflow___( Stream_ )
		{
			reset( false );
		}
		virtual ~file_oflow___( void )
		{
			reset( false );
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



};

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
