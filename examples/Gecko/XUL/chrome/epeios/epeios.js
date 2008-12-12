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
	egeckocom = Components.classes["@zeusw.org/egeckocom;1"].createInstance().QueryInterface(Components.interfaces.iegeckocom);
} catch (err) {
	alert(err);
}

// alert( eprivate );

function register (
	windows,
	identification 	)
 {
	try {
		egeckocom.Register( window, identification );
	} catch (err ) {
		alert( err );
	}
 }
 
function registeringStart ()
 {
	try {
		egeckocom.RegisteringStart();
	} catch (err ) {
		alert( err );
	}
 }

function registeringEnd ()
 {
	try {
		egeckocom.RegisteringEnd();
	} catch (err ) {
		alert( err );
	}
 }

function unregister ()
 {
	try {
		egeckocom.Unregister();
	} catch (err ) {
		alert( err );
	}
 }


	commandLineConfigFile = "";

	commandLineConfigFile = window.arguments[0].QueryInterface(Components.interfaces.nsICommandLine).getArgument( 0 );
	
	alert( commandLineConfigFile );

	commandLineConfigFile = window.arguments[0].QueryInterface(Components.interfaces.nsICommandLine).getArgument( 1 );

	alert( commandLineConfigFile );
	
	commandLineConfigFile = window.arguments[1].QueryInterface(Components.interfaces.nsICommandLine).getArgument( 0 );

	alert( commandLineConfigFile );
	
	commandLineConfigFile = window.arguments[1].QueryInterface(Components.interfaces.nsICommandLine).getArgument( 1 );
	
	alert( commandLineConfigFile );
	
