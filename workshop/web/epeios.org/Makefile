# $Id$

include Makefile.header

filelist = i/epeios.xml i/ebkd.xml i/ecgi.xml i/etmc.xml

all: $(filelist)
	$(mkdir) -p i
	reveal $(crpfile) site.crp i/site.crp
	reveal i/site.crp site.xml i/site.xml
	sabcmd make.xsl site.xml i/Makefile '$$os=$(EPEIOS_OS)'
	make -f i/Makefile
	make -f Makefile.data

i/%.xml: i i/%.lst
	gendoc i/$*.lst $(cvs)/$*/stable $@
	
i/%.lst: i
	ls $(cvs)/$*/stable | grep "\.h" >$@

i:
	$(mkdir) -p $@
