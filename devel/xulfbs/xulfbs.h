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

#define XULFBS__WIDGET_COMMON( widget, name )\
	class name\
	: public _widget__<widget<xulftk::trunk___>>\
	{

// Sensitive widget (reacts to events).
#define XULFBS_SWIDGET( widget, name )\
	XULFBS__WIDGET_COMMON( widget, name )\
	protected:\
		virtual void XULWDGOnEvent( nsxpcm::event__ Event );\
	}

// Unsensitive widget (reacts to no event).
#define XULFBS_UWIDGET( widget, name )\
	XULFBS__WIDGET_COMMON( widget, name )\
	}

#define XULFBS_WINDOW( name )		XULFBS_SWIDGET( xulwdg::window__, name )
#define XULFBS_COMMAND( name )		XULFBS_SWIDGET( xulwdg::command__, name )
//#define XULFBS_BROADCASTER( name )	XULFBS_UWIDGET( xulwdg::broadcaster__, name )


namespace xulfbs {

	using xulftk::trunk___;
	using xulfrd::ui___;

	typedef nsxpcm::ui_core__ _ui_core__;

	template <typename widget> class _widget__
	: public widget
	{
	protected:
		virtual void XULWDGReport( const char *Message )
		{
			_Report( Target(), Message );
		}
	};

	typedef _widget__<xulwdg::broadcaster__<trunk___>> broadcaster__;

	void _Report(
		xulftk::trunk___ &Trunk,
		const char *Message );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
