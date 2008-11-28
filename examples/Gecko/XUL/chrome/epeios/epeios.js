/* $Id$ */
alert("From 'epeios.js' !");

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
