/*
	'trunk.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eDemo' software.

    'eDemo' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eDemo' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eDemo'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "trunk.h"

using namespace trunk;

void trunk::user_callback__::XULFTKFormatedInformations(
	const char *LauncherIdentification,
	str::string_ &Informations )

{
ERRProlog
	tol::buffer__ Buffer;
	str::string ProtocolVersion, BackendLabel, APIVersion, BackendInformations;
ERRBegin
	Informations.Append( NAME " V" VERSION DEBUG_FLAG );
	Informations.Append( " by " AUTHOR_NAME " (" AUTHOR_CONTACT ")"".\n\t" );
	Informations.Append( __DATE__ " " __TIME__ );
	Informations.Append( " (" );
	Informations.Append( LauncherIdentification );
	Informations.Append( " <" );
	Informations.Append( tol::DateAndTime( Buffer ) );
	Informations.Append( ">)\n" );
	Informations.Append( COPYRIGHT "\n" );
	Informations.Append( '\n' );
	Informations.Append( PROJECT_AFFILIATION "\n" );
	Informations.Append( '\n' );
	Informations.Append( SOFTWARE_AFFILIATION );

	if ( _T().Kernel().IsConnected() ) {
		Informations.Append( '\n' );
		Informations.Append( '\n' );

		ProtocolVersion.Init();
		BackendLabel.Init();
		APIVersion.Init();
		BackendInformations.Init();

		_T().Kernel().AboutBackend( ProtocolVersion, BackendLabel, APIVersion, BackendInformations );

		Informations.Append( "Backend : " );
		Informations.Append( BackendInformations );
	}
ERRErr
ERREnd
ERREpilog
}

void trunk::trunk___::GetDirectoryName( void )
{
ERRProlog
	str::string SymbolicName, Directory;
	STR_BUFFER___ Buffer;
ERRBegin
	SymbolicName.Init();
	UI().SessionView.Widgets.txbDirectorySymbolicName.GetValue( SymbolicName );

	Directory.Init();
	if ( !nsxpcm::GetDirectory( SymbolicName.Convert( Buffer ), Directory, err::hUserDefined ) )
		UI().SessionView.Widgets.txbDirectory.SetAttribute( "placeholder", "Error" );

	UI().SessionView.Widgets.txbDirectory.SetValue( Directory );

ERRErr
ERREnd
ERREpilog

}
