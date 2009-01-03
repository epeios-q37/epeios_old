/*
	'mbdbkd.h' by Claude SIMON (http://zeusw.org/).

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


#ifndef MBDBKD__INC
#define MBDBKD__INC

#include "mbdapi.h"
#include "bkduac.h"
#include "csducl.h"

namespace mbdbkd{
	class backend___
	{
	private:
		emobda_backend_access___<bkduac::backend_universal_access___> _BackendAccess;
		mbd_manager_common__	_ManagerCommon;
		csducl::universal_client _Flow;
	public:
		mbd_manager___ Manager;
		void reset( bso::bool__ P = true )
		{
			Manager.reset( P );
			// _ManagerCommon.reset( P );
			_BackendAccess.reset( P );
			_Flow.reset( P );
		}
		void Init( csducl::universal_client_core &ClientCore )
		{
			_Flow.Init( ClientCore );
			switch ( ClientCore.GetType() ) {
			case csducl::tLibrary:
				_BackendAccess.Init( _Flow, bkduac::tLocal );
				break;
			case csducl::tShared:
				_BackendAccess.Init( _Flow, bkduac::tRemote );
				break;
			default:
				ERRu();
				break;
			}
			_ManagerCommon.Init( _BackendAccess );
			Manager.Init( _ManagerCommon );
			
		}
		const char *GetRawMessage( void ) const
		{
			return _BackendAccess.GetRawMessage();
		}
		const char *GetI18Message( void ) const
		{
			return _BackendAccess.GetI18Message();
		}
	};
}

#endif