/*
	Header for the 'ltf' library by Claude SIMON (csimon@epeios.org)
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

#ifndef LTF__INC
#define LTF__INC

#define LTF_NAME		"LTF"

#define	LTF_VERSION	"$Revision$"

#define LTF_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &LTFTutor;

#if defined( XXX_DBG ) && !defined( LTF_NODBG )
#define LTF_DBG
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
//D Line Text Flow. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "txf.h"

namespace ltf {
	#define LTF__SIZE_MAX	BSO_UBYTE_MAX

	class _line_text_flow__
	: public flw::oflow__
	{
	private:
		flw::datum__ *Data_;
		bso::ubyte__ Size_;
		bso::ubyte__ Amount_;
		txf::text_oflow__ &TFlow_;
	protected:
		virtual flw::size__ FLWWrite(
			const flw::datum__ *Buffer,
			flw::size__ Wanted,
			flw::size__ Minimum,
			bso::bool__ Synchronization )
		{
			if ( ( Amount_ + Wanted ) > Size_ ) {
				if ( Wanted >= Size_ ) {
					memcpy( Data_, Buffer + ( Size_ - Wanted ), Size_ );
				} else {
					memcpy( Data_, Data_ + Wanted, Size_ - Wanted );
					memcpy( Data_ + Size_ - Wanted, Buffer, Wanted );
				}

				Amount_ = Size_ + 1;
			} else {
				memcpy( Data_ + Amount_, Buffer, Wanted );
				Amount_ += Wanted;
			}

			if ( Amount_ < Size_ ) {
				TFlow_.Put( Buffer, Wanted );
			} else if ( Amount_ > Size_ )
				Data_[0] = '<';

			if ( Synchronization ) {
				if ( Amount_ > Size_ ) {
					TFlow_ << txf::rfl;
					TFlow_.Put( Data_, Size_ );
				}
				TFlow_ << txf::sync;
			}

			return Wanted;
		}
	public:
		_line_text_flow__(
			txf::text_oflow__ &TFlow,
			flw::datum__ *Data,
			flw::size__ Size )
		: TFlow_( TFlow ),
		  flw::oflow__( NULL, 0, BSO_ULONG_MAX )
		{
			if ( Size > LTF__SIZE_MAX )
				ERRl();

			Data_ = Data;
			Size_ = Size;

			memset( Data_, ' ', Size_ );
			Data_[Size_] = 0;
			Amount_ = 0;
		}
		void Clear( void )
		{
			CR();
			memset( Data_, ' ', Size_ );
			TFlow_.Put( Data_, Size_ );
			TFlow_ << txf::rfl;
		}
		void CR( void )
		{
			TFlow_ << txf::rfl;
			Amount_ = 0;
		}
	};


	template <int size = 79> class line_text_flow__
	: public txf::text_oflow__
	{
	private:
		flw::datum__ Data_[size];
	protected:
		_line_text_flow__ Flow_;
	public:
		line_text_flow__( txf::text_oflow__ &TFlow )
		: Flow_( TFlow, Data_, size ),
		  txf::text_oflow__( Flow_ )
		{}
		void CR( void )
		{
			Flow_.CR();
		}
		void Clear( void )
		{
			Flow_.Clear();
		}
	};

/*
	template <int size> inline line_console__<size> & CR( line_console__<size> &Flow )
	{
		Flow.Line_.CR();

		return *this;
	}
*/

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
