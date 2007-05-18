/*
	Header for the 'mmm2' library by Claude SIMON (csimon@epeios.org)
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

#ifndef MMM2__INC
#define MMM2__INC

#define MMM2_NAME		"MMM2"

#define	MMM2_VERSION	"$Revision$"

#define MMM2_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &MMM2Tutor;

#if defined( XXX_DBG ) && !defined( MMM2_NODBG )
#define MMM2_DBG
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
//D MultiMeMory v2 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* NOTA : Lorsque le fragment est utilisé, la taille totoale est la taille utilisable (taille des données).
Lorsque le fragment est disponible, c'est la taille total du fragment qui est stockée. */

#include "err.h"
#include "flw.h"
#include "mmm0.h"
#include "uym.h"

#define MMM2_FLAG_POSITION	7
#define MMM2_FLAG_MASK ( (unsigned char)1 << MMM2_FLAG_POSITION )
#define MMM2_L1	((unsigned char)~MMM2_FLAG_MASK )
#define MMM2_L2 ( 0xffffUL + MMM2_L1 )
#define MMM2_L3 0xffffffffUL
#define MMM2_SIZE_BUFFER_MAX_LENGTH	7
#define MMM2_LINK_SIZE	sizeof( mdr::row_t__ )
#define MMM2_ORPHAN_MAX_SIZE	6

namespace mmm {
	typedef mdr::datum__ size_buffer__[MMM2_SIZE_BUFFER_MAX_LENGTH];

