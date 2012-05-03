/*
	Header for the 'tol' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (http://zeusw.org/intl/contact.html).
$_RAW_$
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

#ifndef TOL__INC
#define TOL__INC

# define TOL_NAME		"TOL"

# define	TOL_VERSION	"$Revision$"

# define TOL_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

# include "ttr.h"

extern class ttr_tutor &TOLTutor;

# if defined( XXX_DBG ) && !defined( TOL_NODBG )
#  define TOL_DBG
# endif

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
//D TOoL 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/
//D TOoL

#include <string.h>
#include <stddef.h>
#include <time.h>
#include <signal.h>
#include <locale.h>

#include "cpe.h"

#if defined( CPE__T_LINUX ) || defined( CPE__P_CYGWIN ) || defined( CPE__T_MAC )
#	define TOL__POSIX
#elif defined( CPE__T_MS )
#	define TOL__MS
#else
#	error "Undefined compilation enviroment."
#endif


#ifdef TOL__MS
#	include <sys/utime.h>
#elif defined( TOL__POSIX )
#	include <utime.h>
#else
#	error
#endif

#if defined( CPE__C_VC ) || defined( CPE__C_GCC )
#	include <sys/timeb.h>
#else
#	error "Unknown compiler"
#endif

#include "err.h"

# include "bso.h"

namespace tol
{
	typedef char buffer__[20];
}


#ifdef CPE__C_VC
#	ifdef CPE__WARNING_SUPPRESSION_ENABLED
#		pragma warning( disable : 4284 )
#	endif
#endif

// Usage interne.
# define E__TMIMIC__( type ,alias )\
	private:\
		type V_;\
	public:\
		alias( void ) {}\
		alias( type T )\
		{\
			V_ = T;\
		}\
		type *operator ->( void )\
		{\
			return &V_;\
		}\
		const type *operator ->( void ) const\
		{\
			return &V_;\
		}\
		type &operator *( void )\
		{\
			return V_;\
		}\
		const type &operator *( void ) const\
		{\
			return V_;\
		}\

// Similaire � un 'typedef type alias', sauf que 'type' et 'alias' ne sont pas interchangeable.
// Uniqumement pour des objets statiques.
// Utiliiser 'E_TMIMIC( type, alias)' pour des objets dynamiques.
# define E_TMIMIC__( type, alias )\
	struct alias\
	{\
		E__TMIMIC__( type, alias )\
		bool operator ==( alias A ) const\
		{\
			return V_ == A.V_;\
		}\
		bool operator !=( alias A ) const\
		{\
			return V_ != A.V_;\
		}\
		bool operator ==( type T ) const\
		{\
			return V_ == T;\
		}\
		bool operator !=( type T ) const\
		{\
			return V_ != T;\
		}\
	}

// Similaire � un 'T_MIMIC__( type, alias )', mais sans op�rateurs de comparaisons.
# define E_TRMIMIC__( type, alias )\
struct alias\
{\
	E__TMIMIC__( type, alias )\
}


// Similaire � 'E_TMIMIC__( type, alias)', mais pour des objets dynamiques.
// La version instanciable et la version '_' sont toutes deux cr�ees.
# define E_TMIMIC( type, alias )\
class alias##_\
: public type##_\
{\
public:\
	alias##_( s &S )\
	: type##_( S )\
	{}\
	struct s\
	: public type##_::s\
	{};\
	alias##_ &operator =( const alias##_ &S )\
	{\
		((type##_ *)this)->operator =( S );\
\
		return *this;\
	}\
	alias##_ &operator =( const type##_ &T )\
	{\
		((type##_ *)this)->operator =( T );\
		\
		return *this;\
	}\
};\
\
class alias\
: public alias##_\
{\
public:\
	alias##_::s static_;\
	alias( void )\
	: alias##_( static_ )\
	{\
		reset( false );\
	}\
	~alias( void )\
	{\
		reset( true );\
	}\
	alias &operator =( const alias &S )\
	{\
		alias##_::operator =( S );\
\
		return *this;\
	}\
	alias &operator =( const alias##_ &S )\
	{\
		alias##_::operator =( S );\
\
		return *this;\
	}\
	alias &operator =( const type##_ &T )\
	{\
		((type##_ *)this)->operator =( T );\
		\
		return *this;\
	}\
};


// Pour les objets dynamiques, similaire � 'typedef type alias', mais
// avec cr�ation de la version instanciable et de la version '_'.
# define E_TCLONE( type, alias )\
	typedef type##_ alias##_;\
	typedef type alias;


// Similaire � 'typedef type alias', mais permet la mise en oeuvre de 'template'.
// Pour objets statiques uniquement.
// Exemple d'utilisation.
// template <typename r> E_TTCLONE__( tree_seeker__<r>, seeker__ );
# define E_TTCLONE__( type, alias )\
	class alias\
	: public type\
	{\
	}


// Similaire � 'typedef type  alias', mais permet la mise en oeuvre de 'template'.
// Pour objets dynamiques uniquement.
// Exemple :
// template <typename field_ref__> E_TTCLONE( lstctn::E_LXMCONTAINERt_( comparison_item_<field_ref__>, item_row__ ), comparison_items_ );
# define E_TTCLONE_( type, alias )\
	class alias\
	: public type\
	{\
	public:\
		struct s\
		: public type::s\
		{};\
		alias( s &S )\
		: type( S )\
		{}\
	}\


// Similaire � 'E_TTCLONE_( type ,alias )', mais cr�e la version instanciable en plus de la version '_'.
// ATTENTION : 'type' doit �tre la version AVEC '_', 'alias' la version SANS '_'.
// Version pour 1 'template".
// Exemple :
// template <typename field_ref__> E_TT1CLONE( lstctn::E_LXMCONTAINERt_( comparison_item_<field_ref__>, item_row__ ), comparison_items );
# define E_TT1CLONE( type, alias )\
	E_TTCLONE_( type, alias##_ );\
	E_AUTO1( alias )

// Idem que ci-dessus, mais pour 2 'templates's.
# define E_TT2CLONE( type, alias )\
	E_TTCLONE_( type, alias##_ );\
	E_AUTO2( alias )

// Idem que ci-dessus, mais pour 3 'templates's.
# define E_TT3CLONE( type, alias )\
	E_TTCLONE_( type, alias##_ );\
	E_AUTO3( alias )


//d Create a new type for a row named 'Type'.
# define E_ROW( Type )	E_TMIMIC__( mdr::row_t__, Type )

/* Permet de transformer 2 arguments en 1; si un argument d'une macro
a besoin de contenir une virgule, cette macro est l� pour �a
'E_COVER2( a, b )' donne 'a, b' */
# define E_COVER2(a, b)	a, b

