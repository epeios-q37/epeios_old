# $Id$

include Makefile.header

filelist = i/epeios.xml i/ebkd.xml i/ecgi.xml i/etmc.xml i/exml.xml i/emth.xml

ifeq ("$(EPEIOS_OS)","linux")
sab_arg='$$os=$(EPEIOS_OS)'
endif

ifeq ("$(EPEIOS_OS)","windows")
sab_arg="$$os=$(EPEIOS_OS)"
endif


all: $(filelist)
	$(mkdir) -p i
	sabcmd file://$(templates)/map.xsl site.xml i/map.xml
	reveal $(crpfile) site.crp i/site.crp
	reveal i/site.crp site.xml i/site.xml
	sabcmd make.xsl site.xml i/Makefile $(sab_arg)
	make -f i/Makefile
	make -f Makefile.data

i/%.xml: i i/%.lst
	gendoc i/$*.lst $(cvs)/$*/stable $@
	
i/%.lst: i
	ls $(cvs)/$*/stable | grep "\.h" >$@

i:
	$(mkdir) -p $@
