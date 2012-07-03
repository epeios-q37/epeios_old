/*
	'XMLPreprocessorDemo.java' by Claude SIMON (claude.simon@zeusw.org)
	XML Preprocessor library for 'XPPInputStream' JAVA component.
	Copyright (C) 2007-2011 Claude SIMON

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    'XMLPreprocessorDemo.java' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XMLPreprocessorDemo.java' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'XMLPreprocessorDemo.java'.  If not, see <http://www.gnu.org/licenses/>.
*/

import java.io.*;
import org.zeusw.*;
import javax.xml.parsers.*;
import org.w3c.dom.*;
import org.xml.sax.*;

class XMLPreprocessorDemo {

	private static native void mallocCygwinBug();

	public static void main ( String[] args )	throws Exception
	{
		int A = 0;
		System.out.println( A++ );
		System.out.flush();
		System.loadLibrary("jexpp");
		System.out.println( A++ );
		System.out.flush();
		mallocCygwinBug();

		System.out.println( A++ );
		System.out.flush();
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		System.out.println( A++ );
		System.out.flush();
		DocumentBuilder builder = factory.newDocumentBuilder();
		System.out.println( A++ );
		System.out.flush();
		File file = new File("../Contacts.xml");
		System.out.println( A++ );
		System.out.flush();
		FileInputStream fstream = new FileInputStream(file);
		System.out.println( A++ );
		System.out.flush();
		XPPInputStream xppstream = new XPPInputStream(fstream);
		System.out.println( A++ );
		System.out.flush();
		
		Document doc = builder.parse(xppstream);
		NodeList list = doc.getElementsByTagName("*");
		for (int i=0; i<list.getLength(); i++) {
			Element element = (Element)list.item(i);
			System.out.print(element.getNodeName());
			if ( ( element.getFirstChild() != null ) && ( element.getFirstChild().getNodeValue().trim().length() != 0 ) ) {
				System.out.print( " : " );
				System.out.println(element.getFirstChild().getNodeValue());
			} else
				System.out.println();
			System.out.flush();
		}
	}
}

