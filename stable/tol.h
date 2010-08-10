/*
	Header for the 'tol' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#define TOL_NAME		"TOL"

#define	TOL_VERSION	"$Revision$"

#define TOL_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &TOLTutor;

#if defined( XXX_DBG ) && !defined( TOL_NODBG )
#define TOL_DBG
#endif

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
#include "bso.h"

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

namespace tol
{
	typedef char buffer__[20];
}


#ifdef CPE__C_VC
#	ifdef CPE__WARNING_SUPPRESSION_ENABLED
#		pragma warning( disable : 4284 )
#	endif
#endif


/*m Same as 'typedef type alias', but 2 'alias' with same 'type' are not interchangeable.
This is only for static objects. Use 'TYPEDEF_( type, alias )' for dynamic objects. */
#define E_TYPEDEF( type, alias )\
struct alias\
{\
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

/*m Same as 'TYPEDEF( type, alias ), but for dynamic objects
Both '_' version and instanciable version are created. */
#define E_TYPEDEF_( type, alias )\
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

//m Same as 'TYPEDEF( type, alias ), but light version, without comparison operators.
#define E_TYPEDEF__( type, alias )\
struct alias\
{\
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
}

// Example of use :
// template <typename field_ref__> E_TTYPEDEF( lstctn::E_LXMCONTAINERt_( comparison_item_<field_ref__>, item_row__ ), comparison_items_ );
#define E_TTYPEDEF( type, alias )	class alias\
	: public type\
	{\
	public:\
		struct s\
		: public type::s\
		{};\
		alias( s &S )\
		: type( S )\
		{}\
	}

// Example of use :
// template <typename r> E_TTYPEDEF__( tree_seeker__<r>, tree_seeker__ );
#define E_TTYPEDEF__( type, alias )	class alias\
	: public type\
	{\
	}

#define E_TTYPEDEF___	E_TTYPEDEF__


//d Create a new type for a row named 'Type'.
#define E_ROW( Type )	E_TYPEDEF( epeios::row_t__, Type )


/* Permet de transformer 2 arguments en 1; si un argument d'une macro
a besoin de contenir une virgule, cette macro est là pour ça
'E_COVER2( a, b )' donne 'a, b' */
#define E_COVER2(a, b)	a, b


//m Create the autonomous definition of the 'name' object based on the 'name'_ object.
#define E_AUTO( Name )	\
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
	}\
	Name##_ &operator *( void )\
	{\
		return *this;\
	}\
	const Name##_ &operator *( void ) const\
	{\
		return *this;\
	}\
};


//m Same as 'E_AUTO()' but with one template parameter of type 'TypeName'
#define E_AUTO1( Name )	\
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
#define E_AUTO2( Name )	\
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
#define E_AUTO3( Name )	\
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
#define E_AUTO4( Name )	\
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

	//f Return current date é time.
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
		// Attention: pas standard, mais présent sur toutes les plateformes utilisées.
		/* Cependant, la valeur de retour diffère selon les plateformes, mais ne signale
		jamais une erreur. */

		return T.time * 1000UL + T.millitm;
	}

	inline time_t Clock( bso::bool__ Discrimination )	// Mettre 'Discrimination' à 'true' pour être sûr que deux
													// appels successifs à cette focntion renvoit deux valeurs différentes.
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
}

//m Define navigation functions ( 'First', 'Next', Amount', ... ) using 'Object' and 'Type'.
#define E_NAVt( Object, Type )\
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
	epeios::size__ Amount( void ) const\
	{\
		return Object Amount();\
	}\
	epeios::size__ Extent( void ) const\
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
	
/* Lorsque 'Object' contient une virgule, on ne peut utiliser E_XNAVt(...), même en utilisant E_COVER2(...)
car 'E_XNAVt(...)' fait elle-même appel à une macro. Aussi faudra-t'il explicitement appelé 'E_NAVt(...)' et cette macro-ci
pour parvenir au même résultat que 'E_XNAVt(...)'. */
#define E_NAVXt( Object, Type )\
	Type First( epeios::size__ Offset ) const\
	{\
		return Object  First( Offset );\
	}\
	Type Last( epeios::size__ Offset ) const\
	{\
		return Object Last( Offset );\
	}\
	Type Previous(\
		Type P,\
		epeios::size__ Offset ) const\
	{\
		return Object Previous( P, Offset );\
	}\
	Type Next(\
		Type P,\
		epeios::size__ Offset ) const\
	{\
		return Object Next( P, Offset  );\
	}\
	void Allocate(\
		epeios::size__ Size,\
		aem::mode__ Mode = aem::mDefault )\
	{\
		Object Allocate( Size, Mode );\
	}

