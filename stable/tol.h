/*
	Header for the 'tol' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2004 Claude SIMON (csimon@epeios.org).

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
		reset( false );\
	}\
	~Name ( void )\
	{\
		reset( true );\
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
		reset( false );\
	}\
	~Name ( void )\
	{\
		reset( true );\
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
		reset( false );\
	}\
	~Name ( void )\
	{\
		reset( true );\
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
		reset( false );\
	}\
	~Name ( void )\
	{\
		reset( true );\
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
		const char *Extension = TOL_DEFAULT_BACKUP_FILE_EXTENSION,
		err::handle = err::hUsual  );

	//f Recover the backup file 'Name' with 'Extension' as extension.
	rbf RecoverBackupFile(
		const char *Name,
		const char *Extension = TOL_DEFAULT_BACKUP_FILE_EXTENSION,
		err::handle = err::hUsual  );

	//f Return the current date.
	const char *Date( void );

	//f Return current time.
	const char *Time( void );

	//f Return current date é time.
	const char *DateAndTime( void );

#ifndef CPE__CW
	/*f Return a time in ms. Only usefull by susbstracting 2 value.
	Is different from 'clock()' because 'ckock()' only return how long
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
#endif

	//f Wait 'Seconds' seconds.
	void Wait( unsigned int Seconds );

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
	
#define E_NAV( Object )	E_NAVt( Object, epeios::row__ )

namespace tol {
	/*f Force the program to exit after 'Seconds' second.
	Usefull to force a server to exit to obtain the profiling file. */
	void ForceExit( unsigned int Seconds );

	//f Tell the remainder to give hand to the next thread.
	void Defer( void );
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

	//f Free 'Pointer' only if != NULL. Pointer value becomes 'NULL'.
	template <typename t> inline void Free( t *&Pointer )
	{
		if ( Pointer != NULL ) {
			free( (void *)Pointer );
			Pointer = NULL;
		}
	}

//d Make accessible the static member named 'name' of type 'type__'.
#define E_DISCLOSE(type__, name )\
	const type__ &name( void ) const\
	{\
		return S_.name;\
	}\
	type__ &name( void )\
	{\
		return S_.name;\
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
