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

namespace cio {

	typedef iof::io_oflow__	_oflow__;
	typedef iof::io_iflow__	_iflow__;

	//o Standard output as a pure flow (not a text flow).
	extern _oflow__ coutf;

	//o Error output as a pure flow (not a text flow).
	extern _oflow__ cerrf;

	//o Standard input as a pure flow (not a text flow).
	extern _iflow__ cinf;

	//o Standard output as a text flow.
	extern txf::text_oflow__ cout;

	//o Error output as a text flow.
	extern txf::text_oflow__ cerr;

	//o Standard input as a text flow.
	extern txf::text_iflow__ cin;

	template <int limit = 80> class _line_console__
	: public flw::oflow__
	{
	private:
		flw::datum__ Data_[limit+1];
		bso::ubyte__ Length_;
		txf::text_oflow__ &TFlow_;
	protected:
		virtual flw::size__ FLWWrite(
			const flw::datum__ *Buffer,
			flw::size__ Wanted,
			flw::size__ Minimum,
			bso::bool__ Synchronization )
		{
			if ( Length_ == 0 )
				TFlow_ << txf::rfl << (char *)Data_ << txf::rfl;

			if ( ( Length_ + Wanted ) > limit ) {
				if ( Wanted >= limit ) {
					memcpy( Data_, Buffer + ( limit - Wanted ), limit );
				} else {
					memcpy( Data_, Data_ + Wanted, limit - Wanted );
					memcpy( Data_ + limit - Wanted, Buffer, Wanted );
				}

				Length_ = limit + 1;
			} else {
				memcpy( Data_ + Length_, Buffer, Wanted );
				Length_ += Wanted;
			}

			if ( Length_ < limit ) {
				TFlow_.Put( Buffer, Wanted );
			} else if ( Length_ > limit )
				Data_[0] = '<';

			if ( Synchronization ) {
				if ( Length_ > limit )
					TFlow_ << txf::rfl << (char *)Data_;
				TFlow_ << txf::sync;
			}

			return Wanted;
		}
	public:
		_line_console__( txf::text_oflow__ &TFlow )
		: TFlow_( TFlow ),
		  flw::oflow__( NULL, 0, BSO_ULONG_MAX )
		{
			if ( limit > BSO_UBYTE_MAX )
				ERRl();

			memset( Data_, ' ', limit );
			Data_[limit] = 0;
			Length_ = 0;
		}
		void nl( void )
		{
			TFlow_ << txf::rfl << (char *)Data_ << txf::sync;
			Length_ = 0;
			memset( Data_, ' ', limit );
		}
	};

	template <int limit = 79> class line_console__
	: public txf::text_oflow__
	{
	private:
		_line_console__<limit> Line_;
	public:
		line_console__( txf::text_oflow__ &TFlow )
		: Line_( TFlow ),
		  txf::text_oflow__( Line_ )
		{}
		void nl( void )
		{
			Line_.nl();
		}
	};


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
