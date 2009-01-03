/*
	'kernel.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'emobda' software.

    'emobda' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'emobda' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#ifndef KERNEL__INC
#define KERNEL__INC

#include "../../mbdbkd.h"
#include "ui.h"

#include "csdsnc.h"
#include "csducl.h"
#include "xml.h"


namespace kernel {

	typedef mbdbkd::backend___ _backend___;

	class log_functions__
	: public csdsnc::log_functions__
	{
	protected:
		virtual void CSDSNCLog(
			csdsnc::log__ Log,
			const void *Flow,
			epeios::size__ Amount )
		{}
	};

	class kernel___
	: public ui::bridge_functions__,
	  public _backend___
	{
	private:
		log_functions__ _LogFunctions;
		csducl::universal_client_core _ClientCore;
		void _DumpFields( xml::writer_ &Writer );
		void _DumpFieldsAsXML( str::string_ &XML );
		void _H( bso::bool__ Result )
		{
		ERRProlog
			tol::E_FPOINTER___( bso::char__ ) Buffer;
			str::string Message;
		ERRBegin
			if ( !Result ) {
				Message.Init();
				Message.Append( GetRawMessage() );
				Message.Append( '\n' );
				Message.Append( GetI18Message() );

				nsxpcm::Alert( UI.Main.Window, Buffer = Message.Convert() );
				ERRReturn;
			}
		ERRErr
		ERREnd
		ERREpilog
		}
	protected:
		virtual const kernel___ &K_( void ) const
		{
			return *this;
		}
		virtual kernel___ &K_( void )
		{
			return *this;
		}
	public:
		ui::ui__ UI;
		void reset( bso::bool__ P = true )
		{
			_backend___::reset( P );
			_ClientCore.reset( P );
		}
		kernel___( void )
		{
			reset( false );
		}
		~kernel___( void )
		{
			reset();
		}
		void Init( void )
		{
			reset();

			_ClientCore.Init( "localhost:1234", NULL, _LogFunctions, csducl::tShared );
			_backend___::Init( _ClientCore );
			UI.Init();
		}
		bso::bool__ CreateTable( const str::string_ &Location )
		{
			return Manager.CreateTable( Location );
		}
		void CreateTable( void )
		{
		ERRProlog
			str::string Location;
		ERRBegin
			Location.Init();
			UI.Main.TableLocationTextbox.GetValue( Location );

			if ( !CreateTable( Location ) )
				nsxpcm::Alert( UI.Main.Window, GetRawMessage() );
		ERRErr
		ERREnd
		ERREpilog
		}
		bso::bool__ AddField( const str::string_ &Name )
		{
			bkdacc::id32__ Row = NONE;

			return Manager.AddField( Name, Row );
		}
		void AddField( void )
		{
		ERRProlog
			str::string Name;
		ERRBegin
			Name.Init();
			UI.Main.FieldNameTextbox.GetValue( Name );

			if ( !AddField( Name ) )
				nsxpcm::Alert( UI.Main.Window, GetRawMessage() );

			RefreshFieldList();
		ERRErr
		ERREnd
		ERREpilog
		}
		void RefreshFieldList( void );
		void ModifyField( void )
		{
			nsxpcm::Alert( UI.Main.Window, "FieldListModification detected !" );
			RefreshFieldList();
		}
	};
}

#endif