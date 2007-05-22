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
#include "mmm0.h"
#include "uym.h"
#include "bch.h"
#include "stk.h"

#define MMM2_FLAG_POSITION	7
#define MMM2_FLAG_MASK ( 1 << MMM2_FLAG_POSITION )
#define MMM2_L1	((unsigned char)~MMM2_FLAG_MASK )
#define MMM2_L2 ( 0xffffUL + MMM2_L1 )
#define MMM2_L3 0xffffffffUL
#define MMM2_SIZE_BUFFER_MAX_LENGTH	7
#define MMM2_LINK_SIZE	sizeof( mmm::row__ )
#define MMM2_ORPHAN_MAX_SIZE	5

namespace mmm {
	typedef mdr::datum__ size_buffer__[MMM2_SIZE_BUFFER_MAX_LENGTH];
	E_ROW( row__ );

	class multimemory_
	{
	private:
		multimemory_driver__ _MultimemoryDriver;
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

			return 0;	// Pour �viter un 'warning'.
		}
		// Retourne la taile maximale telle que cette taille + l'indicateur de taille soit inf�rieur � 'MaxSize'.
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

			Size = Datum[0] & ~MMM2_FLAG_MASK;

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
		bso::bool__ _IsFragmentFree( row__ Position ) const
		{
			return ( Memory.Get( *Position ) & ~MMM2_FLAG_MASK ) == 0;
		}
		bso::bool__ _IsFragmentUsed( row__ Position ) const
		{
			return ( Memory.Get( *Position ) & ~MMM2_FLAG_MASK ) != 0;
		}
		mdr::size__ _GetRawSize( row__ Position ) const
		{
			size_buffer__ SizeBuffer;
			mdr::size__ Amount = sizeof( SizeBuffer );
			mdr::size__ Size = 0;

			if ( ( *Position + Amount ) > S_.Extent )
				Amount = S_.Extent - *Position;

			Memory.Recall( *Position, Amount, SizeBuffer );

			Size = _GetSize( SizeBuffer );

			if ( _GetSizeLength( Size ) > Amount )
				ERRm();

			return Size;
		}
		void _SetRawSize(
			mdr::size__ Size,
			row__ Position,
			bso::bool__ Flag )
		{
			size_buffer__ SizeBuffer;

			SizeBuffer[0] = Flag << MMM2_FLAG_POSITION;

			_PutSize( Size, SizeBuffer );

#ifdef MMM2_DBG
			if ( ( SizeBuffer[0] & MMM2_FLAG_MASK ) != ( Flag << MMM2_FLAG_POSITION ) )
				ERRc();
#endif
			Memory.Store( SizeBuffer, _GetSizeLength( Size ), *Position );
		}
		bso::bool__ _IsFlagSet( row__ Position ) const
		{
			return ( Memory.Get( *Position ) & MMM2_FLAG_MASK ) != 0;
		}
		bso::bool__ _IsUsedFragmentLinked( row__ Position ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Position ) )
				ERRc();
#endif
			return _IsFlagSet( Position );
		}
		mdr::size__ _GetFreeFragmentHeaderLength( row__ Position ) const
		{
			return _GetSizeLength( _GetFreeFragmentSize( Position ) ) + 1;
		}
		void _SetFreeFragmentLink(
			row__ Position,
			row__ Link )
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Position ) )
				ERRc();

			if ( _IsOrphan( Position ) )
				ERRc();
#endif
			Memory.Store( (const mdr::datum__ *)&Link, sizeof( Link ), *Position + _GetFreeFragmentHeaderLength( Position ) );
		}
		row__ _GetFreeFragmentLink( row__ Position ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Position ) )
				ERRc();

			if ( _IsOrphan( Position ) )
				ERRc();
