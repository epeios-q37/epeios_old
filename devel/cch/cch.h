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
#include "ctn.h"

namespace cch {
	//t Size of a buffer.
	typedef bso::bsize__ bsize__;

	//t Position in the buffer.
	typedef bso::bsize__ position__;

	//t Amount of data in a buffer.
	typedef bso::bsize__ amount__;

	template <class type__, typename r> class const_bunch_caller__
	{
	protected:
		virtual const bch::E_BUNCHt_( type__, r ) &CCHGetBunch( void ) = 0;
	};

	template <class type__, typename r> class volatile_bunch_caller__
	{
	protected:
		virtual bch::E_BUNCHt_( type__, r ) &CCHGetBunch( void ) = 0;
	};

	// The core of a cache of static objects of type 'type'.
	template <class type__, typename r, class bunch_caller__> class const_bunch_cache___
	: public bunch_caller__
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
			memcpy( Buffer, *Position - Position_ + Buffer_, Amount * sizeof( type__ ) );
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
			memcpy( *Position - Position_ + Buffer_, Buffer, Amount * sizeof( type__ ) );
		}
		epeios::size__ BunchAmount_( void )
		{
			return CCHGetBunch().Amount();
		}
		void ReadDirectlyFromBunch_(
			r Position,
			epeios::size__ Amount,
			type__ *Buffer )
		{
			CCHGetBunch().Read( Position, Amount, Buffer );
		}
		void FillCache_(
			r Position,
			epeios::size__ Amount )
		{
			epeios::size__ BunchAmount = BunchAmount_();

#ifdef CCH_DBG
			if ( Amount > Size_ )
				ERRc();
#endif
			if ( BunchAmount <= Size_ ) {
				Position_ = 0;
				Amount_ = BunchAmount;
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

				if ( ( Position_ + Size_ ) > BunchAmount )
					Position_ = BunchAmount - Size_;
			}

			if ( Amount_ != 0 )
				ReadDirectlyFromBunch_( Position_, Amount_, Buffer_ );
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
		const_bunch_cache___( void )
		{
			reset( false );
		}
		~const_bunch_cache___( void )
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

	// The core of a cache of static objects of type 'type'.
	template <class type__, typename r, class bunch_caller__> class volatile_bunch_cache___
	: public const_bunch_cache___< type__, r, bunch_caller__>
	{
	protected:
		void Allocate_( epeios::size__ Size )
		{
			CCHGetBunch().Allocate( Size );
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
			memcpy( *Position - Position_ + Buffer_, Buffer, Amount * sizeof( type__ ) );
		}
		void WriteDirectlyIntoBunch_(
			const type__ *Buffer,
			epeios::size__ Amount,
			r Position )
		{
			CCHGetBunch().Write( Buffer, Amount, Position );
		}
		void DumpCache_( bso::bool__ Adjust )
		{
			if ( Amount_ ) {

				if ( Adjust )
					Allocate_( Position_ + Amount_ );
#ifdef CCH_DBG
				if ( ( Position_ + Amount_ ) > BunchAmount_() )
					ERRc();
#endif
				WriteDirectlyIntoBunch_( Buffer_, Amount_, Position_ );
			}

			Amount_ = 0;
			Position_ = NONE;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			const_bunch_cache___< type__, r, bunch_caller__>::reset( P );
		}
		volatile_bunch_cache___( void )
		{
			reset( false );
		}
		~volatile_bunch_cache___( void )
		{
			reset( true );
		}
	};

	//c A read-only cache of static object of type 't'.
	template <class type__, typename r> class core_read_only_cache___
	: public const_bunch_cache___<type__, r, const_bunch_caller__<type__,r> >
	{
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Synchronize();
			}

			const_bunch_cache___<type__, r, const_bunch_caller__<type__,r> >::reset( P );
		}
		core_read_only_cache___( void )
		{
			reset( false );
		}
		~core_read_only_cache___( void )
		{
			reset( true );
		}
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
					ReadDirectlyFromBunch_( Position, Amount, Buffer );
				} else {
					FillCache_( Position, Amount );
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
		void Synchronize( void )
		{
			Amount_ = 0;
			Position_ = NONE;
		}
	};

	//c A read-write cache of static object of type 't'.
	template <class type__, typename r> class core_read_write_cache___
	: public volatile_bunch_cache___<type__, r, volatile_bunch_caller__<type__, r> >
	{
	protected:
		// At true if you add data.
		bso::bool__ AddMode_;
	public:
		void reset( bso::bool__ P = true )
		{
			volatile_bunch_cache___<type__, r, volatile_bunch_caller__<type__, r> >::reset( P );

			AddMode_ = false;
		}
		core_read_write_cache___( void )
		{
			reset( false );
		}
		~core_read_write_cache___( void )
		{
			reset( true );
		}
		//f Initialisation with bunch 'Bunch', end 'Buffer' of size 'Size'.
		void Init(
			type__ *Buffer,
			bsize__ Size )
		{
			volatile_bunch_cache___<type__, r, volatile_bunch_caller__<type__, r> >::Init( Buffer, Size );
		}
		//f Initialisation and creation of a buffer of size 'Size'.
		void Init( bsize__ Size )
		{
			volatile_bunch_cache___<type__, r, volatile_bunch_caller__<type__, r> >::Init( Size );
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
				DumpCache_( AddMode_ );
				AddMode_ = false;

				if ( Amount > Size_ ) {
					ReadDirectlyFromBunch_( Position, Amount, Buffer );
				} else {
					FillCache_( Position, Amount );
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
				DumpCache_( false );

				if ( Amount > Size_ ) {
					WriteDirectlyIntoBunch_( Buffer, Amount, Position );
				} else {
					FillCache_( Position, Amount );
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
		//f Add 'Data' at 'Position'.
		r Add( type__ Data )
		{
			r Position;

			if ( AddMode_ && ( Amount_ == Size_ ) )
			{
				Synchronize();
				AddMode_ = false;
			}

			if ( !AddMode_ ) {
				Synchronize();
				Position_ = BunchAmount_();
				Amount_ = 0;
				AddMode_ = true;
			}

			WriteIntoBuffer_( &Data, 1, Position = Position_ + Amount_++ );

			return Position;
		}
		/*f Return the amount of data contained in the underlying bunch, 
		considering data added in the cache. */
		epeios::size__ Amount( void )
		{
			if ( AddMode_ )
				return Position_ + Amount_;
			else
				return BunchAmount_();
		}

		//f Synchronize the content of the cache and the content of the bunch.
		void Synchronize( void )
		{
			DumpCache_( AddMode_ );
			AddMode_ = false;
		}
	};

	//c A read-only cache of static object of type 't'.
	template <class type__, typename r> class read_only_cache___
	: public core_read_only_cache___<type__, r>
	{
	private:
		const bch::E_BUNCHt_( type__, r ) *Bunch_;
	protected:
		virtual const bch::E_BUNCHt_( type__, r ) &CCHGetBunch( void )
		{
			return *Bunch_;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			core_read_only_cache___<type__, r>::reset( P );

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
			core_read_only_cache___<type__, r>::Init( Buffer, Size );

			Bunch_ = &Bunch;
		}
		//f Initialisation and creation of a buffer of size 'Size'.
		void Init( 
			const bch::bunch_<type__, r> &Bunch,
			bsize__ Size )
		{
			core_read_only_cache___<type__, r>::Init( Size );

			Bunch_ = &Bunch;
		}
	};

	//c A read-only cache of static object of type 't'.
	template <class type__, typename r> class read_write_cache___
	: public core_read_write_cache___<type__, r>
	{
	private:
		bch::E_BUNCHt_( type__, r ) *Bunch_;
	protected:
		virtual bch::E_BUNCHt_( type__, r ) &CCHGetBunch( void )
		{
			return *Bunch_;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			core_read_write_cache___<type__, r>::reset( P );

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
			core_read_write_cache___<type__, r>::Init( Buffer, Size );

			Bunch_ = &Bunch;
		}
		//f Initialisation with bunch 'Bunch', and creation of a buffer of size 'Size'.
		void Init(
			bch::bunch_<type__, r> &Bunch,
			bsize__ Size )
		{
			core_read_write_cache___<type__, r>::Init( Size );

			Bunch_ = &Bunch;
		}
	};

		//c A read-only cache of object 't' using an item (in 'CTN4 library meaning).
	template <class type__, typename rb, typename rc> class item_read_only_cache___
	: public core_read_only_cache___<type__, rb>
	{
	private:
		ctn::E_CMITEMt( bch::E_BUNCHt_( type__, rb ), rc ) *Item_;
		rc PositionInContainer_;
	protected:
		virtual const bch::E_BUNCHt_( type__, rb ) &CCHGetBunch( void )
		{
			return Item_->operator()( PositionInContainer_ );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Synchronize();
			}

			core_read_only_cache___<type__, r>::reset( P );

			PositionInContainer_ = NONE;
		}
		item_read_only_cache___( void )
		{
			reset( false );
		}
		~item_read_only_cache___( void )
		{
			reset( true );
		}
		//f Initialisation with bunch 'Bunch', end 'Buffer' of size 'Size'.
		void Init(
			ctn::E_CMITEMt( bch::E_BUNCHt_( type__, rb ), rc ) &Item,
			rc PositionInContainer,
			bsize__ Size )
		{
			core_read_only_cache___<type__, rb>::Init( Size );

			PositionInContainer_ = PositionInContainer;
			Item_ = &Item;
		}
	};

	//c A read-only cache of static object of type 't'.
	template <class type__, typename rb, typename rc> class item_read_write_cache___
	: public core_read_write_cache___<type__, rb>
	{
	private:
		ctn::E_MITEMt( bch::E_BUNCHt_( type__, rb ), rc ) *Item_;
		rc PositionInContainer_;
	protected:
		virtual bch::E_BUNCHt_( type__, rb ) &CCHGetBunch( void )
		{
			return Item_->operator()( PositionInContainer_ );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Synchronize();
			}

			core_read_write_cache___<type__, rb>::reset( P );
		}
		item_read_write_cache___( void )
		{
			reset( false );
		}
		~item_read_write_cache___( void )
		{
			reset( true );
		}
		//f Initialisation with bunch 'Bunch', end 'Buffer' of size 'Size'.
		void Init(
			ctn::E_MITEMt( bch::E_BUNCHt_( type__, rb ), rc ) &Item,
			rc PositionInContainer,
			bsize__ Size )
		{
			core_read_write_cache___<type__, rb>::Init( Size );

			PositionInContainer_ = PositionInContainer;
			Item_ = &Item;
		}
	};

	template <class type__,
		      typename item, typename item_cache,
			  typename rb, typename rc>
			  class read_only_caches___
	{
	protected:
		bch::E_BUNCHt( item_cache *, rc ) Caches_;
		read_only_cache___<item_cache *, rc> Cache_;
		item Item_;
		bsize__ BufferSize_;
		void Fill_(
			rc First,
			rc Last )
		{
			Caches_.Fill( NULL, *Last - *First + 1, First );
		}
		void Erase_(
			rc First,
			rc Last )
		{
			epeios::row_t__ &Current = *First;

			for( ;Current <= *Last; Current++ )
				if ( Caches_( Current ) != NULL ) {
					delete Caches_( Current );
					Caches_.Write( NULL, Current );
				}
		}
		item_cache &GetCache_( rc P )
		{
#ifdef CCH_DBG
			if ( *P >= Caches_.Amount() )
				ERRu();
#endif
			if ( Cache_.Get( P ) == NULL ) {
				Cache_.Synchronize();
				item_cache *IC;
				
				if ( ( IC = new item_cache ) == NULL )
					ERRa();

				IC->Init( Item_, P, BufferSize_ );
				Caches_.Write( IC, P );
			}

			return *Cache_.Get( P );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Synchronize();
				if ( Caches_.Amount() )
					Erase_( Caches_.First(), Caches_.Last() );
			}

			Item_.reset( P );

			Caches_.reset( P );
			BufferSize_ = 0;

		}
		read_only_caches___( void )
		{
			reset( false );
		}
		~read_only_caches___( void )
		{
			reset( true );
		}
		/*f Initialization with 'Container' and, for e&ch item,
		creation of a buffer of size 'Size'. */
		void Init( 
			const ctn::E_MCONTAINERt_( bch::E_BUNCHt_( type__, rb ), rc ) &Container,
			bsize__ Size,
			bsize__ IntermediateSize )
		{
			reset();

			BufferSize_ = Size;

			Item_.Init( Container );

			Caches_.Init();

			Allocate( Container.Amount() );

			Cache_.Init( Caches_, IntermediateSize );
		}
		//f Put 'Amount' data at 'Position' in 'Buffer'.
		void Read(
			rc PositionInContainer,
			rb PositionInBunch,
			epeios::size__ Amount,
			type__ *Buffer )
		{
#ifdef CCH_DBG
			if ( *PositionInContainer >= Caches_.Amount() )
				ERRu();
#endif
			GetCache_( PositionInContainer ).Read( PositionInBunch, Amount, Buffer );

		}
		//f Return data at 'Position'.
		type__ Get(
			rc PositionInContainer,
			rb PositionInBunch )
		{
			type__ Data;

			Read( PositionInContainer, PositionInBunch, 1, &Data );

			return Data;
		}
		//f Allocate room for 'Amount' items.
		void Allocate( epeios::size__ Amount )
		{
			epeios::size__ OldAmount = Caches_.Amount();

			Cache_.Synchronize();

			if ( OldAmount > Amount ) {
				Erase_( OldAmount - 1, Amount );
				Caches_.Allocate( Amount );
			} else {
				Caches_.Allocate( Amount );
				Fill_( OldAmount, Amount - 1 );
			}
		}
		/*f Return the amount of data contained in the bunch correspondinf to 'Position', 
		considering data added in the cache. */
		epeios::size__ Amount( rc Position )
		{
			return GetCache_( Position ).Amount();
		}
		//f Synchronizing with the underlying container.
		void Synchronize( void )
		{
			item_cache *IC;

			rc R = Caches_.First();

			while( R != NONE ) {
				IC = &GetCache_( R );

				if ( IC != NULL )
					IC->Synchronize();

				R = Caches_.Next( R );
			}

			Item_.Sync();
		}
	};

	template <class type__, typename rb, typename rc> class read_write_caches___
	: public read_only_caches___<type__,ctn::E_MITEMt( bch::E_BUNCHt_( type__, rb ), rc ),item_read_write_cache___<type__, rb,rc>,rb,rc>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			read_only_caches___<type__,ctn::E_MITEMt( bch::E_BUNCHt_( type__, rb ), rc ),item_read_write_cache___<type__, rb,rc>,rb,rc>::reset( P );	
		}
		read_write_caches___( void )
		{
			reset( false );
		}
		~read_write_caches___( void )
		{
			reset( true );
		}
		/*f Initialization with 'Container' and, for e&ch item,
		creation of a buffer of size 'Size'. */
		void Init( 
			ctn::E_MCONTAINERt_( bch::E_BUNCHt_( type__, rb ), rc ) &Container,
			bsize__ Size,
			bsize__ IntermediateSize )
		{
			reset();

			BufferSize_ = Size;

			Item_.Init( Container );

			Caches_.Init();

			Allocate( Container.Amount() );

			Cache_.Init( Caches_, IntermediateSize );
		}
		//f Put 'Amount' data at 'Position' in 'Buffer'.
		void Write(
			type__ *Buffer,
			epeios::size__ Amount,
			rc PositionInContainer,
			rb PositionInBunch )
		{
			GetCache_( PositionInContainer ).Write( Buffer, Amount, PositionInBunch );

		}
		//f Return data at 'Position'.
		void Put(
			type__ Data,
			rc PositionInContainer,
			rb PositionInBunch )
		{
			Write( &Data, 1, PositionInContainer, PositionInBunch );
		}
		//f Return data at 'Position'.
		rb Add(
			type__ Data,
			rc PositionInContainer )
		{
			return GetCache_( PositionInContainer ).Add( Data );
		}
	};

}

