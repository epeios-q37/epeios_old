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

public class XPPInputStream extends InputStream {
	private long core;
	private InputStream in;
	
	native private void contructor();
	native private void destructor();
	
	native public int available();
	native public int read();
	native public int read(byte[] b);
	native public int read(byte[] b, int off, int len);

    public XPPInputStream( InputStream in  )
	{
		this.in = in;
		contructor();
	}

	public void close() throws IOException
	{
		destructor();
		super.close();
	}
}