/*
	'FlowInputStream.java' by Claude SIMON (claude.simon@zeusw.org)
	XML Preprocessor library for 'XPPInputStream' JAVA component.
	Copyright (C) 2007-2011 Claude SIMON

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    'FlowInputStream.java' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'FlowInputStream.java' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'FlowInputStream.java'.  If not, see <http://www.gnu.org/licenses/>.
*/

package org.zeusw.epeios;

import java.io.*;

public class FlowInputStream extends InputStream {
	private long core;

	protected native void constructor( long iflow );
	private native void destructor();

	public void close()
	{
		destructor();
	}
	
	native public int available();
	native public int read();
	native public int read(byte[] b);
	native public int read(byte[] b, int off, int len);
}