#define E_RO_CACHEt___( type, r )	read_only_cache___<type, r>

#define E_RO_CACHE___( type )	E_RO_CACHEt___( type, epeios::row__ )

#define E_RW_CACHEt___( type, r )	read_write_cache___<type, r>

#define E_RW_CACHE___( type )	E_RW_CACHEt___( type, epeios::row__ )

#define E_RO_CACHEt___( type, r )	read_only_cache___<type, r>

#define E_RO_CACHE___( type )	E_RO_CACHEt___( type, epeios::row__ )

#define E_RW_CACHEt___( type, r )	read_write_cache___<type, r>

#define E_RW_CACHE___( type )	E_RW_CACHEt___( type, epeios::row__ )

#define E_RO_CACHESt___( type, rb, rc )	read_only_caches___<type, ctn::E_CMITEMt( bch::E_BUNCHt_( type, rb ), rc ), cch::item_read_only_cache___<type,rb,rc>, rb, rc>

#define E_RO_CACHES___( type, rb )	E_RO_CACHESt___( type, rb, epeios::row__ )

#define E_RW_CACHESt___( type, rb, rc )	read_write_caches___<type, rb, rc>

#define E_RW_CACHES___( type, rb ) E_RW_CACHESt___( type, rb, epeios::row__ )

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
