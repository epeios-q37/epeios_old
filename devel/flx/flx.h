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

	using flw::iflow___;

	//c Buffer as a standard input flow.
	class buffer_iflow___
	: public iflow___
	{
	private:
		// Pointeur sur le prochain caract�re � lire.
		const flw::datum__ *Tampon_;
		// Nombre de caract�re pouvant encore �tre lus.
		bso::bsize__ Taille_;
		// The cache.
		flw::datum__ Cache_[FLX_BUFFER_BUFFER_SIZE];
	protected:
		//v Is called if there is asked for more data as availble.
		virtual void FLXUnavailable( void )
		{
			ERRf();
		}
		virtual flw::amount__ FLWGet(
			flw::amount__ Minimum,
			flw::datum__ *Buffer,
			flw::amount__ Wanted )
		{
			if ( !Taille_ )
				if ( Minimum )
					FLXUnavailable();
				else
					Wanted = 0;
			else
			{
				if ( Wanted > (flw::amount__)Taille_ )
					Wanted = Taille_;

				if ( Wanted < Minimum )
					FLXUnavailable();
					
				memcpy( Buffer, Tampon_, Wanted );

				Tampon_ += Wanted;

				Taille_ -= Wanted;

			}

			return Wanted;
		}
	public:
		void reset( bool P = true )
		{
			iflow___::reset( P );

			Taille_ = 0;
			Tampon_ = NULL;
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
			flw::datum__ *Buffer,
			bso::bsize__ Size = BSO_BSIZE_MAX )
		{
			iflow___::Init( Cache_, sizeof( Cache_ ) );

			Tampon_ = Buffer;
			Taille_ = Size;
		}
	};

	using flw::oflow___;

	//c Buffer as a standard ouput flow.driver
	class buffer_oflow___
	: public oflow___
	{
	private:
		// Pointeur sur le prochain caract�re � �crire.
		flw::datum__ *Tampon_;
		// Nombre de caract�res pouvant encore �tre �cris.
		bso::bsize__ Taille_;
		// The cache.
		flw::datum__ Cache_[FLX_BUFFER_BUFFER_SIZE];
	protected:
		virtual flw::amount__ FLWPut(
			const flw::datum__ *Buffer,
			flw::amount__ Wanted,
			flw::amount__ Minimum,
			bool Synchronization )
		{
			if ( Wanted > (flw::amount__)Taille_ )
				ERRu();

			memcpy( Tampon_, Buffer, Wanted );

			Tampon_ += Wanted;
			Taille_ -= Wanted;

			return Wanted;
		}
	public:
		void reset( bool P = true )
		{
			oflow___::reset( P );

			Tampon_ = NULL;
			Taille_ = 0;
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
			bso::bsize__ Size )
		{
			oflow___::Init( Cache_, sizeof( Cache_ ) );

			Tampon_ = Buffer;
			Taille_ = Size;
		}
	};



	typedef bch::E_BUNCH( datum__ ) bunch;

	typedef bch::E_BUNCH_( datum__ ) bunch_;

	//c A bunch as input flow.driver.
	class bunch_iflow___
	: public flw::iflow___
	{ 
	protected:
		virtual flw::amount__ FLWGet(
			flw::amount__ Minimum,
			flw::datum__ *Buffer,
			flw::amount__ Wanted )
		{
			if ( Wanted > (flw::amount__)( Set_->Amount() - Position_ ) )
			{
				Wanted = Set_->Amount() - Position_;

				if ( Wanted < Minimum )
					ERRf();
			}

			if ( Wanted )
			{
				Set_->Read( Position_, Wanted, Buffer );
				Position_ += Wanted;
			}

			return Wanted;
		}
	private:
		const bunch_ *Set_;
		epeios::row_t__ Position_;
		// The cache.
		flw::datum__ Cache_[FLX_SET_BUFFER_SIZE];
	public:
		bunch_iflow___( void )
		{
			reset( false );
		}
		~bunch_iflow___( void )
		{
			reset( true );
		}
		void reset( bool P = true )
		{
			iflow___::reset( P );
			Set_ = NULL;
			Position_ = 0;
		}
		//f Initializing with the bunch buffer 'Set'.
		void Init( const bunch_ &Set )
		{
			iflow___::Init( Cache_, sizeof( Cache_ ) );
			Set_ = &Set;
			Position_ = 0;
		}
	};

	//c A bunch as output flow.driver.
	class bunch_oflow___
	: public flw::oflow___
	{
	protected:
		virtual flw::amount__ FLWPut(
			const flw::datum__ *Buffer,
			flw::amount__ Wanted,
			flw::amount__ Minimum,
			bool Synchronization )
		{
			Set_->Add( Buffer, Wanted );

			return Wanted;
		}
	private:
		bunch_ *Set_;
		// The cache.
		flw::datum__ Cache_[FLX_SET_BUFFER_SIZE];
	public:
		bunch_oflow___( void )
		{
			reset( false );
		}
		~bunch_oflow___( void )
		{
			oflow___::reset( true );

			reset( true );
		}
		void reset( bool P = true )
		{
			oflow___::reset( P );
			Set_ = NULL;
		}
		//f Initializing with the buffer bunch 'BufferSet'.
		void Init( bunch_ &BufferSet )
		{
			oflow___::Init( Cache_, sizeof( Cache_ ) );
			Set_ = &BufferSet;
		}
	};
	
	//c A output flow which write to nothing.
	class dump_oflow___
	: public flw::oflow___
	{
	protected:
		virtual flw::amount__ FLWPut(
			const flw::datum__ *,
			flw::amount__ Wanted,
			flw::amount__,
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
			oflow___::reset( P );
		}
		dump_oflow___( void )
		{
			reset( false );
		}
		~dump_oflow___( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{
			reset();
			
			oflow___::Init( Cache_, sizeof( Cache_ ) );
		}
	};
}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
