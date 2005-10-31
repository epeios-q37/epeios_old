function Test()
{
	Body = document.getElementById('body');
	
	H = document.createElementNS( "http://www.w3.org/1999/xhtml", "h2" );
	H.appendChild( document.createTextNode( "Hello the world !" ) );
	Body.appendChild( H );
}

