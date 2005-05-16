try {
	netscape.security.PrivilegeManager.enablePrivilege("UniversalXPConnect");
	etest = Components.classes["@epeios.org/etest;1"].createInstance().QueryInterface(Components.interfaces.ietest);
} catch (err) {
	alert(err);
}

function Test()
{
	document.getElementById('Test').value = "Texte javascript";
//	alert( document.firstChild.lastChild.nodeName );
	alert( etest.XPCOM() );
//	document.getElementById('Test').value = "Ce qui va devenir le texte C++";
}

function ArraySetting( array )
{
	alert( array.arr );
	array.arr =  "ab,bc,ca".split();
	alert( array.arr );
}

function ArrayTest()
{
	var o = new Object();
	ArraySetting( o );
	alert( o.arr );
}