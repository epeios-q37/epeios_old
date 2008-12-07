/* $Id$ */
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

log("From 'epeios.js' !");

JSConsoleWindow = null;

function jsconsole ()
{
	if ( ( JSConsoleWindow == null ) || ( JSConsoleWindow.closed ) )
		JSConsoleWindow = window.open('chrome://global/content/console.xul', '_blank', 'chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar');
	else {
		JSConsoleWindow.focus();
	}
}

jsconsole();


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
	eshared = Components.classes["@zeusw.org/eshared;1"].getService().QueryInterface(Components.interfaces.ieshared);
} catch (err) {
	alert(err);
}

try {
	eprivate = Components.classes["@zeusw.org/eprivate;1"].createInstance().QueryInterface(Components.interfaces.ieprivate);
} catch (err) {
	alert(err);
}

// alert( eprivate );

function Set ( Value )
{
	try {
		eshared.Set( Value );
	} catch (err) {
		alert(err);
	}
}

function Get ()
{
	try {
		Value = eshared.Get();
	} catch (err) {
		alert(err);
	}
	
	return Value;
}

function setUI ( document )
 {
	try {
		eprivate.SetUI( document );
	} catch (err ) {
		alert( err );
	}
 }

