function Test()
{
	Body = document.getElementById('body');
	
	H = document.createElementNS( "http://www.w3.org/1999/xhtml", "h2" );
	H.appendChild( document.createTextNode( "Hello the world !" ) );
	Body.appendChild( H );
}


function TestXSLT(){
var XMLString="<collection><cd><title>Fight for your mind</title><artist>Ben arper</artist><year>1995</year></cd><cd><title>Electric Ladyland</title><artist>Jimi Hendrix</artist><year>1997</year></cd></collection>";

var xslStylesheet;
var xsltProcessor = new XSLTProcessor();


var xmlDoc;

	var parser = new DOMParser();
	
	var XML = parser.parseFromString( XMLString, "text/xml" );

  // load the xslt file, example1.xsl
  var myXMLHTTPRequest = new XMLHttpRequest();
  myXMLHTTPRequest.open("GET", "PListView.xsl", false);
  myXMLHTTPRequest.send(null);

  xslStylesheet = myXMLHTTPRequest.responseXML;
  xsltProcessor.importStylesheet(xslStylesheet);

  // load the xml file, example1.xml
  myXMLHTTPRequest = new XMLHttpRequest();
  myXMLHTTPRequest.open("GET", "example.xml", false);
  myXMLHTTPRequest.send(null);

  xmlDoc = myXMLHTTPRequest.responseXML;

  var fragment = xsltProcessor.transformToFragment(xmlDoc, document);

  document.getElementById("example").innerHTML = "";

  document.getElementById("example").appendChild(fragment);
}	