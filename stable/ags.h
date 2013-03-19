/*
	Header for the 'ags' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20132004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#ifndef AGS__INC
#define AGS__INC

#define AGS_NAME		"AGS"

#define	AGS_VERSION	"$Revision$"

#define AGS_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &AGSTutor;

#if defined( XXX_DBG ) && !defined( AGS_NODBG )
#define AGS_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Aggregated Storage 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "sdr.h"
# include "txf.h"
// # include "uys.h"	// d�port�, parce 'ags.h' est inclus par 'uys.h'.

# ifdef UYS__INC
#  ifndef UYS__HEADER_HANDLED
#   define AGS__HANDLE_PART_ONE
#   undef AGS__INC
#  else
#   define AGS__HANDLE_PART_TWO
#  endif
# else
#  define AGS__HANDLE_PART_ONE
#  define AGS__HANDLE_PART_TWO
# endif

# ifdef AGS__HANDLE_PART_ONE
#  ifdef	AGS__PART_ONE_HANDLED
#   error
#  endif
#  ifdef	AGS__PART_TWO_HANDLED
#   error
#  endif

# define AGS_UNDEFINED_DESCRIPTOR	( (ags::descriptor__)NONE )

namespace ags {

	E_TMIMIC__( sdr::row_t__, descriptor__ );

	class aggregated_storage_;

	class aggregated_storage_driver__
	: public sdr::E_STORAGE_DRIVER__
	{
	private:
		descriptor__ &_Descriptor;
		// memoire � laquelle il a �t� affect�
		class aggregated_storage_ *_AStorage;
		void _Free( void );
	protected:
		virtual void SDRAllocate( sdr::size__ Size );
		// Fonction d�port�e.
		virtual sdr::size__ SDRUnderlyingSize( void );
		// fonction d�port�e
		// lit � partir de 'Position' et place dans 'Tampon' 'Nombre' octets;
		virtual void SDRRecall(
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::datum__ *Buffer );
		// fonction d�port�e
		// �crit 'Nombre' octets � la position 'Position'
		virtual void SDRStore(
			const sdr::datum__ *Buffer,
			sdr::size__ Amount,
			sdr::row_t__ Position );
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( _AStorage != NULL )
					_Free();
			} else
				_AStorage = NULL;

			E_STORAGE_DRIVER__::reset( P );

			// On ne touche ni � '_Descriptor', ni � '_Addendum' car ils sont g�r�s ext�rieurement (ce sont des r�f�rences).
		}
		aggregated_storage_driver__( descriptor__ &Descriptor )
		: _Descriptor( Descriptor ),
		  E_STORAGE_DRIVER__()
		{
			reset( false );
		}
		E_VDTOR( aggregated_storage_driver__ )
		void Init( aggregated_storage_ &AStorage )
		{
			reset();

			_AStorage = &AStorage;
			E_STORAGE_DRIVER__::Init();

			// On ne touche ni � '_Descriptor', ni � '_Addendum' car ils sont g�r�s ext�rieurement (ce sont des r�f�rences).
		}
		descriptor__ Descriptor( void ) const
		{
			return _Descriptor;
		}
		aggregated_storage_ *AStorage( void ) const
		{
			return _AStorage;
		}
	};

}

#  define AGS__PART_ONE_HANDLED
# endif

# ifdef AGS__HANDLE_PART_TWO
#  ifndef AGS__PART_ONE_HANDLED
#   error
#  endif
#  ifdef AGS__PART_TWO_HANDLED
#   error
# endif

# include "uys.h"

namespace ags {

	/*
	NOTA :
	- Par 'Size', on entend : 
		- pour les fragments occup�s (used), la taille disponible pour les donn�es, et non pas la taille totale occup�e par le fragment,
		- pour les fragments libres (free), la taille totale du fragment.
	- Un 'descriptor__' pointe sur le d�but des donn�es. Les m�tadonn�es sot situ�s juste avant. Seul les fragment occup�s ('used') ont un descritpeur.
	- Une 'Value' est une donn�e brute, sans ajustement, telle que stock�e.
	*/		


	enum status__ {
		sFree,
		sUsed,
		s_amount,
		s_Undefined
	};

	inline void Display(
		status__ Status,
		txf::text_oflow__ &Flow )
	{
		switch ( Status ) {
		case sFree:
			Flow << 'F';
			break;
		case sUsed:
			Flow << 'U';
			break;
		default:
			ERRc();
			break;
		}
	}

	enum flag_position__ {
		/*
		Statut du fragment : 
			- 0 : libre (free),
			- 1 : occup� (used).
		*/
		fpStatus,
		/*
		- Pour n'importe quel type de fragment (libre ou occup�), s'il est en premi�re position, statut du fragment en derni�re position :
		- Pour un fragment occup�, s'il n'est pas en premi�re position, statut de son pr�decesseur :
			- 0 : libre (free),
			- 1 : occup� (used).
		Pour un fragment libre, non situ� en premi�re position, n'a pas de signification.
		*/
		fpPredecessorStatus,
		/*
		Type de la taille d'un fragment :
			- 0 : court (short) la taille du fragment �tant encod� dans les bits restant de l'octet,
			- 1 : long (large), la taille du fragment �tant encod� sur, au maximum, les 2^(valeurs des bits restants de l'octet).
		*/
		/*
			Les 5 bits qui suivent contiennent la taille (-1) du fragment lorsqu'elle est suffisament petit pour y �tre contenue
			(elle est alors dite 'embedded') et qu'il s'agit d'un fragment libre (free).
		*/
		fp_SizeBegin,
		fp_SizeEnd = 6,
		/*
			Type de la taille :
				- 0 : longue (dynamique, 'long'), elle est stock�e dans les octets qui suivent (nombre d'octets variables).
				- 1 : courte ('short') dans le 'header' (taille - 1).

			Pour des raisons de simplification, lorsque ce bit est � 1 pour un fragment occup� (used), alors la taille de ce fragment est
			de 1. Lorsque la taille d'un fragment occup� est > 1, alors elle est stock�e de mani�re dynamique.

			Ce bit sert �galement de marqueur (lorsqu'il est � 0) pour signaler que l'on est sur l'octet pr�c�dent le premier
			d'une taille dynamique. Le 7�me bit du dernier octet d'une taille dynamique est toujours � 0. Les autres ont leur 7�me bits � 1.
		*/
		fpSizeType = 7,
		fp_amount
	};

	enum flag_ {
		fStatus = 1 << fpStatus,
		fPredecessorStatus = 1 << fpPredecessorStatus,
		fSizeType = 1 << fpSizeType,
		f_All = fStatus | fPredecessorStatus | fSizeType,
	};

