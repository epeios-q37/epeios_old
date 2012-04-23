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