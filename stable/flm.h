/*
	Header for the 'flm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

#ifndef FLM__INC
#define FLM__INC

#define FLM_NAME		"FLM"

#define	FLM_VERSION	"$Revision$"

#define FLM_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &FLMTutor;

#if defined( XXX_DBG ) && !defined( FLM_NODBG )
#define FLM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D FiLe Memory 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/
//D FiLe Memory.

#include "cpe.h"
#include "tol.h"
#include "err.h"
#include "flw.h"
#include "mdr.h"
#include "iop.h"
#include "fil.h"
#include "epeios.h"

#if defined( CPE__T_MS )
#	define FLM_DEFAULT_MAX_FILE_AMOUNT	1000
#elif defined ( CPE__T_LINUX )
#	define FLM_DEFAULT_MAX_FILE_AMOUNT	800	// Linux, par défaut, ne peut ouvrir que 1024 descripteurs (socket comprises).
#elif defined ( CPE__T_MAC )
#	define FLM_DEFAULT_MAX_FILE_AMOUNT	200	// Mac, par défaut, ne peut ouvrir que 256 descripteurs (socket comprises).
#else
#	error "Unimplemented target !"
#endif


#ifdef FLM_MAX_FILE_AMOUNT
#	define FLM__MAX_FILE_AMOUNT	FLM_MAX_FILE_AMOUNT
#else
#	define FLM__MAX_FILE_AMOUNT	FLM_DEFAULT_MAX_FILE_AMOUNT
#endif


#ifndef FLM_NO_MT
#	ifdef CPE__T_MT
#		define FLM__MT
#	endif
#endif


#ifndef FLM_NO_AUTOFLUSH
#	ifdef FLM__MT
#		define FLM__AUTOFLUSH
#	endif
#endif


namespace flm {
	extern epeios::size__ MaxFileAmount;

	typedef iop::amount__ position__;
	// type définissant une position dans la mémoire

	// Identifiant sous lequel est regroupé un ensemble de fichiers.
	E_ROW( id__ );
	#define FLM_UNDEFINED_ID	NONE

	id__ GetId( void );

	void ReleaseId( id__ ID );

	/*******************************************************************/
	/* CLASSE DE BASE DE GESTION D'UNE MEMOIRE STOCKEE DANS UN FICHIER */
	/*******************************************************************/

	enum creation
	{
		cNow,
		cFirstUse
	};

	typedef iop::io__		_io__;

	class _file___
	{
	private:
		iop::descriptor__ _D;
		_io__ _Core;
#ifdef FLM__MT
		mtx::mutex_handler__ _Mutex;
#endif
		void _Lock( void )
		{
#ifdef FLM__MT
			mtx::Lock( _Mutex );
#endif
		}
		void _Unlock( void )
		{
#ifdef FLM__MT
			mtx::Unlock( _Mutex );
#endif
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _D != IOP_UNDEFINED_DESCRIPTOR )
					fil::Close( _D );
#ifdef FLM__MT
				if ( _Mutex != MTX_INVALID_HANDLER )
					mtx::Delete( _Mutex );
#endif
			}

			_D = IOP_UNDEFINED_DESCRIPTOR;
#ifdef FLM__MT
			_Mutex = MTX_INVALID_HANDLER;