#endif
			row__ Link = NONE;

			Memory.Recall( *Position + _GetFreeFragmentHeaderLength( Position ), sizeof( Link ), (mdr::datum__ *)&Link );

			return Link;
		}
		bso::bool__ _IsFreeFragmentLinked( row__ Position ) const
		{
			return ( _GetFreeFragmentLink( Position ) != NONE );
		}
		row__ _GetUsedFragmentLink( row__ Position ) const
		{
			row__ Link = NONE;
#ifdef MMM2_DBG
			if ( !_IsUsedFragmentLinked( Position ) )
				ERRc();
#endif
			Memory.Recall( *Position + _GetUsedFragmentTotalSize( Position ) - 4, sizeof( Link ), (mdr::datum__ *)&Link );

			return Link;
		}
		bso::bool__ _IsOrphan( row__ Position ) const
		{
			return _GetFreeFragmentSize( Position ) <= MMM2_ORPHAN_MAX_SIZE;
		}
		row__ _GetUsedFragmentNextFragmentPosition( row__ Position ) const
		{
			return *Position + _GetUsedFragmentTotalSize( Position );
		}
		row__ _GetFreeFragmentNextFragmentPosition( row__ Position ) const
		{
			return *Position + _GetFreeFragmentSize( Position );
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
				Memory.Put( MMM2_FLAG_MASK, *Position );
				_SetRawSize( Size, *Position + 1, false );
				_SetFreeFragmentLink( Position, NONE );
			}
		}
		mdr::size__ _GetUsedFragmentDataSize( row__ Position ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Position ) )
				ERRu();
#endif
			return _GetRawSize( Position );
		}
		mdr::size__ _GetUsedFragmentTotalSize( row__ Position ) const
		{
			mdr::size__ DataSize = _GetUsedFragmentDataSize( Position );

			return DataSize + _GetSizeLength( DataSize ) + ( _IsUsedFragmentLinked( Position ) ? MMM2_LINK_SIZE : 0 );
		}
		mdr::size__ _GetFreeFragmentSize( row__ Position ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentFree( Position ) )
				ERRc();
