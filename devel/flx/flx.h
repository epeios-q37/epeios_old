/*
	Header for the 'flx' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (http://zeusw.org/intl/contact.html).
$_RAW_$
	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

#ifndef FLX__INC
#define FLX__INC

#define FLX_NAME		"FLX"

#define	FLX_VERSION	"$Revision$"

#define FLX_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &FLXTutor;

#if defined( XXX_DBG ) && !defined( FLX_NODBG )
#define FLX_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D FLow eXtension 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "fwf.h"
#include "flw.h"
#include "bso.h"
#include "bch.h"
#include "cpe.h"

#ifdef CPE__T_MT
#	define FLX__MT
#endif

#ifdef FLX__MT
#	include "mtx.h"
#endif

#ifndef FLX_BUFFER_BUFFER_SIZE
//d Size of the buffer of a 'flx::buffer_flow___'.
#define FLX_BUFFER_BUFFER_SIZE	100
#endif

#ifndef FLX_SET_BUFFER_SIZE
//d Size of the buffer of a 'flx::bunch_flow___'.
#define FLX_SET_BUFFER_SIZE		500
#endif

#ifndef FLX_DUMP_BUFFER_SIZE
//d Size of the buffer of a 'flx::dump_oflow___'.
#define FLX_DUMP_BUFFER_SIZE		500
#endif

namespace flx {

	//c Buffer as a standard input flow.
	class buffer_iflow_functions___
	: public fwf::iflow_functions___<>
	{
	private:
		// Pointeur sur le prochain caract�re � lire.
		const fwf::datum__ *Tampon_;
		// Nombre de caract�re pouvant encore �tre lus.
		bso::size__ Taille_;
	protected:
		//v Is called if there is asked for more data as availble.
		virtual void FLXUnavailable( void )
		{
			ERRf();
		}
		virtual fwf::size__ FWFRead(
			fwf::size__ Maximum,
			fwf::datum__ *Buffer )
		{
			if ( !Taille_ )
				FLXUnavailable();
			else
			{
				if ( Maximum > Taille_ )
					Maximum = Taille_;
		
				memcpy( Buffer, Tampon_, (size_t)Maximum );

				Tampon_ += Maximum;

				Taille_ -= Maximum;
			}

			return Maximum;
		}
		virtual void FWFDismiss( void )
		{}
	public:
		void reset( bool P = true )
		{
			iflow_functions___::reset( P );
			Taille_ = 0;
			Tampon_ = NULL;
		}
		buffer_iflow_functions___( void )
		{
			reset( false );
		}
		~buffer_iflow_functions___( void )
		{
			reset();
		}
		/*f Initialization with the buffer 'Buffer' of size 'Size'..'Size' is not
		needed if you are sure that you don't exceed the buffer size. */
		void Init(
			const fwf::datum__ *Buffer,
			fwf::thread_safety__ ThreadSafety,
			bso::size__ Size = BSO_SIZE_MAX )
		{
			reset();

			iflow_functions___::Init( ThreadSafety );

			Tampon_ = Buffer;
			Taille_ = Size;
		}
	};

	//c Buffer as a standard input flow.
	class buffer_iflow___
	: public flw::iflow__
	{
	private:
		buffer_iflow_functions___ _Functions;
	public:
		void reset( bool P = true )
		{
			_Functions.reset( P );
			iflow__::reset( P );
		}
		buffer_iflow___( void )
		{
			reset( false );
		}
		~buffer_iflow___( void )
		{
			reset( true );
		}
		/*f Initialization with the buffer 'Buffer' of size 'Size'..'Size' is not
		needed if you are sure that you don't exceed the buffer size. */
		void Init(
			const flw::datum__ *Buffer,
			bso::size__ Size = BSO_SIZE_MAX,
			flw::size__ AmountMax = FLW_SIZE_MAX )
		{
			_Functions.Init( Buffer, fwf::tsDisabled, Size );
			iflow__::Init( _Functions, AmountMax );
		}
	};

	//c Buffer as a standard ouput flow.driver
	class buffer_oflow_functions___
	: public fwf::oflow_functions___<>
	{
	private:
		// Pointeur sur le prochain caract�re � �crire.
		fwf::datum__ *Tampon_;
		// Nombre de caract�res pouvant encore �tre �cris.
		bso::size__ Taille_;
	protected:
		virtual fwf::size__ FWFWrite(
			const fwf::datum__ *Buffer,
			fwf::size__ Maximum )
		{
			if ( Maximum > Taille_ )
				Maximum = Taille_;

			memcpy( Tampon_, Buffer, Maximum );

			Tampon_ += Maximum;
			Taille_ -= Maximum;

			return Maximum;
		}
		virtual void FWFCommit( void )
		{}
	public:
		void reset( bool P = true )
		{
			oflow_functions___::reset( P );

			Tampon_ = NULL;
			Taille_ = 0;
		}
		buffer_oflow_functions___( void )
		{
			reset( false );
		}
		~buffer_oflow_functions___( void )
		{
			reset( true );
		}
		//f Initialization with 'Buffer' of size 'Size'.
		void Init(
			fwf::datum__ *Buffer,
			fwf::thread_safety__ ThreadSafety,
			bso::size__ Size )
		{
			reset();

			oflow_functions___::Init( ThreadSafety );

			Tampon_ = Buffer;
			Taille_ = Size;
		}
	};

	//c Buffer as a standard ouput flow.driver
	class buffer_oflow___
	: public flw::oflow__
	{
	private:
		buffer_oflow_functions___ _Functions;
		// The cache.
		flw::datum__ _Cache[FLX_BUFFER_BUFFER_SIZE];
	public:
		void reset( bool P = true )
		{
			_Functions.reset( P );
		}
		buffer_oflow___( void )
		{
			reset( false );
		}
		~buffer_oflow___( void )
		{
			reset( true );
		}
		//f Initialization with 'Buffer' of size 'Size'.
		void Init(
			flw::datum__ *Buffer,
			bso::size__ Size,
			flw::size__ AmountMax = FLW_SIZE_MAX )
		{
			reset();

			_Functions.Init( Buffer, fwf::tsDisabled, Size );
			oflow__::Init( _Functions, _Cache, sizeof( _Cache ), AmountMax );
		}
	};

	//c A bunch as input flow.driver.
	template < typename bunch_, typename so__, int cache_size = FLX_SET_BUFFER_SIZE> class bunch_iflow_functions___
	: public fwf::iflow_functions___<cache_size>
	{ 
	protected:
		virtual fwf::size__ FWFRead(
			fwf::size__ Maximum,
			fwf::datum__ *Buffer )
		{
			if ( (fwf::size__)Maximum > ( Bunch_->Amount() - Position_ ) )
				Maximum = ( Bunch_->Amount() - Position_ );

			if ( Maximum )
			{
				Bunch_->Recall( Position_, Maximum, (so__ *)Buffer );
				Position_ += Maximum;
			}

			return Maximum;
		}
		virtual void FWFDismiss( void )
		{}
	private:
		const bunch_ *Bunch_;
		epeios::row_t__ Position_;
	public:
		bunch_iflow_functions___( void )
		{
			reset( false );
		}
		~bunch_iflow_functions___( void )
		{
			reset( true );
		}
		void reset( bool P = true )
		{
			iflow_functions___::reset( P );
			Bunch_ = NULL;
			Position_ = 0;
		}
		//f Initializing with the bunch buffer 'Set'.
		void Init(
			const bunch_ &Bunch,
			fwf::thread_safety__ ThreadSafety,
			epeios::row_t__ Position = 0 )
		{
			reset();

			Bunch_ = &Bunch;
			Position_ = Position;

			iflow_functions___::Init( ThreadSafety );
		}
	};

	//c A bunch as input flow.driver.
	template < typename bunch_, typename so__> class bunch_iflow__
	: public flw::iflow__
	{ 
	private:
		bunch_iflow_functions___<bunch_, so__> _Functions;
	public:
		bunch_iflow__( void )
		{
			reset( false );
		}
		~bunch_iflow__( void )
		{
			reset( true );
		}
		void reset( bool P = true )
		{
			_Functions.reset( P );
		}
		//f Initializing with the bunch buffer 'Set'.
		void Init(
			const bunch_ &Bunch,
			epeios::row_t__ Position = 0,
			flw::size__ AmountMax = FLW_SIZE_MAX )
		{
			reset();

			_Functions.Init( Bunch, fwf::tsDisabled, Position );
			iflow__::Init( _Functions, AmountMax );
		}
	};

	#define E_STRING_IFLOW__	bunch_iflow__<str::string_, bso::char__>

	//c A bunch as output flow.driver.
	template < typename bunch_, typename so__> class bunch_oflow_functions___
	: public fwf::oflow_functions___<>
	{
	protected:
		virtual fwf::size__ FWFWrite(
			const fwf::datum__ *Buffer,
			fwf::size__ Maximum )
		{
			_Bunch->Append( (const so__ *)Buffer, Maximum );

			return Maximum;
		}
		virtual void FWFCommit()
		{}
	private:
		bunch_ *_Bunch;
	public:
		bunch_oflow_functions___( void )
		{
			reset( false );

			_Bunch = NULL;
		}
		~bunch_oflow_functions___( void )
		{
			reset();
		}
		void reset( bool P = true )
		{
			oflow_functions___::reset( P );

			_Bunch = NULL;
		}
		//f Initializing with the buffer bunch 'Bunch'.
		void Init(
			bunch_ &Bunch,
			fwf::thread_safety__ ThreadSafety )
		{
			reset();

			_Bunch = &Bunch;

			oflow_functions___::Init( ThreadSafety );
		}
	};
	//c A bunch as output flow.driver.
	template < typename bunch_, typename so__> class bunch_oflow___
	: public flw::oflow__
	{
	private:
		bunch_oflow_functions___<bunch_, so__> _Functions;
		flw::datum__ _Cache[FLX_SET_BUFFER_SIZE];
	public:
		bunch_oflow___( )
		{
			reset( false );
		}
		~bunch_oflow___( void )
		{
			reset( true );
		}
		void reset( bool P = true )
		{
			oflow__::reset( P );
			_Functions.reset( P );
		}
		//f Initializing with the buffer bunch 'Bunch'.
		void Init(
			bunch_ &Bunch,
			flw::size__ AmountMax = FLW_SIZE_MAX )
		{
			reset();

			_Functions.Init( Bunch, fwf::tsDisabled );
			oflow__::Init( _Functions, _Cache, sizeof( _Cache ), AmountMax );
		}
	};

	#define E_STRING_OFLOW___	bunch_oflow___<str::string_, bso::char__>

	//c A output flow which write to nothing.
	class dump_oflow_functions___
	: public fwf::oflow_functions___<>
	{
	protected:
		virtual fwf::size__ FWFWrite(
			const fwf::datum__ *,
			fwf::size__ Maximum )
		{
			return Maximum;
		}
		virtual void FWFCommit()
		{}
	public:
		void Init( fwf::thread_safety__ ThreadSafety )
		{
			oflow_functions___::Init( ThreadSafety );
		}

	};	

	extern dump_oflow_functions___ dump;

	//c A output flow which write to nothing.
	class dump_oflow__
	: public flw::oflow__
	{
	private:
		dump_oflow_functions___ _Functions;
			// The cache.
		flw::datum__ _Cache[FLX_DUMP_BUFFER_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			oflow__::reset( P );
			_Functions.reset( P );
		}
		dump_oflow__( void )
		{
			reset( false );
		}
		~dump_oflow__( void )
		{
			reset();
		}
		void Init( flw::size__ AmountMax = FLW_SIZE_MAX )
		{
			_Functions.Init( fwf::tsDisabled );
			oflow__::Init( _Functions, _Cache, sizeof( _Cache ), AmountMax );
		}
	};

