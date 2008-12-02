/* $Id$ */
alert("From 'epeios.js' !");

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
Components.classes['@mozilla.org/consoleservice;1']
            .getService(Components.interfaces.nsIConsoleService)
            .logStringMessage("logStringMessage");
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
	gesbib = Components.classes["@logiplus.fr/gesbib;1"].getService().QueryInterface(Components.interfaces.igesbib);
} catch (err) {
	alert(err);
}

try {
	eprivate = Components.classes["@zeusw.org/eprivate;1"].createInstance().QueryInterface(Components.interfaces.ieprivate);
} catch (err) {
	alert(err);
}

alert( eshared );

eshared.Test();
