/*
	'kernel.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "kernel.h"
#include "flx.h"

using namespace kernel;

#define USERDATA_ATTRIBUTE_NAME	"userdata"

nsxpcm::repository< kernel::kernel___, krow__> kernel::Repository;

void kernel::kernel___::Init( const char *ConfigFile )
{
ERRProlog
	message__ Message = mbdkernl::m_Undefined;
	str::string ConfigurationId;
	str::string UserData;
	str::string UserConfigurationPath;
	flx::E_STRING_IFLOW__ SIFlow;
	xtf::extended_text_iflow__ XTFlow;
ERRBegin
	ConfigurationId.Init();

	if ( ( Message = _kernel___::Init( ConfigFile, ConfigurationId ) ) != mbdkernl::m_Undefined )
		Alert( Message );
	else {
		table__ Table;

		UserConfigurationPath.Init( "Configuration[id=\"" );
		UserConfigurationPath.Append( ConfigurationId );
		UserConfigurationPath.Append( "\"]" );

		UserData.Init();

		nsxpcm::GetAttribute( nsxpcm::GetWindowElement( UI().Main.Window ), USERDATA_ATTRIBUTE_NAME, UserData );

		if ( UserData.Amount() == 0 )
			UserData.Init( "<Configurations/>" );

		SIFlow.Init( UserData );
		XTFlow.Init( SIFlow );

		_LocalRegistryRoot = SetLocalRegistry( XTFlow, UserConfigurationPath );

		if ( GetCurrentTable( Table ) )
			SelectTable( Table );
		else
			ApplySession();
	}
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::_SaveLocalRegistry( void ) const
{
ERRProlog
	str::string UserData;
	flx::E_STRING_OFLOW___ SOFlow;
	txf::text_oflow__ TOFlow( SOFlow );
ERRBegin
	UserData.Init();
	SOFlow.Init( UserData );

	DumpRegistry( _LocalRegistryRoot, TOFlow );

	SOFlow.reset();	// To flush buffer.

	nsxpcm::SetAttribute( nsxpcm::GetWindowElement( UI().Main.Window ), USERDATA_ATTRIBUTE_NAME, UserData );

ERRErr
ERREnd
ERREpilog
}


static class kernel_starter
{
public:
	kernel_starter( void )
	{
		kernel::Repository.Init();
	}
	~kernel_starter( void )
	{}
} Starter_;
