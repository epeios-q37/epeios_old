/*
  Header for the 'tol' library by Claude L. Simon (simon@epeios.org)
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

#ifndef TOL__INC
#define TOL__INC

#define TOL_NAME		"TOL"

#define	TOL_VERSION	"$Revision$"	

#define TOL_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &TOLTutor;

#if defined( XXX_DBG ) && !defined( TOL_NODBG )
#define TOL_DBG 
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
#include <string.h>
#include <stddef.h>
#include <time.h>

#include "cpe.h"
#include "err.h"

#if defined( CPE__MS ) || defined( CPE__UNIX ) || defined( CPE__BEOS )
#include <sys/timeb.h>
#else
#error "Unknown compilation enviroment"
#endif

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
};


#if 0 // to delete ?
template <class t> class tol__manipulateur
{
private:
	t M_;
public:
	tol__manipulateur( t M = 0 )
	{
		M_ = M;
	}
	operator t( void )
	{
		return M_;
	}
	t &operator()( void )
	{
		return M_;
	}
};
#endif

//m Same as 'typedef type alias', but 2 'alias' whit same 'type' are not interchangeable.
#define TYPEDEF( type, alias )\
struct alias\
{\
public:\
	type V;\
	alias( void ) {}\
	alias( type T )\
	{\
		V = T;\
	}\
	bool operator ==( alias A ) const\
	{\
		return V == A.V;\
	}\
	bool operator !=( alias A ) const\
	{\
		return V != A.V;\
	}\
	bool operator ==( type T ) const\
	{\
		return V == T;\
	}\
	bool operator !=( type T ) const\
	{\
		return V != T;\
	}\
}

#if 0	// to delete ?
/* Permet de transformer 2 arguments en 1; si un argument d'une macro
a besoin de contenir une virgule, cette macro est là pour ça
'TOL_2EN1( a, b )' donne 'a, b' */
#define TOL_2EN1(a, b)	a, b
#endif

