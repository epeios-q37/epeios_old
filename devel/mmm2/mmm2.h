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

/* NOTA : Lorsque le fragment est utilis�, la taille totoale est la taille utilisable (taille des donn�es).
Lorsque le fragment est disponible, c'est la taille total du fragment qui est stock�e. */

#include "err.h"
#include "flw.h"
#include "epeios.h"
#include "mmm0.h"
#include "uym.h"
#include "bch.h"
#include "stk.h"

// Ce drapeau indique si la taille est stock� dans le fragment. Ne concerne que les fragments libres.
#define MMM2_FREE_FRAGMENT_SIZE_FLAG_POSITION	7
#define MMM2_FREE_FRAGMENT_SIZE_FLAG_MASK ( 1 << MMM2_FREE_FRAGMENT_SIZE_FLAG_POSITION )


// Ce drapeau indique si le fragment est li� ou non (donn�e r�parties sur deux fragments ou non). Ne concerne que les fragments utilis�s.
#define MMM2_USED_FRAGMENT_LINK_FLAG_POSITION	7
#define MMM2_USED_FRAGMENT_LINK_FLAG_MASK ( 1 << MMM2_USED_FRAGMENT_LINK_FLAG_POSITION )

// Ce drapeau indique si le fragment pr�c�dent est libre ou non. Ne concerne que les fragments utilis�s.
#define MMM2_USED_FRAGMENT_FREE_FLAG_POSITION	6
#define MMM2_USED_FRAGMENT_FREE_FLAG_MASK ( 1 << MMM2_USED_FRAGMENT_FREE_FLAG_POSITION )

#define MMM2_USED_FRAGMENT_FLAGS_MASK	( MMM2_USED_FRAGMENT_LINK_FLAG_MASK | MMM2_USED_FRAGMENT_FREE_FLAG_MASK )

#define MMM2_L1	((unsigned char)~MMM2_USED_FRAGMENT_FLAGS_MASK )
#define MMM2_L2 ( 0xffffUL + MMM2_L1 )
#define MMM2_L3 0xffffffffUL
#define MMM2_SIZE_BUFFER_MAX_LENGTH	7
#define MMM2_LINK_SIZE	sizeof( mmm::row__ )
#define MMM2_ORPHAN_MAX_SIZE	10
#define MMM2_NORMAL_FREE_FRAGMENT_MIN_SIZE	( 1 + 1 + MMM2_LINK_SIZE + MMM2_LINK_SIZE + MMM2_LINK_SIZE + 1 )
#define MMM2_HEADER_MAX_LENGTH	( 1+ MMM2_SIZE_BUFFER_MAX_LENGTH + 2 * MMM2_LINK_SIZE  )	// Correspond � la struture d'un 'header' d'un fragment libre,
																							// ce dernier �tant plus grang que le 'header' d'un fragment utilis�.
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

			return 0;	// Pour �viter un 'warning'.
		}
		// Retourne la taile maximale telle que cette taille + l'indicateur de taille soit inf�rieur ou �gal � 'MaxSize'.
		mdr::size__ _AdjustSize( mdr::size__ MaxSize ) const
		{
#ifdef MMM2_DBG
			if ( MaxSize <= 1 )
				ERRc();
#endif
			mdr::size__ Size = MaxSize - 1;	// au moins un octet pour stocker la taille.

			while ( ( Size + _GetSizeLength( Size ) ) > MaxSize )
				Size--;

			return Size;
		}
		void _PutSize(
			mdr::size__ Size,
			mdr::datum__ *Datum )
		{
			if ( Size < MMM2_L1 ) {
				Datum[0] = ( Datum[0] & MMM2_USED_FRAGMENT_FLAGS_MASK ) | (mdr::datum__)Size;
				return;
			}

			Datum[0] = ( Datum[0] & MMM2_USED_FRAGMENT_FLAGS_MASK ) | ~MMM2_USED_FRAGMENT_FLAGS_MASK;

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

			Size = Datum[0] & ~MMM2_USED_FRAGMENT_FLAGS_MASK;

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
			return ( Header[0] & ~MMM2_USED_FRAGMENT_FLAGS_MASK ) != 0;
		}
		bso::bool__ _IsFragmentFree( const mdr::datum__ *Header ) const
		{
			return ( Header[0] & ~MMM2_USED_FRAGMENT_FLAGS_MASK ) == 0;
		}
		mdr::size__ _GetRawSize( const mdr::datum__ *SizeBuffer ) const
		{
			return _GetSize( SizeBuffer );
		}
		void _SetRawSize(
			mdr::size__ Size,
			bso::bool__ LinkFlag,
			bso::bool__ FreeFlag,
			mdr::datum__ *SizeBuffer )
		{
			SizeBuffer[0] = ( LinkFlag << MMM2_USED_FRAGMENT_LINK_FLAG_POSITION ) | ( FreeFlag << MMM2_USED_FRAGMENT_FREE_FLAG_POSITION );

			_PutSize( Size, SizeBuffer );
		}
		void _SetRawSize(
			mdr::size__ Size,
			row__ Position,
			bso::bool__ LinkFlag,
			bso::bool__ FreeFlag )
		{
			mdr::datum__ SizeBuffer[MMM2_SIZE_BUFFER_MAX_LENGTH];

			_SetRawSize( Size, LinkFlag, FreeFlag, SizeBuffer );

			Memory.Store( SizeBuffer, _GetSizeLength( Size ), *Position );
		}
		bso::bool__ _IsUsedFragmentFreeFlagSet( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return ( Header[0] & MMM2_USED_FRAGMENT_FREE_FLAG_MASK ) != 0;
		}
		void _SetUsedFragmentFreeFlag(
			row__ Position,
			const mdr::datum__ *Header,
			bso::bool__ Value )
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			Memory.Put( ( Header[0] & ~MMM2_USED_FRAGMENT_FREE_FLAG_MASK ) | ( Value << MMM2_USED_FRAGMENT_FREE_FLAG_POSITION ), *Position );
		}
		void _SetUsedFragmentFreeFlag(
			row__ Position,
			bso::bool__ Value )
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			_SetUsedFragmentFreeFlag( Position, Header, Value );
		}
		bso::bool__ _IsUsedFragmentLinkFlagSet( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return ( Header[0] & MMM2_USED_FRAGMENT_LINK_FLAG_MASK ) != 0;
		}
		bso::bool__ _IsFreeFragmentSizeFlagSet( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentFree( Header ) )
				ERRc();
