class jcmc {
	static private native void bug();

	static public void main ( String[] args )
	{
		System.loadLibrary("jcmc");
		bug();
	}
}