/*m Create the autonomous definition of the 'Name' object
based on the 'Name'_ object and handling row of type 'r'. */
#define AUTOt( Name )	\
template <typename r> class Name\
: public Name##_<r>\
{\
public:\
	Name##_<r>::s static_;\
	Name( void )\
	: Name##_<r>( static_ )\
	{\
		reset( false );\
	}\
	~Name( void )\
	{\
		reset( true );\
	}\
	Name &operator =( const Name &S )\
	{\
		Name##_<r>::operator =( S );\
\
		return *this;\
	}\
	Name &operator =( const Name##_<r> &S )\
	{\
		Name##_<r>::operator =( S );\
\
		return *this;\
	}\
};



//m Create the autonomous definition of the 'name' object based on the 'name'_ object.
#define AUTO( Name )	\
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

//m Same as 'AUTO()' but with one template parameter of type 'TypeName'
#define AUTO1t( Name, TypeName )	\
template < TypeName t, typename r > class Name\
: public Name##_<t,r>\
{\
public:\
	Name##_<t,r>::s static_;\
	Name ( void )\
	: Name##_<t,r>( static_ )\
	{\
		reset( false );\
	}\
	~Name ( void )\
	{\
		reset( true );\
	}\
	Name &operator =( const Name &S )\
	{\
		Name##_<t,r>::operator =( S );\
\
		return *this;\
	}\
	Name &operator =( const Name##_<t,r> &S )\
	{\
		Name##_<t,r>::operator =( S );\
\
		return *this;\
	}\
};


//m Same as 'AUTO()' but with one template parameter.
#define AUTO1( Name )	AUTO1t( Name, class )


//m Same as 'AUTO()' but with two template parameter.
#define AUTO2( Name )	\
template < class t, class st, typename r > class Name\
: public Name##_<t,st,r>\
{\
public:\
	Name##_<t,st,r>::s static_;\
	Name ( void )\
	: Name##_<t,st,r>( static_ )\
	{\
		reset( false );\
	}\
	~Name ( void )\
	{\
		reset( true );\
	}\
	Name &operator =( const Name &S )\
	{\
		Name##_<t,st,r>::operator =( S );\
\
		return *this;\
	}\
	Name &operator =( const Name##_<t,st,r> &S )\
	{\
		Name##_<t,st,r>::operator =( S );\
\
		return *this;\
	}\
};



/*********************************************/
/* CLASSE DE GESTION DU POINTEUR UTILISATEUR */
/*********************************************/

//c Handle user pointer.
class tol__UP_
{
protected:
	void *UP_;
public:
	tol__UP_( void )
	{
		reset( false );
	}
	~tol__UP_( void )
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



#if 0	// to delete ?
/**************************************************************/
/* CLASSE DE GESTION DIRECTE DE LA PARTIE STATIQUE D'UN OBJET */
/**************************************************************/

template <class T> class tol_D_
{
public:
	T *operator ->( void )
	{
		return this;
	}
};
#endif

//f Return true if the file 'Name' exists, false otherwise.
bool TOLFileExists( const char *Nom );

/*f Make a backup file from the file 'File', if exist, in adding 'Extension'.
If 'Handle' == 'tol::hbfDuplicate', the backup file is create by duplicating the original one.
If 'Handle' == 'tol::hbfRename', the bachup file is create by renaming the original one. */
tol::rbf TOLCreateBackupFile(
	const char *Name,
	tol::hbf Handle,
	const char *Extension = TOL_DEFAULT_BACKUP_FILE_EXTENSION,
	err::handle = err::hUsual  );

//f Recover the backup file 'Name' with 'Extension' as extension.
tol::rbf TOLRecoverBackupFile(
	const char *Name,
	const char *Extension = TOL_DEFAULT_BACKUP_FILE_EXTENSION,
	err::handle = err::hUsual  );

//f Return the current date.
const char *TOLDate( void );

//f Return current time.
const char *TOLTime( void );

//f Return current date é time.
const char *TOLDateAndTime( void );

/*f Return a time in ms. Only usefull by susbstracting 2 value.
Is different from 'clock()' because 'ckock()' only return how long
the application is usibg the processor.*/
inline unsigned long TOLClock( void )
{
	timeb T;

	::ftime( &T );
	// Attention: pas standard, mais présent sur toutes les plateformes utilisées.
	/* Cependant, la valeur de retour diffère selon les plateformes, mais ne signale
	jamais une erreur. */

	return T.time * 1000UL + T.millitm;
}

//f Wait 'Seconds' seconds.
void TOLWait( unsigned int Seconds );

/*f Initialize the random generator using the date & time.
The used value is returned to be used with the following
function to make the random generator always start with the same value. */
inline unsigned int TOLInitializeRandomGenerator( void )
{
	unsigned int Seed = (unsigned int)time( NULL );

	srand( Seed );

	return Seed;
}

//f Initialize the random generator with 'Seed'.
inline void TOLInitializeRandomGenerator( unsigned int Seed )
{
	srand( Seed );
}

//m Define navigation functions ( 'First', 'Next', Amount', ... ) using 'Object' and 'Type'.
#define NAVt( Object, Type )\
	Type First( void ) const\
	{\
		return Object##First();\
	}\
	Type Last( void ) const\
	{\
		return Object##Last();\
	}\
	Type Previous( tym::row__ P ) const\
	{\
		return Object##Previous( P );\
	}\
	Type Next( Type P ) const\
	{\
		return Object##Next( P );\
	}\
	epeios::size__ Amount( void ) const\
	{\
		return Object##Amount();\
	}\
	epeios::size__ Extent( void ) const\
	{\
		return Object##Extent();\
	}\
	bso::bool__ IsEmpty( void ) const\
	{\
		return Object##IsEmpty();\
	}
	
#define NAV( Object )	NAVt( Object, epeios::row__ )

#if 0
	// probably to delete	
//m Define navigation functions ( 'First', 'Next', Amount', ... ) using 'Object'.
#define NAV( Object )	NAVt( Object, tym::row__ )
#endif

//f Tell the remainder to give hand to the next thread.
void TOLYield( void );


#ifdef CPE__MT
/*f Force the program to exit after 'Seconds' second.
Usefull to force a server to exit to obtain the profiling file. */
void TOLForceExit( unsigned int Seconds );
#endif

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

	AUTO1( object )

	//f Free 'Pointer' only if != NULL. Pointer value becomes 'NULL'.
	template <typename t> inline void Free( t *&Pointer )
	{
		if ( Pointer != NULL ) {
			free( Pointer );
			Pointer = NULL;
		}
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
