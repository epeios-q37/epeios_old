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
#include "epeios.h"
#include "mmm0.h"
#include "uym.h"
#include "bch.h"
#include "stk.h"

// Ce drapeau indique si la taille est stocké dans le fragment. Ne concerne que les fragments libres.
#define MMM2_SIZE_FLAG_POSITION	7
#define MMM2_SIZE_FLAG_MASK ( 1 << MMM2_SIZE_FLAG_POSITION )


// Ce drapeau indique si le fragment est lié ou non (donnée réparties sur deux fragments ou non). Ne concerne que les fragments utilisés.
#define MMM2_LINK_FLAG_POSITION	7
#define MMM2_LINK_FLAG_MASK ( 1 << MMM2_LINK_FLAG_POSITION )

// Ce drapeau indique si le fragment précédent est libre ou non. Ne concerne que les fragments utilisés.
#define MMM2_FREE_FLAG_POSITION	6
#define MMM2_FREE_FLAG_MASK ( 1 << MMM2_FREE_FLAG_POSITION )

#define MMM2_FLAGS_MASK	( MMM2_LINK_FLAG_MASK | MMM2_FREE_FLAG_MASK )

#define MMM2_L1	((unsigned char)~MMM2_FLAGS_MASK )
#define MMM2_L2 ( 0xffffUL + MMM2_L1 )
#define MMM2_L3 0xffffffffUL
#define MMM2_SIZE_BUFFER_MAX_LENGTH	7
#define MMM2_LINK_SIZE	sizeof( mmm::row__ )
#define MMM2_ORPHAN_MAX_SIZE	5
#define MMM2_HEADER_MAX_LENGTH	( MMM2_SIZE_BUFFER_MAX_LENGTH + 1  )	// + 1 pour la gestion des fragments libres.
#define MMM2_FREE_FRAGMENT_TAIL_MAX_SIZE	( MMM2_LINK_SIZE + 1 )

namespace mmm {
	E_ROW( row__ );

	class multimemory_
	{
	private:
		multimemory_driver__ _MultimemoryDriver;
		bso::ubyte__ _GetSizeLength( mdr::size__ Size ) const
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

			return 0;	// Pour éviter un 'warning'.
		}
		// Retourne la taile maximale telle que cette taille + l'indicateur de taille soit inférieur à 'MaxSize'.
		mdr::size__ _AdjustSize( mdr::size__ MaxSize ) const
		{
#ifdef MMM2_DBG
			if ( MaxSize <= 1 )
				ERRc();
#endif
			mdr::size__ Size = MaxSize - 1;	// au moins un octet pour stockerla taille.

			while ( ( Size + _GetSizeLength( Size ) ) > MaxSize )
				Size--;

			return Size;
		}
		void _PutSize(
			mdr::size__ Size,
			mdr::datum__ *Datum )
		{
			if ( Size < MMM2_L1 ) {
				Datum[0] = ( Datum[0] & MMM2_FLAGS_MASK ) + (mdr::datum__)Size;
				return;
			}

			Datum[0] = ( Datum[0] & MMM2_FLAGS_MASK ) | ~MMM2_FLAGS_MASK;

			Size -= MMM2_L1;

			if ( Size < ( MMM2_L2 - MMM2_L1 ) ) {
				Datum[1] = (mdr::datum__)( Size >> 8 );
				Datum[2] = (mdr::datum__)( Size & 0xff );

				return;
			}

			Datum[1] = 0xff;
			Datum[2] = 0xff;

			Size -= 0xffff;

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

			Size = Datum[0] & ~MMM2_FLAGS_MASK;

			if ( Size == MMM2_L1 ) {
				Size += Datum[1] << 8;
				Size += Datum[2];

				if ( Size == MMM2_L2 ) {
					Size += ( Datum[3] << 24 )
						    + ( Datum[4] << 16 )
							+ ( Datum[5] << 8 )
							+ Datum[6];
				}
			}

			return Size;
		}
		void _GetHeader(
			row__ Position,
			mdr::datum__ *Header,
			mdr::size__ Amount = MMM2_HEADER_MAX_LENGTH ) const
		{
			Memory.Recall( *Position, ( *Position + Amount ) > S_.Extent ? S_.Extent - *Position : Amount, Header );
		}
		bso::bool__ _IsFragmentUsed( const mdr::datum__ *Header ) const
		{
			return ( Header[0] & ~MMM2_FLAGS_MASK ) != 0;
		}
		bso::bool__ _IsFragmentFree( const mdr::datum__ *Header ) const
		{
			return ( Header[0] & ~MMM2_FLAGS_MASK ) == 0;
		}
		mdr::size__ _GetRawSize( const mdr::datum__ *SizeBuffer ) const
		{
			return _GetSize( SizeBuffer );
		}
		void _SetRawSize(
			mdr::size__ Size,
			row__ Position,
			bso::bool__ LinkFlag,
			bso::bool__ FreeFlag )
		{
			mdr::datum__ SizeBuffer[MMM2_SIZE_BUFFER_MAX_LENGTH];

			SizeBuffer[0] = ( LinkFlag << MMM2_LINK_FLAG_POSITION ) | ( FreeFlag << MMM2_FREE_FLAG_POSITION );

			_PutSize( Size, SizeBuffer );

			Memory.Store( SizeBuffer, _GetSizeLength( Size ), *Position );
		}
		bso::bool__ _IsFreeFlagSet( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return ( Header[0] & MMM2_FREE_FLAG_MASK ) != 0;
		}
		void _SetFreeFlag(
			row__ Position,
			const mdr::datum__ *Header,
			bso::bool__ Value )
		{
#ifdef MMM2_DBG
			if ( _IsFreeFlagSet( Header ) )
				ERRc();
#endif
			Memory.Put( ( Header[0] & ~MMM2_FREE_FLAG_MASK ) | ( Value << MMM2_FREE_FLAG_POSITION ), *Position );
		}
		bso::bool__ _IsLinkFlagSet( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return ( Header[0] & MMM2_LINK_FLAG_MASK ) != 0;
		}
		bso::bool__ _IsSizeFlagSet( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentFree( Header ) )
				ERRc();
#endif
			return ( Header[0] & MMM2_SIZE_FLAG_MASK ) != 0;
		}
		bso::bool__ _IsUsedFragmentLinked( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return _IsLinkFlagSet( Header );
		}
		mdr::size__ _GetFreeFragmentSize( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentFree( Header ) )
				ERRc();