#endif
			if ( Memory.Get( *Position ) & MMM2_FLAG_MASK )
				return _GetRawSize( *Position + 1 );
			else
				return 1;
		}
		row__ _SetAsFreeFragment(
			row__ Position,
			mdr::size__ Size )	// Retourne la position du fragment libre ainsi cr�e.
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();
#endif
			if ( ( ( *Position + Size ) != S_.Extent ) && _IsFragmentFree( *Position + Size ) && _IsOrphan( *Position + Size ) )
				Size = _GetFreeFragmentSize( *Position + Size );

			if ( ( *Position < MMM2_ORPHAN_MAX_SIZE )
				 && _IsFragmentFree( 0 )
				 && _IsOrphan( 0 )
				 && ( _GetFreeFragmentNextFragmentPosition( 0 ) == Position ) ) {
				Size += _GetFreeFragmentSize( 0 );
				Position = 0;
			}

			_MarkAsFreeFragment( Position, Size );

			return Position;

		}
		void _MarkAsUsedFragment(
			row__ Position,
			mdr::size__ Size,
			row__ Link )	// Si pas li�, est �gal � 'NONE'
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();
#endif
			_SetRawSize( Size, Position, ( Link != NONE ) );

			if ( Link != NONE )
				Memory.Store( (const mdr::datum__ *)&Link, MMM2_LINK_SIZE, *Position + _GetSizeLength( Size ) + Size );
		}
		row__ _ConvertToUsedFragment(
			row__ Position,
			mdr::size__ Size,
			row__ Link,
			mdr::size__ FragmentCurrentSize,
			row__ NextFragmentPosition )
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();
#endif
			row__ RemainderPosition = *Position + FragmentCurrentSize;

			if ( ( RemainderPosition != S_.Extent ) && ( RemainderPosition != NextFragmentPosition ) )
				RemainderPosition =_SetAsFreeFragment( RemainderPosition, Size - FragmentCurrentSize );
			else
				RemainderPosition = NONE;

			_MarkAsUsedFragment( Position, Size, Link );

			return RemainderPosition;
		}
		row__ _ConvertFreeToUsedFragment(
			row__ Position,
			mdr::size__ Size,
			row__ Link )	// Si pas li�, est �gal � 'NONE'
			// Retourne la position du reliquat si existant.
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();
#endif
			return _ConvertToUsedFragment( Position, Size, Link, _GetFreeFragmentSize( Position ), _GetFreeFragmentNextFragmentPosition( Position ) );
		}
		row__ _ResizeUsedFragment(
			row__ Position,
			mdr::size__ Size,
			row__ Link )	// Si pas li�, est �gal � 'NONE'
			// Retourne la position du reliquat si existant.
		{
#ifdef MMM2_DBG
			if ( Size == 0 )
				ERRc();

			if ( Size > _GetUsedFragmentDataSize( Position ) )
				ERRc();
#endif
			return _ConvertToUsedFragment( Position, Size, Link, _GetUsedFragmentTotalSize( Position ), _GetUsedFragmentNextFragmentPosition( Position ) );
		}
		mdr::size__ _GetFragmentAvailableSize(
			row__ Position,
			bso::bool__ Linked ) const
		{
#ifdef MMM2_DBG
			if ( _IsOrphan( Position ) )
				ERRc();
#endif
			return _AdjustSize( _GetFreeFragmentSize( Position ) - ( Linked ? MMM2_LINK_SIZE : 0 ) );
		}
		bso::bool__ _IsFragmentBigEnough(
			row__ Position,
			mdr::size__ Size ) const
		{
			return ( _GetFragmentAvailableSize( Position, false ) >= Size );
		}
		row__ _AppendNewUnlinkedFragment( mdr::size__ Size )
		{
			row__ Row = S_.Extent;

			S_.Extent += _GetSizeLength( Size ) + Size;

			Memory.Allocate( S_.Extent );

			_SetRawSize( Size, Row, false );

			return Row;
		}
		bso::bool__ _Allocate(
			row__ FirstFragmentPosition,
			row__ SecondFragmentPosition,
			mdr::size__ Size,
			row__ &FirstRemainderPosition,
			row__ &SecondRemainderPosition )
		{
#ifdef MMM2_DBG
			if ( FirstFragmentPosition == NONE )
				ERRc();
#endif
			if ( _IsFragmentBigEnough( FirstFragmentPosition, Size ) ) {
				FirstRemainderPosition = _ConvertFreeToUsedFragment( FirstFragmentPosition, Size, NONE );

				return false;
			}

			mdr::size__ FirstFragmentDataSize = _GetFragmentAvailableSize( FirstFragmentPosition, true );
			
			if ( ( SecondFragmentPosition != NONE ) && _IsFragmentBigEnough( SecondFragmentPosition, Size - FirstFragmentDataSize ) ) {
				FirstRemainderPosition = _ConvertFreeToUsedFragment( FirstFragmentPosition, FirstFragmentDataSize, SecondFragmentPosition );

				SecondRemainderPosition = _ConvertFreeToUsedFragment( SecondFragmentPosition, Size - FirstFragmentDataSize, NONE );

				return true;
			}

			SecondFragmentPosition = _AppendNewUnlinkedFragment( Size - FirstFragmentDataSize );

			FirstRemainderPosition = _ConvertFreeToUsedFragment( FirstFragmentPosition, FirstFragmentDataSize, SecondFragmentPosition );

			return false;
		}
		bso::bool__ _TestAndPush( row__ Row )
		{
			if ( ( Row != NONE ) && !_IsOrphan( Row ) ) {
				_Push( Row );
				return true;
			} else
				return false;
		}
		row__ _GetNewUnlinkedFragment( mdr::size__ Size )
		{
			row__ Row = NONE;

			if ( _IsFreeFragmentAvailable() && _IsFragmentBigEnough( _Top(), Size ) ) {
				row__ RemainderPosition = NONE;

				Row = _Pop();

				RemainderPosition = _ConvertFreeToUsedFragment( Row, Size, NONE );

				_TestAndPush( RemainderPosition );
			} else
				Row = _AppendNewUnlinkedFragment( Size );

			return Row;
		}
		row__ _ReallocateLesser(
			row__ Descriptor,
			mdr::size__ Size )
		{
#ifdef MMM2_DBG
			if ( this->Size( *Descriptor ) <= Size )
				ERRc();
#endif
			row__ Remainder = NONE;

			if ( _IsUsedFragmentLinked( Descriptor ) ) {
				row__ Link = _GetUsedFragmentLink( Descriptor );

				if ( Size > _GetUsedFragmentDataSize( Descriptor ) ) {
					Remainder = _ResizeUsedFragment( Link, Size - _GetUsedFragmentDataSize( Descriptor ), NONE );

					_TestAndPush( Remainder );
				} else {
					Remainder = _SetAsFreeFragment( Link, _GetUsedFragmentTotalSize( Link ) );

					_TestAndPush( Remainder );

					Remainder = _ResizeUsedFragment( Descriptor, Size, NONE );

					_TestAndPush( Remainder );
				}
			} else {
				Remainder = _ResizeUsedFragment( Descriptor, Size, NONE );

				_TestAndPush( Remainder );
			}

			return Descriptor;
		}
		row__ _ReallocateGreater(
			row__ Descriptor,
			mdr::size__ Size )
		{
#ifdef MMM2_DBG
			if ( this->Size( *Descriptor ) >= Size )
				ERRc();
#endif
			if ( _IsUsedFragmentLinked( Descriptor ) ) {
				row__ Link = _GetUsedFragmentLink( Descriptor );
				mdr::size__
					LinkedFragmentCurrentSize = _GetUsedFragmentDataSize( Link ),
					LinkedFragmentNewSize = Size - _GetUsedFragmentDataSize( Descriptor );
				row__ NewLink = _GetNewUnlinkedFragment( LinkedFragmentNewSize );
				row__ Remainder = NONE;

				Memory.Store_(
					Memory,
					LinkedFragmentCurrentSize,
					*NewLink + _GetSizeLength( LinkedFragmentNewSize ),
					*Link + _GetSizeLength( LinkedFragmentCurrentSize ) );

				// Pour mettre en place le nouveu lien.
				if ( _ResizeUsedFragment( Descriptor, _GetUsedFragmentDataSize( Descriptor ), NewLink ) != NONE )
					ERRc();

				Remainder = _SetAsFreeFragment( Link, _GetUsedFragmentTotalSize( Link ) );

				_TestAndPush( Remainder );

				return Descriptor;
			} else {
				if ( _GetUsedFragmentTotalSize( Descriptor ) <= MMM2_ORPHAN_MAX_SIZE ) {
					row__ NewDescriptor = _GetNewUnlinkedFragment( Size );
					mdr::size__ FirstFragmentCurrentDataSize = _GetUsedFragmentDataSize( Descriptor );

					Memory.Store_(
						Memory,
						FirstFragmentCurrentDataSize,
						*NewDescriptor + _GetSizeLength( Size ),
						*Descriptor + _GetSizeLength( FirstFragmentCurrentDataSize ) );

					if ( !_IsOrphan( _SetAsFreeFragment( Descriptor, FirstFragmentCurrentDataSize ) ) )
						ERRc();

					return NewDescriptor;
				} else {
					mdr::size__ FirstFragmentNewDataSize = _GetUsedFragmentDataSize( Descriptor ) - MMM2_LINK_SIZE;
					mdr::size__ LinkedFragmentDataSize = Size - FirstFragmentNewDataSize;
					row__ Link = _GetNewUnlinkedFragment( LinkedFragmentDataSize );
					row__ Remainder = NONE;

					// D�placement des donn�es qui vont �tre �cras�e par le lien.
					Memory.Store_(
						Memory,
						MMM2_LINK_SIZE,
						*Link + _GetSizeLength( LinkedFragmentDataSize ),
						*Descriptor + _GetSizeLength( _GetUsedFragmentDataSize( Descriptor ) ) + FirstFragmentNewDataSize );

					Remainder = _ResizeUsedFragment( Descriptor, FirstFragmentNewDataSize, Link );

					_TestAndPush( Remainder );

					return Descriptor;
				}
			}
		}
		mdr::size__ _ReadFromFragment(
			descriptor__ Descriptor,
			mdr::row_t__ Position,
			mdr::size__ Size,
			mdr::datum__ *Buffer ) const
		{
			mdr::size__ FragmentDataSize = _GetUsedFragmentDataSize( Descriptor );

#ifdef MMM2_DBG
			if ( Position >= FragmentDataSize )
				ERRc();
#endif

			if ( Size > ( FragmentDataSize - Position ) )
				Size = FragmentDataSize - Position;

			Memory.Recall( Descriptor + _GetSizeLength( FragmentDataSize ) + Position, Size, Buffer );

			return Size;
		}
		mdr::size__ _WriteToFragment(
			const mdr::datum__ *Buffer,
			descriptor__ Descriptor,
			mdr::row_t__ Position,
			mdr::size__ Size )
		{
			mdr::size__ FragmentDataSize = _GetUsedFragmentDataSize( Descriptor );

#ifdef MMM2_DBG
			if ( Position >= FragmentDataSize )
				ERRc();
#endif

			if ( Size > ( FragmentDataSize - Position ) )
				Size = FragmentDataSize - Position;

			Memory.Store( Buffer, Size, Descriptor + _GetSizeLength( FragmentDataSize ) + Position );

			return Size;
		}
		bso::bool__ _IsFreeFragmentAvailable( void )
		{
			return S_.Free != NONE;
		}
		row__ _Top( void ) const
		{
			return S_.Free;
		}
		row__ _Pop( void )
		{
			row__ Row = S_.Free;

			S_.Free = _GetFreeFragmentLink( Row );

			return Row;
		}
		void _Push( row__ Position )
		{
#ifdef MMM2_DBG
			if ( _GetFreeFragmentLink( Position ) != NONE )
				ERRc();
#endif
			_SetFreeFragmentLink( Position, S_.Free );

			S_.Free = Position;
		}
	public:
		uym::untyped_memory_ Memory;
		struct s 
		{
			uym::untyped_memory_ ::s Memory;
			mdr::size__ Extent;
			row__ Free;	
			descriptor__ MultimemoryDriverDescriptor;
			mdr::size__ MultimemoryDriverExtent;
		} &S_;
		multimemory_( s &S )
		: S_( S ) ,
		  _MultimemoryDriver( S.MultimemoryDriverDescriptor, S.MultimemoryDriverExtent ),
		  Memory( S.Memory )
		{}
		void reset( bso::bool__ P = true )
		{
			_MultimemoryDriver.reset( P );
			Memory.reset( P );
			S_.Extent = 0;
			S_.Free = NONE;
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
			S_.Free = M.S_.Free;

			return *this;
		}
		void Init( void )
		{
			Memory.Init();

			S_.Extent = 0;
			S_.Free = NONE;
		}
		void Flush( void ) const
		{
			Memory.Flush();
		}
		mdr::size__ Size( descriptor__ Descriptor ) const
		{
			mdr::size__ Size = _GetUsedFragmentDataSize( Descriptor );

			if ( _IsUsedFragmentLinked( Descriptor ) ) {
				Size += _GetUsedFragmentDataSize( _GetUsedFragmentLink( Descriptor ) );

#ifdef MMM2_DBG
				if ( _IsUsedFragmentLinked( _GetUsedFragmentLink( Descriptor ) ) )
					ERRc();
#endif
			}

			return Size;
		}
		descriptor__ Allocate( mdr::size__ Size )
		{	
			if ( _IsFreeFragmentAvailable() ) {
				row__ Free1 = NONE, Free2 = NONE, Remainder1 = NONE, Remainder2 = NONE;

				Free1 = _Pop();

				if ( _IsFreeFragmentAvailable()  )
					Free2 = _Top();

				if ( _Allocate( Free1, Free2, Size, Remainder1, Remainder2 ) )
					_Pop();

				_TestAndPush( Remainder1 );

				_TestAndPush( Remainder2 );

				return *Free1;
			} else
				return *_AppendNewUnlinkedFragment( Size );
		}
		void Free( descriptor__ Descriptor )
		{
			row__ Remainder = NONE;

			if ( _IsUsedFragmentLinked( Descriptor ) ) {
				row__ Link = _GetUsedFragmentLink( Descriptor );
				Remainder = _SetAsFreeFragment( Link, _GetUsedFragmentTotalSize( Link ) );
				_TestAndPush( Remainder );
			}

			Remainder = _SetAsFreeFragment( Descriptor, _GetUsedFragmentTotalSize( Descriptor ) );

			_TestAndPush( Remainder );
		}
		descriptor__ Reallocate(
			descriptor__ Descriptor,
			mdr::size__ Size )
		{
			if ( Descriptor == NONE )
				return Allocate( Size );

			mdr::size__ CurrentSize = this->Size( Descriptor );

			if ( Size == 0 ) {
				ERRu();
				return NONE; // Pour �viter un 'warning'.
			}

			if ( Size == CurrentSize )
				return Descriptor;
			else if ( Size < CurrentSize )
				return *_ReallocateLesser( Descriptor, Size );
			else
				return *_ReallocateGreater( Descriptor, Size );
		}
		void Read(
			descriptor__ Descriptor,
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer ) const
		{
#ifdef MMM2_DBG
			if ( !_IsFragmentUsed( Descriptor ) )
				ERRu();

			if ( Size( Descriptor ) < ( Position + Amount ) )
				ERRu();
#endif
			mdr::size__ AmountRed = 0;
			mdr::size__ FirstFragmentDataSize = _GetUsedFragmentDataSize( Descriptor );

			if ( Position <  FirstFragmentDataSize )
				AmountRed = _ReadFromFragment( Descriptor, Position, Amount, Buffer );

			if ( AmountRed < Amount ) {
				descriptor__ Link = *_GetUsedFragmentLink( Descriptor );

				AmountRed += _ReadFromFragment( Link, Position + AmountRed - FirstFragmentDataSize, Amount - AmountRed, Buffer + AmountRed );
			}

#ifdef MMM2_DBG
			if ( Amount != AmountRed )
				ERRc();
#endif
		}
		void Write(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			descriptor__ Descriptor,
			mdr::row_t__ Position )
		{
#ifdef MMM2_DBG
			mdr::size__ Size = 0;

			if ( !_IsFragmentUsed( Descriptor ) )
				ERRu();

			Size = this->Size( Descriptor );

			if ( Size < ( Position + Amount ) )
				ERRu();
#endif
			mdr::size__ AmountWritten = 0;
			mdr::size__ FirstFragmentDataSize = _GetUsedFragmentDataSize( Descriptor );

			if ( Position <  FirstFragmentDataSize )
				AmountWritten = _WriteToFragment( Buffer, Descriptor, Position, Amount );

			if ( AmountWritten < Amount ) {
				descriptor__ Link = *_GetUsedFragmentLink( Descriptor );

				AmountWritten += _WriteToFragment( Buffer + AmountWritten, Link, Position + AmountWritten - FirstFragmentDataSize, Amount - AmountWritten );
			}

#ifdef MMM2_DBG
			if ( Amount != AmountWritten )
				ERRc();
#endif
		}
	};

	typedef uym::untyped_memory_file_manager___	multimemory_file_manager___;

	inline bso::bool__ Connect(
		multimemory_ &Multimemory,
		multimemory_file_manager___ &FileManager )
	{
#pragma message( __LOC__ "Voir que faire avec 'S_.Free'" )
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