#endif
			return ( Header[0] & MMM2_FREE_FRAGMENT_SIZE_FLAG_MASK ) != 0;
		}
		bso::bool__ _IsUsedFragmentLinked( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Header ) )
				ERRc();
#endif
			return _IsUsedFragmentLinkFlagSet( Header );
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
			else if ( Size <= MMM2_ORPHAN_MAX_SIZE )
				return _GetSizeLength( Size ) + 1;
			else
				return _GetSizeLength( Size ) + 1 + MMM2_LINK_SIZE + MMM2_LINK_SIZE;
		}
		bso::bool__ _IsFreeFragmentOrphan( const mdr::datum__ *Header ) const
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
		mdr::size__ _GetFreeFragmentFreeFragmentPointersRelativePosition( const mdr::datum__ *Header ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentFree( Header ) )
				ERRc();

			if ( _IsFreeFragmentOrphan( Header ) )
				ERRc();
#endif
			return 1 + _GetSizeLength( _GetFreeFragmentSize( Header ) );
		}
		mdr::size__ _GetFreeFragmentPreviousFreeFragmentPointerRelativePosition( const mdr::datum__ *Header ) const
		{
			return _GetFreeFragmentFreeFragmentPointersRelativePosition( Header );
		}
		mdr::size__ _GetFreeFragmentNextFreeFragmentPointerRelativePosition( const mdr::datum__ *Header ) const
		{
			return _GetFreeFragmentPreviousFreeFragmentPointerRelativePosition( Header ) + MMM2_LINK_SIZE;
		}
		void _MarkAsFreeFragment(
			row__ Position,
			mdr::size__ Size,
			row__ PreviousFragmentPosition,
			row__ NextFragmentPosition )
		{
			if ( Size == 0 )
				ERRc();
			else if ( Size == 1 )
				Memory.Put( 0, *Position );
			else {
				mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

				Header[0] = MMM2_FREE_FRAGMENT_SIZE_FLAG_MASK;

				_SetRawSize( Size, false, false, Header + 1 );

				if ( Size <= MMM2_ORPHAN_MAX_SIZE )
					Memory.Put( (mdr::datum__)Size, *Position + Size - 1 );	// Marqueur � destination du fragement utilis� qui suit.
				else {
					if ( Size < MMM2_NORMAL_FREE_FRAGMENT_MIN_SIZE )
						Memory.Put( (mdr::datum__)Size, *Position + Size - 1 );	// Marqueur � destination du fragement utilis� qui suit.
					else {
						Memory.Put( 0xff, *Position + Size - 1 );	// Marqueur � destination du fragement utilis� qui suit.
						Memory.Store( (const mdr::datum__ *)&Position, MMM2_LINK_SIZE, *Position + Size - 5 );	// Position du d�but du fragment � destination du fragment utilis� qui suit.
					}
					// Bien que 'Header' soit en cours de construction, son contenu est suffisant pour pouvoir l'utiliser.
					memcpy( Header + _GetFreeFragmentPreviousFreeFragmentPointerRelativePosition( Header ), &PreviousFragmentPosition, MMM2_LINK_SIZE );
					memcpy( Header + _GetFreeFragmentNextFreeFragmentPointerRelativePosition( Header ), &NextFragmentPosition, MMM2_LINK_SIZE );
				}

				Memory.Store( Header, _GetFreeFragmentHeaderLength( Header ), *Position );
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
			if ( !_IsUsedFragmentFreeFlagSet( Header ) )
				ERRc();
#endif
			const mdr::datum__ *Pointer = FreeFragmentTail + MMM2_FREE_FRAGMENT_TAIL_MAX_SIZE - 1;

			if ( *Pointer == 0 )
				return *Position - 1;
			else if ( *Pointer != 0xff )
				return *Position - *Pointer;
			else
				return *(row__ *)( Pointer - MMM2_LINK_SIZE );
		}
		void _GetUsedFragmentPreviousFreeFragmentTail(
			row__ Position,
			const mdr::datum__ *Header,
			mdr::datum__ *FreeFragmentTail ) const
		{
#ifdef MMM2_DBG
			if ( !_IsUsedFragmentFreeFlagSet( Header ) )
				ERRc();

			if ( Position == 0 )
				ERRc();
#endif

			mdr::size__ Amount = MMM2_FREE_FRAGMENT_TAIL_MAX_SIZE;

			if ( *Position < Amount )
				Amount = *Position;

			Memory.Recall( *Position - Amount, Amount, FreeFragmentTail + MMM2_FREE_FRAGMENT_TAIL_MAX_SIZE - Amount );
		}

		row__ _GetUsedFragmentPreviousFreeFragmentPosition(
			row__ Position,
			const mdr::datum__ *Header ) const
		{
			mdr::datum__ FreeFragmentTail[MMM2_FREE_FRAGMENT_TAIL_MAX_SIZE];

			_GetUsedFragmentPreviousFreeFragmentTail( Position, Header, FreeFragmentTail );

			return _GetUsedFragmentPreviousFreeFragmentPosition( Position, Header, FreeFragmentTail );
		}
		row__ _GetFreeFragmentPreviousFreeFragmentPointerPosition(
			row__ Position,
			const mdr::datum__ *Header ) const
		{
			return *Position + _GetFreeFragmentPreviousFreeFragmentPointerRelativePosition( Header );
		}
		row__ _GetFreeFragmentNextFreeFragmentPointerPosition(
			row__ Position,
			const mdr::datum__ *Header ) const
		{
			return *Position + _GetFreeFragmentNextFreeFragmentPointerRelativePosition( Header );
		}
		row__ _GetFreeFragmentPreviousFreeFragmentPosition( const mdr::datum__ *Header ) const
		{
			row__ Row;

			memcpy( &Row, Header + _GetFreeFragmentPreviousFreeFragmentPointerRelativePosition( Header ), MMM2_LINK_SIZE );

			return Row;
		}
		void _SetFreeFragmentPreviousFreeFragmentPosition(
			row__ Position,
			const mdr::datum__ *Header,
			row__ PreviousFragmentPosition )
		{
			Memory.Store( (const mdr::datum__ *)&PreviousFragmentPosition, MMM2_LINK_SIZE, *_GetFreeFragmentPreviousFreeFragmentPointerPosition( Position, Header ) );
		}
		void _SetFreeFragmentPreviousFreeFragmentPosition(
			row__ Position,
			row__ PreviousFragmentPosition )
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			_SetFreeFragmentPreviousFreeFragmentPosition( Position, Header, PreviousFragmentPosition );
		}
		row__ _GetFreeFragmentNextFreeFragmentPosition( const mdr::datum__ *Header ) const
		{
			row__ Row;

			memcpy( &Row, Header + _GetFreeFragmentNextFreeFragmentPointerRelativePosition( Header ), MMM2_LINK_SIZE );

			return Row;
		}
		void _SetFreeFragmentNextFreeFragmentPosition(
			row__ Position,
			const mdr::datum__ *Header,
			row__ NextFragmentPosition )
		{
			Memory.Store( (const mdr::datum__ *)&NextFragmentPosition, MMM2_LINK_SIZE, *_GetFreeFragmentNextFreeFragmentPointerPosition( Position, Header ) );
		}
		void _SetFreeFragmentNextFreeFragmentPosition(
			row__ Position,
			row__ NextFragmentPosition )
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			_SetFreeFragmentNextFreeFragmentPosition( Position, Header, NextFragmentPosition );
		}
		void _UpdateFreeFragmentMarker(
			row__ RemovedFragmentPosition,
			const mdr::datum__ *Header )
		{
#ifdef MMM2_DBG
			if  ( _IsFreeFragmentOrphan( Header ) )
				ERRc();
#endif
			if ( S_.FreeFragment == RemovedFragmentPosition )
				if ( *RemovedFragmentPosition & 1 )	// Petit g�n�rateur al&atoire.
					S_.FreeFragment = _GetFreeFragmentNextFreeFragmentPosition( Header );
				else
					S_.FreeFragment = _GetFreeFragmentPreviousFreeFragmentPosition( Header );
		}
		void _ExciseFreeFragment(
			row__ RemovedFragmentPosition,
			const mdr::datum__ *Header )
		{
			_UpdateFreeFragmentMarker( RemovedFragmentPosition, Header );

			row__
					Previous = _GetFreeFragmentPreviousFreeFragmentPosition( Header ),
					Next = _GetFreeFragmentNextFreeFragmentPosition( Header );
			
			if ( Previous != NONE ) {
#ifdef MMM2_DBG
				if ( Next == NONE )
					ERRc();
#endif

				if ( Previous == Next ) {	// Il ne reste plus q'un fragment libre (non orphelin) ; on ne va pas le faire pointer sur lui-m�me.
					_SetFreeFragmentNextFreeFragmentPosition( Previous, NONE );
					_SetFreeFragmentPreviousFreeFragmentPosition( Next, NONE );
				} else {
					_SetFreeFragmentNextFreeFragmentPosition( Previous, Next );
					_SetFreeFragmentPreviousFreeFragmentPosition( Next, Previous );
				}
			}
		}
		void _ExciseFreeFragment_( row__ RemovedFreeFragmentPosition )
		{
			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( RemovedFreeFragmentPosition, Header );

			_ExciseFreeFragment( RemovedFreeFragmentPosition, Header );
		}
		void _ReportNewFreeFragment(
			row__ NewFreeFragmentPosition,
			row__ &PreviousFreeFragmentPosition,
			row__ &NextFreeFragmentPosition )
		{
			if ( S_.FreeFragment != NONE ) {
				mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];
				_GetHeader( S_.FreeFragment, Header );

				PreviousFreeFragmentPosition = S_.FreeFragment;
				NextFreeFragmentPosition = _GetFreeFragmentNextFreeFragmentPosition( Header );

				_SetFreeFragmentNextFreeFragmentPosition( S_.FreeFragment, NewFreeFragmentPosition );

				if ( NextFreeFragmentPosition != NONE )
					_SetFreeFragmentPreviousFreeFragmentPosition( NextFreeFragmentPosition, NewFreeFragmentPosition );
				else {
					NextFreeFragmentPosition = S_.FreeFragment;
					_SetFreeFragmentPreviousFreeFragmentPosition( S_.FreeFragment, NewFreeFragmentPosition );
				}

			} else {
				PreviousFreeFragmentPosition = NextFreeFragmentPosition = NONE;
			}

			S_.FreeFragment =  NewFreeFragmentPosition;
		}
		void _SetAsFreeFragment(
			row__ Position,	// 'Position' ne pointe pas n�cessairement sur un 'header'.
			mdr::size__ Size )	// Retourne la position du fragment libre ainsi cr�e.
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();
#endif
			if ( ( *Position + Size ) != S_.Extent ) {
				mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( *Position + Size, Header );

				if ( _IsFragmentFree( Header ) ) {
					if ( !_IsFreeFragmentOrphan( Header ) )
						_ExciseFreeFragment( *Position + Size, Header );
					Size += _GetFreeFragmentSize( Header );
				} else if ( _IsFragmentUsed( Header ) ) {
					if ( !_IsUsedFragmentFreeFlagSet( Header ) )
						_SetUsedFragmentFreeFlag( *Position + Size, Header, true );
				} else
					ERRc();
			} else
				S_.LastFragmentIsFree = true;

			if  ( Size > MMM2_ORPHAN_MAX_SIZE ) {
				row__ PreviousFreeFragmentPosition = NONE, NextFreeFragmentPosition = NONE;

				_ReportNewFreeFragment( Position, PreviousFreeFragmentPosition, NextFreeFragmentPosition );

				_MarkAsFreeFragment( Position, Size, PreviousFreeFragmentPosition, NextFreeFragmentPosition );
			} else
				_MarkAsFreeFragment( Position, Size, NONE, NONE );
		}
