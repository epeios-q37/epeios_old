/*
  Header for the 'uym' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org) 

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
  along with this program; if not, go to http://www.fsf.org/
  or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef UYM__INC
#define UYM__INC

#define UYM_NAME		"UYM"

#define	UYM_VERSION	"$Revision$"	

#define UYM_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &UYMTutor;

#if defined( XXX_DBG ) && !defined( UYM_NODBG )
#define UYM_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (simon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "mdr.h"
#include "cvm.h"
#include "fam.h"

//d Value that a position can not have.
#define UYM_UNREACHABLE_POSITION	((uym__position)-1)

//d Maximal size of a memory.
#define UYM_MAX_SIZE			UYM_UNREACHABLE_POSITION

#ifndef UYM_MAX_BUFFER_SIZE
//d Max. size max. of a copy buffer.
	#define UYM_MAX_BUFFER_SIZE	10000
#endif

#ifndef UYM_MIN_BUFFER_SIZE
//d Min. size of a copy buffer.
	#define UYM_MIN_BUFFER_SIZE	10
#endif

#ifndef UYM_DEFAULT_MEMORY_DRIVER
#define UYM_DEFAULT_MEMORY_DRIVER cvm_conventional_memory_driver
#endif
/*

#ifdef DED_MS
	#ifdef __MSDOS__
		#include "mfg.h"
		#define UYM_PILOTE_DEFAUT mfg_pilote_memoire_fragmentee
	#else
		#include "mmb.h"
		#define UYM_PILOTE_DEFAUT mmb_pilote_memoire_monobloc
	#endif
#else
	#include "mmb.h"
	#define UYM_PILOTE_DEFAUT mmb_pilote_memoire_monobloc
#endif
*/

//t Data handled by the memory.
typedef mdr__data uym__data;

//t Size, in byte, of a memory.
typedef mdr__size	uym__size;

//t Position in a memory.
typedef mdr__position	uym__position;

//t Size of a buffer.
typedef	mdr__bsize		uym__bsize;

// Pilote mémoire.
class uym_memory_driver
{
private:
	// Le pilote.
	MEMORY_DRIVER_ *Pilote_;
	// Indique si le pilote a été défini de manière interne ou non.
	bso__bool Interne_;
public:
	void reset( bool P = true )
	{
		if ( P )
		{
			if ( Interne_ )
			{
				delete Pilote_;
				Interne_ = false;
			}

		}

		Pilote_ = NULL;
		Interne_ = false;
	}
	uym_memory_driver( void )
	{
		reset( false );
	}
	virtual ~uym_memory_driver( void )
	{
		reset( true );
	}
	void plug( MEMORY_DRIVER_ &Pilote )
	{
		if ( &Pilote != NULL )
		{
			if ( Interne_ )
				delete Pilote_;

			Pilote_ = &Pilote;
			Interne_ = false;
		}
		else if ( !Interne_ )
			Pilote_ = NULL;
	}
	// Initialization.
	void Init( void )
	{
		if ( Pilote_ == NULL )
		{
			if ( ( Pilote_ = new UYM_DEFAULT_MEMORY_DRIVER ) == NULL )
				ERRa();
			else
			{
				Interne_ = true;
				((UYM_DEFAULT_MEMORY_DRIVER *)Pilote_)->Init();
			}
		}
	}
	MEMORY_DRIVER_ *Driver( bso__bool Ignore = false ) const
	{
#ifdef UYM_DBG
		if ( !Ignore && !Pilote_ )
			ERRu();
#endif
		return Pilote_;
	}
	// Operateur d'indirection.
	MEMORY_DRIVER_ *operator ->( void ) const
	{
		return Driver();
	}
	operator int( void ) const
	{
		return Pilote_ != NULL;
	}
};

