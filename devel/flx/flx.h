/*
	Header for the 'flx' library by Claude SIMON (csimon@epeios.org)
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

#ifndef FLX__INC
#define FLX__INC

#define FLX_NAME		"FLX"

#define	FLX_VERSION	"$Revision$"

#define FLX_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &FLXTutor;

#if defined( XXX_DBG ) && !defined( FLX_NODBG )
#define FLX_DBG
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
//D FLow eXtension 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

//D FLow eXtension. Some class which extend the 'FLW' library.

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "bch.h"

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
	using flw::datum__;

	using flw::iflow__;

	//c Buffer as a standard input flow.
	class buffer_iflow__
	: public iflow__
	{
	private:
		// Pointeur sur le prochain caractère à lire.
		const flw::datum__ *Tampon_;
		// Nombre de caractère pouvant encore être lus.
		bso::bsize__ Taille_;
		// The cache.
		flw::datum__ Cache_[FLX_BUFFER_BUFFER_SIZE];
	protected:
		//v Is called if there is asked for more data as availble.
		virtual void FLXUnavailable( void )
		{
			ERRf();
		}
		virtual flw::size__ FLWRead(
			flw::size__ Minimum,
			flw::datum__ *Buffer,
			flw::size__ Wanted )
		{
			if ( !Taille_ )
				if ( Minimum )
					FLXUnavailable();
				else
					Wanted = 0;
			else
			{
				if ( Wanted > Taille_ )
					Wanted = Taille_;

				if ( Wanted < Minimum )
					FLXUnavailable();
					
				memcpy( Buffer, Tampon_, (size_t)Wanted );

				Tampon_ += Wanted;

				Taille_ -= Wanted;

			}

			return Wanted;
		}
	public:
		void reset( bool P = true )
		{
			Taille_ = 0;
			Tampon_ = NULL;
		}
		buffer_iflow__( void )
		: iflow__( Cache_, sizeof( Cache_ ), FLW_AMOUNT_MAX )
		{
			reset( false );
		}
		~buffer_iflow__( void )
		{
			reset( true );
		}
		/*f Initialization with the buffer 'Buffer' of size 'Size'..'Size' is not
		needed if you are sure that you don't exceed the buffer size. */
		void Init(
			const flw::datum__ *Buffer,
			bso::bsize__ Size = BSO_BSIZE_MAX )
		{
			iflow__::SetAmountMax( Size );

			Tampon_ = Buffer;
			Taille_ = Size;
		}
	};

	using flw::oflow__;

	//c Buffer as a standard ouput flow.driver
	class buffer_oflow__
	: public oflow__
	{
	private:
		// Pointeur sur le prochain caractère à écrire.
		flw::datum__ *Tampon_;
		// Nombre de caractères pouvant encore être écris.
		bso::bsize__ Taille_;
		// The cache.
		flw::datum__ Cache_[FLX_BUFFER_BUFFER_SIZE];
	protected:
		virtual flw::size__ FLWWrite(
			const flw::datum__ *Buffer,
			flw::size__ Wanted,
			flw::size__ Minimum,
			bool Synchronization )
		{
			if ( Wanted > (flw::amount__)Taille_ )
				ERRu();

			memcpy( Tampon_, Buffer, (size_t)Wanted );

			Tampon_ += Wanted;
			Taille_ -= Wanted;

			return Wanted;
		}
	public:
		void reset( bool P = true )
		{
			oflow__::Synchronize();

			Tampon_ = NULL;
			Taille_ = 0;
		}
		buffer_oflow__( void )
		: oflow__( Cache_, sizeof( Cache_ ), FLW_AMOUNT_MAX )
		{
			reset( false );
		}
		~buffer_oflow__( void )
		{
			reset( true );
		}
		//f Initialization with 'Buffer' of size 'Size'.
		void Init(
			flw::datum__ *Buffer,
			bso::bsize__ Size )
		{
			reset();

			oflow__::SetAmountMax( Size );

			Tampon_ = Buffer;
			Taille_ = Size;
		}
	};

	//c A bunch as input flow.driver.
	template < typename bunch_, typename so__> class bunch_iflow__
	: public flw::iflow__
	{ 
	protected:
		virtual flw::size__ FLWRead(
			flw::size__ Minimum,
			flw::datum__ *Buffer,
			flw::size__ Wanted )
		{
			if ( Wanted > (flw::amount__)( Bunch_->Amount() - Position_ ) )
			{
				Wanted = (flw::amount__)( Bunch_->Amount() - Position_ );

				if ( Wanted < Minimum )
					ERRf();
			}

			if ( Wanted )
			{
				Bunch_->Recall( Position_, (epeios::bsize__)Wanted, (so__ *)Buffer );
				Position_ += Wanted;
			}

			return Wanted;
		}
	private:
		const bunch_ *Bunch_;
		epeios::row_t__ Position_;
		// The cache.
		flw::datum__ Cache_[FLX_SET_BUFFER_SIZE];
	public:
		bunch_iflow__( void )
		: iflow__( Cache_, sizeof( Cache_ ), FLW_AMOUNT_MAX )
		{
			reset( false );
		}
		~bunch_iflow__( void )
		{
			reset( true );
		}
		void reset( bool P = true )
		{
			Bunch_ = NULL;
			Position_ = 0;
		}
		//f Initializing with the bunch buffer 'Set'.
		void Init( const bunch_ &Bunch )
		{
			Bunch_ = &Bunch;
			Position_ = 0;
		}
	};

	#define E_STRING_IFLOW__	bunch_iflow__<str::string_, bso::char__>

	//c A bunch as output flow.driver.
	template < typename bunch_, typename so__> class bunch_oflow__
	: public flw::oflow__
	{
	protected:
		virtual flw::size__ FLWWrite(
			const flw::datum__ *Buffer,
			flw::size__ Wanted,
			flw::size__ Minimum,
			bool Synchronization )
		{
			Bunch_->Append( (const so__ *)Buffer, (epeios::bsize__)Wanted );

			return Wanted;
		}
	private:
		bunch_ *Bunch_;
		// The cache.
		flw::datum__ Cache_[FLX_SET_BUFFER_SIZE];
	public:
		bunch_oflow__( void )
		: oflow__( Cache_, sizeof( Cache_ ), FLW_AMOUNT_MAX )
		{
			reset( false );
		}
		~bunch_oflow__( void )
		{
			reset( true );
		}
		void reset( bool P = true )
		{
			oflow__::Synchronize();
			Bunch_ = NULL;
		}
		//f Initializing with the buffer bunch 'Bunch'.
		void Init( bunch_ &Bunch )
		{
			reset();

			Bunch_ = &Bunch;
		}
	};

#define E_STRING_OFLOW__	bunch_oflow__<str::string_, bso::char__>

	//c A output flow which write to nothing.
	class dump_oflow__
	: public flw::oflow__
	{
	protected:
		virtual flw::size__ FLWWrite(
			const flw::datum__ *,
			flw::size__ Wanted,
			flw::size__,
			bool )
		{
			return Wanted;
		}
	private:
			// The cache.
		flw::datum__ Cache_[FLX_DUMP_BUFFER_SIZE];
	public:
		void reset( bool P = true )
		{
			oflow__::Synchronize();
		}
		dump_oflow__( void )
		: oflow__( Cache_, sizeof( Cache_ ), FLW_AMOUNT_MAX )
		{
			reset( false );
		}
		~dump_oflow__( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{
			reset();
		}
	};
}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
