jdk=$(HOME)/jdk
# Modify the above line so that the 'jdk' variable contains the root directory where the JDK is installed.

name = jcmc

os := $(shell uname -o 2>/dev/null || uname -s)

co = -g -I$(jdk)/include -I$(jdk)/include/win32

lo =

ifeq ("$(os)","Cygwin")
	target = $(name).dll
	os := Cygwin-mingw
	# To use the default Cygwin compiler, comment above line, and uncomment belove line.
	# os := Cygwin-base
endif

ifeq ("$(os)","Cygwin-base")
	cc = g++
	co += "-D__int64=long long"
	lo += -shared -Wl,--add-stdcall-alias
endif

ifeq ("$(os)","Cygwin-mingw")
	cc = i686-pc-mingw32-g++	# Should be changed when compiling for 64 bits.
	co += 
	lo += -shared -Wl,--kill-at
endif

ifeq ("$(os)","GNU/Linux")
	target=lib$(name).so
	cc = g++
	lo += -W1,-soname,$(target) -shared
endif

ifeq ("$(os)","Darwin")
	target=lib$(name).jnilib
	cc = g++
	lo += -dynamiclib
endif

all : $(target) $(name).class

$(target) : $(name).o
	$(cc) $(lo) -o $(target) $<
	
$(name).o : $(name).cpp $(name).h
	$(cc) -c $(co)  $<

$(name).class : $(name).java
	$(jdk)/bin/javac $(name).java

$(name).h : $(name).java
	$(jdk)/bin/javah $(name)

clean :
	rm $(name).o $(name).dll $(name).class $(name).h
