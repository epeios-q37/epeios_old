/*
  Header for the 'cch' library by Claude L. Simon (csimon@epeios.org)
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@epeios.org) 

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

#ifndef CCH__INC
#define CCH__INC

#define CCH_NAME		"CCH"

#define	CCH_VERSION	"$Revision$"	

#define CCH_OWNER		"the Epeios project (http://epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &CCHTutor;

#if defined( XXX_DBG ) && !defined( CCH_NODBG )
#define CCH_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (csimon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D CaCHe
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "bch.h"

namespace cch {
	//t Size of a buffer.
	typedef bso::bsize__ bsize__;

	//t Position in the buffer.
	typedef bso::bsize__ position__;

	//t Amount of data in a buffer.
	typedef bso::bsize__ amount__;

	// The core of a cache of static objects of type 'type'.
	template <class type__, typename r> class core_cache___
	{
	protected:
		// The buffer which acts as cache.
		type__ *Buffer_;
		// At true if 'Buffer' was created internally.
		bso::bool__ Internal_;
		// Size of 'Buffer'.
		bsize__ Size_;
		// Amount of data in 'Buffer'
		amount__ Amount_;
		// Position of the first data of the cache in the bunch.
		epeios::row_t__ Position_;
		bso::bool__ IsInsideBuffer_(
			r Position,
			epeios::size__ Amount )
		{
			return ( *Position >= Position_ ) && ( ( *Position + Amount ) <= ( Position_ + Amount_ ) );
		}
		void ReadFromBuffer_(
			r Position,
			epeios::size__ Amount,
			type__ *Buffer )
		{
#ifdef CCH_DBG
			if ( !IsInsideBuffer_( Position, Amount ) )
				ERRc();
#endif
			memcpy( Buffer, *Position - Position_ + Buffer_, Amount );
		}
		void WriteIntoBuffer_(
			const type__ *Buffer,
			epeios::size__ Amount,
			r Position )
		{
#ifdef CCH_DBG
			if ( !IsInsideBuffer_( Position, Amount ) )
				ERRc();
#endif
			memcpy( *Position - Position_ + Buffer_, Buffer, Amount );
		}
		void ReadDirectlyFromBunch_(
			const bch::bunch_<type__, r> &Bunch,
			r Position,
			epeios::size__ Amount,
			type__ *Buffer )
		{
#ifdef CCH_DBG
			if ( ( *Position + Amount ) > Bunch.Amount() )
				ERRu();
#endif
			Bunch.Read( Position, Amount, Buffer );
		}
		void WriteDirectlyIntoBunch_(
			const type__ *Buffer,
			epeios::size__ Amount,
			r Position,
			bch::bunch_<type__, r> &Bunch )
		{
#ifdef CCH_DBG
			if ( ( *Position + Amount ) > Bunch.Amount() )
				ERRu();
#endif
			Bunch.Write( Buffer, Amount, Position );
		}
		void FillCache_(
			const bch::bunch_<type__, r> &Bunch,
			r Position,
			epeios::size__ Amount )
		{
#ifdef CCH_DBG
			if ( Amount > Size_ )
				ERRc();
#endif
			if ( Bunch.Amount() <= Size_ ) {
				Position_ = 0;
				Amount_ = Bunch.Amount();
			} else {
				Amount_ = Size_;

				if ( Position_ == NONE ) {
					if ( ( ( Size_ - Amount ) / 2 ) > *Position )
						Position_ = 0;
					else
						Position_ = *Position - ( Size_ - Amount ) / 2;
				} else if ( *Position < Position_ ) {
					if ( ( *Position + Amount ) < Size_ )
						Position_ = 0;
					else
						Position_ = *Position + Amount - Size_;
				} else {
					Position_ = *Position;
				}

				if ( ( Position_ + Size_ ) > Bunch.Amount() )
					Position_ = Bunch.Amount() - Size_;
			}

			if ( Amount_ != 0 )
				ReadDirectlyFromBunch_( Bunch, Position_, Amount_, Buffer_ );
		}
		void DumpCache_( bch::bunch_<type__, r> &Bunch )
		{
			if ( Amount_ ) {
#ifdef CCH_DBG
				if ( ( Position_ + Amount_ ) > Bunch.Amount() )
					ERRc();
#endif
				WriteDirectlyIntoBunch_( Buffer_, Amount_, Position_, Bunch );
			}

			Amount_ = 0;
			Position_ = NONE;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( Internal_ )
					tol::Free( Buffer_ );
			}

			Buffer_ = NULL;
			Internal_ = false;
			Size_ = 0;
			Amount_ = 0;

			Position_ = NONE;

		}
		core_cache___( void )
		{
			reset( false );
		}
		~core_cache___( void )
		{
			reset( true );
		}
		//f Initialisation with 'Buffer' of size 'Size'.
		void Init(
			type__ *Buffer,
			bsize__ Size )
		{
			reset();

			Buffer_ = Buffer;
			Size_ = Size;
		}
		//f Initialisation and creation of a buffer of size 'Size'.
		void Init( bsize__ Size )
		{
			type__ *Buffer = (type__ *)malloc( Size * sizeof( type__ ) );

			if ( Buffer == NULL )
				ERRa();

			Init( Buffer, Size );

			Internal_ = true;
		}
	};

	//c A read-only cache of static object of type 't'.
	template <class type__, typename r> class read_only_cache___
	: private core_cache___<type__, r>
	{
	private:
		// The bunch which is cached.
		const bch::bunch_<type__, r> *Bunch_;
	public:
		void reset( bso::bool__ P = true )
		{
			core_cache___<type__, r>::reset( P );

			Bunch_ = NULL;
		}
		read_only_cache___( void )
		{
			reset( false );
		}
		~read_only_cache___( void )
		{
			reset( true );
		}
		//f Initialisation with bunch 'Bunch', end 'Buffer' of size 'Size'.
		void Init(
			const bch::bunch_<type__, r> &Bunch,
			type__ *Buffer,
			bsize__ Size )
		{
			core_cache___<type__, r>::Init( Buffer, Size );

			Bunch_ = &Bunch;
		}
		//f Initialisation and creation of a buffer of size 'Size'.
		void Init( 
			const bch::bunch_<type__, r> &Bunch,
			bsize__ Size )
		{
			core_cache___<type__, r>::Init( Size );

			Bunch_ = &Bunch;
		}
		//f Put 'Amount' data at 'Position' in 'Buffer'.
		void Read(
			r Position,
			epeios::size__ Amount,
			type__ *Buffer )
		{
			if ( IsInsideBuffer_( Position, Amount ) )
				ReadFromBuffer_( Position, Amount, Buffer );
			else {
				Amount_ = 0;
				Position_ = NONE;

				if ( Amount > Size_ ) {
					ReadDirectlyFromBunch_( *Bunch_, Position, Amount, Buffer );
				} else {
					FillCache_( *Bunch_, Position, Amount );
					ReadFromBuffer_( Position, Amount, Buffer );
				}
			}
		}
		//f Return data at 'Position'.
		type__ Get( r Position )
		{
			type__ Data;

			Read( Position, 1, &Data );

			return Data;
		}
		/*f Synchronize the cache with the bunch. To call if you want to do
		an operation directly on the underlying bunch. */
	};

	//c A read-write cache of static object of type 't'.
	template <class type__, typename r> class read_write_cache___
	: private core_cache___<type__, r>
	{
	private:
		// The bunch which is cached.
		bch::bunch_<type__, r> *Bunch_;
	public:
		void reset( bso::bool__ P = true )
		{
			core_cache___<type__, r>::reset( P );

			Bunch_ = NULL;
		}
		read_write_cache___( void )
		{
			reset( false );
		}
		~read_write_cache___( void )
		{
			reset( true );
		}
		//f Initialisation with bunch 'Bunch', end 'Buffer' of size 'Size'.
		void Init(
			bch::bunch_<type__, r> &Bunch,
			type__ *Buffer,
			bsize__ Size )
		{
			core_cache___<type__, r>::Init( Buffer, Size );

			Bunch_ = &Bunch;
		}
		//f Initialisation and creation of a buffer of size 'Size'.
		void Init( 
			bch::bunch_<type__, r> &Bunch,
			bsize__ Size )
		{
			core_cache___<type__, r>::Init( Size );

			Bunch_ = &Bunch;
		}
		//f Put 'Amount' data at 'Position' in 'Buffer'.
		void Read(
			r Position,
			epeios::size__ Amount,
			type__ *Buffer )
		{
			if ( IsInsideBuffer_( Position, Amount ) )
				ReadFromBuffer_( Position, Amount, Buffer );
			else {
				DumpCache_( *Bunch_ );

				if ( Amount > Size_ ) {
					ReadDirectlyFromBunch_( *Bunch_, Position, Amount, Buffer );
				} else {
					FillCache_( *Bunch_, Position, Amount );
					ReadFromBuffer_( Position, Amount, Buffer );
				}
			}
		}
		//f Return data at 'Position'.
		type__ Get( r Position )
		{
			type__ Data;

			Read( Position, 1, &Data );

			return Data;
		}
		//f Put 'Amount' data from 'Buffer' at 'Position'.
		void Write(
			const type__ *Buffer,
			epeios::size__ Amount,
			r Position )
		{
			if ( IsInsideBuffer_( Position, Amount ) )
				WriteIntoBuffer_( Buffer, Amount, Position );
			else {
				DumpCache_( *Bunch_ );

				if ( Amount > Size_ ) {
					WriteDirectlyIntoBunch_( Buffer, Amount, Position, *Bunch_ );
				} else {
					FillCache_( *Bunch_, Position, Amount );
					WriteIntoBuffer_( Buffer, Amount, Position );
				}
			}
		}
		//f Put 'Data' at 'Position'.
		void Put(
			type__ Data,
			r Position )
		{
			Write( &Data, 1, Position );
		}
	};
}

#define E_RO_CACHEt___( type, r )	read_only_cache___<type, r>

#define E_RO_CACHE___( type )	E_RO_CACHEt___( type, epeios::row__ )

#define E_RW_CACHEt___( type, r )	read_write_cache___<type, r>

#define E_RW_CACHE___( type )	E_RW_CACHEt___( type, epeios::row__ )

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
