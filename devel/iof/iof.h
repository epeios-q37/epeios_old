/*
	Header for the 'iof' library by Claude SIMON (csimon@epeios.org)
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

#ifndef IOF__INC
#define IOF__INC

#define IOF_NAME		"IOF"

#define	IOF_VERSION	"$Revision$"

#define IOF_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &IOFTutor;

#if defined( XXX_DBG ) && !defined( IOF_NODBG )
#define IOF_DBG
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
//D Input/Output Flows 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "cpe.h"
#include "fwf.h"
#include "flw.h"
#include "iop.h"

namespace iof {
	typedef iop::input__		_input__;
	typedef iop::output__		_output__;
	typedef iop::io__			_io__;

	class io_oflow_functions___
	: public _output__,
	  public fwf::oflow_functions___
	{
	protected:
		fwf::bsize__ FWFWrite(
			const fwf::datum__ *Tampon,
			fwf::bsize__ Minimum,
			fwf::bsize__ Demande )
		{
#ifdef IOF_DBG
			if ( ( Tampon == NULL ) && ( Minimum || Demande ) )
				ERRu();
#endif
			fwf::bsize__ Written = 0;

			while ( Written < Minimum )
				Written += _output__::Write( Tampon, Demande - Written );

			return Written;
		}
		void FWFSynchronize( void )
		{
			Flush();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_output__::reset( P );
			oflow_functions___::reset( P );
		}
		io_oflow_functions___( void )
		{
			reset( false );
		}
		virtual ~io_oflow_functions___( void )
		{
			reset();
		}
		void Init( iop::descriptor__ D )
		{
			_output__::Init( D );
			oflow_functions___::Init();
		}
	};

	class io_oflow___
	: public flw::oflow__
	{
	private:
		io_oflow_functions___ _Functions;
		flw::datum__ _Cache[IOP__BUFFER_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			oflow__::reset( P );
		}
		io_oflow___( iop::amount__ AmountMax )
		: oflow__( _Functions, _Cache, sizeof( _Cache ), AmountMax )
		{
			reset( false );
		}
		virtual ~io_oflow___( void )
		{
			reset();
		}
		void Init( iop::descriptor__ D )
		{
			_Functions.Init( D );
		}
	};

	class io_iflow_functions___
	: public _input__,
	  public fwf::iflow_functions___
	{
	protected:
		fwf::bsize__ FWFRead(
			fwf::bsize__ Minimum,
			fwf::datum__ *Tampon,
			fwf::bsize__ Desire )
		{
	#ifdef STF_DBG
			if( Tampon == NULL )
				ERRu();
	#endif
			fwf::bsize__ NombreLus = 0;

			while ( !OnEOF() && ( NombreLus < Minimum ) )
				NombreLus += _input__::Read( Desire - NombreLus, Tampon );

			return NombreLus;
		}
		void FWFDismiss( void )
		{}
	public:
		void reset( bso::bool__ P = true )
		{
			_input__::reset( P );
			iflow_functions___::reset( P );
		}
		io_iflow_functions___( void )
		{
			reset( false );
		}
		virtual ~io_iflow_functions___( void )
		{
			reset();
		}
		void Init( iop::descriptor__ D )
		{
			_input__::Init( D );
			iflow_functions___::Init();
		}
	};

	class io_iflow___
	: public flw::iflow__
	{
	private:
		flw::datum__ _Cache[IOP__BUFFER_SIZE];
		io_iflow_functions___ _Functions;
	public:
		void reset( bso::bool__ P = true )
		{
			_Functions.reset( P );
			iflow__::reset( P );
		}
		io_iflow___( iop::amount__ AmountMax )
		: iflow__( _Functions, _Cache, sizeof( _Cache ), AmountMax )
		{
			reset( false );
		}
		virtual ~io_iflow___( void )
		{
			reset();
		}
		void Init( iop::descriptor__ D )
		{
			_Functions.Init( D );
		}
	};

	class io_flow_functions___
	: public _output__,
	  public _input__,
	  public fwf::ioflow_functions___
	{
	protected:
		fwf::bsize__ FWFWrite(
			const fwf::datum__ *Tampon,
			fwf::bsize__ Minimum,
			fwf::bsize__ Demande )
		{
#ifdef IOF_DBG
			if ( ( Tampon == NULL ) && ( Minimum || Demande ) )
				ERRu();
#endif
			fwf::bsize__ Written = 0;

			while ( Written < Minimum )
				Written += _output__::Write( Tampon, Demande - Written );

			return Written;
		}
		void FWFSynchronize( void )
		{
			Flush();
		}
		fwf::bsize__ FWFRead(
			fwf::bsize__ Minimum,
			fwf::datum__ *Tampon,
			fwf::bsize__ Desire )
		{
	#ifdef STF_DBG
			if( Tampon == NULL )
				ERRu();
	#endif
			fwf::bsize__ NombreLus = 0;

			while ( !OnEOF() && ( NombreLus < Minimum ) )
				NombreLus += _input__::Read( Desire - NombreLus, Tampon );

			return NombreLus;
		}
		void FWFDismiss( void )
		{}
	public:
		void Init( iop::descriptor__ D  )
		{
			_input__::Init( D );
			_output__::Init( D );
		}

	};

	class io_flow___
	: public _io__,
	  public flw::ioflow__
	{
	private:
		io_flow_functions___ _Functions;
		flw::datum__ _Cache[2 * IOP__BUFFER_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			_io__::reset( P );
			ioflow__::reset( P );
		}
		io_flow___( iop::amount__ AmountMax )
		: ioflow__( _Functions, _Cache, sizeof( _Cache ), AmountMax )
		{
			reset( false );
		}
		virtual ~io_flow___( void )
		{
			reset();
		}
		void Init(
			iop::descriptor__ D,
			iop::amount__ AmountMax )
		{
			_io__::Init( D );
			_Functions.Init( D );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
