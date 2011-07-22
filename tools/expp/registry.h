/*
	Header for the 'registry' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of th 'xxx_app' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*/
// $Id$


#ifndef REGISTRY__INC
# define REGISTRY__INC

# include "expp.h"
# include "rgstry.h"

namespace registry {
	extern rgstry::multi_level_registry Registry;

	using rgstry::value_;
	E_AUTO( value );

	inline rgstry::status__ FillRegistry(
		const char *FileName,
		rgstry::context___ &Context )
	{
		return Registry.Fill( Registry.CreateNewLevel(), FileName, xpp::criterions___(),  "Configurations/Configuration[target=\"" NAME "\"]", Context );
	}

	struct path {
		static const char *Language;
		static const char *LocaleFileName;
	};

	inline bso::bool__ GetValue(
		const char *Path,
		value_ &Value )
	{
		return Registry.GetValue( Path, Value );
	}

# define GET_VALUE( name, path )\
	inline bso::bool__ Get##name( value_ &Value )\
	{\
		return GetValue( path, Value );\
	}\


# define GET_ROOT_VALUE( name )\
	GET_VALUE( name, path::name )


	GET_ROOT_VALUE( Language );
	GET_ROOT_VALUE( LocaleFileName )
}

#endif