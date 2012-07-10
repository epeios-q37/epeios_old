jdk=$(HOME)/jdk
name=jcmc

os := $(shell uname -o 2>/dev/null || uname -s)

co = -g -I$(jdk)/include -I$(jdk)/include/win32

lo = -shared -static

ifeq ("$(os)","Cygwin")
	target=$(name).dll
	lo += -Wl,--add-stdcall-alias  -Wl,--kill-at
	co += "-D__int64=long long"
endif

ifeq ("$(os)","GNU/Linux")
	target=lib$(name).so
	lo += -W1,-soname,$(target)
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

