/*
	Header for the 'flm' library by Claude SIMON (csimon@epeios.org)
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
#include "fil.h"

namespace flm {
	using namespace mdr;

	using iof::amount__;

	typedef amount__ position__;
	// type définissant une position dans la mémoire

	/*******************************************************************/
	/* CLASSE DE BASE DE GESTION D'UNE MEMOIRE STOCKEE DANS UN FICHIER */
	/*******************************************************************/

	struct flm
	{
		enum creation
		{
			cNow,
			cFirstUse
		};
	};

	class memoire_fichier_base_
	{
	private:
		fil::file___ File_;
		// nom du fichier
		char *Nom_;
		// taille du fichier
		amount__ TailleFichier_;
		// différents témoins
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
				mdr::mode Mode;
		} Temoin_;
	// Fonctions
		void Ouvrir_( void )
		{
			if ( !Temoin_.Ouvert )
			{
				if ( Temoin_.Mode == mdr::mReadOnly ) {
					if ( File_.Init( Nom_, fil::mReadOnly ) == iof::sFailure )
						ERRd();
				} else {
					if ( File_.Init( Nom_, fil::mReadWrite ) == iof::sFailure )
						ERRd();
				}
				Temoin_.Ouvert = 1;
			}
		}
	protected:
		void Lire(
			position__ Position,
			unsigned int Nombre,
			void *Tampon )
		{
			Ouvrir_();

			amount__ Amount;

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
			/* lit à partir de 'Taille' et place dans 'Tampon' 'Taille' octets
			retourne le nombre d'octets effectivement lus */
		void Ecrire(
			const void *Tampon,
			unsigned int Nombre,
			position__ Position )
		{
			Ouvrir_();

			amount__ Amount;

			File_.Seek( Position );

			while( Nombre > 0 ) {
			
				Amount = File_.Write( Tampon, Nombre );
			
				Tampon = (char *)Tampon + Amount;
				Nombre -= Amount;
			}

			if ( !Temoin_.Manuel )
				Liberer();
		}
			/* écrit 'Taille' octet à la position 'Position' dans 'Tampon';
			agrandit la mémoire si nécessaire */
		void Allouer( amount__ Capacite )
		{
			if ( Capacite > TailleFichier_ )
			{
				Ouvrir_();
				
				File_.Seek( Capacite - (amount__)1 );

				if ( File_.Write( &Capacite, 1 ) != 1 )
				{
					if ( !Temoin_.Manuel )
						Liberer();

					ERRm();
				}
				else
					if ( !Temoin_.Manuel )
						Liberer();

				TailleFichier_ = Capacite;
			}
		}
		// alloue 'Capacite' octets
	public:
		struct s
		{
			// Standardization.
		};
		memoire_fichier_base_( s &S )
		{
			// Standardization.
		}
		void reset( bool P  = true )
		{
			if ( P )
			{
				Liberer();

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
		}
		memoire_fichier_base_ &operator =( const memoire_fichier_base_ &MFB )
		{
			ERRu();	// The '=' operator has no sens for this object. It is defined only because it is neede by some macros.

			return *this;
		}
		void Init(
			const char *NomFichier = NULL,
			mdr::mode Mode = mdr::mReadWrite,
			flm::creation Creation = flm::cFirstUse )
		{
			if ( NomFichier )
			{
				reset();

				if ( ( Nom_ = (char *)malloc( strlen( NomFichier ) + 1 ) ) == NULL )
					ERRa();

				strcpy( Nom_, NomFichier );

				Temoin_.Interne = false;
			}
			else if ( !Temoin_.Interne )
			{
				reset();

				if ( ( Nom_ = (char *)malloc( L_tmpnam ) ) == NULL )
					ERRa();

				tmpnam( Nom_ );

				Temoin_.Interne = true;
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
			// initialise l'objet avec le nom 'NomFichier'; si NULL, création d'un nom
		void Liberer( void )
		{
			if ( Temoin_.Ouvert )
				File_.reset();

			Temoin_.Ouvert = 0;
		}
			// libère le File Descriptor
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
		void Mode( mdr::mode Mode )
		{
			if ( Temoin_.Mode != Mode )
			{
				Temoin_.Mode = Mode;
				Liberer();
			}
		}
		// Retourne le mode d'accés.
		mdr::mode Mode( void )
		{
			return Temoin_.Mode;
		}
	};

	//c The standard memory driver which handle a file as memory.
	class file_memory_driver_
	: public E_MEMORY_DRIVER_,
	  public memoire_fichier_base_
	{
	protected:
		virtual void MDRRecall(
			row__ Position,
			mdr::bsize__ Amount,
			datum__ *Buffer )
		{
			memoire_fichier_base_::Lire( (position__)Position, Amount, Buffer );
		}
		// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets
		virtual void MDRStore(
			const datum__ *Buffer,
			mdr::bsize__ Amount,
			row__ Position )
		{
			memoire_fichier_base_::Ecrire( Buffer, Amount, (position__)Position );
		}
		// écrit 'Nombre' octets à la position 'Position'
		virtual void MDRAllocate( mdr::size__ Size )
		{
			memoire_fichier_base_::Allouer( (amount__)Size );
		}
		// alloue 'Taille' octets
	public:
		struct s
		: public E_MEMORY_DRIVER_::s,
		  memoire_fichier_base_::s
		{};
		file_memory_driver_( s &S )
		: E_MEMORY_DRIVER_( S ),
		  memoire_fichier_base_( S )
		{}
		void reset( bool P = true )
		{
			memoire_fichier_base_::reset( P );
			E_MEMORY_DRIVER_::reset( P );
		}
		//f Return the mode.
		mdr::mode Mode( void )
		{
			return memoire_fichier_base_::Mode();
		}
		//f 'Mode' becomes the mode.
		void Mode( mdr::mode Mode )
		{
			memoire_fichier_base_::Mode( Mode );
		}
		file_memory_driver_ &operator =( const file_memory_driver_ &FMD )
		{
			memoire_fichier_base_::operator =( FMD );
			E_MEMORY_DRIVER_::operator =( FMD );

			return *this;
		}
		//f Initialize using 'Filename' as file, open it in mode 'Mode'.
		void Init(
			const char *FileName = NULL,
			mdr::mode Mode = mdr::mReadWrite,
			flm::creation Creation = flm::cFirstUse )
		{
			memoire_fichier_base_::Init( FileName, Mode, Creation );
			E_MEMORY_DRIVER_::Init();
		}
	};

	E_AUTO( file_memory_driver )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
