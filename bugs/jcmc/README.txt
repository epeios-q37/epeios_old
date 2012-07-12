You will find at http://cvs.savannah.gnu.org/viewvc/epeios/bugs/jcmc/?root=epeios a test case to demonstrate the crash which happends when the 'malloc()' function is called within a Java native componenent compiled with g++ under Cygwin. There are 5 files :
	- Makefile : to build the Java native component, and the Java class calling this component,
	- README.txt : this file,
	- jcmc.cpp : the native component C++ source code,
	- jcmc.h : the C++ header for the native component source code,
	- jcmc.java : the Java source code which uses the native componenent.
	
Given that you have installed 'Cygwin' with 'make' and 'g++', and also the JDK and JRE, you have to adapt the first line of the 'Makefile' file to set the 'jdk' variable to the root directory where your JDK is installed. Then, you have to go with 'Cygwin' to the directory containing all the above files, and launch the 'make' command.
Once having the 'jcmc.dll' and the 'jcmc.class' files, when you launch 'java jcmc', the program will either freeze, or the JVM will print a 'fatal error' message.
If you open the 'jcmc.cpp' file, which is the source code of the Java native component, you will see that there is only a 'malloc(10)' call, which shoudn't behave like that.
The same test case compiled under Windows with Visual C++ doesn't crash. This test case compiled with 'g++' under GNU/Linux or Mac OS (the given 'Makefile' should also work for this OSes) also doesn't crash.