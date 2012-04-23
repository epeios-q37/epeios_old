package org.zeusw;

import java.io.*;
import org.zeusw.epeios.*;

public class XPPInputStream extends FlowInputStream {
	private long core;
	private InputStream in;
	
	native private void contructor();
	native private void destructor();
	
    public XPPInputStream( InputStream in  )
	{
		this.in = in;
		contructor();
	}

	public void close()
	{
		destructor();
		super.close();
	}
}