/*
  Header for the 'stf' library by Claude L. Simon (simon@epeios.org)
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

#ifndef STF__INC
#define STF__INC

#define STF_NAME		"STF"

#define	STF_VERSION	"$Revision$"	

#define STF_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &STFTutor;

#if defined( XXX_DBG ) && !defined( STF_NODBG )
#define STF_DBG 
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

#include <fstream.h>

#include "err.h"
#include "flw.h"
#include "txf.h"

#ifndef STF_STREAM_FLOW_BUFFER_SIZE
//d Buffer size for a stream output flow. 100 by default.
#define STF_STREAM_FLOW_BUFFER_SIZE	100
#endif


namespace stf {
	using flw::oflow___;
	using flw::iflow___;
	
	//c A stream output flow driver.
	class ostream_oflow___
	: public oflow___
	{
	private:
		ostream &Stream_;
		flw::data__ Cache_[STF_STREAM_FLOW_BUFFER_SIZE];
	protected:
		virtual flw::amount__ FLWPut(
			const flw::data__ *Tampon,
			flw::amount__ Nombre,
			flw::amount__,
			bool Synchronize )
		{
#ifdef STF_DBG
			if ( ( Tampon == NULL ) && Nombre )
				ERRu();
#endif
			if ( Stream_.write( (char *)Tampon, Nombre ).fail() )
			{
				if ( Tampon == NULL )
					Stream_.clear();	// Some compiler (BC++ V5.5) doesn't like 'Tampon' = NULL, even if 'Nombre' = 0.
				else
					ERRd();
			}

			if ( Synchronize )
				Stream_.flush();

			return Nombre;
		}
	public:
		void reset( bool P = true )
		{
			oflow___::reset( P );
		}
		ostream_oflow___( ostream &Stream )
		: Stream_( Stream ),
		  oflow___()
		{
			reset( false );
		}
		virtual ~ostream_oflow___( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{
			reset();
		
			oflow___::Init( Cache_, sizeof( Cache_ ) );
		}
	};


	namespace {			
		//c Internal use. Core of an input flow from an 'istream'.
		class istream_iflow_core___
		: public iflow___
		{
		private:
			flw::data__ Cache_[STF_STREAM_FLOW_BUFFER_SIZE];
		protected:
			// Le stream en question.
			istream &Stream_;
			flw::amount__ HandleAmount_(
				flw::amount__ Minimum,
				flw::data__ *Tampon,
				flw::amount__ Desire,
				flw::amount__ AmountRead )
			{
				if ( AmountRead < Minimum )
				{
					if ( !Stream_.eof() )
						ERRd();
					else
						AmountRead += iflow___::HandleEOFD( Tampon + AmountRead, Desire - AmountRead );

					if ( AmountRead < Minimum )
						ERRd();
				}

				return AmountRead;
			}
		public:
			void reset( bool = true )
			{
			}
			istream_iflow_core___( istream &Stream )
			: iflow___(),
			  Stream_( Stream )
			{
				reset( false );
			}
			virtual ~istream_iflow_core___( void )
			{
				reset( true );
			}
			//f Initialisation.
			void Init( void )
			{
				iflow___::Init( Cache_, sizeof( Cache_ ) );
			}
		};
	}

	//c Internal use. File input flow from an 'istream'.
	class istream_iflow___
	: public istream_iflow_core___
	{
	protected:
		virtual flw::amount__ FLWGet(
			flw::amount__ Minimum,
			flw::data__ *Tampon,
			flw::amount__ Desire )
		{
	#ifdef STF_DBG
			if( Tampon == NULL )
				ERRu();
	#endif
			flw::amount__ NombreLus = 0;

			if ( !Stream_.eof() )
			{
				if ( Stream_.read( (char *)Tampon, Desire ).bad() )
					ERRd();

				NombreLus = Stream_.gcount();
			}

			return HandleAmount_( Minimum, Tampon, Desire, NombreLus );
		}
	public:
		void reset( bool P = true )
		{
			istream_iflow_core___::reset( P );
		}
		istream_iflow___( istream &Stream )
		: istream_iflow_core___( Stream )
		{
			reset( false );
		}
		virtual ~istream_iflow___( void )
		{
			reset( true );
		}
		//f Initialisation.
		void Init( void )
		{
			istream_iflow_core___::Init();
		}
	};



	//c Internal use. File input flow from an 'istream'.
	class istream_iflow_line___
	: public istream_iflow_core___
	{
	protected:
		virtual flw::amount__ FLWGet(
			flw::amount__ Minimum,
			flw::data__ *Tampon,
			flw::amount__ Desire )
		{
			flw::amount__ NombreLus = 0;

			if ( !Stream_.eof() )
			{
				if ( Stream_.getline( (char *)Tampon, Desire ).bad() )
					ERRd();

				NombreLus = Stream_.gcount();

				if ( NombreLus && !Tampon[NombreLus-1] )
					Tampon[NombreLus-1]='\n';
			}

			return HandleAmount_( Minimum, Tampon, Desire, NombreLus );
		}
	public:
		void reset( bool P = true )
		{
			istream_iflow_core___::reset( P );
		}
		istream_iflow_line___( istream &Stream )
		: istream_iflow_core___( Stream )
		{
			reset( false );
		}
		virtual ~istream_iflow_line___( void )
		{
			reset( true );
		}
		//f Initialisation.
		void Init( void )
		{
			istream_iflow_core___::Init();
		}
	};


	//o Standard output as a pure flow (not a text flow).
	extern ostream_oflow___ foutF;

	//o Error output as a pure flow (not a text flow).
	extern ostream_oflow___ ferrF;

	//o Standard input as a pure flow (not a text flow).
	extern istream_iflow___ finF;

	//o Standard input as a pure flow (not a text flow), but red line by line.
	extern istream_iflow_line___ finLF;

	//o Standard output as a text flow.
	extern txf::text_oflow___ fout;

	//o Error output as a text flow.
	extern txf::text_oflow___ ferr;

	//o Standard input as a text flow.
	extern txf::text_iflow___ fin;
}

using stf::fout;
using stf::ferr;
using stf::fin;

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
