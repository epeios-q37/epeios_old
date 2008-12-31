/*
	'emobda.js' by Claude SIMON (http://zeusw.org/).

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

function log ( text )
{
	try {
		Components.classes['@mozilla.org/consoleservice;1']
            .getService(Components.interfaces.nsIConsoleService)
            .logStringMessage( text );
	} catch (err) {
		alert(err);
	}
}

JSConsoleWindow = null;

function jsconsole ()
{
	if ( ( JSConsoleWindow == null ) || ( JSConsoleWindow.closed ) )
		JSConsoleWindow = window.open('chrome://global/content/console.xul', '_blank', 'chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar');
	else {
		JSConsoleWindow.focus();
	}
}

// jsconsole();


try {
Components.utils.reportError("reportError");
} catch (err) {
	alert(err);
}

try {
	netscape.security.PrivilegeManager.enablePrivilege("UniversalXPConnect");
} catch (err) {
	alert(err);
}

try {
	emobdacom = Components.classes["@zeusw.org/emobdacom;1"].createInstance().QueryInterface(Components.interfaces.iemobdacom);
} catch (err) {
	alert(err);
}

// alert( eprivate );

function register (
	windows,
	identification 	)
 {
	try {
		emobdacom.Register( window, identification );
	} catch (err ) {
		alert( err );
	}
 }
 
function registeringStart ()
 {
	try {
		emobdacom.RegisteringStart();
	} catch (err ) {
		alert( err );
	}
 }

function registeringEnd ()
 {
	try {
		emobdacom.RegisteringEnd();
	} catch (err ) {
		alert( err );
	}
 }

function unregister ()
 {
	try {
		emobdacom.Unregister();
	} catch (err ) {
		alert( err );
	}
 }