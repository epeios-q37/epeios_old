/*
  Header for the 'bso' library by Claude L. Simon (epeios@epeios.org)
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
//C Claude L. SIMON (epeios@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "txf.h"

// Basic data, without any basically signification.
typedef unsigned char bso__raw;

// Boolean.
typedef bool bso__bool;

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

//d Maximal value of a 'bso__sbyte'.
#define BSO_SBYTE_MAX		SCHAR_MAX
//d Minimal value of a 'bso__sbyte'.
#define BSO_SBYTE_MIN		SCHAR_MIN
//d Size, in bit, of a 'sbyte'.
#define BSO_NB_BITS_SBYTE	8

// Signe byte.
typedef signed char	bso__sbyte;


//d Maximal value of a 'bso__ubyte'.
#define BSO_UBYTE_MAX	UCHAR_MAX
//d Minimal value of a 'bso__ubyte'.
#define BSO_UBYTE_MIN	0
//d Size, in bit, of a 'bso__ubyte'.
#define BSO_NB_BITS_UBYTE	8

// Unsigned byte.
typedef unsigned char bso__ubyte;

//d Maximal value of a 'bso__sshort'.
#define BSO_SSHORT_MAX	SHRT_MAX
//d Minimal value of a 'bso__sshort'.
#define BSO_SSHORT_MIN	SHRT_MIN
//d Size, in bit, of a 'bso__sshort'.
#define BSO_NB_BITS_SSHORT	16

// Signed short.
typedef signed short bso__sshort;


//d Maximal value of a 'bso__ushort'.
#define BSO_USHORT_MAX	USHRT_MAX
//d Minimal value of a 'bso__ushort'.
#define BSO_USHORT_MIN	0
//d Size, in bit, of a 'bso__ushort'.
#define BSO_NB_BITS_USHORT	16

// Unsigned short
typedef unsigned short bso__ushort;


//d Maximal value of a 'bso__slong'.
#define BSO_SLONG_MAX	LONG_MAX
//d Minimal value of a 'bso__slong'.
#define BSO_SLONG_MIN   LONG_MIN
//d Size, in bit, of a 'bso__slong'.
#define BSO_NB_BITS_SLONG	32

// Signed long.
typedef signed long bso__slong;


//d Maximal value of a 'bso__ulong'.
#define BSO_ULONG_MAX	ULONG_MAX
//d Minimal value of a 'bso__ulong'.
#define BSO_ULONG_MIN   0
//d Size, in bit, of a 'bso__ulong'.
#define BSO_NB_BITS_ULONG	32

// Unsigned long.
typedef unsigned long bso__ulong;

//d Maximal value of a 'bso__size'.
#define BSO_BSIZE_MAX	UINT_MAX
//d Minimal value of a 'bso__size'.
#define BSO_BSIZE_MIN	0

// Size of a buffer.
typedef size_t bso__bsize;

// Short-sized float.
typedef float bso__sfloat;

// Middle-sized float.
typedef double bso__float;

// Long sized float.
typedef long double bso__lfloat;

// Character
typedef char bso__char;

// A basic object 't' becomes a normal object.
template <class t> class bso_object_
{
public:
	struct s
	{
		t Object;
	} &S_;
	bso_object_( s &S )
	: S_( S )
	{}
	void reset( bool = true )
	{
		// Pour des raisons de standardisation
	}
	void plug( class mmm_multimemoire_ &)
	{
		// Pour des raisons de standardisation.
	}
	bso_object_ &operator =( const bso_object_ &O )
	{
		S_.Object = O.S_.Object;

		return *this;
	}
	t &operator()( void )
	{
		return S_.Object;
	}
	const t &operator()( void ) const
	{
		return S_.Object;
	}
	operator t( void )
	{
		return S_.Object;
	}
};

AUTO1( bso_object )


template <class t> inline txf::text_oflow___ &operator <<(
	txf::text_oflow___ &Stream,
	const bso_object_<t> &O )
{
	return Stream << O();
}

template <class t> inline txf::text_iflow___ &operator >>(
	txf::text_iflow___ &Stream,
	bso_object_<t> &O )
{
	return Stream >> O();
}



/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
