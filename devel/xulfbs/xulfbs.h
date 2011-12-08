/*
	Header for the 'xulfbs' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#ifndef XULFBS__INC
#define XULFBS__INC

#define XULFBS_NAME		"XULFBS"

#define	XULFBS_VERSION	"$Revision$"

#define XULFBS_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFBSTutor;

#if defined( XXX_DBG ) && !defined( XULFBS_NODBG )
#define XULFBS_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D XUL Frontend BaSe 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

#include "xulwdg.h"

// Predeclarations
namespace xulftk {
	class trunk___;
};

namespace xulfrd {
	class ui___;
}

# define XULFBS_REH( name )	XULFBS_EH( name, nsxpcm::efCommand );

# define XULFBS_W( widget, name )	typedef xulfbs::widget__<nsxpcm::widget> name;

namespace xulfbs {

	using xulftk::trunk___;

	void _Report(
		trunk___ &Trunk,
		const char *Message );

	typedef nsxpcm::ui_core__ _ui_core__;

	template <typename widget> E_TTCLONE__( xulwdg::widget__<E_COVER2( xulftk::trunk___, widget )>, widget__ );

	template <int events> E_TTCLONE__( xulwdg::event_handler__<E_COVER2( xulftk::trunk___, events )>, _event_handler__ );

	template <int events> class event_handler__
	: public _event_handler__<events>
	{
	protected:
		virtual void NSXPCMOnErr( const char *Message )
		{
			_Report( Target(), Message );
		}
	};

# define XULFBS_EH( name, events )\
	class name\
	: public event_handler__<events>\
	{\
	protected:\
		virtual void NSXPCMOnEvent( nsxpcm::event__ Event );\
	};

	XULFBS_W( broadcaster__, broadcaster__ );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
