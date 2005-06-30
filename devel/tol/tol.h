/*
	Header for the 'tol' library by Claude SIMON (csimon@epeios.org)
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

#ifndef TOL__INC
#define TOL__INC

#define TOL_NAME		"TOL"

#define	TOL_VERSION	"$Revision$"

#define TOL_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &TOLTutor;

#if defined( XXX_DBG ) && !defined( TOL_NODBG )
#define TOL_DBG
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
//D TOoL 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/
//D TOoL

#include <string.h>
#include <stddef.h>
#include <time.h>

#include "cpe.h"
#include "txf.h"

#ifdef CPE__CONSOLE
#	include "cio.h"
#endif

#if defined( CPE__VC ) || defined( CPE__UNIX ) || defined( CPE__BEOS )
#	include <sys/timeb.h>
#elif !defined( CPE__CW )
#	error "Unknown compilation enviroment"
#endif

#include "err.h"

//d The default backup file extension.
#define TOL_DEFAULT_BACKUP_FILE_EXTENSION	".bak"

namespace tol
{
	typedef char buffer__[20];

	//e Error code which can occurs during backup file operation.
	enum rbf
	{
		//i No error.
		rbfOK,
		//i error by renaming.
		rbfRenaming,
		//i Error by duplication. Occurs only with 'TOLCreateBackupFile()'.
		rbfDuplication,
		//i Error by suppression. 
		rbfSuppression,
		//i Erreur by allocation. Occurs only with 'TOLRecoverBackupFile()'.
		rbfAllocation
	};
	//e How handle the backuped file.
	enum hbf
	{
		//i Rename it.
		hbfRename,
		//i Duplicate it.
		hbfDuplicate
	};
}


#ifdef CPE__VC
#	pragma warning( disable : 4284 )
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

	//f Return true if the file 'Name' exists, false otherwise.
	bool FileExists( const char *Nom );

	/*f Make a backup file from the file 'File', if exist, in adding 'Extension'.
	If 'Handle' == 'tol::hbfDuplicate', the backup file is create by duplicating the original one.
	If 'Handle' == 'tol::hbfRename', the bachup file is create by renaming the original one. */
	rbf CreateBackupFile(
		const char *Name,
		hbf Handle,
#ifdef CPE__CONSOLE
		txf::text_oflow__ &Flow = cio::cerr,
#else
		txf::text_oflow__ &Flow,
#endif
		const char *Extension = TOL_DEFAULT_BACKUP_FILE_EXTENSION,
		err::handle = err::hUsual  );

	//f Recover the backup file 'Name' with 'Extension' as extension.
	rbf RecoverBackupFile(
		const char *Name,
#ifdef CPE__CONSOLE
		txf::text_oflow__ &Flow = cio::cerr,
#else
		txf::text_oflow__ &Flow,
#endif
		const char *Extension = TOL_DEFAULT_BACKUP_FILE_EXTENSION,
		err::handle = err::hUsual  );

	//f Return the current date.
	const char *Date( buffer__ &Buffer );

	//f Return current time.
	const char *Time( buffer__ &Buffer );

	//f Return current date é time.
	const char *DateAndTime( buffer__ &Buffer );


#ifndef CPE__MT
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

#ifndef CPE__CW
	/*f Return a time in ms. Only usefull by susbstracting 2 value.
	Is different from 'clock()' because 'clock()' only return how long
	the application is using the processor.*/
	inline unsigned long Clock( void )
	{
		timeb T;

		::ftime( &T );
		// Attention: pas standard, mais présent sur toutes les plateformes utilisées.
		/* Cependant, la valeur de retour diffère selon les plateformes, mais ne signale
		jamais une erreur. */

		return T.time * 1000UL + T.millitm;
	}
#else	// CW temporary workaround.
	inline unsigned long Clock( void )
	{
		return 1000UL * ::clock() / CLOCKS_PER_SEC;
	}
#endif

	//f Suspend te current foe 'Delay' milliseconds.
	void Suspend( unsigned long Delay );

	//f Wait 'Seconds' seconds.
	inline void Wait( unsigned int Seconds )
	{
		Suspend( Seconds * 1000 );
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
	
#define E_XNAVt( Object, Type )\
	E_NAVt( Object, Type )\
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

#define E_NAV( Object )	E_NAVt( Object, epeios::row__ )
#define E_XNAV( Object )	E_XNAVt( Object, epeios::row__ )

namespace tol {
	/*f Force the program to exit after 'Seconds' second.
	Usefull to force a server to exit to obtain the profiling file. */
	void ForceExit( unsigned int Seconds );

	//f Tell the remainder to give hand to the next thread.
	void Defer( void );

	//f Tell the remainder to give hand to the next thread and wait 'Delay' millisecond.
	inline void Defer( unsigned long Delay )
	{
		Defer();

		Suspend( Delay );
	}
}

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

	template <typename t> class core_pointer___	// Classe de base de gestion d'un pointeur.
	{
	protected:
		t *P_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( P_ != NULL )
					free( P_ );

			P_ = NULL;
		}
		core_pointer___( void )
		{
			reset( false );
		}
		~core_pointer___( void )
		{
			reset( true );
		}
		void Init( void )
		{
			reset();
		}
		core_pointer___ &operator =( const core_pointer___ &Pointer )
		{
			ERRu();	// Otherwise the same ressource is used twice ; which delete them ?

			return *this;
		}
		t *operator =( t *P )
		{
			if ( P_ != NULL )
				free( (void *)P_ );

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
			return P_;
		}
		t &operator *( void )
		{
			return P_;
		}
	};

	template <typename t> class free_pointer___	// Classe de gestion d'un pointeur.devant être déalloué par un 'free'.
	: public core_pointer___<t>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( P_ != NULL )
					free( P_ );

			P_ = NULL;
		}
		t *operator =( t *P )
		{
			return core_pointer___<t>::operator =( P );
		}
		t *operator =( void *P )
		{
			return core_pointer___<t>::operator =( P );
		}
	};

	#define E_FPOINTER___( t )	free_pointer___<t>

	template <typename t> class delete_pointer___	// Classe de gestion d'un pointeur.devant être déalloué par un 'delete'.
	: public core_pointer___<t>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( P_ != NULL )
					delete P_;

			P_ = NULL;
		}
		t *operator =( t *P )
		{
			return core_pointer___<t>::operator =( P );
		}
		t *operator =( void *P )
		{
			return core_pointer___<t>::operator =( P );
		}
	};

	#define E_DPOINTER___( t )	delete_pointer___<t>

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

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
