/*
	Header for the 'ltf' library by Claude SIMON (csimon@epeios.org)
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

	class _line_text_oflow_functions___
	: public flw::oflow_functions___
	{
	private:
		flw::datum__ *Data_;
		bso::ubyte__ Size_;
		bso::ubyte__ Amount_;
		txf::text_oflow__ &TFlow_;
	protected:
		virtual flw::bsize__ FLWWrite(
			const flw::datum__ *Buffer,
			flw::bsize__ Wanted,
			flw::bsize__ Minimum )
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

			return Wanted;
		}
		virtual void FLWSynchronize( void )
		{
			if ( Amount_ > Size_ ) {
				TFlow_ << txf::rfl;
				TFlow_.Put( Data_, Size_ );
			}

			TFlow_ << txf::sync;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			oflow_functions___::reset( P );
		}
		_line_text_oflow_functions___( txf::text_oflow__ &TFlow )
		: TFlow_( TFlow )
		{
			reset( false );
		}
		virtual ~_line_text_oflow_functions___( void )
		{
			reset();
		}
		void Init(
			flw::datum__ *Data,
			flw::size__ Size )
		{
			if ( Size > LTF__SIZE_MAX )
				ERRl();

			oflow_functions___::Init();
			
			Data_ = Data;

			Size_ = (bso::ubyte__)Size;

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




	class _line_text_oflow___
	: public flw::oflow__
	{
	private:
		_line_text_oflow_functions___ _Functions;
	public:
		void reset( bso::bool__ P = true )
		{
			_Functions.reset( P );
		}
		_line_text_oflow___( txf::text_oflow__ &TFlow )
		: _Functions( TFlow ),
		 oflow__( _Functions, NULL, 0, FLW_SIZE_MAX )
		{
			reset( false );
		}
		virtual ~_line_text_oflow___( void )
		{
			reset();
		}
		void Init(
			flw::datum__ *Data,
			flw::size__ Size )
		{
			_Functions.Init( Data, Size );
		}
		void Clear( void )
		{
			_Functions.Clear();
		}
		void CR( void )
		{
			_Functions.CR();
		}
	};


	template <int size = 79> class line_text_oflow___
	: public txf::text_oflow__
	{
	private:
		flw::datum__ Data_[size];
	protected:
		_line_text_oflow___ Flow_;
	public:
		void reset( bso::bool__ P = true )
		{
//			text_oflow__::operator()();
			Flow_.reset( P );
		}
		line_text_oflow___( txf::text_oflow__ &TFlow )
		: Flow_( TFlow ),
		  text_oflow__( Flow_ )
		{
			reset( false );
		}
		virtual ~line_text_oflow___( void )
		{
			reset();
		}
		void Init()
		{
			Flow_.Init( Data_ );
//			txf::text_oflow__::Init( Flow_ );
		}
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