	class multimemory_ {
	private:
		uym::untyped_memory_ _Memory;
	public:
		mdr::size__ _GetSizeLength( mdr::size__ Size ) const
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();
#endif
			if ( Size < MMM2_L1 )
				return 1;
			else if ( Size < MMM2_L2 )
				return 3;
			else if ( Size <= MMM2_L3 )
				return 7;
			else
				ERRc();
		}
		// Retourne la taile maximale telle que cette taille + l'indicateur de taille soit inférieur à 'MaxSize'.
		mdr::size__ _AdjustSize( mdr::size__ MaxSize ) const
		{
#ifdef MMM2_DBG
			if ( MaxSize == 0 )
				ERRc();
#endif
			mdr::size__ Size = MaxSize - 1;

			while ( ( Size + _GetSizeLength( Size ) ) > MaxSize )
				Size--;

			return Size;
		}
		void _PutSize(
			mdr::size__ Size,
			mdr::datum__ *Datum )
		{
			if ( Size < MMM2_L1 ) {
				Datum[0] = ( Datum[0] & MMM2_FLAG_MASK ) + (mdr::datum__)Size;
				return;
			}

			Datum[0] = ( Datum[0] & MMM2_FLAG_MASK ) | ~MMM2_FLAG_MASK;

			Size -= MMM2_L1;

			if ( Size < ( MMM2_L2 - MMM2_L1 ) ) {
				Datum[1] = (mdr::datum__)( Size >> 8 );
				Datum[2] = (mdr::datum__)( Size & 0xff );

				return;
			}

			Datum[1] = 0xff;
			Datum[2] = 0xff;

			Size -= MMM2_L2;

			if ( Size < ( MMM2_L3 - MMM2_L2 - MMM2_L1 ) ) {
				Datum[3] = (mdr::datum__)( Size >> 24 );
				Datum[4] = (mdr::datum__)( ( Size >> 16 ) & 0xff );
				Datum[5] = (mdr::datum__)( ( Size >> 8 ) & 0xff );
				Datum[6] = (mdr::datum__)( Size & 0xff );
			} else
				ERRc();
		}
		mdr::size__ _GetSize( const mdr::datum__ *Datum ) const
		{	
			mdr::size__ Size = 0;

			Size = Datum[0] & MMM2_FLAG_MASK;

			if ( Size == MMM2_L1 ) {
				Size += Datum[1] << 8;
				Size += Datum[2];

				if ( Size == MMM2_L2 ) {
					Size += Datum[3] << 24;
					Size += Datum[4] << 16;
					Size += Datum[5] << 8;
					Size += Datum[6];
				}
			}

			return Size;
		}
		void _Recall(
			mdr::row_t__ Position,
			mdr::size__ Amount, 
			mdr::datum__ *Buffer ) const
		{
#ifdef MMM2_DBG
			if ( ( Position + Amount ) > S_.Extent )
				ERRc();
#endif
			_Memory.Recall( Position, Amount, Buffer );
		}
		mdr::datum__ _Get( mdr::row_t__ Position ) const
		{
			mdr::datum__ Datum;

			_Recall( Position, sizeof( Datum ), &Datum );

			return Datum;
		}
		void _Allocate( mdr::size__ Size )
		{
#ifdef MMM2_DBG
			if ( Size <= S_.Extent )
				ERRc();
#endif
			_Memory.Allocate( Size );

			S_.Extent = Size;
		}
		void _Store(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount, 
			mdr::row_t__ Position )
		{
#ifdef MMM2_DBG
			if ( ( Position + Amount ) > S_.Extent )
				ERRc();
#endif
			_Memory.Store( Buffer, Amount, Position );
		}
		void _Put(
			mdr::datum__ Datum,
			mdr::row_t__ Position )
		{
			_Store( &Datum, sizeof( Datum ), Position );
		}
		bso::bool__ _IsFragmentFree( mdr::row_t__ Position ) const
		{
			return ( _Get( Position ) & ( MMM2_FLAG_MASK ) ) != 0;
		}
		bso::bool__ _IsFragmentUsed( mdr::row_t__ Position ) const
		{
			return !_IsFragmentUsed( Position );
		}
		mdr::size__ _GetRawSize( mdr::row_t__ Position ) const
		{
			size_buffer__ SizeBuffer;
			mdr::size__ Amount = sizeof( SizeBuffer );
			mdr::size__ Size = 0;

			if ( ( Position + Amount ) > S_.Extent )
				Amount = S_.Extent - Position;
#ifdef MMM2_DBG
			if ( !_IsFragmentFree( Position ) )
				ERRc();
#endif
			_Recall( Position, sizeof( SizeBuffer ), SizeBuffer );

			Size = _GetSize( SizeBuffer );

			if ( _GetSizeLength( Size ) > Amount )
				ERRm();

			return Size;
		}
		void _SetRawSize(
			mdr::size__ Size,
			mdr::row_t__ Position,
			bso::bool__ Flag )
		{
			size_buffer__ SizeBuffer;

			SizeBuffer[0] = Flag << MMM2_FLAG_POSITION;

			_PutSize( Size, SizeBuffer );

#ifdef MMM2_DBG
			if ( ( SizeBuffer[0] & MMM2_FLAG_MASK ) != ( Flag << MMM2_FLAG_POSITION ) )
				ERRc();
#endif
			_Store( SizeBuffer, _GetSizeLength( Size ), Position );
		}
		bso::bool__ _IsFlagSet( mdr::row_t__ Position ) const
		{
			return ( _Get( Position ) & MMM2_FLAG_MASK ) != 0;
		}
		bso::bool__ _IsFragmentLinked( mdr::row_t__ Position ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Position ) )
				ERRc();
#endif
			return _IsFlagSet( Position );
		}
		bso::bool__ _IsOrphan( mdr::row_t__ Position ) const
		{
			return _GetFreeFragmentSize( Position ) <= MMM2_ORPHAN_MAX_SIZE;
		}
		mdr::row_t__ _GetUsedFragmentNextFragmentPosition( mdr::row_t__ Position ) const
		{
			return Position + _GetUsedFragmentTotalSize( Position );
		}
		mdr::row_t__ _GetFreeFragmentNextFragmentPosition( mdr::row_t__ Position ) const
		{
			return Position + _GetFreeFragmentSize( Position );
		}
		void _SetAsFreeFragment(
			mdr::row_t__ Position,
			mdr::size__ Size )
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();
#endif
			if ( Size == 1 )
				_Put( 0, Position );
			else {
				_Put( MMM2_FLAG_MASK, Position );
				_SetRawSize( Size, Position + 1, false );
			}
		}
		void _SetAsUsedFragment(
			mdr::row_t__ Position,
			mdr::size__ Size,
			mdr::row_t__ Link,	// Si pas lié, est égal à 'NONE'.
			mdr::row_t__ &RemainderPosition )	// Position du reliquat, si existant, et si pas orphelin.
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();
#endif
			_SetRawSize( Size, Position, ( Link != NONE ) );

			if ( Link != NONE )
				_Store( (const mdr::datum__ *)&Link, MMM2_LINK_SIZE, _GetSizeLength( Size ) + Size );

			RemainderPosition = Position + _GetUsedFragmentTotalSize( Position );

			if ( ( RemainderPosition != S_.Extent ) && ( RemainderPosition != _GetUsedFragmentNextFragmentPosition( Position ) ) )
				_SetAsFreeFragment( RemainderPosition, Size - _GetUsedFragmentTotalSize( Position ) );
			else
				RemainderPosition = NONE;
		}
		mdr::size__ _GetUsedFragmentDataSize( mdr::row_t__ Position ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Position ) )
				ERRu();