/*
		row__ _FreeUsedFragment(
			row__ Position,
			const mdr::datum__ *Header,
			mdr::size__ Size )	// Retourne la position du fragment libre ainsi cr�e.Peut �tre diff�rent de 'Position' (fusion avec le fragment libre pr�c�dent).
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();
#endif

			if ( _IsUsedFragmentFreeFlagSet( Header ) ) {
				mdr::datum__ TempHeader[MMM2_HEADER_MAX_LENGTH];
				Position = _GetUsedFragmentPreviousFreeFragmentPosition( Position, Header );
				_GetHeader( Position, TempHeader );

				if ( !_IsFreeFragmentOrphan( TempHeader ) )
					_ExciseFreeFragment( Position, TempHeader );

				Size += _GetFreeFragmentSize( TempHeader );
			}

			_SetAsFreeFragment( Position, Size );

			return Position;
		}
*/		mdr::size__ _GuessTotalSizeForUsedFragment(
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
			row__ Link,	// Si pas li�, est �gal � 'NONE'.
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
		void _ConvertFreeFragmentToUsedFragment(
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
			_ExciseFreeFragment( Position, Header );

			if ( RemainderPosition != S_.Extent )
				if ( Size != FragmentCurrentSize )
					_SetAsFreeFragment( RemainderPosition, FragmentCurrentSize - Size );
				else
					_SetUsedFragmentFreeFlag( RemainderPosition, false );	// 'RemainderPosition' is a used fragment, then report him
																			// that he is no more preceded by a free fragment.

			_MarkAsUsedFragment_( Position, DataSize, Link, false );
		}
		mdr::size__ _GetFreeFragmentAvailableSize(
			const mdr::datum__ *Header,
			bso::bool__ Linked ) const
		{
#ifdef MMM2_DBG
			if ( _IsFreeFragmentOrphan( Header ) )
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
		row__ _AppendNewUnlinkedUsedFragment( mdr::size__ DataSize )
		{
			row__ Row = S_.Extent;

			S_.Extent += _GetSizeLength( DataSize ) + DataSize;

			Memory.Allocate( S_.Extent );

			_SetRawSize( DataSize, Row, false, S_.LastFragmentIsFree );

			S_.LastFragmentIsFree = false;

			return Row;
		}
		row__ _AllocateRetrievingFreeFragments(
			mdr::size__ DataSize,
			bso::ubyte__ &Addendum )
		{
			row__ FirstFragmentPosition = _GetFreeFragment(), SecondFragmentPosition = NONE;
			mdr::datum__ FirstFragmentHeader[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( FirstFragmentPosition, FirstFragmentHeader );

			if ( _IsFreeFragmentBigEnough( FirstFragmentHeader, DataSize ) ) {
				_ConvertFreeFragmentToUsedFragment( FirstFragmentPosition, FirstFragmentHeader, DataSize, NONE );

				Addendum = _GetSizeLength( DataSize );

				return FirstFragmentPosition;
			}

			_ExciseFreeFragment( FirstFragmentPosition, FirstFragmentHeader );

			Addendum = 0;

			mdr::size__ FirstFragmentDataSize = _GetFreeFragmentAvailableSize( FirstFragmentHeader, true );

			if ( _IsFreeFragmentAvailable() ) {
				SecondFragmentPosition = _GetFreeFragment();
				mdr::datum__ SecondFragmentHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( SecondFragmentPosition, SecondFragmentHeader );

				if ( _IsFreeFragmentBigEnough( SecondFragmentHeader, DataSize - FirstFragmentDataSize ) ) {
					_ConvertFreeFragmentToUsedFragment( FirstFragmentPosition, FirstFragmentHeader, FirstFragmentDataSize, SecondFragmentPosition );

					_ConvertFreeFragmentToUsedFragment( SecondFragmentPosition, SecondFragmentHeader, DataSize - FirstFragmentDataSize, NONE );

					return FirstFragmentPosition;
				}
			}

			SecondFragmentPosition = _AppendNewUnlinkedUsedFragment( DataSize - FirstFragmentDataSize );

			_ConvertFreeFragmentToUsedFragment( FirstFragmentPosition, FirstFragmentHeader, FirstFragmentDataSize, SecondFragmentPosition );

			return FirstFragmentPosition;
		}
/*		row__ _GetNewUnlinkedFreeFragment( mdr::size__ DataSize )
		{
			row__ Row = NONE;

			if ( _IsFreeFragmentAvailable() ) {

				mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

				Row = _GetFreeFragment();

				_GetHeader( Row, Header );
			
				if ( _IsFreeFragmentBigEnough( Header, DataSize ) )
					_ConvertFreeFragmentToUsedFragment( Row, Header, DataSize, NONE );
				else
					Row = _AppendNewUnlinkedUsedFragment( DataSize );
			} else
				Row = _AppendNewUnlinkedUsedFragment( DataSize );

			return Row;
		}
*/		descriptor__ _Allocate(
			mdr::size__ Size,
			bso::ubyte__ &Addendum )
		{	
			if ( _IsFreeFragmentAvailable() )
				return *_AllocateRetrievingFreeFragments( Size, Addendum );
			else {
				Addendum = _GetSizeLength( Size );

				return *_AppendNewUnlinkedUsedFragment( Size );
			}
		}
		/* Free used fragment and merge with possibly preceding and following free fragments. */
		void _FreeUsedFragment(
			row__ Position,
			const mdr::datum__ *Header )
		{
			if ( _IsUsedFragmentFreeFlagSet( Header ) ) {
				row__ FreeFragmentPosition = _GetUsedFragmentPreviousFreeFragmentPosition( Position, Header );

				mdr::datum__ FreeFragmentHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( FreeFragmentPosition, FreeFragmentHeader );

				if ( !_IsFreeFragmentOrphan( FreeFragmentHeader ) )
					_ExciseFreeFragment( FreeFragmentPosition, FreeFragmentHeader );

				_SetAsFreeFragment( FreeFragmentPosition, _GetFreeFragmentSize( FreeFragmentHeader ) + _GetUsedFragmentTotalSize( Header ) );
			} else
				_SetAsFreeFragment( Position, _GetUsedFragmentTotalSize( Header ) );
		}
		void _Free( row__ Position )
		{
			if ( Position == MMM_UNDEFINED_DESCRIPTOR )
				return;

			mdr::datum__ Header[MMM2_HEADER_MAX_LENGTH];

			_GetHeader( Position, Header );

			if ( _IsUsedFragmentLinked( Header ) ) {
				row__ Link = _GetUsedFragmentLink( Position, Header );
				mdr::datum__ LinkHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( Link, LinkHeader );

				_FreeUsedFragment( Link, LinkHeader );
			}

			_FreeUsedFragment( Position, Header );
		}
		void _ResizeUsedFragment(
			row__ Position,
			const mdr::datum__ *Header,
			mdr::size__ Size,
			row__ Link )
		{
#ifdef MMM2_DBG
			if ( Size > _GetUsedFragmentTotalSize( Header ) )
				ERRc();
#endif
			_SetAsFreeFragment( *Position + _GetSizeLength( Size ) + Size, _GetUsedFragmentTotalSize( Header ) - _GetSizeLength( Size ) - Size );

			_MarkAsUsedFragment_( Position, Size, Link, _IsUsedFragmentFreeFlagSet( Header ) );
		}
		row__ _ReallocateLesser(
			row__ Descriptor,
			const mdr::datum__ *Header,
			mdr::size__ DataSize,
			bso::ubyte__ &Addendum )
		{
			if ( _IsUsedFragmentLinked( Header ) ) {
				row__ Link = _GetUsedFragmentLink( Descriptor, Header );

				mdr::datum__ LinkHeader[MMM2_HEADER_MAX_LENGTH];

				_GetHeader( Link, LinkHeader );

				if ( DataSize > _GetUsedFragmentDataSize( Header ) ) {
					_ResizeUsedFragment( Link, LinkHeader, DataSize - _GetUsedFragmentDataSize( Header ), NONE );

					Addendum = 0;
				} else {
					_SetAsFreeFragment( Link, _GetUsedFragmentTotalSize( LinkHeader ) );

					_ResizeUsedFragment( Descriptor, Header, DataSize, NONE );

					Addendum = _GetSizeLength( DataSize );
				}
			} else {
				_ResizeUsedFragment( Descriptor, Header, DataSize, NONE );

				Addendum = _GetSizeLength( DataSize );
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
			/* ATTENTION : Les diff�rents 'row' pointent directement sur les donn�es des fragment respectifs (apr�s l'indicateur de taille),
			et non pas sur le d�but des fragments respectis. */
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
			/* ATTENTION : Les diff�rents 'row' pointent directement sur les donn�es des fragment respectifs (apr�s l'indicateur de taille),
			et non pas sur le d�but des fragments respectis. */
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
			/* ATTENTION : Les diff�rents 'row' pointent directement sur les donn�es des fragment respectifs (apr�s l'indicateur de taille),
			et non pas sur le d�but des fragments respectis. */
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
			/* ATTENTION : Les diff�rents 'row' pointent directement sur les donn�es des fragment respectifs (apr�s l'indicateur de taille),
			et non pas sur le d�but des fragments respectis. */
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

				_SetRawSize( DataSize, Descriptor, _IsUsedFragmentLinkFlagSet( Header ), _IsUsedFragmentFreeFlagSet( Header ) );

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

					// D�placement des donn�es qui vont �tre �cras�e par le lien.
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
			return S_.FreeFragment != NONE;
		}
		row__ _GetFreeFragment( void ) const
		{
#ifdef MMM2_DBG
			if ( S_.FreeFragment == NONE )
				ERRc();
#endif
			return S_.FreeFragment;
		}
/*
		row__ _RetrieveFreeFragment( void )
		{
			row__ Row = _ConsultAvailableFreeFragment();

			_ExciseFreeFragment_( Row );

			return Row;
		}
*/	public:
		uym::untyped_memory_ Memory;
		struct s 
		{
			uym::untyped_memory_ ::s Memory;
			mdr::size__ Extent;
			row__ FreeFragment;	// Position d'un fragment libre. NONE si aucun.
			bso::bool__ LastFragmentIsFree;
			descriptor__ MultimemoryDriverDescriptor;
			bso::ubyte__ MultimemoryDriverAddendum;
			mdr::size__ MultimemoryDriverExtent;
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
			S_.FreeFragment = NONE;
			S_.LastFragmentIsFree = false;
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
			S_.FreeFragment = M.S_.FreeFragment;
			S_.LastFragmentIsFree = M.S_.LastFragmentIsFree;

			return *this;
		}
		void Init( void )
		{
			Memory.Init();

			S_.Extent = 0;
			S_.FreeFragment = NONE;
			S_.LastFragmentIsFree = false;
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
				return NONE; // Pour �viter un 'warning'.
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
		void DisplayStructure( txf::text_oflow__ &Flow ) const;
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
