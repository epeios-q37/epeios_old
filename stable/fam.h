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
	1.1.0 27/05/2000 10:26:17

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	Fast Allocation Memory
*/

/* Begin of automatic documentation generation part. */

//V 1.1.0
//C Claude SIMON
//D Fast Allocation Memory
//R 27/05/2000 10:26:17

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.3 */

#ifndef FAM__INC
#define FAM__INC

#define FAM_NAME		"FAM"

#define	FAM_VERSION		"1.1.0"	
#define FAM_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#include "ttr.h"

extern class ttr_tutor &FAMTutor;

#if defined( XXX_DBG ) && !defined( FAM_NODBG )
#define FAM_DBG 
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "cpe.h"

typedef bso__bsize	fam__size;

// Taille à vide du tas.
#ifndef FAM_SIZE
#define FAM_SIZE	60000UL
#endif

//c A fast allocation memory.
class fam_fast_allocation_memory {
	// Le tas.
	char Tas_[FAM_SIZE];
	// La réserve disponible; se situe toujours en fin de tas.
	fam__size Restant_;
	/* Retourne la taille pouvant être accordée sur la réserve.
	'Optimal', un multiple de 'Min', ou 0 (dans cet ordre). */
	fam__size Taille_(
		fam__size Optimal,
		fam__size Min )
	{
		if ( ( 2UL * Optimal ) > Restant_ )
			return Min * ( Restant_ / ( 2UL * Min ) );
		else
			return Optimal;
	}
public:
	fam_fast_allocation_memory( void )
	{
		Restant_ = sizeof( Tas_ );
	}
	/*f Return pointer of memory of 'Optimal' size or a size multiple of 'Minimum'.
	If NULL returned, allocation cannot be done. The allocated amount is taken in 'Optimal'. */
	void *Allocate(
		fam__size &Optimal,
		fam__size Minimum )
	{
		void *Resultat;
#ifdef CPE__MT
		fam__size Taille = 0;
#else
		fam__size Taille = Taille_( Optimal, Minimum );
#endif

		if ( !Taille )
		{
			if ( ( Resultat = malloc( Optimal ) ) == NULL )
				Resultat = malloc( Minimum );

			return Resultat;
		}
		else
		{
			Resultat = Tas_ + ( sizeof( Tas_ ) - Restant_ );

			Restant_ -= Taille;

			return Resultat;
		}
	}
	/*f Free 'Bloc'. The bloc must be allocate/free like a FILO system (first allocated, last freed) */
	void Free( void *Bloc )
	{
		if ( ( Bloc >= Tas_ ) && ( Bloc < ( Tas_ + sizeof( Tas_ ) ) ) )
			Restant_ = sizeof( Tas_ ) - ( (char *)Bloc - Tas_ );
		else
			free( Bloc );
	}
};

// A predefined fast allocation memory.
extern fam_fast_allocation_memory FAM;


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
/* end of 'xxx.h' template file */