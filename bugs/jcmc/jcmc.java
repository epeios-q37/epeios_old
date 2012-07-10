class jcmc {
	static private native void bug();

	static public void main ( String[] args )
	{
		System.out.print( "Loading library 'jcmc'..." );
		System.out.flush();
		System.loadLibrary("jcmc");
		System.out.println( " 'jcmc' library loaded !" );
		System.out.print( "Trying to call 'malloc' function from a native component..." );
		System.out.flush();
		bug();
		System.out.println( " 'malloc' calling succeed !" );
	}
}