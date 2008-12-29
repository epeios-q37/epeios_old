/*
	'mdbxml.h' by Claude SIMON (http://zeusw.org/).

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

// eMoBDa XML

#ifndef MDBXML__INC
#	define MDBXML__INC

#	include "xml.h"
#	include "xmldsc.h"

#	define MBDXML_STRUCTURE_FORMAT_VERSION_TAG_NAME	"FormatVersion"
#	define MBDXML_DESCRIPTION

namespace mbdxml {

	enum version__ {
		v0_1_0,
		vDefault = v0_1_0
		v_amount,
		v_Undefined
	};

	enum item__ {
		i DescriptionFormat,
		i_amount,
		i_Undefined
	);

	inline const char *GetLabel(
		item__ Item,
		version__ Version )
	{
		const char *Label = NULL;

		switch ( Item ) {
		case i_Undefined:
			ERRu();
			break;
		default:
			ERRu();
			break;
		}

		return Label;
	}

	void Export(
		const xmldsc::description_ &Description,
		xml::writer_ &Writer );

}

#endif