/*
	Header for the 'fblbrr' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#ifndef FBLBRR__INC
#define FBLBRR__INC

#define FBLBRR_NAME		"FBLBRR"

#define	FBLBRR_VERSION	"$Revision$"

#define FBLBRR_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLBRRTutor;

#if defined( XXX_DBG ) && !defined( FBLBRR_NODBG )
#define FBLBRR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout Backend Remote Request 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "fblbrq.h"

namespace fblbrr {
	using namespace fblbrq;

	struct parameter___
	{
		void *Content;
		cast__ Cast;
		void reset( bso::bool__ = true )
		{
			Content = NULL;
			Cast = fblcst::c_Unknow;
		}
		parameter___( cast Cast = c_Unknow )
		{
			reset( false );

			this->Cast = Cast;
		}
		~parameter___( void )
		{
			reset();
		}
		void Init(
			void *Content,
			cast__ Cast )
		{
			reset();

			this->Content = Content;
			this->Cast = Cast;
		}
	};

	typedef bch::E_BUNCH_( parameter___ ) parameters_;
	E_AUTO( parameters );


	class remote_request_functions___
	: public functions__
	{
	private:
		parameters _Parameters;
		void *_Get(
			epeios::row__ Row,
			cast__ Cast )
		{
			parameter___ Parameter = _Parameters( Row );

			if ( Parameter.Cast != Cast )
				ERRb();

			return Parameter.Content;
		}
		void _CreateAll(
			flw::iflow__ &Flow,
			const casts_ &Casts );
		void _DeleteAll( void );
	protected:
		virtual const void *FBLBRQGet(
			epeios::row__ Row,
			cast__ Cast )
		{
			return _Get( Row, Cast );
		}
		virtual void *FBLBRQPut(
			epeios::row__ Row,
			cast__ Cast )
		{
			return _Get( Row, Cast );
		}
		virtual void FBLBRQPop(
			flw::iflow__ &Flow,
			const casts_ &Casts )
		{
			_Parameters.Init();
			_CreateAll( Flow, Casts );

			if ( Flow.Get() != fblcst::cEnd )
				ERRb();
		}
		virtual void FBLBRQPush(
			flw::oflow__ &Flow,
			const casts_ &Casts );
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				_DeleteAll();

			functions__::reset( P );
			_Parameters.reset( P );
		}
		remote_request_functions___( void ) 
		{
			reset( false );
		}
		virtual ~remote_request_functions___( void ) 
		{
			reset();
		}
		void Init( void )
		{
			reset();

			_Parameters.Init();
			functions__::Init();
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