#define E_XNAVt( Object, Type )\
		E_NAVt( Object, Type )\
		E_NAVXt( Object, Type )

#define E_NAV( Object )		E_NAVt( Object, epeios::row__ )
#define E_NAVX( Object )	E_NAVXt( Object, epeios::row__ )
#define E_XNAV( Object )	E_XNAVt( Object, epeios::row__ )

namespace mmm {
	class multimemory_;
}

namespace epeios {
	struct row__;
}

namespace tol {

	// A basic object 't' becomes a normal object.
	template <class t, typename r = epeios::row__> class object_
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

	template <typename t> class free_pointer___	// Classe de gestion d'un pointeur devant être désalloué par un 'free'.
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

	#define E_FPOINTER___( t )	free_pointer___<t>

	template <typename t> class delete_pointer___	// Classe de gestion d'un pointeur devant être déalloué par un 'delete'.
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

	#define E_DPOINTER___( t )	delete_pointer___<t>

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

// Création d'un constructeur standardisé.
#define E_CTOR( name )\
	name( void )\
	{\
		reset( false );\
	}\

// Création d'un destructeur standardisé.
#define E_DTOR( name )\
	~name( void )\
	{\
		reset();\
	}\

// Création d'un destructeur virtuel standardisé.
#define E_VDTOR( name )	virtual E_DTOR( name )

// Création d'un constructeur et d'un destructeur standardisé.
#define E_CDTOR( name )\
	E_CTOR( name )\
	E_DTOR( name )

// Création d'un constructeur et d'un destructeur virtuel standardisé.
#define E_CVDTOR( name )\
	E_CTOR( name )\
	E_VDTOR( name )


//d Make accessible the static member, for read-only access, of a dynamic object, named 'name' of type 'type__'.
#define E_RRODISCLOSE_(type__, name )\
	const type__ Get##name( void ) const\
	{\
		return S_.name;\
	}


//d Make accessible the static member, for read-only access, of a dynamic object, named 'name' of type 'type__'.
#define E_RODISCLOSE_(type__, name )\
	E_RRODISCLOSE_( type__, name )\
	const type__ &name( void ) const\
	{\
		return S_.name;\
	}

#define E_RWODISCLOSE_(type__, name )\
	void Set##name( const type__ &V )\
	{\
		S_.name = V;\
	}

#define E_WODISCLOSE_(type__, name )\
	E_RWODISCLOSE_( type__, name )\
	type__ &name( void )\
	{\
		return S_.name;\
	}

//d Make accessible the static member, for read-write access, of a dynamic object, named 'name' of type 'type__'.
#define E_RRWDISCLOSE_(type__, name )\
	E_RRODISCLOSE_( type__, name )\
	E_RWODISCLOSE_( type__, name )

//d Make accessible the static member, for read-write access, of a dynamic object, named 'name' of type 'type__'.
#define E_RWDISCLOSE_(type__, name )\
	E_RODISCLOSE_( type__, name )\
	E_WODISCLOSE_( type__, name )



#define E_RRODISCLOSE__(type__, name )\
	const type__ Get##name( void ) const\
	{\
		return _##name;\
	}

//d Make accessible the member, for read-only access, of a static object, named 'name' of type 'type__'.
#define E_RODISCLOSE__(type__, name )\
	E_RRODISCLOSE__( type__, name )\
	const type__ &name( void ) const\
	{\
		return _##name;\
	}

#define E_RWODISCLOSE__(type__, name )\
	void Set##name( const type__ &V )\
	{\
		_##name = V;\
	}

#define E_WODISCLOSE__(type__, name )\
	E_RWODISCLOSE__( type__, name )\
	type__ &name( void )\
	{\
		return _##name;\
	}

#define E_RRWDISCLOSE__(type__, name )\
	E_RRODISCLOSE__( type__, name )\
	E_RWODISCLOSE__( type__, name )

#define E_RWDISCLOSE__(type__, name )\
	E_RODISCLOSE__( type__, name )\
	E_WODISCLOSE__( type__, name )

// Inspiré du site msdn.microsoft.com.
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : "

// Utilisation :
// #pragma message(__LOC__"Message")




/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