#ifdef FLX__MT

	// Permet de lire � partir d'un 'iflow' ce qui est �crit dans un 'oflow'.
	class mediator_ioflow_functions___
	: public fwf::ioflow_functions___<>
	{
	private:
		mtx::mutex_handler__ 
			_Read,
			_Write;
		const fwf::datum__ *_Buffer;
		fwf::size__ _Size;
		fwf::size__ _Red;
	protected:
		virtual fwf::size__ FWFWrite(
			const fwf::datum__ *Buffer,
			fwf::size__ Maximum )
		{
			mtx::Lock( _Write );
			mtx::Unlock( _Write );

			_Buffer = Buffer;
			_Size = Maximum;
			_Red = 0;

			mtx::Unlock( _Read );
			mtx::Lock( _Write );

			return _Red;
		}
		virtual void FWFCommit( void )
		{}
		virtual fwf::size__ FWFRead(
			fwf::size__ Maximum,
			fwf::datum__ *Buffer )
		{

			mtx::Lock( _Read );
			mtx::Unlock( _Read );

			if ( Maximum > _Size )
				Maximum = _Size;

			memcpy( Buffer, _Buffer, Maximum );

			_Red = Maximum;

			mtx::Unlock( _Write );
			mtx::Lock( _Read );

			return Maximum;
		}
		virtual void FWFDismiss( void )
		{}
	public:
		void reset( bso::bool__ P = true )
		{

			if ( P ) {
				if ( _Read != MTX_INVALID_HANDLER )
					mtx::Delete( _Read, true );

				if ( _Write != MTX_INVALID_HANDLER )
					mtx::Delete( _Write,true );
			}

			fwf::ioflow_functions___<>::reset( P );

			_Buffer = NULL;
			_Red = _Size = NULL;
			_Read = _Write = MTX_INVALID_HANDLER;
		}
		mediator_ioflow_functions___( void )
		{
			reset( false );
		}
		~mediator_ioflow_functions___( void )
		{
			reset();
		}
		void Init( fwf::thread_safety__ ThreadSafety )
		{
			if ( _Read != MTX_INVALID_HANDLER )
				mtx::Delete( _Read, true );

			if ( _Write != MTX_INVALID_HANDLER )
				mtx::Delete( _Write, true );

			fwf::ioflow_functions___<>::Init( ThreadSafety );

			_Size = _Red = 0;

			_Buffer = NULL;

			_Read = mtx::Create( mtx::mFree );
			_Write = mtx::Create( mtx::mFree );

			mtx::Lock( _Read );
		}
	};

	template <int OutCacheSize = FLW__OUTPUT_CACHE_SIZE> class mediator_ioflow___
		: public flw::standalone_ioflow__<OutCacheSize>
	{
	private:
		mediator_ioflow_functions___ _Functions;
	public:
		void Init( void )
		{
			_Functions.Init( fwf::tsDisabled );
			flw::standalone_ioflow__<OutCacheSize>Init( Functions );
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
