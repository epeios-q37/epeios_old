# Makefile template file V1.0.2

name = mll2html
copt = -DXXX_DBG
lopt = 
libs = ttr tol flw stf err str gnumll tagdtc clnarg uym fam mmi txmpgh xtf ctn

ifeq ("$(ECPL)","bcc")
ox = obj
cc = bcc32
co = -q -c -w-aus
lo = -q -e$(name)
endif

ifeq ("$(ECPL)","gcc")
ox = o
cc = g++ 
co = -c
lo = -o $(name)
endif

co += -I$(ESRC) $(ECOPT) $(copt)
lo += $(ELOPT) $(lopt)

objs = $(name).$(ox) $(libs:=.$(ox))

$(name):  $(objs) $(name).$(ox)
	$(cc) $(lo) *.$(ox)
	
%.$(ox): %.cpp
	$(cc) $(co) $< 

%.$(ox): $(ESRC)/%.cpp
	$(cc) $(co) $<
