/*
  Header for the 'bso' library by Claude L. Simon (simon@epeios.org)
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

#ifndef BSO__INC
#define BSO__INC

#define BSO_NAME		"BSO"

#define	BSO_VERSION	"$Revision$"	

#define BSO_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &BSOTutor;

#if defined( XXX_DBG ) && !defined( BSO_NODBG )
#define BSO_DBG 
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
#include "txf.h"


namespace bso {
	//t Basic data, without any basically signification.
	typedef unsigned char raw__;

	//t Boolean.
	typedef bool bool__;

#if 0 // to delete ?
	template <class T> class bso__pointeur
	{
		T *Objet_;
		// l'objet
	public:
		bso__pointeur( T *Objet = NULL )
		{
			Objet_ = Objet;
		}
		operator T *( void ) const
		{
			return Objet_;
		}
		// conversion
	#pragma warning( disable: 4284 )
		T *operator ->( void ) const
		{
			return Objet_;
		}
	#pragma warning( default: 4284 )
		T *Valeur( void ) const
		{
			return Objet_;
		}
	};

	template <class t> acs_ascii_ostream_ &operator <<(
		acs_ascii_ostream_ &OStream,
		const bso__pointeur<t> &P )
	{
		return OStream << *P;
	}

	template <class t> acs_ascii_istream_ &operator >>(
		acs_ascii_istream_ &IStream,
		bso__pointeur<t> &P )
	{
		return IStream >> *P;
	}
#endif


#if 0	// to delete ?
	/*****************************************/
	/* COMPORTEMENT COMMUN A TOUT LES OBJETS */
	/*****************************************/

	template <class T> class bso__base
	{
	private:
		T Objet_;
		// l'objet proprement dit
	protected:
		T &Objet( void )
		{
			return Objet_;
		}
		// retourne l'objet
		const T &Objet( void ) const
		{
			return Objet_;
		}
		// retourne l'objet
		T &Objet( T Objet )
		{
			return Objet_ = Objet;
		}
		// initialise l'objet
	public:
		bso__base( T Objet )
		{
			Objet_ = Objet;
		}
		// conversion
	/*	bso__base( T Valeur )
		{
			Objet_ = Valeur;
		}
	*/	// constructeur
		T operator +( void ) const
		{
			return Objet_;
		}
		// addition
		T operator -( void ) const
		{
			return (T)-Objet_;
		}
		// soustraction
		T operator ++( void )
		{
			return ++Objet_;
		}
		// incrémentation préfixée
		T operator --( void )
		{
			return --Objet_;
		}
		// decrémentation préfixée
		T operator ++( int )
		{
			return Objet_++;
		}
		/* incrémentation postfixée
		si si, c'est bien comme ça que ca doit être déclaré! */
		T operator --( int )
		{
			return Objet_--;
		}
		/* decrémentation postfixée
		si si, c'est bien comme ça que ca doit être déclaré! */
		T operator +=( T Valeur )
		{
			return Objet_ = Objet_ + Valeur;
		}
		// addition et affectation
		T operator -=( T Valeur )
		{
			return Objet_ = Objet_ - Valeur;
		}
		// soustraction et affectation
		T operator *=( T Valeur )
		{
			return Objet_ = Objet_ * Valeur;
		}
		// multiplication et affectation
		T operator /=( T Valeur )
		{
			return Objet_ = Objet_ / Valeur;
		}
		// division et affectation
	/*	T operator !( void ) const
		{
			return !Objet_;
		}
		// inverseur
		operator class bso__bool( void ) const
		{
			return Objet() != 0;
		}
		// convertit en booleen
	*/
		operator T( void )
		{
			return Objet_;
		}
		T Valeur( void )
		{
			return Objet_;
		}
	};

	/***********************************/
	/* COMPORTEMENT PROPRE AUX ENTIERS */
	/***********************************/

	template <class T> class bso__base_entier
	: public bso__base<T>
	{
	public:
		bso__base_entier( T Valeur ) : bso__base<T>( Valeur ) {}
		// affectation
		bso__base_entier( const bso__base_entier &Valeur ) : bso__base<T>( Valeur ) {}
		// duplication
		T operator %=( T Valeur )
		{
			return Objet( (T)( Objet() % Valeur ) );
		}
		// modulo et affectation
		T operator &=( T Valeur )
		{
			return Objet( Objet() & Valeur );
		}
		// ET bit à bit ert affectation
		T operator |=( T Valeur )
		{
			return Objet( Objet() | Valeur );
		}
		// OU bit à bit et affectation
		T operator ^=( T Valeur )
		{
			return Objet( Objet() ^ Valeur );
		}
		// OU EXCLUSIF bit à bit et affectation
		T operator <<=( T Valeur )
		{
			return Objet( (T)( Objet() << Valeur ) );
		}
		// decalage à gauche et affectation
		T operator >>=( T Valeur )
		{
			return Objet( (T)( Objet() >> Valeur ) );
		}
		// decalage à droite et affectation
		T operator ~( void ) const
		{
			return (T)~Objet();
		}
		// inversion bit à bit
		friend inline acs_ascii_ostream_ &operator <<(
			acs_ascii_ostream_ &Stream,
			bso__base_entier O );
		friend inline acs_ascii_istream_ &operator >>(
			acs_ascii_istream_ &Stream,
			bso__base_entier &O );
	};

