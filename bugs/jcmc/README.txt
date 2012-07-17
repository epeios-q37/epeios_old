You will find at http://cvs.savannah.gnu.org/viewvc/epeios/bugs/jcmc/?root=epeios a test case for the creation of a 'Java' native component using 'Cygwin'. Initially, this test case was written to demonstrate the crash which happends when the 'malloc()' function is called within a Java native componenent compiled with 'g++' under 'Cygwin', hence the call to 'malloc' in the '.cpp' file. By updating the 'JRE', 'JDK' and 'Cygwin', another error occurs, which prevents the loading of the native component. Finally, the only way I found to compile a 'Java' native componenent under 'Cygwin' is to use 'MinGW'.

There are 4 files :
	- Makefile : to build the 'Java' native component, and the 'Java' class calling this component,
	- README.txt : this file,
	- jcmc.cpp : the native component C++ source code,
	- jcmc.java : the Java source code which uses the native componenent.
	
Given that you have installed 'Cygwin' with 'make' and 'mingw-g++', and also the JDK and JRE, you have to adapt the first line of the 'Makefile' file to set the 'jdk' variable to the root directory where your JDK is installed. Then, you have to go with 'Cygwin' to the directory containing all the above files, and launch the 'make' command. You have also to install the 'libcgj11' 'Cygwin' package and do the following tweaks :
	- copy '/usr/i686-pc-mingw32/sys-root/mingw/bin/libgcc_s_dw2-1.dll' in '/bin'
	- in '/bin', make a copy of 'cyggcj-11.dll' and anme it 'libgcj-11.dll'.
As 'java' is a Windows program, it doesn't recognize 'Cygwin' links ('ln -s ...'), this is why you have to make copies instead of links.

The 'Makefile' works for a 32 bits installation. If you have a 64 bits one, you probably have to modify the 'Makefile' accordingly. The 'Makefile' should also work for GNU/Linux and Mac OS.

Once having the 'jcmc.dll' and the 'jcmc.class' files, you can run the test case by launching 'java jcmc'. Actually, it works, so this test case seems not to be very usefull. But you can try it using the native 'Cygwin' compiler instaed of the 'MinGW' one, by following the instructions given in the 'Makefile' itself, to see what happends.

Given that the 'MinGW' compiler is less often installed as the native 'Cygwin' one, and the tweaks to make so the former can be used, I would like to find a way to use the 'Cygwin' native compiler, and this test case should help to reach this goal.


Claude SIMON.

Web site (in French) : http://zeusw.org/
Some software (in English) : http://zeusw.org/intl/