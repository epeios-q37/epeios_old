# $id$

include Makefile.header

all: i/epeios.xml i/ebkd.xml i/ecgi.xml i/etmc.xml
	$(mkdir) -p i
	reveal $(crpfile) site.crp i/site.crp
	reveal i/site.crp site.xml i/site.xml
	sabcmd make.xsl site.xml i/Makefile $$web_dir=$(dest)
	make -f i/Makefile
	
i/%.xml: i i/%.lst
	gendoc i/$*.lst $(cvs)/$*/stable $@
	
i/%.lst: i
	ls $(cvs)/$*/stable | grep "\.h" >$@

i:
	$(mkdir) $@