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

/*
	commandLineConfigFile = "";

	commandLineConfigFile = window.arguments[0].QueryInterface(Components.interfaces.nsICommandLine).getArgument( 0 );
	
	alert( commandLineConfigFile );

	commandLineConfigFile = window.arguments[0].QueryInterface(Components.interfaces.nsICommandLine).getArgument( 1 );

	alert( commandLineConfigFile );
	
	commandLineConfigFile = window.arguments[1].QueryInterface(Components.interfaces.nsICommandLine).getArgument( 0 );

	alert( commandLineConfigFile );
	
	commandLineConfigFile = window.arguments[1].QueryInterface(Components.interfaces.nsICommandLine).getArgument( 1 );
	
	alert( commandLineConfigFile );
*/	


function circle(ctx, color, x, y, radius)	// function from xul.fr
{
	var radians = (Math.PI / 180)* 0.1;
	
	ctx.beginPath();
	ctx.arc(x, y, radius, 0, radians, true);
	ctx.strokeStyle = color;		
	ctx.stroke();
}

function roundedRect(ctx,x,y,width,height,radius)		// function from Mozilla
{
	ctx.beginPath();
	ctx.moveTo(x,y+radius);
	ctx.lineTo(x,y+height-radius);
	ctx.quadraticCurveTo(x,y+height,x+radius,y+height);
	ctx.lineTo(x+width-radius,y+height);
	ctx.quadraticCurveTo(x+width,y+height,x+width,y+height-radius);
	ctx.lineTo(x+width,y+radius);
	ctx.quadraticCurveTo(x+width,y,x+width-radius,y);
	ctx.lineTo(x+radius,y);
	ctx.quadraticCurveTo(x,y,x,y+radius);
	ctx.stroke();
}

function draw()
{
	var canvas = document.getElementById('mycanvas');
	if (canvas.getContext)
	{
		var ctx = canvas.getContext('2d');

		ctx.strokeStyle = "green"; 
		ctx.strokeRect(50,50,50,50);

		ctx.fillStyle = "orange";
		ctx.fillRect(25,125,100,100);

		ctx.strokeStyle = "blue"; 
		roundedRect(ctx, 12,120,60,60,15);
		
		circle(ctx, "red", 100, 150, 90);
	}
}