# define AGS_HEADER_SIZE			sizeof( ags::header__ )
# define AGS_EMBEDDED_VALUE_MAX		( (bso::ubyte__)( ~ags::f_All >> ags::fp_SizeBegin ) )
# define AGS_SHORT_SIZE_MAX			( AGS_EMBEDDED_VALUE_MAX + 1 )
# define AGS_LONG_SIZE_SIZE_MAX		sizeof( sdr::dsize__ )
# define AGS_XHEADER_SIZE_MAX		( AGS_HEADER_SIZE + AGS_LONG_SIZE_SIZE_MAX )

	using sdr::size__;

	E_TMIMIC__( sdr::datum__, header__ );

	inline bso::bool__ IsUsed( header__ Header )
	{
		return ( *Header & fStatus ) != 0;
	}

	inline bso::bool__ IsFree( header__ Header )
	{
		return !IsUsed( Header );
	}

	inline status__ Status( header__ Header )
	{
		return IsUsed( Header ) ? sUsed : sFree;
	}

	inline bso::bool__ IsSizeShort( header__ Header )
	{
		return ( *Header & fSizeType ) != 0;
	}

	inline bso::bool__ IsSizeLong( header__ Header )
	{
		return !IsSizeShort( Header );
	}

	inline bso::bool__ IsPredecessorUsed( header__ Header )
	{
		return ( *Header & fPredecessorStatus ) != 0;
	}

	inline bso::bool__ IsPredecessorFree( header__ Header )
	{
		return !IsPredecessorUsed( Header );
	}

	inline status__ PredecessorStatus( header__ Header )
	{
		return IsPredecessorUsed( Header ) ? sUsed : sFree;
	}

	inline size__ GetEmbeddedValue( header__ Header )
	{
		return ( ( *Header & ~f_All ) >> fp_SizeBegin );
	}

	inline size__ GetShortValue( header__ Header )
	{
		if ( !IsSizeShort( Header ) )
			ERRc();

		return GetEmbeddedValue( Header );
	}

	inline size__ GetShortSize( header__ Header )
	{
		return GetShortValue( Header ) + 1;
	}

	inline size__ ConvertValueToFreeFragmentLongSize( size__ Value )
	{
		if ( Value < 1 )
			ERRc();

		return Value;
	}

	inline size__ ConvertValueToUsedFragmentLongSize( size__ Value )
	{
		return Value + AGS_SHORT_SIZE_MAX + 1;
	}

	inline size__ ConvertValueToLongSize(
		size__ Value,
		status__ Status )
	{
		switch ( Status ) {
		case sFree:
			return ConvertValueToFreeFragmentLongSize( Value );
			break;
		case sUsed:
			return ConvertValueToUsedFragmentLongSize( Value );
			break;
		default:
			ERRc();
			break;
		}

		return 0;	// Pour �viter un 'Warning'.

	}

	inline void Display(
		header__ Header,
		txf::text_oflow__ &Flow )
	{
		Display( Status( Header ), Flow );

		if ( IsSizeLong( Header ) )
			Flow << 'L';
		else if ( IsSizeShort( Header ) )
			Flow << 'S';
		else
			ERRc();

		Flow << '(';

		Display( PredecessorStatus( Header ), Flow );

		Flow << ')';
	}

	inline void MarkAsFree( header__ &Header )
	{
		*Header &= ~fStatus;
	}

	inline void MarkAsUsed( header__ &Header )
	{
		*Header |= fStatus;
	}

	inline void Mark(
		header__ &Header,
		status__ Status )
	{
		switch( Status ) {
		case sFree:
			MarkAsFree( Header );
			break;
		case sUsed:
			MarkAsUsed( Header );
			break;
		default:
			ERRc();
			break;
		}
	}

	inline void MarkPredecessorAsFree( header__ &Header )
	{
		*Header &= ~fPredecessorStatus;
	}

	inline void MarkPredecessorAsUsed( header__ &Header )
	{
		*Header |= fPredecessorStatus;
	}

	inline void MarkPredecessorStatus(
		header__ &Header,
		status__ Status )
	{
		switch ( Status ) {
		case sFree:
			MarkPredecessorAsFree( Header );
			break;
		case sUsed:
			MarkPredecessorAsUsed( Header );
			break;
		default:
			ERRc();
			break;
		}
	}

	inline void MarkSizeAsShort( header__ &Header )
	{
		*Header |= fSizeType;
	}

	inline void MarkSizeAsLong( header__ &Header )
	{
		*Header &= ~fSizeType;
	}

	inline bso::bool__ CanValueBeEmbedded( size__ Value )
	{
		return Value <= AGS_EMBEDDED_VALUE_MAX;
	}

	inline bso::bool__ IsFreeFragmentSizeShortSuitable( size__ Size )
	{
		if ( Size == 0 )
			ERRc();

		return Size == 1;
	}

	inline bso::bool__ IsUsedFragmentSizeShortSuitable( size__ Size )
	{
		if ( Size == 0 )
			ERRc();

		return Size <= AGS_SHORT_SIZE_MAX;
	}

	inline bso::bool__ IsSizeShortSuitable(
		size__ Size,
		status__ Status )
	{
		switch ( Status ) {
		case sFree:
			return IsFreeFragmentSizeShortSuitable( Size );
			break;
		case sUsed:
			return IsUsedFragmentSizeShortSuitable( Size );
			break;
		default:
			ERRc();
			break;
		}

		return false;
	}

	inline size__ ConvertFreeFragmentLongSizeToValue( size__ Size )
	{
		if ( IsFreeFragmentSizeShortSuitable( Size ) )
			ERRc();

		return Size;
	}

	inline size__ ConvertUsedFragmentLongSizeToValue( size__ Size )
	{
		if ( IsUsedFragmentSizeShortSuitable( Size ) )
			ERRc();

		return Size - AGS_SHORT_SIZE_MAX - 1;
	}

	inline size__ ConvertLongSizeToValue(
		size__ Size,
		status__ Status )
	{
		switch ( Status ) {
		case sFree:
			return ConvertFreeFragmentLongSizeToValue( Size );
			break;
		case sUsed:
			return ConvertUsedFragmentLongSizeToValue( Size );
			break;
		default:
			ERRc();
			break;
		}

		return 0;	// Pour �viter un 'warning'.
	}

	inline size__ ConvertFreeFragmentShortSizeToValue( size__ Size )
	{
		if ( !IsFreeFragmentSizeShortSuitable( Size ) )
			ERRc();

		return 0;
	}

	inline size__ ConvertUsedFragmentShortSizeToValue( size__ Size )
	{
		if ( !IsUsedFragmentSizeShortSuitable( Size ) )
			ERRc();

		return Size - 1;
	}

	inline size__ ConvertShortSizeToValue(
		size__ Size,
		status__ Status )
	{
		switch ( Status ) {
		case sFree:
			return ConvertFreeFragmentShortSizeToValue( Size );
			break;
		case sUsed:
			return ConvertUsedFragmentShortSizeToValue( Size );
			break;
		default:
			ERRc();
			break;
		}

		return 0;	// Pour �viter un 'warning'.
	}

	inline void SetEmbeddedValue(
		header__ &Header,
		size__ Value )
	{
		if ( !CanValueBeEmbedded( Value ) )
			ERRc();

		Header = ( *Header & f_All ) | ( (bso::ubyte__)Value << fp_SizeBegin );
	}

	typedef sdr::xsize__ _xsize__;

	class xsize__ {
	private:
		_xsize__ _XSize;
		size__ _Size;
		status__ _Status;
	public:
		void reset( bso::bool__ P = true )
		{
			_XSize.reset( P );
			_Size = 0;
			_Status = s_Undefined;
		}
		void Init(
			size__ Size,
			status__ Status )
		{
			_Size = Size;

			if ( !IsSizeShortSuitable( Size, Status ) )
				_XSize = sdr::Convert( ConvertLongSizeToValue( Size, Status ) );

			_Status = Status;
		}
		void Init( const xsize__ &XSize )
		{
			*this = XSize;
		}
		size__ FragmentSize( void ) const
		{
			switch ( _Status ) {
			case sFree:
				return _Size;
				break;
			case sUsed:
				return _XSize.BufferSize() + AGS_HEADER_SIZE + _Size;
				break;
			default:
				ERRc();
				break;
			}

			return 0;	// Pour �viter un 'warning'.
		}
		status__ Status( void ) const
		{
			return _Status;
		}
		size__ Size( void ) const
		{
			return _Size;
		}
		bso::bool__ IsShortSuitable( void ) const
		{
			return _XSize.BufferSize() == 0;
		}
		const sdr::datum__ *DSizeBuffer( void ) const
		{
			return _XSize.DSizeBuffer();
		}
		size__ DSizeBufferLength( void ) const
		{
			return _XSize.BufferSize();
		}
	};

	class xheader__
	: public xsize__
	{
	private:
		header__ _Header;
		void _SubInit( 
			status__ Status,
			status__ PredecessorStatus )
		{
			Mark( _Header, Status );
			MarkPredecessorStatus( _Header, PredecessorStatus );

			if ( IsShortSuitable() ) {
				MarkSizeAsShort( _Header );
				SetEmbeddedValue( _Header, ConvertShortSizeToValue( xsize__::Size(), Status ) );
			} else
				MarkSizeAsLong( _Header );
		}
	public:
		void reset( bso::bool__  P = true )
		{
			xsize__::reset( P );
			_Header = 0;
		}
		E_CDTOR( xheader__ )
		void Init(
			size__ Size,
			status__ Status,
			status__ PredecessorStatus )
		{
			xsize__::Init( Size, Status );

			_SubInit( Status, PredecessorStatus );
		}
		void Init(
			const xsize__ &XSize,
			status__ PredecessorStatus )
		{
			xsize__::Init( XSize );

			_SubInit( XSize.Status(), PredecessorStatus );
		}
		const header__ &Header( void ) const
		{
			return _Header;
		}
	};


	inline const sdr::datum__ *FindLongSizeBegin( const sdr::datum__ *Buffer )
	{
		bso::ubyte__ Counter = AGS_LONG_SIZE_SIZE_MAX + 1;

		if ( *Buffer & fSizeType )
			ERRc();

		Buffer--;
		Counter--;

		while ( Counter-- && ( *Buffer & fSizeType ) )
			Buffer--;

		if ( Counter == 0 )
			ERRc();

		return Buffer + 1;
	}

	// Carat�ristiques d'un fragement libre ('free').
	struct tracker__
	{
	public:
		sdr::row_t__ Row;
		size__ Size;
		void reset( bso::bool__ = true )
		{
			Row = NONE;
			Size = 0;
		}
		E_CDTOR( tracker__ );
		void Init( void )
		{
			reset();
		}
		void Init(
			sdr::row_t__ Row,
			size__ Size )
		{
			if ( ( Row ==  NONE ) || ( Size == 0 ) )
				ERRc();

			this->Row = Row;
			this->Size = Size;
		}
		bso::bool__ IsSuitable( size__ Size ) const
		{
			return Size <= this->Size;
		}
	};

	// R�cup�re les m�ta-donn�es plac�s dont 'Pointer' pointe sur le dernier octet.
	// Retourne la taille du 'xheader'.
	// NOTA : La valuer retourn�e ainsi que le contenu de 'Header' sont � ignorer lorsque'Pointer' pointe juste apr�s un fragment libre ('free').
	inline size__ GetPriorMetaData(
		const sdr::datum__ *Pointer,
		status__ Status,
		header__ &Header,
		size__ &Size )
	{
		if ( *Pointer & fSizeType ) {
			Header = (header__)*Pointer;
			Size = GetShortSize( Header );
			if ( ags::Status( Header ) != Status )
				ERRc();
			return AGS_HEADER_SIZE;
		} else {
			const sdr::datum__ *LongSizePointer = FindLongSizeBegin( Pointer );
			Size = ConvertValueToLongSize( sdr::Convert( LongSizePointer ), Status );
			Header = *--LongSizePointer;

			if ( ( Status == sUsed ) && ( ags::Status( Header ) != sUsed ) )
				ERRc();

			return Pointer - LongSizePointer + 1;
		}
	}

	class aggregated_storage_
	{
	private:
		sdr::size__ _Size( void ) const
		{
			return Storage.Size();
		}
		void _Read(
			sdr::row_t__ Position,
			size__ Size,
			sdr::datum__ *Data ) const
		{
			Storage.Recall( Position, Size, Data );
		}
		void _Write(
			const sdr::datum__ *Data,
			sdr::row_t__ Position,
			size__ Size )
		{
			Storage.Store( Data, Size, Position );
		}
		// R�cup�re les m�ta-donn�es plac�s juste avant 'Row'.
		// Retourne la aille du 'xheader'
		// NOTA : la valeur retourn�e ainsi que le contenu de 'Header' sont � ignorer lorsque 'Row' pointe juste apr�s un fragment libre ('free').
		size__ _GetPriorMetaData(
			sdr::row_t__ Row,
			status__ Status,
			header__ &Header,
			size__ &Size ) const
		{
			sdr::datum__ Buffer[AGS_XHEADER_SIZE_MAX];
			size__ Amount = ( Row < AGS_XHEADER_SIZE_MAX ? Row : AGS_XHEADER_SIZE_MAX );
			sdr::datum__ *Pointer = &Buffer[Amount]-1;

			if ( Amount == 0 )
				ERRc();

			_Read( Row - Amount, Amount, Buffer );

			return GetPriorMetaData( Pointer, Status, Header, Size );
		}
		size__ _GetPriorSize(
			sdr::row_t__ Row,
			status__ Status ) const
		{
			size__ Size;
			header__ Header;

			_GetPriorMetaData( Row, Status, Header, Size );

			return Size;
		}
		void _Get(
			sdr::row_t__ Row,
			header__ &Header ) const
		{
			_Read( Row, AGS_HEADER_SIZE, &*Header );
		}
		void _Set(
			sdr::row_t__ Row,
			header__ Header )
		{
			_Write( &*Header, Row, AGS_HEADER_SIZE );
		}
		status__ _TailFragmentStatus( void ) const
		{
			if ( _Size() == 0 )
				return s_Undefined;
			else {
				header__ Header;

				_Get( 0, Header );

				return PredecessorStatus( Header );
			}
		}
		bso::bool__ _IsTailFragmentFree( void ) const
		{
			if ( _Size() == 0 )
				return false;
			else
				return _TailFragmentStatus() == sFree;
		}
		void _UpdatePredecessorStatus(
			sdr::row_t__ Row,
			status__ Status )	// Le statut du pr�decesseur du premier fragment refl�te en fait le statut du dernier fragment.
		{
			header__ Header;

			_Get( Row, Header );

			MarkPredecessorStatus( Header, Status );

			_Set( Row, Header );
		}
		void _UpdateFirstFragmentPredecessorStatus( status__ Status )	// Le statut du pr�decesseur du premier fragment refl�te en fait le statut du dernier fragment.
		{
			_UpdatePredecessorStatus( 0, Status );
		}
		size__ _GetTailFreeSize( void ) const
		{
			if ( _IsTailFragmentFree() )
				return _GetPriorSize( _Size(), sFree );
			else
				return 0;
		}
		sdr::row_t__ _GetTailFreeFragment( void ) const
		{
			if ( _GetTailFreeSize() == 0 )
				return NONE;
			else
				return _Size() - _GetTailFreeSize();
		}
		size__ _GetLongSize(
			sdr::row_t__ Row,
			status__ Status,
			sdr::size__ &SizeLength ) const
		{
			sdr::dsize__ DSize;
			size__ Limit = _Size() - Row;

			_Read( Row, AGS_LONG_SIZE_SIZE_MAX > Limit ? Limit : AGS_LONG_SIZE_SIZE_MAX, (sdr::datum__ *)&DSize );

			return ConvertValueToLongSize( sdr::Convert( DSize, SizeLength ), Status );
		}
		void _GetMetaData(
			sdr::row_t__ Row,
			header__ &Header,
			size__ &Size,
			size__ &XHeaderLength ) const
		{
			_Get( Row, Header );
			size__ SizeLength = 0;

			if ( IsSizeShort( Header ) )
				Size = GetShortSize( Header );
			else
				Size = _GetLongSize( Row + AGS_HEADER_SIZE, Status( Header), SizeLength );

			XHeaderLength = SizeLength + AGS_HEADER_SIZE;
		}
		void _GetMetaData(
			sdr::row_t__ Row,
			header__ &Header,
			size__ &Size ) const
		{
			size__ Dummy = 0;

			return _GetMetaData( Row, Header, Size, Dummy );
		}
		size__ _GetFragmentSize( sdr::row_t__ Row ) const
		{
			header__ Header;
			size__ Size = 0;
			size__ XHeaderLength = 0;

			_GetMetaData( Row, Header, Size, XHeaderLength );

			return Size + ( IsUsed( Header ) ? XHeaderLength : 0 );
		}
		size__ _GetSize( descriptor__ Descriptor ) const
		{
			return _GetPriorSize( *Descriptor, sUsed );
		}
		bso::bool__ _IsLast( sdr::row_t__ Row ) const
		{
			return ( Row + _GetFragmentSize( Row ) ) == _Size();
		}
		void _WriteHeader(
			sdr::row_t__ Row,
			const header__ Header )
		{
			_Write( &*Header, Row, AGS_HEADER_SIZE );
		}
		descriptor__ _WriteHeadMetaData(
			sdr::row_t__ Row,
			const xheader__ &XHeader )
		{
			_WriteHeader( Row, XHeader.Header() );

			Row += AGS_HEADER_SIZE;

			if ( !XHeader.IsShortSuitable() ) {
				_Write( XHeader.DSizeBuffer(), Row, XHeader.DSizeBufferLength() );
				Row += XHeader.DSizeBufferLength();
			}

			return Row;
		}
		void _WriteTailMetaData(
			sdr::row_t__ Row,
			const xheader__ &XHeader )
		{
			if ( XHeader.IsShortSuitable() )
				ERRc();

			switch ( XHeader.Size() ) {
			case 0:
				ERRc();
				break;
			case 1:
				break;
			default:
				_Write( (const sdr::datum__ *)"\x0", Row + XHeader.Size() - XHeader.DSizeBufferLength() - 1, 1 );
			case 2:
				_Write( XHeader.DSizeBuffer(), Row + XHeader.Size() - XHeader.DSizeBufferLength(), XHeader.DSizeBufferLength() );
				break;
			}
		}
		descriptor__ _SetFragment(
			sdr::row_t__ Row,
			const xheader__ &XHeader )
		{
			if ( XHeader.Status() == sFree )
				_WriteTailMetaData( Row, XHeader );

			return _WriteHeadMetaData( Row, XHeader );
		}
		descriptor__ _SetFragment(
			sdr::row_t__ Row,
			const xsize__ &XSize,
			status__ PredecessorStatus )
		{
			xheader__ XHeader;

			XHeader.Init( XSize, PredecessorStatus );

			return _SetFragment( Row, XHeader );
		}
		void _SetAsFreeFragment(
			sdr::row_t__ Row,
			sdr::size__ Size,
			status__ PredecessorStatus )	// Si 'Row' != 0, alors ce param�tre n'est pas significatif.
		{
			xsize__ XSize;

			XSize.Init( Size, sFree );

			_SetFragment( Row, XSize, PredecessorStatus );
		}
		descriptor__ _AllocateAndSetUsedFragmentAtTail(
			const xsize__ &XSize,
			status__ PredecessorStatus,
			bso::bool__ &UsingTail )
		{
			size__ TailAvailableSize = _GetTailFreeSize();
			sdr::row_t__ Row = _Size() - TailAvailableSize;

			UsingTail = TailAvailableSize != 0;

			if ( TailAvailableSize >= XSize.Size() )
				ERRc();

			Storage.Allocate( _Size() - TailAvailableSize + XSize.FragmentSize() );

			return _SetFragment( Row, XSize, PredecessorStatus );
		}
		descriptor__ _SetUsedFragmentUsingFreeFragment(
			sdr::row_t__ Row,
			const xsize__ &XSize,
			status__ PredecessorStatus,
			bso::bool__ &All )
		{
			size__ AvailableSize = _GetFragmentSize( Row );
			descriptor__ Descriptor = _SetFragment( Row, XSize, PredecessorStatus );
			All = false;

			if ( AvailableSize > XSize.FragmentSize() )
				_SetAsFreeFragment( Row + XSize.FragmentSize(), AvailableSize - XSize.FragmentSize(), sUsed );
			else if ( AvailableSize < XSize.FragmentSize() )
				ERRc();
			else
				All = true;

			return Descriptor;
		}
		sdr::row_t__ _GetUsableFreeFragmentIfAvailable( size__ Size )
		{
			sdr::row_t__ Row = NONE;

			if ( S_.Free.IsSuitable( Size ) ) {
				Row = S_.Free.Row;
				S_.Free.Init();
			}

			return Row;
		}
		descriptor__ _Allocate( sdr::size__ Size )
		{
			xsize__ XSize;
			sdr::row_t__ Row = NONE;
			descriptor__ Descriptor = NONE;
			bso::bool__ All = false;

			XSize.Init( Size, sUsed );

			if ( ( Row = _GetUsableFreeFragmentIfAvailable( XSize.FragmentSize() ) ) == NONE )
				if ( _GetTailFreeSize() >= XSize.FragmentSize() )
					Row = _GetTailFreeFragment();

			if ( Row != NONE ) {
				Descriptor = _SetUsedFragmentUsingFreeFragment( Row, XSize, ( Row == 0 ? _TailFragmentStatus() : sUsed ), All );
			} else { 
				if ( ( Row = _GetTailFreeFragment() ) == NONE )
					Row = _Size();
				Descriptor = _AllocateAndSetUsedFragmentAtTail( XSize, sUsed, All );
			}

			if ( All )
				if ( _IsLast( Row ) )
					_UpdateFirstFragmentPredecessorStatus( sUsed );
				else if ( Row == 0 )
					_UpdateFirstFragmentPredecessorStatus( _TailFragmentStatus() );
				else
					_UpdatePredecessorStatus( Row + XSize.FragmentSize(), sUsed );

			return Descriptor;
		}
		void _Free( descriptor__ Descriptor )
		{
			header__ Header;
			size__ Size;
			sdr::size__ XHeaderLength = _GetPriorMetaData( *Descriptor, sUsed, Header, Size );
			sdr::row_t__ Row = *Descriptor - XHeaderLength;

			if ( !_IsLast( Row ) ) {
				sdr::row_t__ SuccessorRow = *Descriptor + Size;
				header__ SuccessorHeader;
				size__ SuccessorSize = 0;

				_GetMetaData( SuccessorRow, SuccessorHeader, SuccessorSize );

				if ( IsFree( SuccessorHeader ) ) {
					if ( S_.Free.Row == SuccessorRow )
						S_.Free.Init();

					Size += SuccessorSize;
				}
			}

			if ( ( Row != 0 ) && IsPredecessorFree( Header ) ) {
				size__ PredecessorSize = _GetPriorSize( Row, sFree );
				Size += PredecessorSize;
				Row -= PredecessorSize;

				if ( S_.Free.Row == ( Row - PredecessorSize ) )
					S_.Free.Init();
			}

			Size += XHeaderLength;

			_SetAsFreeFragment( Row, Size, ( Row == 0 ? _TailFragmentStatus() : sFree ) );

			if ( _IsLast( Row ) )
				_UpdateFirstFragmentPredecessorStatus( sFree );
			else  {
				_UpdatePredecessorStatus( Row + Size, sFree );

				if ( S_.Free.Size < Size )
					S_.Free.Init( Row, Size );
			}
		}
		void _Display(
			sdr::row_t__ Row,
			txf::text_oflow__ &Flow ) const
		{
			header__ Header;
			sdr::size__ Size;

			Flow << Row << ' ';

			_GetMetaData( Row, Header, Size );
				
			_Get( Row, Header );

			Display( Header, Flow );

			Flow << " : "<<  Size;
		}
	public:
		uys::untyped_storage_ Storage;
		struct s {
			uys::untyped_storage_::s Storage;
			tracker__ Free;	// Ne doit pas pointer sur le dernier fragment, m�me s'il s'agit du seul.
		} &S_;
		aggregated_storage_( s &S )
		: S_( S ),
		  Storage( S.Storage )
		{}
		void reset( bso::bool__  P = true )
		{
			Storage.reset( P );
			S_.Free.reset( P );
		}
		aggregated_storage_ &operator =( const aggregated_storage_ &AS )
		{
			Storage = AS.Storage;
			S_.Free = AS.S_.Free;

			return *this;
		}
		void plug( sdr::E_STORAGE_DRIVER__ &MD )
		{
			Storage.plug( MD );
		}
		void plug( aggregated_storage_ &AS )
		{
			Storage.plug( AS );
		}
		void Init( void )
		{
			Storage.Init();
			S_.Free.Init();
		}
		void Preallocate( sdr::size__ Size )
		{
			if ( _Size() > Size )
				ERRc();
			else if ( _Size() != Size ) {
				sdr::row_t__ Row = _Size();

				Size -= _Size();

				Storage.Allocate( _Size() + Size );

				if ( ( Row != 0 ) && ( _GetTailFreeSize() != 0 ) ) {
					Row = _GetTailFreeFragment();
					Size += _GetTailFreeSize();
				}

				_SetAsFreeFragment( Row, Size, Row == 0 ? sFree : _TailFragmentStatus() );

				_UpdateFirstFragmentPredecessorStatus( sFree );
			}
		}
		descriptor__ Allocate( size__ Size )
		{
			if ( Size == 0 )
				return NONE;
			else
				return _Allocate( Size );
		}
		void Free( descriptor__ Descriptor )
		{
			if ( Descriptor != NONE ) 
				return _Free( Descriptor );
		}
		descriptor__ Reallocate(
			descriptor__ Descriptor,
			size__ Size )
		{
			descriptor__ NewDescriptor = NONE;

			if ( Size == 0 )
				Free( Descriptor );
			else if ( Descriptor == NONE )
				NewDescriptor = Allocate( Size );
			else {
				size__ OldSize = _GetSize( Descriptor );
				NewDescriptor = _Allocate( Size );

				if ( OldSize > Size )
					OldSize = Size;

				Storage.Store( Storage, OldSize, *NewDescriptor, *Descriptor );

				_Free( Descriptor );
			}

			return NewDescriptor;
		}
		void Read(
			descriptor__ Descriptor,
			sdr::row_t__ Position,
			sdr::size__ Amount,
			sdr::datum__ *Buffer ) const
		{
			Storage.Recall( *Descriptor + Position, Amount, Buffer );
		}
		void Write(
			const sdr::datum__ *Buffer,
			sdr::size__ Amount,
			descriptor__ Descriptor,
			sdr::row_t__ Position )
		{
			Storage.Store( Buffer, Amount, *Descriptor + Position );
		}
		size__ Size( descriptor__ Descriptor ) const
		{
			return _GetSize( Descriptor );
		}
		void DisplayStructure( txf::text_oflow__ &Flow ) const;
		friend uys::state__ Plug(
			aggregated_storage_ &Storage,
			class aggrgated_storage_file_manager___ &FileManager );
	};

	E_AUTO( aggregated_storage )

	typedef uys::untyped_storage_file_manager___ aggregated_storage_file_manager___;

	inline uys::state__ Plug(
		aggregated_storage_ &AStorage,
		aggregated_storage_file_manager___ &FileManager )
	{
		AStorage.Init();

		return uys::Plug( AStorage.Storage, FileManager );
	}



# define E_ASTORAGE_	aggregated_storage_
# define E_ASTORAGE	aggregated_storage
}

#endif

# undef AGS__HANDLE_PART_ONE
# undef AGS__HANDLE_PART_TWO

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
