/*
	Header for the 'xulfbs' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

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

// Sensitive widget (reacts to events).
#define XULFBS_SWIDGET( widget, name )\
	class name\
	: public widget<xulftk::trunk___>\
	{\
	protected:\
		virtual void XULWDGOnEvent( nsxpcm::event__ Event );\
	}

// Unsensitive widget (reacts to no event).
#define XULFBS_UWIDGET( widget, name )\
	class name\
	: public widget<xulftk::trunk___>\
	{}

#define XULFBS_WINDOW( name )		XULFBS_SWIDGET( xulwdg::window__, name )
#define XULFBS_COMMAND( name )		XULFBS_SWIDGET( xulwdg::command__, name )
#define XULFBS_BROADCAST( name )	XULFBS_UWIDGET( xulwdg::broadcast__, name )


namespace xulfbs {

	using xulftk::trunk___;
	using xulfrd::ui___;

//	using xulwdg::Register;

	typedef nsxpcm::ui_core__ _ui_core__;
	typedef xulwdg::bare_bridge__<trunk___> bridge__;

	typedef xulwdg::broadcast__<trunk___> broadcast__;
	typedef xulwdg::command__<trunk___> command__;
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