#endif

	//d Maximal value of a 'sbyte__'.
	#define BSO_SBYTE_MAX		SCHAR_MAX
	//d Minimal value of a 'sbyte__'.
	#define BSO_SBYTE_MIN		SCHAR_MIN
	//d Size, in bit, of a 'sbyte__'.
	#define BSO_NB_BITS_SBYTE	8

	//t Signed byte.
	typedef signed char	sbyte__;


	//d Maximal value of a 'ubyte__'.
	#define BSO_UBYTE_MAX	UCHAR_MAX
	//d Minimal value of a 'ubyte__'.
	#define BSO_UBYTE_MIN	0
	//d Size, in bit, of a 'ubyte__'.
	#define BSO_NB_BITS_UBYTE	8

	//t Unsigned byte.
	typedef unsigned char ubyte__;

	//d Maximal value of a 'sshort__'.
	#define BSO_SSHORT_MAX	SHRT_MAX
	//d Minimal value of a 'sshort__'.
	#define BSO_SSHORT_MIN	SHRT_MIN
	//d Size, in bit, of a 'sshort__'.
	#define BSO_NB_BITS_SSHORT	16

	// Signed short.
	typedef signed short sshort__;


	//d Maximal value of a 'ushort__'.
	#define BSO_USHORT_MAX	USHRT_MAX
	//d Minimal value of a 'ushort__'.
	#define BSO_USHORT_MIN	0
	//d Size, in bit, of a 'ushort__'.
	#define BSO_NB_BITS_USHORT	16

	//t Unsigned short
	typedef unsigned short ushort__;


	//d Maximal value of a 'slong__'.
	#define BSO_SLONG_MAX	LONG_MAX
	//d Minimal value of a 'slong__'.
	#define BSO_SLONG_MIN   LONG_MIN
	//d Size, in bit, of a 'slong__'.
	#define BSO_NB_BITS_SLONG	32

	//t Signed long.
	typedef signed long slong__;


	//d Maximal value of a 'ulong__'.
	#define BSO_ULONG_MAX	ULONG_MAX
	//d Minimal value of a 'ulong__'.
	#define BSO_ULONG_MIN   0
	//d Size, in bit, of a 'ulong__'.
	#define BSO_NB_BITS_ULONG	32

	//t Unsigned long.
	typedef unsigned long ulong__;

	//d Maximal value of a 'size__'.
	#define BSO_BSIZE_MAX	UINT_MAX
	//d Minimal value of a 'size__'.
	#define BSO_BSIZE_MIN	0

	//t A sign, for comparaison function.
	typedef bso::sbyte__	sign__;

	//t Size of a buffer.
	typedef size_t bsize__;

	//t Short-sized float.
	typedef float sfloat__;

	//t Middle-sized float.
	typedef double float__;

#if 0	// Just a workaround to a 'gendoc' bug.
	//t Long sized float.
	typedef dummy lfloat__;
#endif

	typedef long double lfloat__;

	// Character
	typedef char char__;
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
