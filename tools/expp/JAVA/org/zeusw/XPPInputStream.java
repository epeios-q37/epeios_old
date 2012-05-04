/*
	'XPPInputStream.java' by Claude SIMON (claude.simon@zeusw.org)
	XML Preprocessor library for 'XPPInputStream' JAVA component.
	Copyright (C) 2007-2011 Claude SIMON

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    'XPPInputStream.java' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XPPInputStream.java' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'XPPInputStream.java'.  If not, see <http://www.gnu.org/licenses/>.
*/

package org.zeusw;

import java.io.*;
import org.zeusw.epeios.*;

public class XPPInputStream extends FlowInputStream {
	private long core;
	private InputStream in;
	
	native private void contructor();
	native private void destructor();
	
	static {
		System.out.println( "Avant" );
		System.out.flush();
		System.loadLibrary("jexpp");
		System.out.println( "Apres" );
		System.out.flush();
	}
	
    public XPPInputStream( InputStream in  )
	{
		
		System.out.println( "constructeur Avant" );
		System.out.flush();
		this.in = in;
		System.out.println( "constructeur Pendant" );
		System.out.flush();
		contructor();
		System.out.println( "constructeur Apres" );
		System.out.flush();
	}

	public void close()
	{
		destructor();
		super.close();
	}
}