#endif
			return _GetRawSize( Position );
		}
		mdr::size__ _GetUsedFragmentTotalSize( mdr::row_t__ Position ) const
		{
			mdr::size__ DataSize = _GetUsedFragmentDataSize( Position );

			return DataSize + _GetSizeLength( DataSize ) + ( _IsFragmentLinked( Position ) ? MMM2_LINK_SIZE : 0 );
		}
		mdr::size__ _GetFreeFragmentSize( mdr::row_t__ Position ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentFree( Position ) )
				ERRc();
#endif
			if ( _Get( Position ) & MMM2_FLAG_MASK )
				return _GetRawSize( Position + 1 );
			else
				return 1;
		}
		mdr::size__ _GetFragmentAvailableSize(
			mdr::row_t__ Position,
			bso::bool__ Linked ) const
		{
#ifdef MMM2_DBG
			if ( _IsOrphan( Position ) )
				ERRc();
#endif
			return _AdjustSize( _GetFreeFragmentSize( Position ) - ( Linked ? MMM2_LINK_SIZE : 0 ) );
		}
		bso::bool__ _IsFragmentBigEnough(
			mdr::row_t__ Position,
			mdr::size__ Size ) const
		{
			return ( _GetFragmentAvailableSize( Position, false ) >= Size );
		}
		bso::bool__ _Allocate(
			mdr::row_t__ FirstFragmentPosition,
			mdr::row_t__ SecondFragmentPosition,
			mdr::size__ Size,
			mdr::row_t__ &RemainderPosition )
		{
#ifdef MMM2_DBG
			if ( FirstFragmentPosition == NONE )
				ERRc();
#endif
			if ( _IsFragmentBigEnough( FirstFragmentPosition, Size ) ) {
				_SetAsUsedFragment( FirstFragmentPosition, Size, NONE, RemainderPosition );

				if ( ( RemainderPosition != NONE ) && ( _IsOrphan( RemainderPosition ) )
					RemainderPosition = NONE;

				return false;
			}

			mdr::size__ FirstFragmentDataSize = _GetFragmentAvailableSize( FirstFreeFragmentPosition, true );
			
			if ( ( SecondFreeFragmentPosition != NONE ) && ( _IsFragmentBigEnough( SecondFreeFragmentPosition, Size - FirstFragmentSize ) ) {
				_SetAsUsedFragment( FirstFreeFragmentPosition, FirstFragmentSize, SecondFreeFragmentPosition, RemainderPosition );

				if ( RemainderPosition != NONE ) && ( !_IsOrphan( RemainderPosition ) )
					ERRc();

				_SetAsUsedFragment( SecondFragmentPosition, Size - FirstFragmentSize, NONE, RemainderPosition );

				if ( RemainderPosition != NONE ) && ( _IsOrphan( RemainderPosition ) )
					RemainderPosition = NONE;

				return true;
			}

			SecondFragmentPosition = _AppendNewUsedFragment( Size - FirstFragmentDataSize );

			_SetAsUsedFragment( FirstFreeFragmentPosition, FirstFragmentSize, SecondFreeFragmentPosition, RemainderPosition );

			if ( RemainderPosition != NONE )
				ERRc();

			return false;
		}
		public:
			struct s 
			{
				mdr::size__ Extent;
			} &S_;
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
