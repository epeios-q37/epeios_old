/*
  Header for the 'flm' library by Claude L. Simon (simon@epeios.org)
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

#ifndef FLM__INC
#define FLM__INC

#define FLM_NAME		"FLM"

#define	FLM_VERSION	"$Revision$"	

#define FLM_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &FLMTutor;

#if defined( XXX_DBG ) && !defined( FLM_NODBG )
#define FLM_DBG 
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

#include <fstream.h>

#include "err.h"
#include "flw.h"
#include "mdr.h"

typedef streampos flm__capacite;
// type d�finissant une capacite m�moire

typedef flm__capacite flm__position;
// type d�finissant une position dans la m�moire

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

class flm_memoire_fichier_base
{
	// le stream servant de m�moire
	fstream Stream_;
	// nom du fichier
	char *Nom_;
	// taille du fichier
	flm__capacite TailleFichier_;
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
			mdr::mode Mode;
	} Temoin_;
// Fonctions
	void Ouvrir_( void )
	{
		if ( !Temoin_.Ouvert )
		{
			Stream_.open( Nom_, ios::in | ( Temoin_.Mode == mdr::mReadOnly ? 0 : ios::out ) | ios::binary );

			if ( Stream_.fail() )
				ERRd();

			Temoin_.Ouvert = 1;
		}
	}
protected:
	void Lire(
		streampos Position,
		int Nombre,
		void *Tampon )
	{
		Ouvrir_();

		if ( Stream_.seekg( Position ).fail() )
			ERRd();

		if ( Stream_.read( (char *)Tampon, Nombre ).fail() )
			ERRd();

		if ( !Temoin_.Manuel )
			Liberer();
	}
		/* lit � partir de 'Taille' et place dans 'Tampon' 'Taille' octets
		retourne le nombre d'octets effectivement lus */
	void Ecrire(
		const void *Tampon,
		int Nombre,
		streampos Position )
	{
		Ouvrir_();

		if ( Stream_.seekp( Position ).fail() )
			ERRd();

		if ( Stream_.write( (char *)Tampon, Nombre ).fail() )
			ERRd();

		if ( !Temoin_.Manuel )
			Liberer();
	}
		/* �crit 'Taille' octet � la position 'Position' dans 'Tampon';
		agrandit la m�moire si n�cessaire */
	void Allouer( flm__capacite Capacite )
	{
		if ( Capacite > TailleFichier_ )
		{
			Ouvrir_();

			if ( Stream_.seekp( Capacite - 1 ).fail() || Stream_.put( (char)0 ).fail() )
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
	void plug( class mmm_multimemory_ & )
	{
		// Standardization.
	}
	flm_memoire_fichier_base( void )
	{
		reset( false );
	}
	virtual ~flm_memoire_fichier_base( void )
	{
		reset( true );
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
		// initialise l'objet avec le nom 'NomFichier'; si NULL, cr�ation d'un nom
	void Liberer( void )
	{
		if ( Temoin_.Ouvert )
			Stream_.close();

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
	void Mode( mdr::mode Mode )
	{
		if ( Temoin_.Mode != Mode )
		{
			Temoin_.Mode = Mode;
			Liberer();
		}
	}
	// Retourne le mode d'acc�s.
	mdr::mode Mode( void )
	{
		return Temoin_.Mode;
	}
};

//c The standard memory driver which handle a file as memory.
class flm_file_memory_driver
: public MEMORY_DRIVER,
  public flm_memoire_fichier_base
{
protected:
	virtual void MDRRead(
		mdr__position Position,
		mdr__bsize Amount,
		mdr__data *Buffer )
	{
		flm_memoire_fichier_base::Lire( Position, Amount, Buffer );
	}
	// lit � partir de 'Position' et place dans 'Tampon' 'Nombre' octets
	virtual void MDRWrite(
		const mdr__data *Buffer,
		mdr__bsize Amount,
		mdr__position Position )
	{
		flm_memoire_fichier_base::Ecrire( Buffer, Amount, Position );
	}
	// �crit 'Nombre' octets � la position 'Position'
	virtual void MDRAllocate( mdr__size Size )
	{
		flm_memoire_fichier_base::Allouer( Size );
	}
	// alloue 'Taille' octets
public:
	flm_file_memory_driver( void )
	{
		reset( false );
	}
	virtual ~flm_file_memory_driver( void )
	{
		reset( true );
	}
	void plug( class mmm_multimemory_ &M )
	{
		flm_memoire_fichier_base::plug( M );
		MEMORY_DRIVER::plug( M );
	}
	void reset( bool P = true )
	{
		flm_memoire_fichier_base::reset( P );
		MEMORY_DRIVER::reset( P );
	}
	//f Return the mode.
	mdr::mode Mode( void )
	{
		return flm_memoire_fichier_base::Mode();
	}
	//f 'Mode' becomes the mode.
	void Mode( mdr::mode Mode )
	{
		flm_memoire_fichier_base::Mode( Mode );
	}
	//f Initialize using 'Filename' as file, open it in mode 'Mode'.
	void Init(
		const char *FileName = NULL,
		mdr::mode Mode = mdr::mReadWrite,
		flm::creation Creation = flm::cFirstUse )
	{
		flm_memoire_fichier_base::Init( FileName, Mode, Creation );
		MEMORY_DRIVER::Init();
	}
};

typedef flm_file_memory_driver flm_file_memory_driver_;

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
