/*
	'trunk.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "trunk.h"

using namespace trunk;

void trunk::user_callback__::XULFTKAboutText(
	const char *LauncherIdentification,
	const char *BuildInformations,
	str::string_ &Text )
{
	xulftk::BuildAboutText(
		LauncherIdentification, BuildInformations, 
		APP_NAME, VERSION, DEBUG_FLAG,
		AUTHOR_NAME, AUTHOR_CONTACT,
		COPYRIGHT,
		AFFILIATED_SOFTWARE_NAME, AFFILIATED_SOFTWARE_URL,
		AFFILIATED_PROJECT_NAME, AFFILIATED_PROJECT_URL,
		_T(), Text );
}

void trunk::trunk___::TestLocaleInProjectFile_( void )
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();
	Kernel().GetTranslation( "Test", Translation );
	UI().LogAndPrompt( Translation );

	Translation.Init();
	Kernel().GetTranslation( "Essai", Translation );
	UI().LogAndPrompt( Translation );
ERRErr
ERREnd
ERREpilog
}



