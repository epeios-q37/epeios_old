jdk=$(HOME)/jdk
# Modify the above line so that the 'jdk' variable contains the root directory where the JDK is installed.

name=jcmc

os := $(shell uname -o 2>/dev/null || uname -s)

co = -g -I$(jdk)/include -I$(jdk)/include/win32

lo =

ifeq ("$(os)","Cygwin")
	target=$(name).dll
	co += "-D__int64=long long"
	lo += -shared
endif

ifeq ("$(os)","GNU/Linux")
	target=lib$(name).so
	lo += -W1,-soname,$(target) -shared
endif

ifeq ("$(os)","Darwin")
	target=lib$(name).jnilib
	lo += -dynamiclib
endif

all : $(target) $(name).class

$(target): $(name).o
	g++ $(lo) -o $(target) $<
	
$(name).o: $(name).cpp $(name).h
	g++ -c $(co)  $<

$(name).class : $(name).java
	javac $(name).java

$(name).h : $(name).java
	javah $(name)
