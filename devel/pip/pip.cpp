/*
  'pip' library by Claude L. Simon (simon@epeios.org)
  Requires the 'pip' header file ('pip.h').
  Copyright (C) 2000 Claude L. SIMON (simon@epeios.org).

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

#define PIP__COMPILATION

#include "pip.h"

class piptutor
: public ttr_tutor
{
public:
	piptutor( void )
	: ttr_tutor( PIP_NAME )
	{
#ifdef PIP_DBG
		Version = PIP_VERSION "\b\bD $";
#else
		Version = PIP_VERSION;
#endif
		Owner = PIP_OWNER;
		Date = "$Date$";
	}
	virtual ~piptutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace pip;

#include "stf.h"
#include "tol.h"

#ifdef PIP__UNIX_LIKE
#define INVALID_HANDLE_VALUE	-1	// Already defined under Windows.
#endif

void pip::pipe___::Vider( void )
{
	data__ Tampon[100];

	do {
		Read( sizeof( Tampon ), Tampon );
	} while( !IsEmpty() );
}

pip::amount__ pip::pipe___::Write(
	const data__ *Buffer,
	amount__ Amount,
	bso__bool Synchronization )
{
	pip::amount__ NombreEcrits;
ERRProlog
	bso__bool Locked = false;
ERRBegin
#ifdef PIP_DBG
	if ( ( Pipe_[pip::eWrite] == INVALID_HANDLE_VALUE )
		 || ( Pipe_[pip::eRead] == INVALID_HANDLE_VALUE ) )
		 ERRu();
#endif

	mtx::Lock( Mutex_ );
	Locked = true;

	Available_ += Amount;

	Synchronization_ = Synchronization;
	
	mtx::Unlock( Mutex_ );
	Locked = false;

	NombreEcrits = pip::Write( Pipe_, Buffer, Amount );

	mtx::Lock( Mutex_ );
	Locked = true;

	Available_ -= Amount - NombreEcrits;

	Synchronization_ = Synchronization && ( NombreEcrits == Amount );
ERRErr
ERREnd
	if ( Locked )
		mtx::Unlock( Mutex_ );
ERREpilog
	return NombreEcrits;
}

pip::amount__ pip::pipe___::Read(
	amount__ Amount,
	data__ *Buffer )
{
	pip::amount__ NombreLus;
ERRProlog
	bso__bool Locked = false;
ERRBegin
#ifdef PIP_DBG
	if ( ( Pipe_[pip::eWrite] == INVALID_HANDLE_VALUE )
		 || ( Pipe_[pip::eRead] == INVALID_HANDLE_VALUE ) )
		 ERRu();
#endif

	NombreLus = pip::Read( Pipe_, Amount, Buffer );

	mtx::Lock( Mutex_ );
	Locked = true;

#ifdef PIP_DBG
	if ( Available_ < NombreLus )
		ERRc();
#endif

	Available_ -= NombreLus;

ERRErr
ERREnd
	if ( Locked )
		mtx::Unlock( Mutex_ );
ERREpilog
	return NombreLus;
}

static flw::amount__ Get_(
	pipe___ &Pipe,
	flw::amount__ Minimum,
	flw::data__ *Buffer,
	flw::amount__ Wanted )
{
	flw::amount__ Amount = 0;

	if ( Minimum )
	{
		while ( Amount < Minimum )
			Amount += Pipe.Read( Wanted - Amount, Buffer + Amount );
	}
	else
		if ( Pipe.Amount() )
			Amount = Pipe.Read( Wanted, Buffer );

	return Amount;
}

static flw::amount__ Put_(
	pipe___ &Pipe,
	const flw::data__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	flw::amount__ Amount = 0;

	if ( Minimum )
	{
		while ( Amount < Minimum )
			Amount += Pipe.Write( Buffer + Amount, Wanted - Amount, Synchronization );
	}
	else if ( !Pipe.Amount() )
		Amount = Pipe.Write( Buffer, Wanted, Synchronization );

	return Amount;
}

flw::amount__ pip::pipe_ioflow___::FLWGet(
	flw::amount__ Minimum,
	flw::data__ *Buffer,
	flw::amount__ Wanted )
{
	return ::Get_( Pipe_, Minimum, Buffer, Wanted );
}

flw::amount__ pip::pipe_ioflow___::FLWPut(
	const flw::data__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	return ::Put_( Pipe_, Buffer, Wanted, Minimum, Synchronization );
}

flw::amount__ pip::master_bipipe_ioflow___::FLWGet(
	flw::amount__ Minimum,
	flw::data__ *Buffer,
	flw::amount__ Wanted )
{
	return ::Get_( Bipipe_->Slave, Minimum, Buffer, Wanted );
}

flw::amount__ pip::master_bipipe_ioflow___::FLWPut(
	const flw::data__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	return ::Put_( Bipipe_->Master, Buffer, Wanted, Minimum, Synchronization );
}


flw::amount__ pip::slave_bipipe_ioflow___::FLWGet(
	flw::amount__ Minimum,
	flw::data__ *Buffer,
	flw::amount__ Wanted )
{
	return ::Get_( Bipipe_->Master, Minimum, Buffer, Wanted );
}

flw::amount__ pip::slave_bipipe_ioflow___::FLWPut(
	const flw::data__ *Buffer,
	flw::amount__ Wanted,
	flw::amount__ Minimum,
	bool Synchronization )
{
	return ::Put_( Bipipe_->Slave, Buffer, Wanted, Minimum, Synchronization );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class pippersonnalization
: public piptutor
{
public:
	pippersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~pippersonnalization( void )
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

static pippersonnalization Tutor;

ttr_tutor &PIPTutor = Tutor;