/* Permet de convertir un entier en cha�ne (l'encadre par des '"').
Utile pour afficher le num�ro de ligne dans un #pragma message (...). */
# define E__STRING(x) #x
# define E_STRING(x) E__STRING(x)

// Inspir� du site msdn.microsoft.com.
# define __LOC__ __FILE__ "(" E_STRING(__LINE__) ")"

// Utilisation :
// #pragma message(__LOC__ " : Message")




# define E_AUTO_( Name )	\
class Name\
: public Name##_\
{\
public:\
	Name##_::s static_;\
	Name( void )\
	: Name##_( static_ )\
	{\
		reset( false );\
	}\
	~Name( void )\
	{\
		reset( true );\
	}\
	Name &operator =( const Name &S )\
	{\
		Name##_::operator =( S );\
\
		return *this;\
	}\
	Name &operator =( const Name##_ &S )\
	{\
		Name##_::operator =( S );\
\
		return *this;\
	}

# define E_AUTO( name )\
E_AUTO_( name )\
};


//m Same as 'E_AUTO()' but with one template parameter of type 'Name'
# define E_AUTO1( Name )	\
template < typename t > class Name\
: public Name##_<t>\
{\
public:\
	typename Name##_<t>::s static_;\
	Name ( void )\
	: Name##_<t>( static_ )\
	{\
		Name##_<t>::reset( false );\
	}\
	~Name ( void )\
	{\
		Name##_<t>::reset( true );\
	}\
	Name &operator =( const Name &S )\
	{\
		Name##_<t>::operator =( S );\
\
		return *this;\
	}\
	Name &operator =( const Name##_<t> &S )\
	{\
		Name##_<t>::operator =( S );\
\
		return *this;\
	}\
	Name##_<t> &operator *( void )\
	{\
		return *this;\
	}\
	const Name##_<t> &operator *( void ) const\
	{\
		return *this;\
	}\
};


//m Same as 'E_AUTO()' but with two template parameter.
# define E_AUTO2( Name )	\
template < typename t, typename u > class Name\
: public Name##_<t,u>\
{\
public:\
	typename Name##_<t,u>::s static_;\
	Name ( void )\
	: Name##_<t,u>( static_ )\
	{\
		Name##_<t,u>::reset( false );\
	}\
	~Name ( void )\
	{\
		Name##_<t,u>::reset( true );\
	}\
	Name &operator =( const Name &S )\
	{\
		Name##_<t,u>::operator =( S );\
\
		return *this;\
	}\
	Name &operator =( const Name##_<t,u> &S )\
	{\
		Name##_<t,u>::operator =( S );\
\
		return *this;\
	}\
	Name##_<t,u> &operator *( void )\
	{\
		return *this;\
	}\
	const Name##_<t,u> &operator *( void ) const\
	{\
		return *this;\
	}\
};

//m Same as 'E_AUTO()' but with three template parameter.
# define E_AUTO3( Name )	\
template < typename t, typename u, typename v > class Name\
: public Name##_<t,u,v>\
{\
public:\
	typename Name##_<t,u,v>::s static_;\
	Name ( void )\
	: Name##_<t,u,v>( static_ )\
	{\
		Name##_<t,u,v>::reset( false );\
	}\
	~Name ( void )\
	{\
		Name##_<t,u,v>::reset( true );\
	}\
	Name &operator =( const Name &S )\
	{\
		Name##_<t,u,v>::operator =( S );\
\
		return *this;\
	}\
	Name &operator =( const Name##_<t,u,v> &S )\
	{\
		Name##_<t,u,v>::operator =( S );\
\
		return *this;\
	}\
	Name##_<t,u,v> &operator *( void )\
	{\
		return *this;\
	}\
	const Name##_<t,u,v> &operator *( void ) const\
	{\
		return *this;\
	}\
};

//m Same as 'E_AUTO()' but with four template parameter.
# define E_AUTO4( Name )	\
template < typename t, typename u, typename v, typename w > class Name\
: public Name##_<t,u,v,w>\
{\
public:\
	typename Name##_<t,u,v,w>::s static_;\
	Name ( void )\
	: Name##_<t,u,v,w>( static_ )\
	{\
		Name##_<t,u,v,w>::reset( false );\
	}\
	~Name ( void )\
	{\
		Name##_<t,u,v,w>::reset( true );\
	}\
	Name &operator =( const Name &S )\
	{\
		Name##_<t,u,v,w>::operator =( S );\
\
		return *this;\
	}\
	Name &operator =( const Name##_<t,u,v,w> &S )\
	{\
		Name##_<t,u,v,w>::operator =( S );\
\
		return *this;\
	}\
	Name##_<t,u,v,w> &operator *( void )\
	{\
		return *this;\
	}\
	const Name##_<t,u,v,w> &operator *( void ) const\
	{\
		return *this;\
	}\
};



/*********************************************/
/* CLASSE DE GESTION DU POINTEUR UTILISATEUR */
/*********************************************/

namespace tol {
	class UP__
	{
	protected:
		void *UP_;
	public:
		UP__( void )
		{
			reset( false );
		}
		~UP__( void )
		{
			reset( true );
		}
		void reset( bool = true )
		{
			UP_ = NULL;
		}
		//f Return the current user pointer
		void *UP( void ) const
		{
			return UP_;
		}
		//f 'UP' becomes the new user pointer
		void UP( void *UP )
		{
			UP_ = UP;
		}
	};


	//f Return the current date.
	const char *Date( buffer__ &Buffer );

	//f Return current time.
	const char *Time( buffer__ &Buffer );

	//f Return current date � time.
	const char *DateAndTime( buffer__ &Buffer );


#ifndef CPE__T_MT
	inline const char *Date( void )
	{
		static buffer__ Buffer;

		return Date( Buffer );
	}

	inline const char *Time( void )
	{
		static buffer__ Buffer;

		return Time( Buffer );
	}

	inline const char *DateAndTime( void )
	{
		static buffer__ Buffer;

		return DateAndTime( Buffer );
	}
#endif

	/*f Return a time in ms. Only usefull by susbstracting 2 value.
	Is different from 'clock()' because 'clock()' only return how long
	the application is using the processor.*/
	inline time_t _Clock( void )
	{
		timeb T;

		::ftime( &T );
		// Attention: pas standard, mais pr�sent sur toutes les plateformes utilis�es.
		/* Cependant, la valeur de retour diff�re selon les plateformes, mais ne signale
		jamais une erreur. */

		return T.time * 1000UL + T.millitm;
	}

	inline time_t Clock( bso::bool__ Discrimination )	// Mettre 'Discrimination' � 'true' pour �tre s�r que deux
													// appels successifs � cette focntion renvoit deux valeurs diff�rentes.
	{
		time_t Time = tol::_Clock();

		if ( Discrimination ) {
			while ( Time == tol::_Clock() );

			Time = tol::_Clock();
		}

		return Time;
	}

	/*f Initialize the random generator using the date & time.
	The used value is returned to be used with the following
	function to make the random generator always start with the same value. */
	inline unsigned int InitializeRandomGenerator( void )
	{
		unsigned int Seed = (unsigned int)time( NULL );
	
		srand( Seed );

		return Seed;
	}

	//f Initialize the random generator with 'Seed'.
	inline void InitializeRandomGenerator( unsigned int Seed )
	{
		srand( Seed );
	}

	class chrono__
	{
	private:
		time_t _Delay;
		time_t _Limit;
	public:
		void Init( time_t Delay )
		{
			_Limit = 0;
			_Delay = Delay;
		}
		void Launch( void )
		{
			_Limit = Clock( false ) + _Delay;
		}
		bso::bool__ IsElapsed( void ) const
		{
			return Clock( false ) > _Limit;
		}
	};

	inline bso::char__ GetLocaleDecimalSeparator( void )
	{
		lconv *LConv = localeconv();

		if ( LConv->decimal_point == NULL )
			ERRs();

		if ( LConv->decimal_point[0] == 0 )
			ERRs();

		if ( LConv->decimal_point[1] != 0 )
			ERRs();

		return *LConv->decimal_point;
	}

	template <typename i> inline void Swap(
		i &I1,
		i &I2 )	// Echange le contenu des entiers I1 et I2. Retourne I1 (avant �change).
	{
		I2 = (i)( I1 ^ I2 );
		I1 = (i)( I2 ^ I1 );
		I2 = (i)( I1 ^ I2 );
	}
}

//m Define navigation functions ( 'First', 'Next', Amount', ... ) using 'Object' and 'Type'.
# define E_NAVt( Object, Type )\
	Type First( void ) const\
	{\
		return Object  First();\
	}\
	Type Last( void ) const\
	{\
		return Object Last();\
	}\
	Type Previous( Type P ) const\
	{\
		return Object Previous( P );\
	}\
	Type Next( Type P ) const\
	{\
		return Object Next( P );\
	}\
	mdr::size__ Amount( void ) const\
	{\
		return Object Amount();\
	}\
	mdr::size__ Extent( void ) const\
	{\
		return Object Extent();\
	}\
	bso::bool__ IsEmpty( void ) const\
	{\
		return Object IsEmpty();\
	}\
	bso::bool__ Exists( Type P ) const\
	{\
		return Object Exists( P );\
	}
	
/* Lorsque 'Object' contient une virgule, on ne peut utiliser E_XNAVt(...), m�me en utilisant E_COVER2(...)
car 'E_XNAVt(...)' fait elle-m�me appel � une macro. Aussi faudra-t'il explicitement appel� 'E_NAVt(...)' et cette macro-ci
pour parvenir au m�me r�sultat que 'E_XNAVt(...)'. */
# define E_NAVXt( Object, Type )\
	Type First( mdr::size__ Offset ) const\
	{\
		return Object  First( Offset );\
	}\
	Type Last( mdr::size__ Offset ) const\
	{\
		return Object Last( Offset );\
	}\
	Type Previous(\
		Type P,\
		mdr::size__ Offset ) const\
	{\
		return Object Previous( P, Offset );\
	}\
	Type Next(\
		Type P,\
		mdr::size__ Offset ) const\
	{\
		return Object Next( P, Offset  );\
	}\
	void Allocate(\
		mdr::size__ Size,\
		aem::mode__ Mode = aem::m_Default )\
	{\
		Object Allocate( Size, Mode );\
	}

# define E_XNAVt( Object, Type )\
		E_NAVt( Object, Type )\
		E_NAVXt( Object, Type )

# define E_NAV( Object )	E_NAVt( Object, mdr::row__ )
# define E_NAVX( Object )	E_NAVXt( Object, mdr::row__ )
# define E_XNAV( Object )	E_XNAVt( Object, mdr::row__ )

namespace mmm {
	class multimemory_;
}

namespace mdr {
	struct row__;
}

namespace tol {

	// A basic object 't' becomes a normal object.
	template <class t, typename r = mdr::row__> class object_
	{
	public:
		struct s
		{
			t Object;
		} &S_;
		object_( s &S )
		: S_( S )
		{}
		void reset( bool = true )
		{
			// Pour des raisons de standardisation
		}
		void plug( class mmm::multimemory_ &)
		{
			// Pour des raisons de standardisation.
		}
		object_ &operator =( const object_ &O )
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

	E_AUTO2( object )

	template <typename t> class _core_pointer___	// Classe de base de gestion d'un pointeur.
	{
	protected:
		t *P_;
	public:
		virtual void reset( bso::bool__ P = true )
		{
			P_ = NULL;
		}
		_core_pointer___( void )
		{
			reset( false );
		}
		virtual ~_core_pointer___( void )
		{
			reset( true );
		}
		void Init( void )
		{
			reset();
		}
		_core_pointer___ &operator =( const _core_pointer___ &Pointer )
		{
			ERRu();	// Otherwise the same ressource is used twice ; which delete them ?

			return *this;
		}
		t *operator =( t *P )
		{
#ifdef TOL_DBG
			if ( P_ != NULL )
				ERRu();
#endif

			P_ = P;

			return P_;
		}
		t *operator =( void *P )
		{
			return operator =( (t *)P );
		}
		operator t *( void )
		{
			return P_;
		}
		operator const t *( void ) const
		{
			return P_;
		}
		const t *operator ()( void ) const
		{
			return P_;
		}
		t *operator ()( void )
		{
			return P_;
		}
		const t *operator ->( void ) const
		{
			return P_;
		}
		t *operator ->( void )
		{
			return P_;
		}
		const t &operator *( void ) const
		{
			return *P_;
		}
		t &operator *( void )
		{
			return *P_;
		}
	};

	template <typename t> class free_pointer___	// Classe de gestion d'un pointeur devant �tre d�sallou� par un 'free'.
	: public _core_pointer___<t>
	{
	public:
		virtual void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _core_pointer___<t>::P_ != NULL )
					free( _core_pointer___<t>::P_ );

			_core_pointer___<t>::reset( P );
		}
		t *operator =( t *P )
		{
			reset();

			return _core_pointer___<t>::operator =( P );
		}
		t *operator =( void *P )
		{
			reset();

			return _core_pointer___<t>::operator =( P );
		}
	};

# define E_FPOINTER___( t )	free_pointer___<t>

	template <typename t> class delete_pointer___	// Classe de gestion d'un pointeur devant �tre d�allou� par un 'delete'.
	: public _core_pointer___<t>
	{
	public:
		virtual void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _core_pointer___<t>::P_ != NULL )
					delete _core_pointer___<t>::P_;

			_core_pointer___<t>::reset( P );
		}
		t *operator =( t *P )
		{
			reset();

			return _core_pointer___<t>::operator =( P );
		}
		t *operator =( void *P )
		{
			reset();

			return _core_pointer___<t>::operator =( P );
		}
	};

# define E_DPOINTER___( t )	delete_pointer___<t>
}

#if 0
	//f Free 'Pointer' only if != NULL. Pointer value becomes 'NULL'.
	template <typename t> inline void Free( t *&Pointer )
	{
		if ( Pointer != NULL ) {
			free( (void *)Pointer );
			Pointer = NULL;
		}
	}
#endif

// Cr�ation d'un constructeur standardis�.
# define E_CTOR( name )\
	name( void )\
	{\
		reset( false );\
	}\

// Cr�ation d'un destructeur standardis�.
# define E_DTOR( name )\
	~name( void )\
	{\
		reset();\
	}\

// Cr�ation d'un destructeur virtuel standardis�.
# define E_VDTOR( name )	virtual E_DTOR( name )

// Cr�ation d'un constructeur et d'un destructeur standardis�.
# define E_CDTOR( name )\
	E_CTOR( name )\
	E_DTOR( name )

// Cr�ation d'un constructeur et d'un destructeur virtuel standardis�.
# define E_CVDTOR( name )\
	E_CTOR( name )\
	E_VDTOR( name )


//d Make accessible the static member, for read-only access, of a dynamic object, named 'name' of type 'type__'.
# define E_RRODISCLOSE_(type__, name )\
	const type__ Get##name( void ) const\
	{\
		return S_.name;\
	}


//d Make accessible the static member, for read-only access, of a dynamic object, named 'name' of type 'type__'.
# define E_RODISCLOSE_(type__, name )\
	E_RRODISCLOSE_( type__, name )\
	const type__ &name( void ) const\
	{\
		return S_.name;\
	}

# define E_RWODISCLOSE_(type__, name )\
	void Set##name( const type__ &V )\
	{\
		S_.name = V;\
	}

# define E_WODISCLOSE_(type__, name )\
	E_RWODISCLOSE_( type__, name )\
	type__ &name( void )\
	{\
		return S_.name;\
	}

//d Make accessible the static member, for read-write access, of a dynamic object, named 'name' of type 'type__'.
# define E_RRWDISCLOSE_(type__, name )\
	E_RRODISCLOSE_( type__, name )\
	E_RWODISCLOSE_( type__, name )

//d Make accessible the static member, for read-write access, of a dynamic object, named 'name' of type 'type__'.
# define E_RWDISCLOSE_(type__, name )\
	E_RODISCLOSE_( type__, name )\
	E_WODISCLOSE_( type__, name )



# define E_RRODISCLOSE__(type__, name )\
	const type__ Get##name( void ) const\
	{\
		return _##name;\
	}

//d Make accessible the member, for read-only access, of a static object, named 'name' of type 'type__'.
# define E_RODISCLOSE__(type__, name )\
	E_RRODISCLOSE__( type__, name )\
	const type__ &name( void ) const\
	{\
		return _##name;\
	}

# define E_RWODISCLOSE__(type__, name )\
	void Set##name( const type__ &V )\
	{\
		_##name = V;\
	}

# define E_WODISCLOSE__(type__, name )\
	E_RWODISCLOSE__( type__, name )\
	type__ &name( void )\
	{\
		return _##name;\
	}

# define E_RRWDISCLOSE__(type__, name )\
	E_RRODISCLOSE__( type__, name )\
	E_RWODISCLOSE__( type__, name )

# define E_RWDISCLOSE__(type__, name )\
	E_RODISCLOSE__( type__, name )\
	E_WODISCLOSE__( type__, name )


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
