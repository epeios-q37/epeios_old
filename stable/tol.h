/*
  Header for the 'tol' library by Claude L. Simon (epeios@epeios.org)
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
//C Claude L. SIMON (epeios@epeios.org)
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

struct tol
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
private:\
	type Objet_;\
public:\
	alias( void ) {}\
	alias( type V )\
	{\
		Objet_ = V;\
	}\
	type &operator()( void ) const\
	{\
		return (type &)Objet_;\
	}\
	bool operator ==( alias A ) const\
	{\
		return Objet_ == A.Objet_;\
	}\
	bool operator !=( alias A ) const\
	{\
		return Objet_ != A.Objet_;\
	}\
	bool operator ==( type A ) const\
	{\
		return Objet_ == A;\
	}\
	bool operator !=( type A ) const\
	{\
		return Objet_ != A;\
	}\
}

#if 0	// to delete ?
/* Permet de transformer 2 arguments en 1; si un argument d'une macro
a besoin de contenir une virgule, cette macro est là pour ça
'TOL_2EN1( a, b )' donne 'a, b' */
#define TOL_2EN1(a, b)	a, b
#endif

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
template < TypeName t > class Name\
: public Name##_<t>\
{\
public:\
	Name##_<t>::s static_;\
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


//m Same as 'AUTO()' but with one template parameter.
#define AUTO1( Name )	AUTO1t( Name, class )


//m Same as 'AUTO()' but with two template parameter.
#define AUTO2( Name )	\
template < class t, class st > class Name\
: public Name##_<t,st>\
{\
public:\
	Name##_<t,st>::s static_;\
	Name ( void )\
	: Name##_<t,st>( static_ )\
	{\
		reset( false );\
	}\
	~Name ( void )\
	{\
		reset( true );\
	}\
	Name &operator =( const Name &S )\
	{\
		Name##_<t,st>::operator =( S );\
\
		return *this;\
	}\
	Name &operator =( const Name##_<t,st> &S )\
	{\
		Name##_<t,st>::operator =( S );\
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

//m Define the 'First', 'Next', Last', Previous', 'Amount',  using 'Object' and 'Type'.
#define FNLPAt( Object, Type )\
	Type First( void ) const\
	{\
		return Object##First();\
	}\
	Type Last( void ) const\
	{\
		return Object##Last();\
	}\
	Type Previous( POSITION__ P ) const\
	{\
		return Object##Previous( P );\
	}\
	Type Next( Type P ) const\
	{\
		return Object##Next( P );\
	}\
	SIZE__ Amount( void ) const\
	{\
		return Object##Amount();\
	}

	
//m Define the 'First', 'Next', Last', Previous', 'Amount',  using 'Object'.
#define FNLPA( Object )	FNLPAt( Object, POSITION__ )

//f Tell the remainder to give hand to the next thread.
void TOLYield( void );

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