//c Untyped memory core. Don't use; for internal use only.
class uym_basic_memory_
{
private:
	// Le pilote mémoire.
	uym_memory_driver Pilote_;
#ifdef UYM_DBG
	void Test_(
		mdr__position Position,
		mdr__bsize Nombre ) const
	{
		if ( !Pilote_ )
			ERRu();

		if ( ( Position + Nombre ) > S_.Size )
			ERRu();
	}
#endif
	void Lire_(
		mdr__position Position,
		mdr__bsize Nombre, 
		mdr__data *Tampon,
		bso__bool Ignore = false ) const
	{
#ifdef UYM_DBG
		if ( !Ignore )
			Test_( Position, Nombre );
#endif
		Pilote_->Read( Position, Nombre, Tampon );
	}
	// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets;
	void Ecrire_(
		const mdr__data *Tampon,
		mdr__bsize Nombre,
		mdr__position Position )
	{
#ifdef UYM_DBG
		Test_( Position, Nombre );
#endif
		Pilote_->Write( Tampon, Nombre, Position );
	}
	// écrit 'Nombre' octets à la position 'Position'
	void Allouer_( mdr__size Size )
	{
#ifdef UYM_DBG
		S_.Size = Size;

		if ( !Pilote_ )
			ERRu();
#endif
		Pilote_->Allocate( Size );
	}
	// alloue 'Capacite' octets
	void Synchroniser_( void ) const
	{
		if ( Pilote_ )
			Pilote_->Synchronize();
	}
public:
	struct s {
#ifdef UYM_DBG
		uym__size Size;
	} &S_;
#else
	};
#endif
	void reset( bool P )
	{
		Pilote_.reset( P );
#ifdef UYM_DBG
		S_.Size = 0;
#endif
	}
	uym_basic_memory_( s &S )
#ifdef UYM_DBG
	: S_( S )
#endif
	{
		reset( false );
	}
	void plug( MEMORY_DRIVER_ &Driver = *(MEMORY_DRIVER_ *)NULL )
	{
		Pilote_.plug( Driver );
	}
	//f Initialization.
	void Init( void )
	{
#ifdef UYM_DBG
		S_.Size = 0;
#endif
		Pilote_.Init();
	}
	void write(
		uym__position Position,
		uym__size Size,
		flw::oflow___ &OFlow ) const;
	void read(
		flw::iflow___  &IFlow,
		uym__position Position,
		uym__size Size );
	//f Allocates 'Capacity' bytes.
	void Allocate( uym__size Size )
	{
		Allouer_( Size );
	}
	/*f Put in 'Buffer' 'Amount' bytes at 'Position'.
	Ignore is only for 'UYM_DBG' mode and for the 'MMG' library.
	When 'true', it didn't make the test about the size. */
	void Read(
		uym__position Position,
		uym__bsize Amount,
		uym__data *Buffer,
		bso__bool Ignore = false ) const
	{
		Lire_( Position, Amount, Buffer, Ignore );
	}
	//f Wrtie to 'Position' 'Amount' bytes from 'Buffer'
	void Write(
		const uym__data *Buffer,
		uym__bsize Amount,
		uym__position Position )
	{
		Ecrire_( Buffer, Amount, Position );
	}
	//f Return byte at 'Position'.
	uym__data Read( uym__position Position ) const
	{
		uym__data D;

		Lire_( Position, 1, &D );

		return D;
	}
	//f Write 'Byte' at 'Position'.
	void Write(
		uym__data Byte,
		uym__position Position )
	{
		Ecrire_( &Byte, 1, Position );
	}
	/*f Write at 'Offset' in 'Destination' 'Quantity' bytes at 'Position'. */
	void Read(
		uym__position Position,
		uym__size Quantity,
		uym_basic_memory_ &Destination,
		uym__position Offset = 0 ) const
	{
		Destination.Write( *this, Quantity, Position, Offset );
	}
	/*f Write to 'Offset' 'Quantity' bytes at 'Position' from 'Source'. */
	void Write(
		const uym_basic_memory_ &Source,
		uym__size Size,
		uym__position Position = 0,
		uym__position Offset = 0 );
	//f Fill at 'Position' with 'Object' of size 'Size' 'Count' times.
	void Fill(
		const uym__data *Object,
		uym__bsize Size,
		uym__size Count,
		uym__position Position = 0 );
	//f Return the position from 'Object' of size 'Size' between 'Begin' and 'End' (excluded) or 'NONE' if non-existant.
	uym__position Position(
		const uym__data *Objet,
		uym__bsize Size,
		uym__position Begin,
		uym__position End );
	//f Return the used memory driver. 'Ignore' is only for 'UYM_DBG' mode and for the 'MMG' library.
	MEMORY_DRIVER_ *Driver( bso__bool Ignore = false )
	{
		return Pilote_.Driver( Ignore );
	}
	//f Flushes all the caches.
	void Synchronize( void ) const
	{
		Synchroniser_();
	}
};

