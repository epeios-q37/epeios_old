/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'entete.h' template file V1.3 */
/*
Version:
	1.0.6 30/03/2000 06:26:01

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	FiLe Memory driver
*/

/* Begin of automatic documentation generation part. */

//V 1.0.6
//C Claude SIMON
//D FiLe Memory driver
//R 30/03/2000 06:26:01

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.3 */

#ifndef FLM__INC
#define FLM__INC

#define FLM_NAME		"FLM"

#define	FLM_VERSION		"1.0.6"	
#define FLM_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#include "ttr.h"

extern class ttr_tutor &FLMTutor;

#if defined( XXX_DBG ) && !defined( FLM_NODBG )
#define FLM_DBG 
#endif

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
// type définissant une capacite mémoire

typedef flm__capacite flm__position;
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

class flm_memoire_fichier_base
{
	// le stream servant de mémoire
	fstream Stream_;
	// nom du fichier
	char *Nom_;
	// taille du fichier
	flm__capacite TailleFichier_;
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
		/* lit à partir de 'Taille' et place dans 'Tampon' 'Taille' octets
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
		/* écrit 'Taille' octet à la position 'Position' dans 'Tampon';
		agrandit la mémoire si nécessaire */
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
		// initialise l'objet avec le nom 'NomFichier'; si NULL, création d'un nom
	void Liberer( void )
	{
		if ( Temoin_.Ouvert )
			Stream_.close();

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
	// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets
	virtual void MDRWrite(
		const mdr__data *Buffer,
		mdr__bsize Amount,
		mdr__position Position )
	{
		flm_memoire_fichier_base::Ecrire( Buffer, Amount, Position );
	}
	// écrit 'Nombre' octets à la position 'Position'
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
/* end of 'xxx.h' template file */