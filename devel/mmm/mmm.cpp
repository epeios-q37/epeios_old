/*
  'mmm' library by Claude SIMON (csimon@epeios.org)
  Requires the 'mmm' header file ('mmm.h').
  Copyright (C) 2002 Claude SIMON (csimon@epeios.org).

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

#define MMM__COMPILATION

#include "mmm.h"

class mmmtutor
: public ttr_tutor
{
public:
	mmmtutor( void )
	: ttr_tutor( MMM_NAME )
	{
#ifdef MMM_DBG
		Version = MMM_VERSION "\b\bD $";
#else
		Version = MMM_VERSION;
#endif
		Owner = MMM_OWNER;
		Date = "$Date$";
	}
	virtual ~mmmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "stf.h"

using namespace mmm;

#define	multimemoire	multimemory_
#define nombre			nombre__
#define descripteur		descriptor__
#define indice_bloc		indice_bloc__
#define taille			taille__

/*
void multimemoire::Liberer_(
	descripteur Descripteur,
	fdm__bool Ajustement )
{
	if ( HorsLimite_( Suivant_( Descripteur ) ) )
	{
		capacite NouvelleCapacite;

		Memoire.Allouer( NouvelleCapacite = Descripteur - sizeof( nombre ) );

		S_.Capacite = NouvelleCapacite;

		if ( Ajustement )
			AjusterCapacite_();
	}
	else
	{
		nombre Nombre = NombreBlocsOccupes_( Descripteur );

		while ( Nombre-- )
		{
			EcrireNombreBlocs_( Descripteur, 0 );
			Descripteur += S_.Taille;
		}
	}
}

nombre multimemoire::NombreBlocsSuccessifsLibres_( descripteur Descripteur )
{
	nombre Nombre = 0;

	while ( !HorsLimite_( Descripteur ) && EstLibre_( Descripteur ) )
	{
		Nombre++;
		Descripteur += S_.Taille;
	}

	return Nombre;
}

void multimemoire::AjusterCapacite_( void )
{
	descripteur Descripteur = PremierDescripteur_();

	if ( !HorsLimite_( Descripteur ) )
	{
		while( !HorsLimite_( Suivant_( Descripteur ) ) )
			Descripteur = Suivant_( Descripteur );

		if ( EstLibre_( Descripteur ) )
		{
			capacite NouvelleCapacite;
			Memoire.Allouer( NouvelleCapacite = Descripteur - sizeof( nombre ) );
			S_.Capacite = NouvelleCapacite;
		}
	}
}

descripteur multimemoire::Allouer_( capacite Capacite )
{
	descripteur Descripteur;

	if ( ( Optimisation_ == mmm::Taille ) || ( S_.Capacite < PremierDescripteur_() ) )
		Descripteur = PremierDescripteur_();
	else
		Descripteur = PremierDescripteur_() + S_.Capacite;

	while ( !HorsLimite_( Descripteur )
			&& ( !EstLibre_( Descripteur ) || ( TailleEspaceLibre_( Descripteur ) < Capacite ) ) )
		Descripteur = Suivant_( Descripteur );

	if ( HorsLimite_( Descripteur ) )
	{
		capacite NouvelleCapacite;

		if ( ( ( Capacite + sizeof( nombre ) - 1 ) / S_.Taille + 1 ) > FDM_USHORT_MAX )
			ERRL();

		Memoire.Allouer( NouvelleCapacite = Descripteur + S_.Taille * ( ( Capacite + sizeof( nombre ) - 1 ) / S_.Taille + 1 ) - sizeof( nombre ) );

		S_.Capacite = NouvelleCapacite;
	}

	Marquer_( Descripteur, Capacite );

	return Descripteur;
}
*/

void multimemoire::AfficherStructure_( void ) const
{
	descripteur Descripteur = PremierDescripteur_();
	descripteur P, D, M, m;
	size__ Nombre = 0, Total;

	while ( !HorsLimite_( Descripteur ) )
	{
		fout << TailleEnNombre_( Descripteur ) << ": ";

		if ( EstLibre_( Descripteur ) )
		{
			fout << '+';

			if ( !Nombre )
			{
				P = D = M = m = Descripteur;
				Total = Nombre_( Descripteur );
			}
			else
			{
				D = Descripteur;

				if ( Nombre_( Descripteur ) > Nombre_( M ) )
					M = Descripteur;
				else if ( Nombre_( Descripteur ) < Nombre_( m ) )
					m = Descripteur;

				Total += Nombre_( Descripteur );
			}

			Nombre++;
		}
		else
			fout << '-';

		fout << Nombre_( Descripteur ) << txf::tab;

		Descripteur = Successeur_( Descripteur );
	}

	fout << txf::nl << TailleEnNombre_( Descripteur ) << ": hors limite ...";

	if ( Nombre )
	{
		fout << txf::tab << "N:" << Nombre << txf::tab << "T: " << Total << txf::tab << "M: " << ( Total / Nombre ) << txf::nl
			 << "P: " << TailleEnNombre_( P ) << ';' << Nombre_( P ) << txf::tab
			 << "D: " << TailleEnNombre_( D ) << ';' << Nombre_( D ) << txf::tab
			 << "m: " << TailleEnNombre_( m ) << ';' << Nombre_( m ) << txf::tab
			 << "M: " << TailleEnNombre_( M ) << ';' << Nombre_( M );

		if ( S_.Libre )
			 fout << txf::tab << "L: " << TailleEnNombre_( S_.Libre ) << ';' << Nombre_( S_.Libre );
	}

	fout << txf::nl;
}
/*
descripteur multimemoire::Descripteur_( indice_bloc IndiceBloc )
{
	descripteur Descripteur = PremierDescripteur_();

	if ( EstLibre_( Descripteur ) )
		Descripteur = Suivant_( Descripteur );

	IndiceBloc--;

	while( IndiceBloc-- )
	{
		Descripteur = Suivant_( Descripteur );

		if ( EstLibre_( Descripteur ) )
			Descripteur = Suivant_( Descripteur );
	}

	return Descripteur;
}

indice_bloc multimemoire::IndiceBloc_( descripteur Descripteur )
{
	indice_bloc Indice = 1;
	descripteur Recherche = PremierDescripteur_();

	if ( EstLibre_( Recherche ) )
		Recherche = Suivant_( Recherche );

	while( Recherche < Descripteur )
	{
		Recherche = Suivant_( Recherche );

		if ( EstLibre_( Recherche ) )
			Recherche = Suivant_( Recherche );

		Indice++;
	}

	return Indice;
}

capacite multimemoire::NombreEspacesUtilises_( void )
{
	capacite Nombre = 0;
	descripteur Descripteur = PremierDescripteur_();

	while ( !HorsLimite_( Descripteur ) )
	{
		if ( !EstLibre_( Descripteur ) )
			Nombre++;

		Descripteur = Suivant_( Descripteur );
	}

	return Nombre;
}
*/


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class mmmpersonnalization
: public mmmtutor
{
public:
	mmmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mmmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static mmmpersonnalization Tutor;

ttr_tutor &MMMTutor = Tutor;
