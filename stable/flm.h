/*
	Header for the 'flm' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2004 Claude SIMON (csimon@epeios.org).

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

#ifndef FLM__INC
#define FLM__INC

#define FLM_NAME		"FLM"

#define	FLM_VERSION	"$Revision$"

#define FLM_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &FLMTutor;

#if defined( XXX_DBG ) && !defined( FLM_NODBG )
#define FLM_DBG
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

namespace flm {
	typedef iop::amount__ position__;
	// type d�finissant une position dans la m�moire

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
	: public _io__
	{
	private:
		iop::descriptor__ D_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( D_ != IOP_UNDEFINED_DESCRIPTOR )
					fil::Close( D_ );
			}

			D_ = IOP_UNDEFINED_DESCRIPTOR;
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
			err::handle ErrHandle = err::hUsual )
		{
			reset();

			D_ = fil::Open( FileName, Mode );

			if ( D_ == IOP_UNDEFINED_DESCRIPTOR ) {
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

			_io__::Init( D_ );

			return fil::sSuccess;
		}
	};

	E_ROW( row__ );

	row__ _Register( class memoire_fichier_base___ &MFB );
	void _Unregister( row__ Row );
	void _ReportFileUsing( row__ Row );
	void _ReportFileClosing( row__ Row );

	class memoire_fichier_base___
	{
	private:
		_file___ File_;
		// nom du fichier
		char *Nom_;
		// taille du fichier
		iop::amount__ TailleFichier_;
		// diff�rents t�moins
		struct {
			int
				// signale si le Stream est ouvert ou non
				Ouvert			:1,
				// signale que la fermeture du fichier se fait manuellement
				Manuel  		:1,
				// Signale que le fichier ne doit pas �tre d�truit � la destruction de l'objet
				Persistant		:1,
				// Signale que le nom du fichier a �t� cr�e de mani�re interne
				Interne			:1;
				// Loi d'acc�s � la m�moire.
				mdr::mode__ Mode;
		} Temoin_;
		row__ _Row;	// Pour le suivi des 'file handler' ouverts.
	// Fonctions
		void Ouvrir_( void )
		{
			if ( !Temoin_.Ouvert )
			{
				if ( Temoin_.Mode == mdr::mReadOnly ) {
					if ( File_.Init( Nom_, fil::mReadOnly ) == fil::sFailure )
						ERRd();
				} else {
					if ( File_.Init( Nom_, fil::mReadWrite ) == fil::sFailure )
						ERRd();
				}

				Temoin_.Ouvert = 1;
			}

			_ReportFileUsing( _Row );
		}
	protected:
		void Lire(
			position__ Position,
			unsigned int Nombre,
			void *Tampon )
		{
			Ouvrir_();

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
				Liberer();
		}
			/* lit � partir de 'Taille' et place dans 'Tampon' 'Taille' octets
			retourne le nombre d'octets effectivement lus */
		void Ecrire(
			const void *Tampon,
			unsigned int Nombre,
			position__ Position )
		{
			Ouvrir_();

			iop::amount__ Amount;

			File_.Seek( Position );

			while( Nombre > 0 ) {
			
				Amount = File_.Write( Tampon, Nombre );
			
				Tampon = (char *)Tampon + Amount;
				Nombre -= Amount;
			}

			if ( !Temoin_.Manuel )
				Liberer();
		}
			/* �crit 'Taille' octet � la position 'Position' dans 'Tampon';
			agrandit la m�moire si n�cessaire */
		void Allouer( iop::amount__ Capacite )
		{
			if ( ( TailleFichier_ == 0 ) && tol::FileExists( Nom_ ) )
				TailleFichier_ = tol::GetFileSize( Nom_ );

			if ( Capacite > TailleFichier_ )
			{
				mdr::datum__ Datum = 0;

				Ouvrir_();
				
				File_.Seek( Capacite - (iop::amount__)1 );

				if ( File_.Write( &Datum, 1 ) != 1 ) {
					if ( !Temoin_.Manuel )
						Liberer();

					ERRm();
				} else
					if ( !Temoin_.Manuel )
						Liberer();

				TailleFichier_ = Capacite;
			}
		}
		// alloue 'Capacite' octets
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
				Liberer();

				if ( _Row != NONE )
					_Unregister( _Row );

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
			Temoin_.Mode = mdr::mReadOnly;
			TailleFichier_ = 0;
			_Row = NONE;
		}
		void Init(
			const char *NomFichier = NULL,
			mdr::mode__ Mode = mdr::mReadWrite,
			flm::creation Creation = flm::cFirstUse )
		{
			if ( NomFichier )
			{
				reset();

				if ( ( Nom_ = (char *)malloc( strlen( NomFichier ) + 1 ) ) == NULL )
					ERRa();

				strcpy( Nom_, NomFichier );

				Temoin_.Interne = false;

				_Row = _Register( *this );
			}
			else if ( !Temoin_.Interne )
			{
				reset();

				if ( ( Nom_ = (char *)malloc( L_tmpnam ) ) == NULL )
					ERRa();

				tmpnam( Nom_ );

				Temoin_.Interne = true;

				_Row = _Register( *this );
			}

			Temoin_.Mode = Mode;

			if ( Creation == flm::cNow  )
				if ( Mode == mdr::mReadWrite )
					Ouvrir_();
				else
					ERRu();
			else if ( Creation != flm::cFirstUse )
				ERRu();
		}
			// initialise l'objet avec le nom 'NomFichier'; si NULL, cr�ation d'un nom
		void Liberer( void )
		{
			if ( Temoin_.Ouvert ) {
				File_.reset();

				_ReportFileClosing( _Row );
			}

			Temoin_.Ouvert = 0;
		}
			// lib�re le File Descriptor
		void Manuel( void )
		{
			Temoin_.Manuel = 1;
		}
			// indique que la fermeture du fichier se fait manuellement
		void Automatique( void )
		{
			Temoin_.Manuel = 0;
		}
			// indique que la fermeture du fichier se fait automatiquement
		void Persistant( void )
		{
			Temoin_.Persistant = true;
		}
		// bascule en mode d'acces 'Acces'.
		void Mode( mdr::mode__ Mode )
		{
			if ( Temoin_.Mode != Mode )
			{
				Temoin_.Mode = Mode;
				Liberer();
			}
		}
		// Retourne le mode d'acc�s.
		mdr::mode__ Mode( void )
		{
			return Temoin_.Mode;
		}
		mdr::size__ Size( void )
		{
			Ouvrir_();

			return File_.Size();
		}
		void Drop( void ) // Efface le fichier sous-jacent, s'il existe.
		{
			Liberer();

			if ( ( Nom_ != NULL ) && tol::FileExists( Nom_ ) )
				if ( remove( Nom_ ) != 0 )
					ERRd();

			TailleFichier_ = 0;
		}
		const char *FileName( void ) const
		{
			return Nom_;
		}
	};

	//c The standard memory driver which handle a file as memory.
	class file_memory_driver___
	: public mdr::memory_driver__,
	  public memoire_fichier_base___
	{
	protected:
		virtual void MDRRecall(
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer )
		{
#ifdef FLM_DBG
			if ( Amount > UINT_MAX )
				ERRl();
#endif
			memoire_fichier_base___::Lire( (position__)Position, (unsigned int)Amount, Buffer );
		}
		// lit � partir de 'Position' et place dans 'Tampon' 'Nombre' octets
		virtual void MDRStore(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			mdr::row_t__ Position )
		{
#ifdef FLM_DBG
			if ( Amount > UINT_MAX )
				ERRl();
#endif
			memoire_fichier_base___::Ecrire( Buffer, (unsigned int)Amount, (position__)Position );
		}
		// �crit 'Nombre' octets � la position 'Position'
		virtual void MDRAllocate( mdr::size__ Size )
		{
			memoire_fichier_base___::Allouer( (iop::amount__)Size );
		}
		// alloue 'Taille' octets
	public:
		file_memory_driver___( mdr::size__ &Extent )
		: memory_driver__( Extent ),
		  memoire_fichier_base___()
		{}
		void reset( bool P = true )
		{
			memoire_fichier_base___::reset( P );
			E_MEMORY_DRIVER__::reset( P );
		}
		//f Return the mode.
		mdr::mode__ Mode( void )
		{
			return memoire_fichier_base___::Mode();
		}
		//f 'Mode' becomes the mode.
		void Mode( mdr::mode__ Mode )
		{
			memoire_fichier_base___::Mode( Mode );
		}
		//f Initialize using 'Filename' as file, open it in mode 'Mode'.
		void Init(
			const char *FileName = NULL,
			mdr::mode__ Mode = mdr::mReadWrite,
			flm::creation Creation = flm::cFirstUse )
		{
			memoire_fichier_base___::Init( FileName, Mode, Creation );
			E_MEMORY_DRIVER__::Init();
		}
	};

	typedef mdr::E_STANDALONE_MEMORY_DRIVER__( flm::file_memory_driver___ ) standalone_file_memory_driver___;

	#define E_FILE_MEMORY_DRIVER___	standalone_file_memory_driver___
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
