/*
  'dtfrmf' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'dtfrmf' header file ('dtfrmf.h').
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

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
  along with this program; if not, go to http://www.fsf.org or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#define DTFRMF__COMPILATION

#include "dtfrmf.h"

class dtfrmftutor
: public ttr_tutor
{
public:
	dtfrmftutor( void )
	: ttr_tutor( DTFRMF_NAME )
	{
#ifdef DTFRMF_DBG
		Version = DTFRMF_VERSION " (DBG)";
#else
		Version = DTFRMF_VERSION;
#endif
		Owner = DTFRMF_OWNER;
		Date = "$Date$";
	}
	virtual ~dtfrmftutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace dtfrmf;

void dtfrmf::Encapsulate(
	const flx::set_ &Data,
	flw::oflow___ &Flow )
{
ERRProlog
	flx::set_iflow___ IFlow;
	SIZE__ Size;
ERRBegin
	IFlow.Init( Data );
	
	Size = Data.Amount();
	
	PutLong( Size, Flow );

	while( Size-- )	// Should be optimized by using a buffer.
		Flow.Put( IFlow.Get() );
ERRErr
ERREnd
ERREpilog
}

void dtfrmf::Decapsulate(
	flw::iflow___ &Flow,
	flx::set_ &Data )
{
ERRProlog
	flx::set_oflow___ OFlow;
	SIZE__ Size;
ERRBegin
	OFlow.Init( Data );

	Size = GetLong( Flow );

	while( Size-- )	// Should be optimized by using a buffer.
		OFlow.Put( Flow.Get() );
ERRErr
ERREnd
ERREpilog
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dtfrmfpersonnalization
: public dtfrmftutor
{
public:
	dtfrmfpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dtfrmfpersonnalization( void )
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

static dtfrmfpersonnalization Tutor;

ttr_tutor &DTFRMFTutor = Tutor;
