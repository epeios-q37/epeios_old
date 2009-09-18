/*
	'frdtrnsnt.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef FRDTRNSNT__INC
#define FRDTRNSNT__INC

#include "frdbcknd.h"

namespace frdtrnsnt {

	enum context__ {
		cStructureManagement,
		cDatabaseIdentification,
		cDatabaseSelection,
		cBackendSelection,
		cRecordInput,
		c_amount,
		c_Undefined
	};

	const char *GetContextLabel( context__ Context );

	template <typename e, e amount> struct _state__
	{
	private:
		e _State;
		e _Undefined( void ) const
		{
			return (e)( amount + 1 );
		}
		int _Amount( void ) const
		{
			return amount;
		}
	public:
		void reset( bso::bool__ = true )
		{
			_State = _Undefined();
		}
		e GetState( void ) const
		{
			return _State;
		}
		void SetState( e State )
		{
			if ( ( _State > _Amount() ) && ( _State != _Undefined() ) )
				ERRu();

			_State = State;
		}
	};

	enum structure_management_state__
	{
		smsModification,
		smsCreation,
		smsDuplication,
		sms_amount,
		sms_Undefined,
	};

	typedef _state__<structure_management_state__, sms_amount> _sms__;

	struct structure_management
	: public _sms__
	{
		void reset( bso::bool__ P = true )
		{
			_sms__::reset( P );
		}

	};

	enum database_identification_state__ {
		disValidated,
		disCancelled,
		dis_amount,
		dis_Undefined
	};

	typedef _state__<database_identification_state__, dis_amount> _dis__;

	struct database_identification
	: public _dis__
	{
	private:
		str::string _Name, _Path, _Comment;
		void _SetState( database_identification_state__ State )
		{
			_dis__::SetState( State );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_dis__::reset( P );

			_Name.reset();
			_Path.reset();
			_Comment.reset();
		}
		void Set(
			const str::string_ &Name,
			const str::string_ &Path,
			const str::string_ &Comment )
		{
			if ( GetState() != dis_Undefined )
				ERRc();

			_SetState( disValidated );

			_Name.Init( Name );
			_Path.Init( Path );
			_Comment.Init( Comment );
		}
		void SetCancelledState( void )
		{
			if ( GetState() != dis_Undefined )
				ERRc();

			_SetState( disCancelled );
		}
		void Get(
			str::string_ &Name,
			str::string_ &Path,
			str::string_ &Comment ) const
		{
			if ( GetState() != disValidated )
				ERRc();

			Name = _Name;
			Path = _Path;
			Comment = _Comment;
		}
		void SetState( void )	// To avoid the use of the '_dis__' one, because it has not to be used directly.
		{
			ERRu();
		}
	};

	enum backend_selection_state__ {
		bssLocal,
		bssRemote,
		bssCancelled,
		bss_amount,
		bss_Undefined,
	};

	typedef _state__<backend_selection_state__, bss_amount> _bss__;

	class backend_selection
	: public _bss__
	{
	private:
		str::string _HostService;
		void _SetState( backend_selection_state__ State )
		{
			_bss__::SetState( State );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_bss__::reset( P );

			_HostService.reset( P );
		}
		void SetLocal( void )
		{
			if ( GetState() != bss_Undefined )
				ERRc();

			_SetState( bssLocal );

			_HostService.Init();
		}
		void SetRemote( const str::string_ &HostService )
		{
			if ( GetState() != bss_Undefined )
				ERRc();

			_SetState( bssRemote );

			_HostService.Init( HostService );
		}
		void SetCancelledState( void )
		{
			if ( GetState() != bss_Undefined )
				ERRc();

			_SetState( bssCancelled );
		}
		void GetHostService( str::string_ &HostService ) const
		{
			if ( GetState() != bssRemote )
				ERRc();

			HostService = _HostService;
		}
		void SetState( void )	// To avoid the use of the '_bss__' one, because it has not to be used directly.
		{
			ERRu();
		}
		E_TYPEDEF_( str::string, HostService );
	};

	enum database_selection_state__ {
		dssValidated,
		dssCancelled,
		dss_amount,
		dss_Undefined,
	};

	typedef _state__<database_selection_state__, dss_amount> _dss__;

	class database_selection
	: public _dss__
	{
	private:
		str::string _Path;
		void _SetState( database_selection_state__ State )
		{
			_dss__::SetState( State );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_dss__::reset( P );

			_Path.reset( P );
		}
		void Set( const str::string_ &Path )
		{
			if ( GetState() != dss_Undefined )
				ERRc();

			_SetState( dssValidated );

			_Path.Init( Path );
		}
		void SetCancelledState( void )
		{
			if ( GetState() != dss_Undefined )
				ERRc();

			_SetState( dssCancelled );
		}
		void Get( str::string_ &Path ) const
		{
			if ( GetState() != dssValidated )
				ERRc();

			Path = _Path;
		}
		void SetState( void )	// To avoid the use of the '_dis__' one, because it has not to be used directly.
		{
			ERRu();
		}
	};

	enum record_input_state__ {
		risCreation,
		risModification,
		risDuplication,
		ris_amount,
		ris_Undefined,
	};

	typedef _state__<record_input_state__, ris_amount> _ris__;

	class record_input
	: public _ris__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_ris__::reset( P );
		}
	};

#define FRDTRNSNT_DEF( type, object )\
	private:\
		type _##object;\
	public:\
		const type &object( void ) const\
		{\
		_Test( c##object );\
			return _##object;\
		}\
		type &object( void )\
		{\
			_TestAndSet( c##object );\
			return _##object;\
		}\


	class transient__
	{
	private:
		context__ _Context;
		void _Test( context__ Context ) const
		{
			if ( _Context != Context )
				ERRu();
		}
		void _TestAndSet( context__ Context )
		{
			if ( _Context == c_Undefined )
				_Context = Context;

			if ( _Context != Context )
				ERRu();
		}
	public:
		FRDTRNSNT_DEF( structure_management, StructureManagement );
		FRDTRNSNT_DEF( database_identification, DatabaseIdentification );
		FRDTRNSNT_DEF( database_selection, DatabaseSelection );
		FRDTRNSNT_DEF( backend_selection, BackendSelection );
		FRDTRNSNT_DEF( record_input, RecordInput );
		void reset( bso::bool__ P = true )
		{
			_Context = c_Undefined;

			_StructureManagement.reset( P );
			_DatabaseIdentification.reset( P );
			_DatabaseSelection.reset( P );
			_BackendSelection.reset( P );
			_RecordInput.reset();
		}
		transient__( void )
		{
			reset( false );
		}
		void Init( void )
		{
			reset();
		}
		E_RODISCLOSE__( context__, Context );
	};
}

#endif