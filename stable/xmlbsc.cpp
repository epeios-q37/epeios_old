/*
  'xmlbsc' library by Claude L. Simon (csimon@webmails.com)
  Requires the 'xmlbsc' header file ('xmlbsc.h').
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@webmails.com).

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

#define XMLBSC__COMPILATION

#include "xmlbsc.h"

class xmlbsctutor
: public ttr_tutor
{
public:
	xmlbsctutor( void )
	: ttr_tutor( XMLBSC_NAME )
	{
#ifdef XMLBSC_DBG
		Version = XMLBSC_VERSION "\b\bD $";
#else
		Version = XMLBSC_VERSION;
#endif
		Owner = XMLBSC_OWNER;
		Date = "$Date$";
	}
	virtual ~xmlbsctutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

namespace xmlbsc {
}

#include "fnm.h"

static void Init_(
	flm::file_memory_driver &Driver,
	char *Buffer,
	const char *Discriminator,
	const char *Name,	
	const char *Directory,
	const char *Suffix )
{
ERRProlog
	char *FileName = NULL;
ERRBegin
ERRErr
ERREnd
	if ( FileName != NULL )
		free( FileName );
		
	sprintf( Buffer, "%s%s", Name, Discriminator );

	FileName = fnm::MakeFileName( Directory, Buffer, Suffix );
	
	Driver.Init( FileName );
ERREpilog
}


void xmlbsc::file_memory_drivers::Init(
	const char *Name,	
	const char *Directory,
	const char *Suffix)
{
ERRProlog
	char *Buffer = NULL;
ERRBegin
	if ( ( Buffer = (char *)malloc( strlen( Name ) + 2 ) ) == NULL )
		ERRa();

	Init_( DTree.Tree, Buffer, "T", Name, Directory, Suffix );			
	Init_( DTree.Queue, Buffer, "Q", Name, Directory, Suffix );
	Init_( List, Buffer, "L", Name, Directory, Suffix );
	Init_( Container.Statics, Buffer, "S", Name, Directory, Suffix );
	Init_( Container.Dynamics.Descriptors, Buffer, "D", Name, Directory, Suffix );
	Init_( Container.Dynamics.Multimemory, Buffer, "M", Name, Directory, Suffix );
	
ERRErr
ERREnd
	if ( Buffer != NULL )
		free( Buffer );
ERREpilog
}
	

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xmlbscpersonnalization
: public xmlbsctutor
{
public:
	xmlbscpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xmlbscpersonnalization( void )
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

static xmlbscpersonnalization Tutor;

ttr_tutor &XMLBSCTutor = Tutor;
