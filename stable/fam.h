/*
  Header for the 'fam' library by Claude L. Simon (epeios@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org) 

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
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef FAM__INC
#define FAM__INC

#define FAM_NAME		"FAM"

#define	FAM_VERSION	"$Revision$"	

#define FAM_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &FAMTutor;

#if defined( XXX_DBG ) && !defined( FAM_NODBG )
#define FAM_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (epeios@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

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