#endif
			if ( Header[0] == 0 )
				return 1;
			else
				return _GetRawSize( Header + 1 );
		}
		mdr::size__ _GetFreeFragmentSize_( row__ Position ) const
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			return _GetFreeFragmentSize( Header );
		}
		mdr::size__ _GetFreeFragmentHeaderLength( const mdr::datum__ *Header ) const
		{
			mdr::size__ Size = _GetFreeFragmentSize( Header );

			if ( Size == 1 )
				return 1;
			else
				return _GetSizeLength( Size ) + 1;
		}
		bso::bool__ _IsOrphan( const mdr::datum__ *Header ) const
		{
			return _GetFreeFragmentSize( Header ) <= MMM2_ORPHAN_MAX_SIZE;
		}
		row__ _GetUsedFragmentLink(
			row__ Position,
			const mdr::datum__ *Header ) const
		{
			row__ Link = NONE;
#ifdef MMM2_DBG
			if ( !_IsUsedFragmentLinked( Header ) )
				ERRc();
#endif
			Memory.Recall( *Position + _GetUsedFragmentTotalSize( Header ) - MMM2_LINK_SIZE, MMM2_LINK_SIZE, (mdr::datum__ *)&Link );

			return Link;
		}
		row__ _GetUsedFragmentNextFragmentPosition(
			row__ Position,
			const mdr::datum__ *Header ) const
		{
			return *Position + _GetUsedFragmentTotalSize( Header );
		}
		row__ _GetFreeFragmentNextFragmentPosition(
			row__ Position,
			const mdr::datum__ *Header ) const
		{
			return *Position + _GetFreeFragmentSize( Header );
		}
		void _MarkAsFreeFragment(
			row__ Position,
			mdr::size__ Size )
		{
			if ( Size == 0 )
				ERRc();
			else if ( Size == 1 )
				Memory.Put( 0, *Position );
			else {
				Memory.Put( MMM2_SIZE_FLAG_MASK, *Position );
				_SetRawSize( Size, *Position + 1, false, false );
				if ( Size < 7 )
					Memory.Put( (mdr::datum__)Size, *Position + Size - 1 );
				else {
					Memory.Put( 0xff, *Position + Size - 1 );
					Memory.Store( (const mdr::datum__ *)&Position, sizeof( Position ), *Position + Size - sizeof( Position ) - 2 );
				}
			}
		}
		mdr::size__ _GetUsedFragmentDataSize( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return _GetRawSize( Header );
		}
		mdr::size__ _GetUsedFragmentTotalSize( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			mdr::size__ DataSize = _GetUsedFragmentDataSize( Header );

			return DataSize + _GetSizeLength( DataSize ) + ( _IsUsedFragmentLinked( Header ) ? MMM2_LINK_SIZE : 0 );
		}
		row__ _GetUsedFragmentPreviousFreeFragmentPosition(
			row__ Position,
			const mdr::datum__ *Header,
			const mdr::datum__ *FreeFragmentTail ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFreeFlagSet( Header ) )
				ERRc();
#endif
			const mdr::datum__ *Pointer = FreeFragmentTail + MMM2_FREE_FRAGMENT_TAIL_MAX_SIZE;

			if ( *Pointer == 0 )
				return *Position - 1;
			else if ( *Pointer != 0xff )
				return *Position - *Pointer;
			else
				return *(row__ *)( Pointer - 1);
		}
		void _GetUsedFragmentPreviousFreeFragmentTail(
			row__ Position,
			const mdr::datum__ *Header,
			mdr::datum__ *Tail ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFreeFlagSet( Header ) )
				ERRc();

			if ( Position == 0 )
				ERRc();