#endif
		}
		_file___( void )
		{
			reset( false );
		}
		~_file___( void )
		{
			reset();
		}
		fil::status__ Init(
			const char *FileName,
			fil::mode__ Mode,
			bso::bool__ FlushToDevice,
			err::handle ErrHandle = err::hUsual )
		{
			reset();

#ifdef FLM__MT
			_Mutex = mtx::Create( mtx::mOwned );
#endif

			_D = fil::Open( FileName, Mode );

			if ( _D == IOP_UNDEFINED_DESCRIPTOR ) {
				switch ( ErrHandle ) {
				case err::hSkip:
					return fil::sFailure;
					break;
				case err::hUsual:
					ERRd();
					break;
				default:
					ERRu();
					break;
				}
			}

			_Core.Init( _D, FlushToDevice );

			return fil::sSuccess;
		}
		void Lock( void )
		{
			_Lock();
		}
		void Unlock( void )
		{
			_Unlock();
		}
		void Seek( long Offset )
		{
			_Lock();

			_Core.Seek( Offset );

			_Unlock();
		}
		unsigned int Read(
			unsigned int Amount,
			void *Buffer )
		{
			_Lock();

			Amount = _Core.Read( Amount, Buffer );

			_Unlock();

			return Amount;
		}
		unsigned int Write(
			const void *Buffer,
			unsigned int Amount )
		{
			_Lock();

			Amount = _Core.Write( Buffer, Amount );

			_Unlock();

			return Amount;
		}
		mdr::size__ Size( void )
		{
			_Lock();

			mdr::size__ Size = _Core.Size();

			_Unlock();

			return Size;
		}
		void ThreadUnsafeFlush( void )
		{
			_Core.Flush();
		}
		void Flush( void )
		{
			_Lock();

			ThreadUnsafeFlush();

			_Unlock();
		}
	};

	E_ROW( row__ );

	row__ _Register(
		class memoire_fichier_base___ &MFB,
		id__ ID );
	void _Unregister(
		row__ Row,
		id__ ID );
	void _ReportFileUsing(
		row__ Row,
		bso::bool__ ToFlush );
	void _ReportFileClosing( row__ Row );

	void ReleaseFiles( id__ ID );

	class memoire_fichier_base___
	{
	private:
		_file___ File_;
		// nom du fichier
		char *Nom_;
		// taille du fichier
		iop::amount__ TailleFichier_;
		struct {
			int
				// signale si le Stream est ouvert ou non
				Ouvert			:1,
				// signale que la fermeture du fichier se fait manuellement
				Manuel  		:1,
				// Signale que le fichier ne doit pas être détruit à la destruction de l'objet
				Persistant		:1,
				// Signale que le nom du fichier a été crée de manière interne
				Interne			:1;
				// Loi d'accés à la mémoire.
				fil::mode__ Mode;
		} Temoin_;
		row__ _Row;	// Pour le suivi des 'file handler' ouverts.
		// différents témoins
		time_t _LastAccessTime;	// Last access time.
		id__ _ID;
		bso::bool__ _FlushToDevice;	// Voir 'wllio' ou 'pllio'.
	// Fonctions
		bso::bool__ Open_(
			bso::bool__ ToFlush,
			err::handle ErrHandle = err::hUsual )	// Si à 'true', le fichier doit être 'flushé' (accés en écriture).
		{
			bso::bool__ Success = true;

			if ( !Temoin_.Ouvert )
			{
				Success = File_.Init( Nom_, Temoin_.Mode, _FlushToDevice, ErrHandle ) == fil::sSuccess;

				if ( Success )
					Temoin_.Ouvert = 1;
			}
			
			if ( Success ) {
				_ReportFileUsing( _Row, ToFlush );
				_LastAccessTime = tol::Clock( false );
			}

			return Success;
		}
	protected:
		void Read(
			position__ Position,
			unsigned int Nombre,
			void *Tampon )
		{
			Open_( false );

			iop::amount__ Amount;

			File_.Seek( Position );
				
			while( Nombre > 0 ) {
				
				Amount = File_.Read( Nombre, Tampon );
					
				if ( Amount <= 0 )
					ERRd();
					
				Nombre -= Amount;
				Tampon = (char *)Tampon + Amount;
			}

			if ( !Temoin_.Manuel )
				ReleaseFile();
		}
			/* lit à partir de 'Taille' et place dans 'Tampon' 'Taille' octets
			retourne le nombre d'octets effectivement lus */
		void Write(
			const void *Tampon,
			unsigned int Nombre,
			position__ Position )
		{
			Open_( true );

			iop::amount__ Amount;

			File_.Seek( Position );

			while( Nombre > 0 ) {
			
				Amount = File_.Write( Tampon, Nombre );
			
				Tampon = (char *)Tampon + Amount;
				Nombre -= Amount;
			}

			if ( !Temoin_.Manuel )
				ReleaseFile();
/*			else
				File_.Flush();
*/		}
		void Allocate( iop::amount__ Capacite )
		{
			if ( ( TailleFichier_ == 0 ) && fil::FileExists( Nom_ ) )
				TailleFichier_ = fil::GetFileSize( Nom_ );

			if ( Capacite > TailleFichier_ )
			{
				mdr::datum__ Datum = 0;

/*				Open_( true );
				
				File_.Seek( Capacite - (iop::amount__)1 );

				if ( File_.Write( &Datum, 1 ) != 1 ) {
					if ( !Temoin_.Manuel )
						ReleaseFile();

					ERRm();
				} else
					if ( !Temoin_.Manuel )
						ReleaseFile();
*/

				TailleFichier_ = Capacite;
			}
		}
	public:
		memoire_fichier_base___( void )
		{
			reset( false );
		}
		~memoire_fichier_base___( void )
		{
			reset();
		}
		void reset( bool P  = true )
		{
			if ( P )
			{
				ReleaseFile();

				if ( _Row != NONE )
					_Unregister( _Row, _ID );

				if ( Nom_ )
				{
					if ( !Temoin_.Persistant )
						remove( Nom_ );

					free( Nom_ );
				}
			}

			Nom_ = NULL;
			Temoin_.Ouvert = false;
			Temoin_.Manuel = true;
			Temoin_.Persistant = false;
			Temoin_.Interne = false;
			Temoin_.Mode = fil::mReadOnly;
			TailleFichier_ = 0;
			_Row = NONE;
			_ID = FLM_UNDEFINED_ID;
			_LastAccessTime = 0;
			_FlushToDevice = false;
		}
		void Init(
			id__ ID,
			bso::bool__ FlushToDevice,
			const char *NomFichier = NULL,
			fil::mode__ Mode = fil::mReadWrite,
			flm::creation Creation = flm::cFirstUse )
		{
			if ( NomFichier )
			{
				reset();

				if ( ( Nom_ = (char *)malloc( strlen( NomFichier ) + 1 ) ) == NULL )
					ERRa();

				strcpy( Nom_, NomFichier );

				Temoin_.Interne = false;

				_ID = ID;

				_Row = _Register( *this, _ID );

				_FlushToDevice = FlushToDevice;
			}
			else if ( !Temoin_.Interne )
			{
				reset();

				if ( ( Nom_ = (char *)malloc( L_tmpnam ) ) == NULL )
					ERRa();

				if ( tmpnam( Nom_ ) == NULL )
					ERRs();

				Temoin_.Interne = true;

				_ID = ID;

				_Row = _Register( *this, _ID );
			}

			Temoin_.Mode = Mode;

			if ( Creation == flm::cNow  )
				if ( Mode == fil::mReadWrite )
					Open_( false );
				else
					ERRu();
			else if ( Creation != flm::cFirstUse )
				ERRu();
		}
			// initialise l'objet avec le nom 'NomFichier'; si NULL, création d'un nom
		void ReleaseFile( bso::bool__ ReportClosing = true )
		{
			if ( Temoin_.Ouvert ) {
				File_.reset();

				if ( ReportClosing )
					_ReportFileClosing( _Row );
			}

			Temoin_.Ouvert = 0;
		}
			// libère le File Descriptor
		void Manual( void )
		{
			Temoin_.Manuel = 1;
		}
			// indique que la fermeture du fichier se fait manuellement
		void Automatic( void )
		{
			Temoin_.Manuel = 0;
		}
			// indique que la fermeture du fichier se fait automatiquement
		void Persistent( void )
		{
			Temoin_.Persistant = true;
		}
		bso::bool__ IsPersistent( void ) const
		{
			return Temoin_.Persistant != 0;
		}
		// bascule en mode d'acces 'Acces'.
		void Mode( fil::mode__ Mode )
		{
			if ( Temoin_.Mode != Mode )
			{
				Temoin_.Mode = Mode;
				ReleaseFile();
			}
		}
		// Retourne le mode d'accés.
		fil::mode__ Mode( void )
		{
			return Temoin_.Mode;
		}
		void Drop( void ) // Efface le fichier sous-jacent, s'il existe.
		{
			ReleaseFile();

			if ( ( Nom_ != NULL ) && fil::FileExists( Nom_ ) )
				if ( remove( Nom_ ) != 0 )
					ERRd();

			TailleFichier_ = 0;
		}
		const char *GetFileName( void ) const
		{
			return Nom_;
		}
		bso::bool__ Exists( void ) const
		{
			return fil::FileExists( Nom_ );
		}
		time_t TimeStamp( void ) const
		{
			return fil::GetFileLastModificationTime( Nom_ );
		}
		mdr::size__ FileSize( void )
		{
#	if 0
			Open_( false );

			return File_.Size();
#	else
			return fil::GetFileSize( Nom_ );
#	endif
		}
#ifdef CPE__C_VC
#	undef CreateFile
#endif
		bso::bool__ CreateFile( err::handle ErrHandle = err::hUsual )
		{
			return Open_( false, ErrHandle );
		}
		E_RODISCLOSE__( time_t, LastAccessTime );
		_file___ &File( void )
		{
			return File_;
		}
	};

	//c The standard memory driver which handle a file as memory.
	class file_memory_driver___
	: public mdr::memory_driver__,
	  public memoire_fichier_base___
	{
	protected:
		virtual void MDRAllocate( mdr::size__ Size )
		{
			memoire_fichier_base___::Allocate( (iop::amount__)Size );
		}
		// alloue 'Taille' octets
		virtual mdr::size__ MDRUnderlyingSize( void )
		{
			if ( Exists() )
				return FileSize();
			else
				return 0;
		}
		virtual void MDRRecall(
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer )
		{
#ifdef FLM_DBG
			if ( Amount > UINT_MAX )
				ERRl();
#endif
			memoire_fichier_base___::Read( (position__)Position, (unsigned int)Amount, Buffer );
		}
		// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets
		virtual void MDRStore(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			mdr::row_t__ Position )
		{
#ifdef FLM_DBG
			if ( Amount > UINT_MAX )
				ERRl();
#endif
			memoire_fichier_base___::Write( Buffer, (unsigned int)Amount, (position__)Position );
		}
		// écrit 'Nombre' octets à la position 'Position'
	public:
		file_memory_driver___( void )
		: memory_driver__(),
		  memoire_fichier_base___()
		{}
		void reset( bool P = true )
		{
			memoire_fichier_base___::reset( P );
			E_MEMORY_DRIVER__::reset( P );
		}
		//f Return the mode.
		fil::mode__ Mode( void )
		{
			return memoire_fichier_base___::Mode();
		}
		//f 'Mode' becomes the mode.
		void Mode( fil::mode__ Mode )
		{
			memoire_fichier_base___::Mode( Mode );
		}
		//f Initialize using 'Filename' as file, open it in mode 'Mode'.
		void Init(
			id__ ID,
			bso::bool__ FlushToDevice,
			const char *FileName = NULL,
			fil::mode__ Mode = fil::mReadWrite,
			flm::creation Creation = flm::cFirstUse )
		{
			memoire_fichier_base___::Init( ID, FlushToDevice, FileName, Mode, Creation );
			E_MEMORY_DRIVER__::Init();
		}
	};

	#define E_FILE_MEMORY_DRIVER___	file_memory_driver___

	void ReleaseInactiveFiles(
		time_t Delay,
		bso::ulong__ MaxAmount = BSO_ULONG_MAX ); // Releases up to 'MaxAmount' files not accessed since 'Delay' ms. Thread-safe.
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
