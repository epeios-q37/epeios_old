/*
  Header for the 'flx' library by Claude L. Simon (epeios@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org) 

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
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef FLX__INC
#define FLX__INC

#define FLX_NAME		"FLX"

#define	FLX_VERSION	"$Revision$"	

#define FLX_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &FLXTutor;

#if defined( XXX_DBG ) && !defined( FLX_NODBG )
#define FLX_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (epeios@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

//D FLow eXtension. Some class which extend the 'FLW' library.

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "set.h"

#ifndef FLX_BUFFER_BUFFER_SIZE
//d Size of the buffer of a 'flx::buffer_flow___'.
#define FLX_BUFFER_BUFFER_SIZE	100
#endif

#ifndef FLX_SET_BUFFER_SIZE
//d Size of the buffer of a 'flx::set_flow___'.
#define FLX_SET_BUFFER_SIZE		500
#endif

#ifndef FLX_DUMP_BUFFER_SIZE
//d Size of the buffer of a 'flx::dump_oflow___'.
#define FLX_DUMP_BUFFER_SIZE		500
#endif

namespace flx {

	//c Buffer as a standard input flow.
	class buffer_iflow___
	: public flw::iflow___
	{
	private:
		// Pointeur sur le prochain caractère à lire.
		const flw::data__ *Tampon_;
		// Nombre de caractère pouvant encore être lus.
		bso__bsize Taille_;
		// The cache.
		flw::data__ Cache_[FLX_BUFFER_BUFFER_SIZE];
	protected:
		//v Is called if there is asked for more data as availble.
		virtual void FLXUnavailable( void )
		{
			ERRf();
		}
		virtual flw::amount__ FLWGet(
			flw::amount__ Minimum,
			flw::data__ *Buffer,
			flw::amount__ Wanted )
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
			flw::data__ *Buffer,
			bso__bsize Size = BSO_BSIZE_MAX )
		{
			iflow___::Init( Cache_, sizeof( Cache_ ) );

			Tampon_ = Buffer;
			Taille_ = Size;
		}
	};



	//c Buffer as a standard ouput flow.driver
	class buffer_oflow___
	: public flw::oflow___
	{
	private:
		// Pointeur sur le prochain caractère à écrire.
		flw::data__ *Tampon_;
		// Nombre de caractères pouvant encore être écris.
		bso__bsize Taille_;
		// The cache.
		flw::data__ Cache_[FLX_BUFFER_BUFFER_SIZE];
	protected:
		virtual flw::amount__ FLWPut(
			const flw::data__ *Buffer,
			flw::amount__ Wanted,
			flw::amount__ Minimum,
			bool Synchronization )
		{
			if ( Wanted > Taille_ )
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
			flw::data__ *Buffer,
			bso__bsize Size )
		{
			oflow___::Init( Cache_, sizeof( Cache_ ) );

			Tampon_ = Buffer;
			Taille_ = Size;
		}
	};



	typedef SET( flw::data__ ) set;

	typedef SET_( flw::data__ ) set_;

	//c A set as input flow.driver.
	class set_iflow___
	: public flw::iflow___
	{ 
	protected:
		virtual flw::amount__ FLWGet(
			flw::amount__ Minimum,
			flw::data__ *Buffer,
			flw::amount__ Wanted )
		{
			if ( Wanted > ( Set_->Amount() - Position_ ) )
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
		const set_ *Set_;
		POSITION__ Position_;
		// The cache.
		flw::data__ Cache_[FLX_SET_BUFFER_SIZE];
	public:
		set_iflow___( void )
		{
			reset( false );
		}
		~set_iflow___( void )
		{
			reset( true );
		}
		void reset( bool P = true )
		{
			iflow___::reset( P );
			Set_ = NULL;
			Position_ = 0;
		}
		//f Initializing with the set buffer 'Set'.
		void Init( const set_ &Set )
		{
			iflow___::Init( Cache_, sizeof( Cache_ ) );
			Set_ = &Set;
			Position_ = 0;
		}
	};

	//c A set as output flow.driver.
	class set_oflow___
	: public flw::oflow___
	{
	protected:
		virtual flw::amount__ FLWPut(
			const flw::data__ *Buffer,
			flw::amount__ Wanted,
			flw::amount__ Minimum,
			bool Synchronization )
		{
			Set_->Add( Buffer, Wanted );

			return Wanted;
		}
	private:
		set_ *Set_;
		// The cache.
		flw::data__ Cache_[FLX_SET_BUFFER_SIZE];
	public:
		set_oflow___( void )
		{
			reset( false );
		}
		~set_oflow___( void )
		{
			oflow___::reset( true );

			reset( true );
		}
		void reset( bool P = true )
		{
			oflow___::reset( P );
			Set_ = NULL;
		}
		//f Initializing with the buffer set 'BufferSet'.
		void Init( set_ &BufferSet )
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
			const flw::data__ *,
			flw::amount__ Wanted,
			flw::amount__,
			bool )
		{
			return Wanted;
		}
	private:
			// The cache.
		flw::data__ Cache_[FLX_DUMP_BUFFER_SIZE];
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