#endif

			mdr::size__ Amount = MMM2_FREE_FRAGMENT_TAIL_MAX_SIZE;

			if ( *Position < Amount )
				Amount = *Position;

			Memory.Recall( *Position - Amount, Amount, Tail + MMM2_FREE_FRAGMENT_TAIL_MAX_SIZE - Amount );
		}

		row__ _GetUsedFragmentPreviousFreeFragmentPosition(
			row__ Position,
			const mdr::datum__ *Header ) const
		{
			mdr::datum__ FreeFragmentTail[MMM2_FREE_FRAGMENT_TAIL_MAX_SIZE];

			_GetUsedFragmentPreviousFreeFragmentTail( Position, Header, FreeFragmentTail );

			return _GetUsedFragmentPreviousFreeFragmentPosition( Position, Header, FreeFragmentTail );
		}
		void _UpdateMarkers( row__ Position ) {
			if ( S_.BiggestFree == Position )
				if ( S_.SeekedIsFree ) {
#ifdef MMM2_DBG
					if ( S_.Seeked == NONE )
						ERRc();
#endif
					S_.BiggestFree = S_.Seeked;
					S_.Seeked = NONE;
					S_.SeekedIsFree = false;
				} else
					S_.BiggestFree = NONE;
			else if ( ( S_.SeekedIsFree ) && ( S_.Seeked == Position ) ) {
				S_.Seeked = NONE;
				S_.SeekedIsFree = false;
			}
		}
		row__ _SetAsFreeFragment(
			row__ Position,	// 'Position' ne pointe pas nécessairement sur un 'header'.
			mdr::size__ Size )	// Retourne la position du fragment libre ainsi crée.
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();
#endif
			if ( ( *Position + Size ) != S_.Extent ) {
				mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( *Position + Size, Header );

				if ( _IsFragmentFree( Header ) ) {
					_UpdateMarkers( *Position + Size );
					Size = +_GetFreeFragmentSize( Header );
				} else if ( _IsFragmentUsed( Header ) ) {
					if ( !_IsFreeFlagSet( Header ) )
						_SetFreeFlag( *Position + Size, Header, true );
				} else
					ERRc();
			}

			if ( ( *Position + Size ) == S_.Extent )
				S_.LastFragmentIsUsed = false;

			_MarkAsFreeFragment( Position, Size );

			return Position;
		}
		row__ _FreeUsedFragment(
			row__ Position,
			const mdr::datum__ *Header,
			mdr::size__ Size )	// Retourne la position du fragment libre ainsi crée.Peut être différent de 'Position' (fusion avec le fragment libre précédent).
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();
#endif

			if ( _IsFreeFlagSet( Header ) ) {
				mdr::datum__ TempHeader[MMM2_HEADER_MAX_LENGTH];
				Position = _GetUsedFragmentPreviousFreeFragmentPosition( Position, Header );
				_GetHeader( Position, TempHeader );

				Size += _GetFreeFragmentSize( TempHeader );
			}

			_SetAsFreeFragment( Position, Size );

			return Position;
		}
		mdr::size__ _GuessTotalSizeForUsedFragment(
			mdr::size__ DataSize,
			bso::bool__ WithLink ) const
		{
			return _GetSizeLength( DataSize ) + DataSize + ( WithLink ? MMM2_LINK_SIZE : 0 );
		}
		mdr::size__ _GuessTotalSizeForUsedFragment(
			mdr::size__ DataSize,
			row__ Link ) const
		{
			return _GuessTotalSizeForUsedFragment( DataSize, Link != NONE );
		}
		void _MarkAsUsedFragment_(
			row__ Position,
			mdr::size__ DataSize,
			row__ Link,	// Si pas lié, est égal à 'NONE'.
			bso::bool__ FreeFlag )
		{
#ifdef MMM2_DBG
			if ( DataSize == 0 )
				ERRc();
#endif
			_SetRawSize( DataSize, Position, ( Link != NONE ), FreeFlag );

			if ( Link != NONE )
				Memory.Store( (const mdr::datum__ *)&Link, MMM2_LINK_SIZE, *Position + _GetSizeLength( DataSize ) + DataSize );
		}
		row__ _ConvertFreeToUsedFragment_(
			row__ Position,
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			row__ Link )
		{
			mdr::size__ Size = _GuessTotalSizeForUsedFragment( DataSize, Link );
			row__ RemainderPosition = *Position + Size;
			mdr::size__ FragmentCurrentSize = _GetFreeFragmentSize( Header );
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();

			if ( *RemainderPosition > S_.Extent )
				ERRc();
#endif
			_UpdateMarkers( Position );

			if ( ( RemainderPosition != S_.Extent ) && ( Size != FragmentCurrentSize ) )
				RemainderPosition =_SetAsFreeFragment( RemainderPosition, FragmentCurrentSize - Size );
			else
				RemainderPosition = NONE;

			_MarkAsUsedFragment_( Position, DataSize, Link, false );

			return RemainderPosition;
		}
		mdr::size__ _GetFreeFragmentAvailableSize(
			const mdr::datum__ *Header,
			bso::bool__ Linked ) const
		{
#ifdef MMM2_DBG
			if ( _IsOrphan( Header ) )
				ERRc();
#endif
			return _AdjustSize( _GetFreeFragmentSize( Header ) - ( Linked ? MMM2_LINK_SIZE : 0 ) );
		}
		bso::bool__ _IsFreeFragmentBigEnough(
			const mdr::datum__ *Header,
			mdr::size__ DataSize ) const
		{
			return ( _GetFreeFragmentAvailableSize( Header, false ) >= DataSize );
		}
		bso::bool__ _IsFreeFragmentBigEnough_(
			row__ Position,
			mdr::size__ DataSize ) const
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			return _IsFreeFragmentBigEnough( Header, DataSize );
		}
		row__ _AppendNewUnlinkedFragment( mdr::size__ DataSize )
		{
			row__ Row = S_.Extent;

			S_.Extent += _GetSizeLength( DataSize ) + DataSize;

			Memory.Allocate( S_.Extent );

			_SetRawSize( DataSize, Row, false, !S_.LastFragmentIsUsed );

			S_.LastFragmentIsUsed = true;

			return Row;
		}
		bso::bool__ _Allocate(
			row__ FirstFragmentPosition,
			row__ SecondFragmentPosition,
			mdr::size__ DataSize,
			row__ &FirstRemainderPosition,
			row__ &SecondRemainderPosition,
			bso::ubyte__ &Addendum )
		{
#ifdef MMM2_DBG
			if ( FirstFragmentPosition == NONE )
				ERRc();
#endif
			mdr::datum__ FirstFragmentHeader[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( FirstFragmentPosition, FirstFragmentHeader );

			if ( _IsFreeFragmentBigEnough( FirstFragmentHeader, DataSize ) ) {
				FirstRemainderPosition = _ConvertFreeToUsedFragment_( FirstFragmentPosition, FirstFragmentHeader, DataSize, NONE );

				Addendum = _GetSizeLength( DataSize );

				return false;
			}

			Addendum = 0;

			mdr::size__ FirstFragmentDataSize = _GetFreeFragmentAvailableSize( FirstFragmentHeader, true );

			if ( SecondFragmentPosition != NONE ) {
				mdr::datum__ SecondFragmentHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( SecondFragmentPosition, SecondFragmentHeader );

				if ( _IsFreeFragmentBigEnough( SecondFragmentHeader, DataSize - FirstFragmentDataSize ) ) {
					FirstRemainderPosition = _ConvertFreeToUsedFragment_( FirstFragmentPosition, FirstFragmentHeader, FirstFragmentDataSize, SecondFragmentPosition );

					SecondRemainderPosition = _ConvertFreeToUsedFragment_( SecondFragmentPosition, SecondFragmentHeader, DataSize - FirstFragmentDataSize, NONE );

					return true;
				}
			}

			SecondFragmentPosition = _AppendNewUnlinkedFragment( DataSize - FirstFragmentDataSize );

			FirstRemainderPosition = _ConvertFreeToUsedFragment_( FirstFragmentPosition, FirstFragmentHeader, FirstFragmentDataSize, SecondFragmentPosition );

			return false;
		}
		bso::bool__ _TestAndReportFreeFragment(
			row__ Row,
			const mdr::datum__ *Header )
		{
			if ( ( Row != NONE ) && !_IsOrphan( Header ) ) {
				_ReportFreeFragment( Row );
				return true;
			} else
				return false;
		}
		bso::bool__ _TestAndReportFreeFragment_( row__ Row )
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			if ( Row != NONE )
				_GetHeader( Row, Header );

			return _TestAndReportFreeFragment( Row, Header );
		}
		row__ _GetNewUnlinkedFragment( mdr::size__ DataSize )
		{
			row__ Row = NONE;

			if ( _IsFreeFragmentAvailable() ) {

				mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

				Row = _RetrieveFreeFragment();

				_GetHeader( Row, Header );
			
				if ( _IsFreeFragmentBigEnough( Header, DataSize ) ) {
					row__ RemainderPosition = NONE;

					RemainderPosition = _ConvertFreeToUsedFragment_( Row, Header, DataSize, NONE );

					_TestAndReportFreeFragment_( RemainderPosition );
				} else {
					_TestAndReportFreeFragment_( Row );
					Row = _AppendNewUnlinkedFragment( DataSize );
				}
			} else
				Row = _AppendNewUnlinkedFragment( DataSize );

			return Row;
		}
		descriptor__ _Allocate(
			mdr::size__ Size,
			bso::ubyte__ &Addendum )
		{	
			if ( _IsFreeFragmentAvailable() ) {
				row__ Free1 = NONE, Free2 = NONE, Remainder1 = NONE, Remainder2 = NONE;

				Free1 = _RetrieveFreeFragment();

				if ( _IsFreeFragmentAvailable()  )
					Free2 = _RetrieveFreeFragment();

				if ( !_Allocate( Free1, Free2, Size, Remainder1, Remainder2, Addendum ) )
					_TestAndReportFreeFragment_( Free2 );

				_TestAndReportFreeFragment_( Remainder1 );

				_TestAndReportFreeFragment_( Remainder2 );

				return *Free1;
			} else {
				Addendum = _GetSizeLength( Size );

				return *_AppendNewUnlinkedFragment( Size );
			}
		}
		void _Free( descriptor__ Descriptor )
		{
			if ( Descriptor == MMM_UNDEFINED_DESCRIPTOR )
				return;

			row__ Remainder = NONE;
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Descriptor, Header );

			if ( _IsUsedFragmentLinked( Header ) ) {
				row__ Link = _GetUsedFragmentLink( Descriptor, Header );
				mdr::datum__ LinkHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( Link, LinkHeader );

				Remainder = _SetAsFreeFragment( Link, _GetUsedFragmentTotalSize( LinkHeader ) );
				_TestAndReportFreeFragment_( Remainder );
			}

			Remainder = _SetAsFreeFragment( Descriptor, _GetUsedFragmentTotalSize( Header ) );

			_TestAndReportFreeFragment_( Remainder );
		}
		row__ _ResizeUsedFragment(
			row__ Position,
			const mdr::datum__ *Header,
			mdr::size__ Size,
			row__ Link )
		{
#ifdef MMM2_DBG
			if ( Size > _GetUsedFragmentTotalSize( Header ) )
				ERRc();
#endif
			row__ Remainder = _SetAsFreeFragment( *Position + _GetSizeLength( Size ) + Size, _GetUsedFragmentTotalSize( Header ) - _GetSizeLength( Size ) - Size );

			_MarkAsUsedFragment_( Position, Size, Link, _IsFreeFlagSet( Header ) );

			return Remainder;
		}
		row__ _ReallocateLesser(
			row__ Descriptor,
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			bso::ubyte__ &Addendum )
		{
			row__ Remainder = NONE;

			if ( _IsUsedFragmentLinked( Header ) ) {
				row__ Link = _GetUsedFragmentLink( Descriptor, Header );

				mdr::datum__ LinkHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( Link, LinkHeader );

				if ( DataSize > _GetUsedFragmentDataSize( Header ) ) {
					Remainder = _ResizeUsedFragment( Link, LinkHeader, DataSize - _GetUsedFragmentDataSize( Header ), NONE );

					_TestAndReportFreeFragment_( Remainder );

					Addendum = 0;
				} else {
					Remainder = _SetAsFreeFragment( Link, _GetUsedFragmentTotalSize( LinkHeader ) );

					_TestAndReportFreeFragment_( Remainder );

					Remainder = _ResizeUsedFragment( Descriptor, Header, DataSize, NONE );

					_TestAndReportFreeFragment_( Remainder );

					Addendum = _GetSizeLength( DataSize );
				}
			} else {
				Remainder = _ResizeUsedFragment( Descriptor, Header, DataSize, NONE );

				Addendum = _GetSizeLength( DataSize );

				_TestAndReportFreeFragment_( Remainder );
			}

			return Descriptor;
		}
		mdr::size__ _Size(
			descriptor__ Descriptor,
			const mdr::datum__ *Header ) const
		{
			mdr::size__ Size = _GetUsedFragmentDataSize( Header );

			if ( _IsUsedFragmentLinked( Header ) ) {
				mdr::datum__ LinkHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( _GetUsedFragmentLink( Descriptor, Header ), LinkHeader );

				Size += _GetUsedFragmentDataSize( LinkHeader );

#ifdef MMM2_DBG
				if ( _IsUsedFragmentLinked( LinkHeader ) )
					ERRc();
#endif
			}

			return Size;
		}
		void _MoveHead(
			row__ SourceFirstFragmentRow,
			const mdr::size__ SourceFirstFragmentSize,
			row__ SourceSecondFragmentRow,
			const mdr::size__ SourceSecondFragmentSize,
			row__ TargetFirstFragmentRow,
			const mdr::size__ TargetFirstFragmentSize,
			row__ TargetSecondFragmentRow,
			const mdr::size__ TargetSecondFragmentSize  )
			/* ATTENTION : Les différents 'row' pointent directement sur les données des fragment respectifs (aprés l'indicateur de taille),
			et non pas sur le début des fragments respectis. */
		{
			Memory.Store_(
				Memory, SourceFirstFragmentSize > TargetFirstFragmentSize ? TargetFirstFragmentSize : SourceFirstFragmentSize,
				*TargetFirstFragmentRow, *SourceFirstFragmentRow );

			if ( ( SourceSecondFragmentSize != 0 ) && ( ( SourceFirstFragmentSize + SourceSecondFragmentSize ) < TargetFirstFragmentSize ) )
				Memory.Store_(
					Memory, SourceSecondFragmentSize,
					*TargetFirstFragmentRow + SourceFirstFragmentSize,
					*SourceSecondFragmentRow );
		}
		void _MoveMiddle(
			row__ SourceFirstFragmentRow,
			const mdr::size__ SourceFirstFragmentSize,
			row__ SourceSecondFragmentRow,
			const mdr::size__ SourceSecondFragmentSize,
			row__ TargetFirstFragmentRow,
			const mdr::size__ TargetFirstFragmentSize,
			row__ TargetSecondFragmentRow,
			const mdr::size__ TargetSecondFragmentSize  )
			/* ATTENTION : Les différents 'row' pointent directement sur les données des fragment respectifs (aprés l'indicateur de taille),
			et non pas sur le début des fragments respectis. */
		{
			if ( SourceFirstFragmentSize > TargetFirstFragmentSize )
				Memory.Store_(
					Memory, SourceFirstFragmentSize - TargetFirstFragmentSize,
					*TargetSecondFragmentRow, *SourceFirstFragmentRow + TargetFirstFragmentSize );
			else
				Memory.Store_(
					Memory, TargetFirstFragmentSize - SourceFirstFragmentSize, 
					*TargetFirstFragmentRow + SourceFirstFragmentSize,
					*SourceSecondFragmentRow );
		}
		void _MoveTail(
			row__ SourceFirstFragmentRow,
			const mdr::size__ SourceFirstFragmentSize,
			row__ SourceSecondFragmentRow,
			const mdr::size__ SourceSecondFragmentSize,
			row__ TargetFirstFragmentRow,
			const mdr::size__ TargetFirstFragmentSize,
			row__ TargetSecondFragmentRow,
			const mdr::size__ TargetSecondFragmentSize  )
			/* ATTENTION : Les différents 'row' pointent directement sur les données des fragment respectifs (aprés l'indicateur de taille),
			et non pas sur le début des fragments respectis. */
		{
			if ( SourceFirstFragmentSize > TargetFirstFragmentSize )
				Memory.Store_(
					Memory, SourceSecondFragmentSize,
					*TargetSecondFragmentRow + SourceFirstFragmentSize - TargetFirstFragmentSize,
					*SourceSecondFragmentRow );
			else
				Memory.Store_(
					Memory, SourceSecondFragmentSize + SourceFirstFragmentSize - TargetFirstFragmentSize,
					*TargetSecondFragmentRow,
					*SourceSecondFragmentRow + TargetFirstFragmentSize - SourceFirstFragmentSize );
		}
		void _Move(
			row__ SourceFirstFragmentRow,
			mdr::size__ SourceFirstFragmentSize,
			row__ SourceSecondFragmentRow,
			mdr::size__ SourceSecondFragmentSize,
			row__ TargetFirstFragmentRow,
			mdr::size__ TargetFirstFragmentSize,
			row__ TargetSecondFragmentRow,
			mdr::size__ TargetSecondFragmentSize  )
			/* ATTENTION : Les différents 'row' pointent directement sur les données des fragment respectifs (aprés l'indicateur de taille),
			et non pas sur le début des fragments respectis. */
		{
			_MoveHead(
				SourceFirstFragmentRow, SourceFirstFragmentSize,
				SourceSecondFragmentRow, SourceSecondFragmentSize,
				TargetFirstFragmentRow, TargetFirstFragmentSize,
				TargetSecondFragmentRow, TargetSecondFragmentSize );

			if ( ( SourceFirstFragmentSize != TargetFirstFragmentSize ) && ( ( SourceFirstFragmentSize + SourceSecondFragmentSize ) > TargetFirstFragmentSize ) )
				_MoveMiddle(
					SourceFirstFragmentRow, SourceFirstFragmentSize,
					SourceSecondFragmentRow, SourceSecondFragmentSize,
					TargetFirstFragmentRow, TargetFirstFragmentSize,
					TargetSecondFragmentRow, TargetSecondFragmentSize );

			if ( ( SourceFirstFragmentSize  + SourceSecondFragmentSize ) > TargetFirstFragmentSize )
				_MoveTail(
					SourceFirstFragmentRow, SourceFirstFragmentSize,
					SourceSecondFragmentRow, SourceSecondFragmentSize,
					TargetFirstFragmentRow, TargetFirstFragmentSize,
					TargetSecondFragmentRow, TargetSecondFragmentSize );
		}
		void _Move(
			row__ SourceFirstFragmentRow,
			const mdr::datum__ *SourceFirstFragmentHeader,
			row__ TargetFirstFragmentRow,
			const mdr::datum__ *TargetFirstFragmentHeader )
		{
			mdr::datum__ SourceSecondFragmentHeader[MMM2_HEADER_MAX_LENGTH], TargetSecondFragmentHeader[MMM2_HEADER_MAX_LENGTH];
			row__ SourceSecondFragmentRow = NONE, TargetSecondFragmentRow = NONE;
			mdr::size__
				SourceFirstFragmentSize = _GetUsedFragmentDataSize( SourceFirstFragmentHeader ), SourceSecondFragmentSize = 0,
				TargetFirstFragmentSize = _GetUsedFragmentDataSize( TargetFirstFragmentHeader ), TargetSecondFragmentSize = 0;

			if ( _IsUsedFragmentLinked( SourceFirstFragmentHeader ) ) {
				SourceSecondFragmentRow = _GetUsedFragmentLink( SourceFirstFragmentRow, SourceFirstFragmentHeader );
				_GetHeader( SourceSecondFragmentRow, SourceSecondFragmentHeader );
				SourceSecondFragmentSize = _GetUsedFragmentDataSize( SourceSecondFragmentHeader );
			}

			if ( _IsUsedFragmentLinked( TargetFirstFragmentHeader ) ) {
				TargetSecondFragmentRow = _GetUsedFragmentLink( TargetFirstFragmentRow, TargetFirstFragmentHeader );
				_GetHeader( TargetSecondFragmentRow, TargetSecondFragmentHeader );
				TargetSecondFragmentSize = _GetUsedFragmentDataSize( TargetSecondFragmentHeader );
			}

			_Move(
				*SourceFirstFragmentRow + _GetSizeLength( SourceFirstFragmentSize ), SourceFirstFragmentSize,
				*SourceSecondFragmentRow + ( SourceSecondFragmentSize ? _GetSizeLength( SourceSecondFragmentSize ) : 0 ), SourceSecondFragmentSize,
				*TargetFirstFragmentRow + _GetSizeLength( TargetFirstFragmentSize ), TargetFirstFragmentSize,
				*TargetSecondFragmentRow + ( TargetSecondFragmentSize ? _GetSizeLength( TargetSecondFragmentSize ) : 0 ), TargetSecondFragmentSize );
		}
		void _Move(
			row__ SourceFirstFragmentRow,
			const mdr::datum__ *SourceFirstFragmentHeader,
			row__ TargetFirstFragmentRow )
		{
			mdr::datum__ TargetFirstFragmentHeader[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( TargetFirstFragmentRow, TargetFirstFragmentHeader );

			_Move( SourceFirstFragmentRow, SourceFirstFragmentHeader, TargetFirstFragmentRow, TargetFirstFragmentHeader );
		}
		row__ _ReallocateGreater(
			row__ Descriptor,
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			bso::ubyte__ &Addendum )
		{
#if 1
			if ( _IsUsedFragmentLinked( Header )
				  || ( ( *Descriptor + _GetUsedFragmentTotalSize( Header ) ) < S_.Extent ) ) {
				row__ NewDescriptor = _Allocate( DataSize, Addendum );

				_Move( Descriptor, Header, NewDescriptor );

				_Free( *Descriptor );

				return NewDescriptor;
			} else {
#ifdef MMM2_DBG
				if ( _IsUsedFragmentLinked( Header ) )
					ERRc();
#endif
				S_.Extent += _GuessTotalSizeForUsedFragment( DataSize, false ) - _GetUsedFragmentTotalSize( Header );

				Memory.Allocate( S_.Extent );

				mdr::size__
					OldSizeLength = _GetSizeLength( _GetUsedFragmentDataSize( Header ) ),
					NewSizeLength = _GetSizeLength( DataSize );

				if ( OldSizeLength != NewSizeLength )
					Memory.Store_( Memory, _GetUsedFragmentDataSize( Header ), *Descriptor + NewSizeLength, *Descriptor + OldSizeLength );

				_SetRawSize( DataSize, Descriptor, _IsLinkFlagSet( Header ), _IsFreeFlagSet( Header ) );

				Addendum = _GetSizeLength( DataSize );

				return Descriptor;
			}
			

#else	// Ancien algorithme. A CONSERVER pour s'en inspirer en vue de futurs optimisations.
			if ( _IsUsedFragmentLinked( Header ) ) {
				row__ Link = _GetUsedFragmentLink( Descriptor, Header );
				mdr::datum__ LinkHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( Link, LinkHeader );

				mdr::size__
					LinkedFragmentCurrentDataSize = _GetUsedFragmentDataSize( LinkHeader ),
					LinkedFragmentNewDataSize = DataSize - _GetUsedFragmentDataSize( Header );
				row__ NewLink = _GetNewUnlinkedFragment( LinkedFragmentNewDataSize );
				row__ Remainder = NONE;

				Memory.Store_(
					Memory,
					LinkedFragmentCurrentDataSize,
					*NewLink + _GetSizeLength( LinkedFragmentNewDataSize ),
					*Link + _GetSizeLength( LinkedFragmentCurrentDataSize ) );

				// Pour mettre en place le nouveu lien.
				if ( _ResizeUsedFragment_( Descriptor, Header, _GetUsedFragmentDataSize( Header ), NewLink ) != NONE )
					ERRc();

				Remainder = _SetAsFreeFragment( Link, _GetUsedFragmentTotalSize( LinkHeader ) );

				_TestAndPush_( Remainder );

				return Descriptor;
			} else {
				if ( _GetUsedFragmentTotalSize( Header ) <= MMM2_ORPHAN_MAX_SIZE ) {
					row__ NewDescriptor = _GetNewUnlinkedFragment( DataSize );
					mdr::size__ FirstFragmentCurrentDataSize = _GetUsedFragmentDataSize( Header );

					Memory.Store_(
						Memory,
						FirstFragmentCurrentDataSize,
						*NewDescriptor + _GetSizeLength( DataSize ),
						*Descriptor + _GetSizeLength( FirstFragmentCurrentDataSize ) );

					return NewDescriptor;
				} else {
					mdr::size__ FirstFragmentNewDataSize = _GetUsedFragmentDataSize( Header ) - MMM2_LINK_SIZE;
					mdr::size__ LinkedFragmentDataSize = DataSize - FirstFragmentNewDataSize;
					row__ Link = _GetNewUnlinkedFragment( LinkedFragmentDataSize );
					row__ Remainder = NONE;

					// Déplacement des données qui vont être écrasée par le lien.
					Memory.Store_(
						Memory,
						MMM2_LINK_SIZE,
						*Link + _GetSizeLength( LinkedFragmentDataSize ),
						*Descriptor + _GetSizeLength( _GetUsedFragmentDataSize( Header ) ) + FirstFragmentNewDataSize );

					Remainder = _ResizeUsedFragment_( Descriptor, Header, FirstFragmentNewDataSize, Link );

					_TestAndPush_( Remainder );

					return Descriptor;
				}
			}
#endif
		}
		mdr::size__ _ReadFromFragment(
			descriptor__ Descriptor,
			const mdr::datum__ *Header,
			bso::size__ HeaderSize,
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer ) const
		{
			mdr::size__ FragmentDataSize = _GetUsedFragmentDataSize( Header );

#ifdef MMM2_DBG
			if ( Position >= FragmentDataSize )
				ERRc();
#endif

			if ( Amount > ( FragmentDataSize - Position ) )
				Amount = FragmentDataSize - Position;

			if ( ( Position + Amount + _GetSizeLength( FragmentDataSize ) ) <= HeaderSize )
				memcpy( Buffer, Header + _GetSizeLength( FragmentDataSize ) + Position, Amount );
			else
				Memory.Recall( Descriptor + _GetSizeLength( FragmentDataSize ) + Position, Amount, Buffer );

			return Amount;
		}
		mdr::size__ _WriteToFragment(
			const mdr::datum__ *Buffer,
			descriptor__ Descriptor,
			const mdr::datum__ *Header,
			mdr::row_t__ Position,
			mdr::size__ Amount )
		{
			mdr::size__ FragmentDataSize = _GetUsedFragmentDataSize( Header );

#ifdef MMM2_DBG
			if ( Position >= FragmentDataSize )
				ERRc();
#endif

			if ( Amount > ( FragmentDataSize - Position ) )
				Amount = FragmentDataSize - Position;

			Memory.Store( Buffer, Amount, Descriptor + _GetSizeLength( FragmentDataSize ) + Position );

			return Amount;
		}
		bso::bool__ _IsFreeFragmentAvailable( void )
		{
			return S_.BiggestFree != NONE;
		}
		row__ _RetrieveFreeFragment( void )
		{
#ifdef MMM2_DBG
			if ( S_.BiggestFree == NONE )
				ERRc();
#endif
			row__ Row = S_.BiggestFree;

			if ( S_.SeekedIsFree ) {
#ifdef MMM2_DBG
				if ( S_.Seeked == NONE )
					ERRc();
#endif
				S_.BiggestFree = S_.Seeked;
				S_.Seeked = NONE;
				S_.SeekedIsFree = false;
			} else
				S_.BiggestFree = NONE;

			return Row;
		}
		void _UpdateSeeked( row__ Position )
		{
			if ( !S_.SeekedIsFree
				 || ( S_.SeekedIsFree
				      && ( _GetFreeFragmentSize_( *S_.Seeked ) < _GetFreeFragmentSize_( *Position ) ) ) ){
				S_.Seeked = Position;
				S_.SeekedIsFree = true;
			}
		}
		void _ReportFreeFragment( row__ Position )
		{
#ifdef MMM2_DBG
			if ( Position == NONE )
				ERRc();
#endif
			if ( S_.BiggestFree != NONE ) {
				if ( _GetFreeFragmentSize_( S_.BiggestFree ) < _GetFreeFragmentSize_( *Position ) ) {
					S_.Seeked = S_.BiggestFree;
					S_.SeekedIsFree = true;
					S_.BiggestFree = Position;
				} else
					_UpdateSeeked( Position );
			} else
				S_.BiggestFree = Position;
		}
	public:
		uym::untyped_memory_ Memory;
		struct s 
		{
			uym::untyped_memory_ ::s Memory;
			mdr::size__ Extent;
			row__
				BiggestFree,	// Position du plus gros fragment libre trouvé jusqu'à présent.
				Seeked;	/* Position du fragment en cours d'investigation.
								Si 'SeekFragmentIsFree' est à 'true', ce fragment est un fragment libre,
								différent est plus petit que 'BiggestFree'. */
			bso::bool__ SeekedIsFree;	// Voir ci-dessus.
			descriptor__ MultimemoryDriverDescriptor;
			bso::ubyte__ MultimemoryDriverAddendum;
			mdr::size__ MultimemoryDriverExtent;
			bso::bool__ LastFragmentIsUsed;
		} &S_;
		multimemory_( s &S )
		: S_( S ) ,
		  _MultimemoryDriver( S.MultimemoryDriverDescriptor, S.MultimemoryDriverAddendum, S.MultimemoryDriverExtent ),
		  Memory( S.Memory )
		{}
		void reset( bso::bool__ P = true )
		{
			_MultimemoryDriver.reset( P );
			Memory.reset( P );
			S_.Extent = 0;
			S_.BiggestFree = S_.Seeked = NONE;
			S_.SeekedIsFree = false;
			S_.LastFragmentIsUsed = false;
		}
		void plug( multimemory_ &MM )
		{
			_MultimemoryDriver.Init( MM );
			Memory.plug( _MultimemoryDriver );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			_MultimemoryDriver.reset();
			Memory.plug( MD );
		}
		multimemory_ &operator =( const multimemory_ &M )
		{
			Memory.Allocate( M.S_.Extent );
			Memory.Store_( M.Memory, M.S_.Extent, 0 );

			S_.Extent = M.S_.Extent;
			S_.BiggestFree = M.S_.BiggestFree;
			S_.Seeked = M.S_.Seeked;
			S_.SeekedIsFree = M.S_.SeekedIsFree;
			S_.LastFragmentIsUsed = M.S_.LastFragmentIsUsed;

			return *this;
		}
		void Init( void )
		{
			Memory.Init();

			S_.Extent = 0;
			S_.BiggestFree = S_.Seeked = NONE;
			S_.SeekedIsFree = false;
			S_.LastFragmentIsUsed = false;
		}
		void Flush( void ) const
		{
			Memory.Flush();
		}
		mdr::size__ Size( descriptor__ Descriptor ) const
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Descriptor, Header );

			return _Size( Descriptor, Header );
		}
		descriptor__ Allocate(
			mdr::size__ Size,
			bso::ubyte__ &Addendum )
		{	
			return _Allocate( Size, Addendum );
		}
		void Free( descriptor__ Descriptor )
		{
			_Free( Descriptor );
		}
		descriptor__ Reallocate(
			descriptor__ Descriptor,
			mdr::size__ Size,
			bso::ubyte__ &Addendum )
		{
			if ( Descriptor == NONE )
				return _Allocate( Size, Addendum );

			if ( Size == 0 ) {
				ERRu();
				return NONE; // Pour éviter un 'warning'.
			}

			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Descriptor, Header );

			mdr::size__ CurrentSize = _Size( Descriptor, Header );

			if ( Size == CurrentSize )
				return Descriptor;
			else if ( Size < CurrentSize )
				return *_ReallocateLesser( Descriptor, Header, Size, Addendum );
			else
				return *_ReallocateGreater( Descriptor, Header, Size, Addendum );
		}
		void Read(
			descriptor__ Descriptor,
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer,
			bso::ubyte__ Addendum ) const
		{
			if ( Addendum == 0 ) {
				//			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];
				mdr::datum__ Header[1024];
				_GetHeader( Descriptor, Header, sizeof( Header ) );

#ifdef MMM2_DBG
				if ( !_IsFragmentUsed( Header ) )
					ERRu();
#endif
				mdr::size__ AmountRed = 0;
				mdr::size__ FirstFragmentDataSize = _GetUsedFragmentDataSize( Header );

				if ( Position <  FirstFragmentDataSize )
					AmountRed = _ReadFromFragment( Descriptor, Header, sizeof( Header ), Position, Amount, Buffer );

				if ( AmountRed < Amount ) {
					descriptor__ Link = *_GetUsedFragmentLink( Descriptor, Header );

					_GetHeader( Descriptor, Header, sizeof( Header ) );

					AmountRed += _ReadFromFragment( Link, Header, sizeof( Header ), Position + AmountRed - FirstFragmentDataSize, Amount - AmountRed, Buffer + AmountRed );
				}

#ifdef MMM2_DBG
				if ( Amount != AmountRed )
					ERRc();
#endif
			} else {
				Memory.Recall( Descriptor + Addendum + Position, Amount, Buffer );
			}
		}
		void Write(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			descriptor__ Descriptor,
			mdr::row_t__ Position,
			bso::ubyte__ Addendum )
		{

			if ( Addendum == 0 ) {
				mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];
				_GetHeader( Descriptor, Header );

#ifdef MMM2_DBG
				if ( !_IsFragmentUsed( Header ) )
					ERRu();
#endif
				mdr::size__ AmountWritten = 0;
				mdr::size__ FirstFragmentDataSize = _GetUsedFragmentDataSize( Header );

				if ( Position <  FirstFragmentDataSize )
					AmountWritten = _WriteToFragment( Buffer, Descriptor, Header, Position, Amount );

				if ( AmountWritten < Amount ) {
					descriptor__ Link = *_GetUsedFragmentLink( Descriptor, Header );
					mdr::datum__ LinkHeader[MMM2_HEADER_MAX_LENGTH];

					_GetHeader( Link, LinkHeader );

					AmountWritten += _WriteToFragment( Buffer + AmountWritten, Link, LinkHeader, Position + AmountWritten - FirstFragmentDataSize, Amount - AmountWritten );
				}

#ifdef MMM2_DBG
				if ( Amount != AmountWritten )
					ERRc();
#endif
			} else
				Memory.Store( Buffer, Amount, Descriptor + Addendum + Position );
		}
	};

	typedef uym::untyped_memory_file_manager___	multimemory_file_manager___;

	inline bso::bool__ Connect(
		multimemory_ &Multimemory,
		multimemory_file_manager___ &FileManager )
	{
#pragma message( __LOC__ "Voir que faire avec 'S_.(...)'" )
		bso::bool__ Exists = uym::Connect( Multimemory.Memory, FileManager );

		if ( Exists )
			Multimemory.S_.Extent = FileManager.FileSize();

		return Exists;
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