//c Untyped memory core. Don't use; for internal use only.
class uym_basic_memory
: public uym_basic_memory_
{
private:
	uym_basic_memory_::s S_;
public:
	uym_basic_memory( void )
	: uym_basic_memory_( S_ )
	{}
};


//d A position take this value if an object cannot be find.
#define NONE	UYM_UNREACHABLE_POSITION

void UYMCopy_(
	const class uym_basic_memory_ &Source,
	uym__position PosSource,
	class uym_basic_memory_ &Dest,
	uym__position PosDest,
	uym__size Quantity,
	uym__data *Buffer,
	uym__bsize BufferSize );

//f Return 'E1' - 'E2' which begin at 'BeginS1' and 'BeginS2' and have a length of 'Quantity'.
bso__sbyte UYMCompare(
	const uym_basic_memory_ &M1,
	const uym_basic_memory_ &M2,
	uym__position BeginM1,
	uym__position BeginM2,
	uym__size Quantity );


//c Fixed-length memory core. Don't use; for internal use only.
class uym__memory_core_
{
private:
	uym__data *Data_;
public:
	uym__memory_core_( uym__data *Data )
	{
		Data_ = Data;
	}
	//f Initialization.
	void Init( void ){}
	//f Put in 'Buffer' 'Amount' bytes at 'Position'.
	void Read(
		uym__position Position,
		uym__bsize Amount,
		uym__data *Buffer ) const
	{
		memcpy( Buffer, Data_ + Position, Amount ); 
	}
	//f Write to 'Position' 'Amount' bytes from 'Buffer'.
	void Write(
		const uym__data *Buffer,
		uym__bsize Amount,
		uym__position Position )
	{
		memcpy( Data_ + Position, Buffer, Amount ); 
	}
	//f Return byte at 'Position'.
	uym__data Read( uym__position Position ) const
	{
		return Data_[Position];
	}
	//f Write 'Byte' at 'Position'.
	void Write(
		uym__data Byte,
		uym__position Position )
	{
		Data_[Position] = Byte;
	}
	/*f Write at 'Offset' in 'Destination' 'Quantity' bytes at 'Position'. */
	void Read(
		uym__position Position,
		uym__size Quantity,
		uym__memory_core_ &Destination,
		uym__position Offset = 0 ) const
	{
		memmove( Destination.Data_ + Position, Data_ + Position, Quantity );
	}
	/*f Write to 'Offset' 'Quantity' bytes at 'Position' from 'Source'. */
	void Write(
		const uym__memory_core_ &Source,
		uym__size Quantity,
		uym__position Position = 0,
		uym__position Offset = 0 )
	{
		memmove( Data_ + Offset, Source.Data_ + Position, Quantity ); 
	}
	//f Fill at 'Position' with 'Object' of size 'Size' 'Count' times.
	void Fill(
		const uym__data *Object,
		uym__bsize Size,
		uym__size Count,
		uym__position Position = 0 );
	//f Return the position from 'Object' of size 'Size' between 'Begin' and 'End' (excluded) oR 'NONE' if non-existant.
	uym__position Position(
		const uym__data *Objet,
		uym__bsize Size,
		uym__position Begin,
		uym__position End );
};


//c A memory with a size of 'size'.
template <int size> class uym__basic_memory_
: public uym__memory_core_
{
private:
	uym__data Data_[size];
public:
	struct s {};	// To simplify use in library 'SET'
	uym__basic_memory_( s &S = *(s *)NULL )
	: uym__memory_core_( Data_ ) {}
	// Simplifies the 'SET' library.
	void Allocate( uym__size Size )
	{
		if ( Size > size )
			ERRl();
	}
};



/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
